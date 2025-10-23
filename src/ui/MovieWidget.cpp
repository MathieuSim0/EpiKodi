#include "ui/MovieWidget.h"
#include "ui/MovieCard.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

MovieWidget::MovieWidget(TMDbApiClient* apiClient, FavoritesManager* favoritesManager,
                         QWidget* parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_favoritesManager(favoritesManager)
{
    setupUI();

    // Connecter les signaux de l'API TMDb
    connect(m_apiClient, &TMDbApiClient::moviesFound, this, &MovieWidget::onMoviesFound);
    connect(m_apiClient, &TMDbApiClient::movieDetailsReceived, this, &MovieWidget::displayMovieDetails);
    connect(m_apiClient, &TMDbApiClient::movieCreditsReceived, this, &MovieWidget::displayMovieCredits);
    // Popular movies on startup
    connect(m_apiClient, &TMDbApiClient::popularMoviesFound, this, &MovieWidget::onMoviesFound);
    connect(m_searchButton, &QPushButton::clicked, this, &MovieWidget::onSearchClicked);
    connect(m_favoriteButton, &QPushButton::clicked, this, &MovieWidget::onAddToFavorites);

    // Charger les films populaires au démarrage
    m_apiClient->getPopularMovies();
}

void MovieWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // Titre de la section
    QLabel* titleLabel = new QLabel("🎬 Découvrez des Films", this);
    titleLabel->setProperty("class", "title");
    mainLayout->addWidget(titleLabel);

    // Barre de recherche moderne
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(12);
    
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("🔍 Rechercher un film (ex: Matrix, Inception...)");
    m_searchEdit->setMinimumHeight(48);
    
    m_searchButton = new QPushButton("🔎 Rechercher", this);
    m_searchButton->setMinimumSize(160, 48);
    
    searchLayout->addWidget(m_searchEdit, 1);
    searchLayout->addWidget(m_searchButton);
    mainLayout->addLayout(searchLayout);

    // Layout horizontal fixe (sans splitter) : résultats à gauche (2 colonnes), détails à droite (plus large)
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(16);

    // Zone de résultats avec scroll et grille (2 colonnes maintenant)
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setMinimumWidth(480);  // Pour 2 cartes de 220px + marges
    scrollArea->setMaximumWidth(520);
    
    QWidget* resultsContainer = new QWidget();
    m_resultsLayout = new QGridLayout(resultsContainer);
    m_resultsLayout->setSpacing(16);
    m_resultsLayout->setContentsMargins(8, 8, 8, 8);
    
    scrollArea->setWidget(resultsContainer);
    contentLayout->addWidget(scrollArea);

    // Panneau de détails élargi au centre/droite
    QWidget* detailsWidget = new QWidget(this);
    detailsWidget->setMinimumWidth(600);
    detailsWidget->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 rgba(15, 23, 42, 0.95),
                                        stop:1 rgba(30, 41, 59, 0.95));
            border-left: 3px solid qlineargradient(y1:0, y2:1,
                                                   stop:0 #8b5cf6,
                                                   stop:0.5 #ec4899,
                                                   stop:1 #06b6d4);
            border-radius: 0px;
            padding: 0px;
        }
    )");
    
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsWidget);
    detailsLayout->setSpacing(16);
    detailsLayout->setContentsMargins(20, 20, 20, 20);

    // ScrollArea pour le contenu des détails
    QScrollArea* detailsScroll = new QScrollArea(this);
    detailsScroll->setWidgetResizable(true);
    detailsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    detailsScroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    
    QWidget* detailsContent = new QWidget();
    detailsContent->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* detailsContentLayout = new QVBoxLayout(detailsContent);
    detailsContentLayout->setSpacing(16);
    detailsContentLayout->setContentsMargins(10, 10, 10, 10);

    // Affiche du film (plus grande)
    m_posterLabel = new QLabel(this);
    m_posterLabel->setFixedSize(300, 450);
    m_posterLabel->setScaledContents(false);
    m_posterLabel->setAlignment(Qt::AlignCenter);
    m_posterLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 12px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(99, 102, 241, 0.15),
                                        stop:1 rgba(6, 182, 212, 0.15));
            border: 2px solid #334155;
            font-size: 14pt; 
            color: #94a3b8;
            padding: 20px;
        }
    )");
    m_posterLabel->setText("🎬\n\nSélectionnez\nun film");
    
    // Titre du film
    m_titleLabel = new QLabel("Titre du film", this);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 18pt; 
            font-weight: 700; 
            color: #f1f5f9;
            padding: 8px 4px;
            background-color: transparent;
        }
    )");
    
    // Date de sortie
    m_releaseDateLabel = new QLabel("Année: -", this);
    m_releaseDateLabel->setStyleSheet(R"(
        QLabel {
            font-size: 11pt; 
            font-weight: 500; 
            color: #94a3b8;
            background-color: transparent;
            padding: 2px 4px;
        }
    )");
    
    // Note
    m_ratingLabel = new QLabel("⭐ Note: -", this);
    m_ratingLabel->setStyleSheet(R"(
        QLabel {
            font-size: 12pt; 
            font-weight: 600; 
            color: #fbbf24;
            background-color: transparent;
            padding: 4px;
        }
    )");
    
    // Genres
    m_genresLabel = new QLabel("Genres: -", this);
    m_genresLabel->setWordWrap(true);
    m_genresLabel->setStyleSheet(R"(
        QLabel {
            font-size: 10pt; 
            color: #a5b4fc;
            background: rgba(139, 92, 246, 0.1);
            border-radius: 6px;
            padding: 6px 10px;
            border: 1px solid rgba(139, 92, 246, 0.3);
        }
    )");
    
    // Description
    m_overviewLabel = new QLabel("Sélectionnez un film pour voir sa description...", this);
    m_overviewLabel->setWordWrap(true);
    m_overviewLabel->setStyleSheet(R"(
        QLabel {
            font-size: 10pt; 
            line-height: 1.5;
            color: #e2e8f0; 
            padding: 12px; 
            background: rgba(99, 102, 241, 0.08); 
            border-radius: 8px;
            border: 1px solid rgba(99, 102, 241, 0.2);
        }
    )");
    m_overviewLabel->setMinimumHeight(100);

    // Titre section acteurs
    QLabel* castTitle = new QLabel("🎭 Distribution", this);
    castTitle->setStyleSheet(R"(
        QLabel {
            font-size: 14pt; 
            font-weight: 700; 
            color: #f1f5f9;
            padding: 8px 4px;
            background-color: transparent;
        }
    )");

    // Container pour les acteurs avec grille
    m_castContainer = new QWidget(this);
    m_castContainer->setStyleSheet("QWidget { background: transparent; }");
    m_castLayout = new QGridLayout(m_castContainer);
    m_castLayout->setSpacing(12);
    m_castLayout->setContentsMargins(0, 0, 0, 0);

    // Boutons d'action
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(10);
    
    m_favoriteButton = new QPushButton("⭐ Ajouter aux Favoris", this);
    m_favoriteButton->setMinimumHeight(42);
    
    m_playTrailerButton = new QPushButton("🎬 Plus d'infos", this);
    m_playTrailerButton->setMinimumHeight(42);
    m_playTrailerButton->setProperty("class", "secondary");
    
    buttonsLayout->addWidget(m_favoriteButton);
    buttonsLayout->addWidget(m_playTrailerButton);

    // Ajouter tout au layout de contenu
    detailsContentLayout->addWidget(m_posterLabel, 0, Qt::AlignCenter);
    detailsContentLayout->addWidget(m_titleLabel);
    detailsContentLayout->addWidget(m_releaseDateLabel);
    detailsContentLayout->addWidget(m_ratingLabel);
    detailsContentLayout->addWidget(m_genresLabel);
    detailsContentLayout->addWidget(m_overviewLabel);
    detailsContentLayout->addWidget(castTitle);
    detailsContentLayout->addWidget(m_castContainer);
    detailsContentLayout->addLayout(buttonsLayout);
    detailsContentLayout->addStretch();
    
    detailsScroll->setWidget(detailsContent);
    detailsLayout->addWidget(detailsScroll);

    contentLayout->addWidget(detailsWidget);

    mainLayout->addLayout(contentLayout);

    // Désactiver les boutons initialement
    m_favoriteButton->setEnabled(false);
    m_playTrailerButton->setEnabled(false);
    
    // Connexion Enter dans le champ de recherche
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MovieWidget::onSearchClicked);
}

void MovieWidget::onSearchClicked() {
    QString query = m_searchEdit->text().trimmed();
    if (!query.isEmpty()) {
        // Nettoyer la grille
        QLayoutItem* item;
        while ((item = m_resultsLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        
        m_apiClient->searchMovies(query);
    }
}

void MovieWidget::onMoviesFound(const QVector<Movie>& movies) {
    m_currentMovies = movies;
    
    // Nettoyer la grille
    QLayoutItem* item;
    while ((item = m_resultsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (movies.isEmpty()) {
        QLabel* noResults = new QLabel("😔 Aucun résultat trouvé", this);
        noResults->setProperty("class", "subtitle");
        noResults->setAlignment(Qt::AlignCenter);
        m_resultsLayout->addWidget(noResults, 0, 0);
        return;
    }

    // Créer une carte pour chaque film (grille de 2 colonnes maintenant)
    int row = 0, col = 0;
    for (const auto& movie : movies) {
        MovieCard* card = new MovieCard(movie, this);
        connect(card, &MovieCard::clicked, this, [this, movie]() {
            // Charger les détails complets depuis TMDb + acteurs
            if (movie.getId() > 0) {
                m_apiClient->getMovieDetails(movie.getId());
                m_apiClient->getMovieCredits(movie.getId());
            } else {
                // Fallback : afficher ce qu'on a
                m_selectedMovie = movie;
                displayMovieDetails(movie);
            }
        });
        
        m_resultsLayout->addWidget(card, row, col);
        
        col++;
        if (col >= 2) {  // 2 colonnes
            col = 0;
            row++;
        }
    }
    
    // Ajouter un stretch à la fin pour aligner en haut
    m_resultsLayout->setRowStretch(row + 1, 1);
}

void MovieWidget::displayMovieDetails(const Movie& movie) {
    // Stocker le film sélectionné
    m_selectedMovie = movie;
    
    // Titre
    m_titleLabel->setText(movie.getTitle());
    
    // Date de sortie / Année
    if (movie.getReleaseDate().isValid()) {
        m_releaseDateLabel->setText(QString("📅 Sortie: %1")
            .arg(movie.getReleaseDate().toString("dd MMMM yyyy")));
    } else {
        m_releaseDateLabel->setText("📅 Sortie: Non disponible");
    }
    
    // Note avec durée si disponible
    QString ratingText = QString("⭐ Note: %1/10").arg(movie.getRating(), 0, 'f', 1);
    if (movie.getRuntime() > 0) {
        ratingText += QString(" | ⏱️ %1 min").arg(movie.getRuntime());
    }
    m_ratingLabel->setText(ratingText);
    
    // Genres
    QVector<QString> genres = movie.getGenres();
    if (!genres.isEmpty()) {
        m_genresLabel->setText("🎭 " + genres.join(", "));
    } else {
        m_genresLabel->setText("🎭 Genres: Non disponibles");
    }
    
    // Description
    QString overview = movie.getOverview();
    if (overview.isEmpty() || overview == "N/A") {
        overview = "Aucune description disponible pour ce film.";
    }
    m_overviewLabel->setText(overview);
    
    // Charger le poster (taille augmentée à 300x450)
    if (!movie.getPosterPath().isEmpty() && movie.getPosterPath() != "N/A") {
        // Construire l'URL complète TMDb
        QString posterUrl = "https://image.tmdb.org/t/p/w500" + movie.getPosterPath();
        
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        QNetworkRequest request(posterUrl);
        QNetworkReply* reply = manager->get(request);
        
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                QPixmap pixmap;
                pixmap.loadFromData(reply->readAll());
                if (!pixmap.isNull()) {
                    m_posterLabel->setPixmap(pixmap.scaled(300, 450, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    m_posterLabel->setStyleSheet(R"(
                        QLabel {
                            border-radius: 12px;
                            border: 3px solid #6366f1;
                            background-color: #0f172a;
                        }
                    )");
                }
            }
            reply->deleteLater();
        });
    } else {
        m_posterLabel->setPixmap(QPixmap());
        m_posterLabel->setText("🎬\n\nPas d'affiche\ndisponible");
        m_posterLabel->setAlignment(Qt::AlignCenter);
        m_posterLabel->setStyleSheet(R"(
            QLabel {
                border-radius: 12px;
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                            stop:0 rgba(99, 102, 241, 0.15),
                                            stop:1 rgba(6, 182, 212, 0.15));
                border: 2px solid #334155;
                font-size: 14pt; 
                color: #94a3b8;
                padding: 20px;
            }
        )");
    }
    
    // Activer/désactiver le bouton favoris selon l'état
    bool isFavorite = m_favoritesManager->isMovieInFavorites(movie.getId());
    m_favoriteButton->setText(isFavorite ? "❌ Retirer des Favoris" : "⭐ Ajouter aux Favoris");
    m_favoriteButton->setEnabled(true);
    m_playTrailerButton->setEnabled(true);
}

void MovieWidget::displayMovieCredits(const QVector<QPair<QString, QString>>& cast) {
    // Nettoyer le layout des acteurs
    QLayoutItem* item;
    while ((item = m_castLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    if (cast.isEmpty()) {
        QLabel* noCast = new QLabel("Aucune information sur la distribution", m_castContainer);
        noCast->setStyleSheet("QLabel { color: #94a3b8; font-size: 10pt; }");
        m_castLayout->addWidget(noCast, 0, 0);
        return;
    }
    
    // Afficher les acteurs (3 par ligne)
    int row = 0, col = 0;
    for (const auto& actor : cast) {
        QString name = actor.first;
        QString profilePath = actor.second;
        
        // Container pour chaque acteur
        QWidget* actorWidget = new QWidget(m_castContainer);
        actorWidget->setStyleSheet("QWidget { background: transparent; }");
        QVBoxLayout* actorLayout = new QVBoxLayout(actorWidget);
        actorLayout->setSpacing(6);
        actorLayout->setContentsMargins(0, 0, 0, 0);
        
        // Photo de l'acteur
        QLabel* photoLabel = new QLabel(actorWidget);
        photoLabel->setFixedSize(80, 120);
        photoLabel->setScaledContents(false);
        photoLabel->setAlignment(Qt::AlignCenter);
        photoLabel->setStyleSheet(R"(
            QLabel {
                border-radius: 8px;
                background: rgba(99, 102, 241, 0.1);
                border: 2px solid rgba(99, 102, 241, 0.3);
            }
        )");
        photoLabel->setText("👤");
        photoLabel->setStyleSheet(photoLabel->styleSheet() + "font-size: 24pt; color: #64748b;");
        
        // Nom de l'acteur
        QLabel* nameLabel = new QLabel(name, actorWidget);
        nameLabel->setWordWrap(true);
        nameLabel->setMaximumWidth(80);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet(R"(
            QLabel {
                font-size: 8pt;
                color: #e2e8f0;
                background: transparent;
            }
        )");
        
        actorLayout->addWidget(photoLabel);
        actorLayout->addWidget(nameLabel);
        
        // Charger la photo si disponible
        if (!profilePath.isEmpty() && profilePath != "N/A") {
            QString photoUrl = "https://image.tmdb.org/t/p/w185" + profilePath;
            
            QNetworkAccessManager* manager = new QNetworkAccessManager(this);
            QNetworkRequest request(photoUrl);
            QNetworkReply* reply = manager->get(request);
            
            connect(reply, &QNetworkReply::finished, this, [photoLabel, reply]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QPixmap pixmap;
                    pixmap.loadFromData(reply->readAll());
                    if (!pixmap.isNull()) {
                        photoLabel->setPixmap(pixmap.scaled(80, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        photoLabel->setStyleSheet(R"(
                            QLabel {
                                border-radius: 8px;
                                border: 2px solid #6366f1;
                                background-color: #0f172a;
                            }
                        )");
                    }
                }
                reply->deleteLater();
            });
        }
        
        m_castLayout->addWidget(actorWidget, row, col);
        
        col++;
        if (col >= 3) {  // 3 acteurs par ligne
            col = 0;
            row++;
        }
    }
}

void MovieWidget::onAddToFavorites() {
    if (m_selectedMovie.getId() == 0) return;

    bool isFavorite = m_favoritesManager->isMovieInFavorites(m_selectedMovie.getId());
    
    if (isFavorite) {
        m_favoritesManager->removeMovieFromFavorites(m_selectedMovie.getId());
        m_favoriteButton->setText("⭐ Favoris");
    } else {
        m_favoritesManager->addMovieToFavorites(m_selectedMovie);
        m_favoriteButton->setText("❌ Retirer");
    }
}
