#include "ui/FavoritesWidget.h"
#include "ui/MovieCard.h"
#include "ui/SeriesCard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGridLayout>

FavoritesWidget::FavoritesWidget(FavoritesManager* favoritesManager, QWidget* parent)
    : QWidget(parent)
    , m_favoritesManager(favoritesManager)
{
    setupUI();
    refreshFavorites();
    
    connect(m_favoritesManager, &FavoritesManager::favoritesChanged,
            this, &FavoritesWidget::onFavoritesChanged);
}

void FavoritesWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    QLabel* titleLabel = new QLabel("⭐ Mes Favoris", this);
    titleLabel->setProperty("class", "title");
    mainLayout->addWidget(titleLabel);

    // Tabs pour les différents types de favoris
    m_tabWidget = new QTabWidget(this);
    
    // Widget Films avec grille
    QWidget* moviesTab = new QWidget();
    QVBoxLayout* moviesLayout = new QVBoxLayout(moviesTab);
    moviesLayout->setContentsMargins(0, 0, 0, 0);
    
    QScrollArea* moviesScrollArea = new QScrollArea();
    moviesScrollArea->setWidgetResizable(true);
    moviesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QWidget* moviesContainer = new QWidget();
    m_moviesLayout = new QGridLayout(moviesContainer);
    m_moviesLayout->setSpacing(16);
    m_moviesLayout->setContentsMargins(8, 8, 8, 8);
    
    moviesScrollArea->setWidget(moviesContainer);
    moviesLayout->addWidget(moviesScrollArea);
    
    // Widget Séries avec grille
    QWidget* seriesTab = new QWidget();
    QVBoxLayout* seriesLayout = new QVBoxLayout(seriesTab);
    seriesLayout->setContentsMargins(0, 0, 0, 0);
    
    QScrollArea* seriesScrollArea = new QScrollArea();
    seriesScrollArea->setWidgetResizable(true);
    seriesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QWidget* seriesContainer = new QWidget();
    m_seriesLayout = new QGridLayout(seriesContainer);
    m_seriesLayout->setSpacing(16);
    m_seriesLayout->setContentsMargins(8, 8, 8, 8);
    
    seriesScrollArea->setWidget(seriesContainer);
    seriesLayout->addWidget(seriesScrollArea);
    
    // Widget Albums (simple liste pour l'instant)
    m_albumsListWidget = new QListWidget();
    
    m_tabWidget->addTab(moviesTab, "🎬 Films");
    m_tabWidget->addTab(seriesTab, "📺 Séries");
    m_tabWidget->addTab(m_albumsListWidget, "🎵 Albums");

    mainLayout->addWidget(m_tabWidget);

    // Connexion pour les albums
    connect(m_albumsListWidget, &QListWidget::itemClicked, this, &FavoritesWidget::onAlbumSelected);
}

void FavoritesWidget::onFavoritesChanged() {
    refreshFavorites();
}

void FavoritesWidget::refreshFavorites() {
    // Nettoyer les grilles
    QLayoutItem* item;
    while ((item = m_moviesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    while ((item = m_seriesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Rafraîchir les films avec cartes
    const auto& movies = m_favoritesManager->getFavoriteMovies();
    if (movies.isEmpty()) {
        QLabel* noMovies = new QLabel("📽️ Aucun film favori\n\nAjoutez des films depuis l'onglet Films", this);
        noMovies->setAlignment(Qt::AlignCenter);
        noMovies->setStyleSheet("QLabel { color: #94a3b8; font-size: 12pt; padding: 40px; }");
        m_moviesLayout->addWidget(noMovies, 0, 0);
    } else {
        int row = 0, col = 0;
        for (const auto& movie : movies) {
            MovieCard* card = new MovieCard(movie, this);
            // Pas besoin de connexion car c'est juste pour affichage
            m_moviesLayout->addWidget(card, row, col);
            
            col++;
            if (col >= 3) {
                col = 0;
                row++;
            }
        }
        m_moviesLayout->setRowStretch(row + 1, 1);
    }

    // Rafraîchir les séries avec cartes
    const auto& series = m_favoritesManager->getFavoriteSeries();
    if (series.isEmpty()) {
        QLabel* noSeries = new QLabel("📺 Aucune série favorite\n\nAjoutez des séries depuis l'onglet Séries", this);
        noSeries->setAlignment(Qt::AlignCenter);
        noSeries->setStyleSheet("QLabel { color: #94a3b8; font-size: 12pt; padding: 40px; }");
        m_seriesLayout->addWidget(noSeries, 0, 0);
    } else {
        int row = 0, col = 0;
        for (const auto& s : series) {
            SeriesCard* card = new SeriesCard(s, this);
            m_seriesLayout->addWidget(card, row, col);
            
            col++;
            if (col >= 3) {
                col = 0;
                row++;
            }
        }
        m_seriesLayout->setRowStretch(row + 1, 1);
    }

    // Rafraîchir les albums (liste simple)
    m_albumsListWidget->clear();
    const auto& albums = m_favoritesManager->getFavoriteAlbums();
    for (const auto& album : albums) {
        QString displayText = QString("%1 - %2 (%3)")
            .arg(album.getArtistName())
            .arg(album.getTitle())
            .arg(album.getReleaseDate().year());
        
        QListWidgetItem* item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, album.getId());
        m_albumsListWidget->addItem(item);
    }
    if (albums.isEmpty()) {
        m_albumsListWidget->addItem("🎵 Aucun album favori");
    }
}

void FavoritesWidget::onMovieSelected(QListWidgetItem* item) {
    // Plus utilisée avec les cartes
}

void FavoritesWidget::onSeriesSelected(QListWidgetItem* item) {
    // Plus utilisée avec les cartes
}

void FavoritesWidget::onAlbumSelected(QListWidgetItem* item) {
    // Plus utilisée avec les cartes
}
