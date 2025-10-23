#include "ui/MainWindow.h"
#include "ui/MovieWidget.h"
#include "ui/SeriesWidget.h"
#include "ui/MusicWidget.h"
#include "ui/FavoritesWidget.h"
#include "ui/PlayerWidget.h"
#include "api/TMDbApiClient.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QResizeEvent>
#include <fstream>
#include <nlohmann/json.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_tabWidget(nullptr)
    , m_movieWidget(nullptr)
    , m_seriesWidget(nullptr)
    , m_musicWidget(nullptr)
    , m_favoritesWidget(nullptr)
    , m_playerWidget(nullptr)
    , m_tmdbClient(nullptr)
    , m_musicBrainzClient(nullptr)
    , m_favoritesManager(nullptr)
    , m_mediaPlayer(nullptr)
{
    setWindowTitle("EpiKodi - Media Center");
    resize(1280, 720);
    setMinimumSize(800, 600);

    loadConfiguration();
    
    // Initialiser les services avec TMDb
    m_tmdbClient = new TMDbApiClient(m_tmdbApiKey, this);
    m_musicBrainzClient = new MusicBrainzApiClient(m_musicBrainzUserAgent, this);
    m_favoritesManager = new FavoritesManager(m_favoritesFilePath, this);
    m_mediaPlayer = new MediaPlayer(this);

    setupUI();
    createMenuBar();
    createStatusBar();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // Widget central avec tabs
    m_tabWidget = new QTabWidget(this);
    
    // Créer les différents widgets avec TMDb
    m_movieWidget = new MovieWidget(m_tmdbClient, m_favoritesManager, this);
    m_seriesWidget = new SeriesWidget(m_tmdbClient, m_favoritesManager, this);
    m_musicWidget = new MusicWidget(m_musicBrainzClient, m_favoritesManager, this);
    m_favoritesWidget = new FavoritesWidget(m_favoritesManager, this);
    m_playerWidget = new PlayerWidget(m_mediaPlayer, this);

    // Ajouter les tabs
    m_tabWidget->addTab(m_movieWidget, "🎬 Films");
    m_tabWidget->addTab(m_seriesWidget, "📺 Séries");
    m_tabWidget->addTab(m_musicWidget, "🎵 Musique");
    m_tabWidget->addTab(m_favoritesWidget, "⭐ Favoris");

    // Layout principal
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addWidget(m_playerWidget);
    
    setCentralWidget(centralWidget);

    // Connecter le lecteur aux widgets
    connect(m_movieWidget, &MovieWidget::playMedia, m_playerWidget, &PlayerWidget::loadAndPlay);
    connect(m_musicWidget, &MusicWidget::playMedia, m_playerWidget, &PlayerWidget::loadAndPlay);
}

void MainWindow::loadConfiguration() {
    try {
        std::ifstream file("config/config.json");
        if (!file.is_open()) {
            QMessageBox::critical(this, "Erreur de configuration",
                "Impossible de charger config/config.json.\n"
                "Veuillez copier config.example.json vers config/config.json "
                "et ajouter votre clé API TMDb.");
            QApplication::quit();
            return;
        }

        nlohmann::json config;
        file >> config;
        file.close();

        // Charger les paramètres API TMDb
        m_tmdbApiKey = QString::fromStdString(
            config["api"]["tmdb"]["api_key"].get<std::string>());
        
        m_musicBrainzUserAgent = QString::fromStdString(
            config["api"]["musicbrainz"]["user_agent"].get<std::string>());

        // Charger les paramètres de l'application
        m_favoritesFilePath = QString::fromStdString(
            config["app"]["favorites_file"].get<std::string>());

        // Vérifier que la clé TMDb est configurée
        if (m_tmdbApiKey.isEmpty()) {
            QMessageBox::warning(this, "Configuration incomplète",
                "Veuillez ajouter votre clé API TMDb dans config/config.json.\n"
                "L'application fonctionnera en mode limité.");
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur de configuration",
            QString("Erreur lors du chargement de la configuration: %1").arg(e.what()));
        QApplication::quit();
    }
}

void MainWindow::createMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    
    // Menu Fichier
    QMenu* fileMenu = menuBar->addMenu("&Fichier");
    QAction* quitAction = fileMenu->addAction("&Quitter");
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);

    // Menu Favoris
    QMenu* favoritesMenu = menuBar->addMenu("&Favoris");
    QAction* viewFavoritesAction = favoritesMenu->addAction("Voir les favoris");
    connect(viewFavoritesAction, &QAction::triggered, [this]() {
        m_tabWidget->setCurrentWidget(m_favoritesWidget);
    });

    // Menu Aide
    QMenu* helpMenu = menuBar->addMenu("&Aide");
    QAction* aboutAction = helpMenu->addAction("À &propos");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "À propos d'EpiKodi",
            "<h2>EpiKodi v1.0.0</h2>"
            "<p>Media Center moderne développé en C++ avec Qt 6</p>"
            "<p><b>Fonctionnalités:</b></p>"
            "<ul>"
            "<li>Recherche de films et séries (OMDb)</li>"
            "<li>Recherche de musique (MusicBrainz)</li>"
            "<li>Lecteur multimédia intégré</li>"
            "<li>Gestion des favoris</li>"
            "</ul>"
            "<p>Développé par Mathieu Sim</p>");
    });

    setMenuBar(menuBar);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Prêt");
    
    // Connecter les erreurs des API au status bar
    connect(m_tmdbClient, &TMDbApiClient::error, [this](const QString& error) {
        statusBar()->showMessage("Erreur TMDb: " + error, 5000);
    });
    
    connect(m_musicBrainzClient, &MusicBrainzApiClient::error, [this](const QString& error) {
        statusBar()->showMessage("Erreur MusicBrainz: " + error, 5000);
    });
    
    connect(m_favoritesManager, &FavoritesManager::error, [this](const QString& error) {
        statusBar()->showMessage("Erreur Favoris: " + error, 5000);
    });
    
    connect(m_mediaPlayer, &MediaPlayer::error, [this](const QString& error) {
        statusBar()->showMessage("Erreur Lecteur: " + error, 5000);
    });
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    // Force la mise à jour de la géométrie pour éviter les erreurs Wayland
    updateGeometry();
}
