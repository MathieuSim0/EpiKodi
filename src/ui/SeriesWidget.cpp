#include "ui/SeriesWidget.h"
#include "ui/SeriesCard.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

SeriesWidget::SeriesWidget(TMDbApiClient* apiClient, FavoritesManager* favoritesManager,
                           QWidget* parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_favoritesManager(favoritesManager)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setupUI();

    // Connecter les signaux de l'API TMDb
    connect(m_apiClient, &TMDbApiClient::seriesFound, this, &SeriesWidget::onSeriesFound);
    connect(m_apiClient, &TMDbApiClient::seriesDetailsReceived, this, &SeriesWidget::displaySeriesDetails);
    connect(m_apiClient, &TMDbApiClient::seriesCreditsReceived, this, &SeriesWidget::onSeriesCreditsReceived);
    connect(m_searchButton, &QPushButton::clicked, this, &SeriesWidget::onSearchClicked);
    connect(m_favoriteButton, &QPushButton::clicked, this, &SeriesWidget::onAddToFavorites);
}

void SeriesWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // Titre de la section
    QLabel* titleLabel = new QLabel("📺 Découvrez des Séries", this);
    titleLabel->setProperty("class", "title");
    mainLayout->addWidget(titleLabel);

    // Barre de recherche moderne
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(12);
    
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("🔍 Rechercher une série (ex: Breaking Bad, Game of Thrones...)");
    m_searchEdit->setMinimumHeight(48);
    
    m_searchButton = new QPushButton("� Rechercher", this);
    m_searchButton->setMinimumSize(160, 48);
    
    searchLayout->addWidget(m_searchEdit, 1);
    searchLayout->addWidget(m_searchButton);
    mainLayout->addLayout(searchLayout);

    // Layout horizontal : résultats à gauche (2 colonnes), détails au centre/droite
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(16);

    // Zone de résultats avec scroll et grille (2 COLONNES)
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setMinimumWidth(480);  // 2 cartes de 220px + spacing
    scrollArea->setMaximumWidth(520);
    
    QWidget* resultsContainer = new QWidget();
    m_resultsLayout = new QGridLayout(resultsContainer);
    m_resultsLayout->setSpacing(16);
    m_resultsLayout->setContentsMargins(8, 8, 8, 8);
    
    scrollArea->setWidget(resultsContainer);
    contentLayout->addWidget(scrollArea);

    // Panneau de détails avec scroll (plus large pour les acteurs)
    m_detailsScrollArea = new QScrollArea(this);
    m_detailsScrollArea->setWidgetResizable(true);
    m_detailsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_detailsScrollArea->setStyleSheet(R"(
        QScrollArea {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 rgba(15, 23, 42, 0.95),
                                        stop:1 rgba(30, 41, 59, 0.95));
            border-left: 3px solid qlineargradient(y1:0, y2:1,
                                                   stop:0 #ec4899,
                                                   stop:0.5 #8b5cf6,
                                                   stop:1 #06b6d4);
            border-radius: 0px;
        }
        QScrollBar:vertical {
            background: rgba(30, 41, 59, 0.5);
            width: 10px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #ec4899;
            border-radius: 5px;
        }
    )");

    m_detailsContent = new QWidget();
    QVBoxLayout* detailsLayout = new QVBoxLayout(m_detailsContent);
    detailsLayout->setSpacing(16);
    detailsLayout->setContentsMargins(20, 20, 20, 20);

    // Poster
    m_posterLabel = new QLabel(this);
    m_posterLabel->setFixedSize(300, 450);
    m_posterLabel->setScaledContents(false);
    m_posterLabel->setAlignment(Qt::AlignCenter);
    m_posterLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 12px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(236, 72, 153, 0.15),
                                        stop:1 rgba(139, 92, 246, 0.15));
            border: 2px solid #334155;
            font-size: 14pt; 
            color: #94a3b8;
            padding: 20px;
        }
    )");
    m_posterLabel->setText("📺\n\nSélectionnez\nune série");
    
    // Titre
    m_titleLabel = new QLabel("Titre de la série", this);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 18pt; 
            font-weight: 700; 
            color: #f1f5f9;
            padding: 10px 0px;
            background-color: transparent;
        }
    )");
    
    // Note et saisons
    m_ratingLabel = new QLabel("⭐ Note: -", this);
    m_ratingLabel->setStyleSheet(R"(
        QLabel {
            font-size: 11pt; 
            font-weight: 600; 
            color: #fbbf24;
            background-color: transparent;
            padding: 5px;
        }
    )");

    m_seasonsLabel = new QLabel("", this);
    m_seasonsLabel->setStyleSheet(R"(
        QLabel {
            font-size: 10pt; 
            color: #cbd5e1;
            background-color: transparent;
            padding: 5px;
        }
    )");

    // Genres
    m_genresLabel = new QLabel("", this);
    m_genresLabel->setWordWrap(true);
    m_genresLabel->setStyleSheet(R"(
        QLabel {
            font-size: 9pt; 
            color: #94a3b8;
            background: rgba(236, 72, 153, 0.1);
            padding: 8px;
            border-radius: 6px;
        }
    )");
    
    m_overviewLabel = new QLabel("Sélectionnez une série pour voir sa description...", this);
    m_overviewLabel->setWordWrap(true);
    m_overviewLabel->setStyleSheet(R"(
        QLabel {
            font-size: 9pt; 
            line-height: 1.4;
            color: #e2e8f0; 
            padding: 10px; 
            background: rgba(236, 72, 153, 0.08); 
            border-radius: 8px;
            border: 1px solid rgba(236, 72, 153, 0.2);
        }
    )");
    m_overviewLabel->setMinimumHeight(140);

    // Titre section acteurs
    m_castTitleLabel = new QLabel("🎭 Distribution", this);
    m_castTitleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 14pt;
            font-weight: 700;
            color: #ec4899;
            padding: 10px 0px;
            background-color: transparent;
        }
    )");
    m_castTitleLabel->hide();

    // Grille des acteurs
    QWidget* castContainer = new QWidget();
    m_castLayout = new QGridLayout(castContainer);
    m_castLayout->setSpacing(12);
    m_castLayout->setContentsMargins(0, 0, 0, 0);

    // Boutons
    m_favoriteButton = new QPushButton("⭐ Ajouter aux Favoris", this);
    m_favoriteButton->setMinimumHeight(45);
    m_favoriteButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #ec4899,
                                        stop:1 #8b5cf6);
            color: white;
            font-weight: 700;
            font-size: 11pt;
            border-radius: 8px;
            padding: 12px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #db2777,
                                        stop:1 #7c3aed);
        }
    )");

    detailsLayout->addWidget(m_posterLabel, 0, Qt::AlignCenter);
    detailsLayout->addWidget(m_titleLabel);
    detailsLayout->addWidget(m_ratingLabel);
    detailsLayout->addWidget(m_seasonsLabel);
    detailsLayout->addWidget(m_genresLabel);
    detailsLayout->addWidget(m_overviewLabel);
    detailsLayout->addWidget(m_castTitleLabel);
    detailsLayout->addWidget(castContainer);
    detailsLayout->addWidget(m_favoriteButton);
    detailsLayout->addStretch();

    m_detailsScrollArea->setWidget(m_detailsContent);
    contentLayout->addWidget(m_detailsScrollArea, 1);

    mainLayout->addLayout(contentLayout);

    // Désactiver les boutons initialement
    m_favoriteButton->setEnabled(false);
    
    // Connexion Enter dans le champ de recherche
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &SeriesWidget::onSearchClicked);
}

void SeriesWidget::onSearchClicked() {
    QString query = m_searchEdit->text().trimmed();
    if (!query.isEmpty()) {
        // Nettoyer la grille
        QLayoutItem* item;
        while ((item = m_resultsLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        
        m_apiClient->searchSeries(query);
    }
}

void SeriesWidget::onSeriesFound(const QVector<Series>& series) {
    m_currentSeries = series;
    
    // Nettoyer la grille
    QLayoutItem* item;
    while ((item = m_resultsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (series.isEmpty()) {
        QLabel* noResults = new QLabel("😔 Aucun résultat trouvé", this);
        noResults->setProperty("class", "subtitle");
        noResults->setAlignment(Qt::AlignCenter);
        m_resultsLayout->addWidget(noResults, 0, 0);
        return;
    }

    // Créer une carte pour chaque série (grille de 3 colonnes)
    int row = 0, col = 0;
    for (const auto& seriesItem : series) {
        SeriesCard* card = new SeriesCard(seriesItem, this);
        connect(card, &SeriesCard::clicked, this, [this, seriesItem]() {
            // Charger les détails complets depuis TMDb avec ID numérique
            if (seriesItem.getId() > 0) {
                m_apiClient->getSeriesDetails(seriesItem.getId());
                m_apiClient->getSeriesCredits(seriesItem.getId());  // Charger aussi les acteurs
            } else {
                // Fallback : afficher ce qu'on a
                m_selectedSeries = seriesItem;
                displaySeriesDetails(seriesItem);
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

void SeriesWidget::displaySeriesDetails(const Series& series) {
    // Stocker la série sélectionnée
    m_selectedSeries = series;
    
    // Titre
    m_titleLabel->setText(QString("%1 (%2)")
        .arg(series.getName())
        .arg(series.getFirstAirDate().year()));
    
    // Note
    m_ratingLabel->setText(QString("⭐ Note: %1/10").arg(series.getRating(), 0, 'f', 1));
    
    // Saisons et épisodes
    if (series.getNumberOfSeasons() > 0) {
        QString seasonsText = QString("📺 %1 saisons").arg(series.getNumberOfSeasons());
        if (series.getNumberOfEpisodes() > 0) {
            seasonsText += QString(" • %1 épisodes").arg(series.getNumberOfEpisodes());
        }
        m_seasonsLabel->setText(seasonsText);
        m_seasonsLabel->show();
    } else {
        m_seasonsLabel->hide();
    }
    
    // Genres
    if (!series.getGenres().isEmpty()) {
        QString genresText = "🎭 Genres: " + series.getGenres().join(", ");
        m_genresLabel->setText(genresText);
        m_genresLabel->show();
    } else {
        m_genresLabel->hide();
    }
    
    // Description
    QString overview = series.getOverview();
    if (overview.isEmpty() || overview == "N/A") {
        overview = "Aucune description disponible pour cette série.";
    }
    m_overviewLabel->setText(overview);
    
    // Charger le poster
    if (!series.getPosterPath().isEmpty() && series.getPosterPath() != "N/A") {
        // Construire l'URL complète TMDb
        QString posterUrl = "https://image.tmdb.org/t/p/w500" + series.getPosterPath();
        
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
                            border: 3px solid #ec4899;
                            background-color: #0f172a;
                        }
                    )");
                }
            }
            reply->deleteLater();
        });
    } else {
        m_posterLabel->setPixmap(QPixmap());
        m_posterLabel->setText("📺\n\nPas d'affiche\ndisponible");
        m_posterLabel->setAlignment(Qt::AlignCenter);
        m_posterLabel->setStyleSheet(R"(
            QLabel {
                border-radius: 10px;
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                            stop:0 rgba(236, 72, 153, 0.15),
                                            stop:1 rgba(139, 92, 246, 0.15));
                border: 2px solid #334155;
                font-size: 12pt; 
                color: #94a3b8;
                padding: 20px;
            }
        )");
    }
    
    // Activer/désactiver le bouton favoris selon l'état
    bool isFavorite = m_favoritesManager->isSeriesInFavorites(series.getId());
    m_favoriteButton->setText(isFavorite ? "❌ Retirer des Favoris" : "⭐ Ajouter aux Favoris");
    m_favoriteButton->setEnabled(true);
}

void SeriesWidget::onSeriesCreditsReceived(const QVector<QPair<QString, QString>>& cast) {
    // Nettoyer l'ancienne grille d'acteurs
    QLayoutItem* item;
    while ((item = m_castLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (cast.isEmpty()) {
        m_castTitleLabel->hide();
        return;
    }

    m_castTitleLabel->show();

    // Afficher jusqu'à 10 acteurs en grille 2 colonnes
    int row = 0, col = 0;
    for (const auto& actor : cast) {
        QString name = actor.first;
        QString profilePath = actor.second;

        // Container pour un acteur
        QWidget* actorWidget = new QWidget();
        actorWidget->setFixedSize(140, 200);
        actorWidget->setStyleSheet(R"(
            QWidget {
                background: rgba(236, 72, 153, 0.1);
                border-radius: 10px;
                border: 2px solid rgba(236, 72, 153, 0.3);
            }
        )");

        QVBoxLayout* actorLayout = new QVBoxLayout(actorWidget);
        actorLayout->setSpacing(8);
        actorLayout->setContentsMargins(8, 8, 8, 8);

        // Photo de l'acteur
        QLabel* photoLabel = new QLabel();
        photoLabel->setFixedSize(124, 150);
        photoLabel->setScaledContents(false);
        photoLabel->setAlignment(Qt::AlignCenter);
        photoLabel->setStyleSheet(R"(
            QLabel {
                background: rgba(139, 92, 246, 0.2);
                border-radius: 8px;
                color: #8b5cf6;
                font-size: 24pt;
            }
        )");
        photoLabel->setText("👤");

        // Nom de l'acteur
        QLabel* nameLabel = new QLabel(name);
        nameLabel->setWordWrap(true);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet(R"(
            QLabel {
                font-size: 8pt;
                font-weight: 600;
                color: #f1f5f9;
                background: transparent;
            }
        )");

        actorLayout->addWidget(photoLabel, 0, Qt::AlignCenter);
        actorLayout->addWidget(nameLabel);

        m_castLayout->addWidget(actorWidget, row, col);

        // Charger la photo de l'acteur si disponible
        if (!profilePath.isEmpty()) {
            QString photoUrl = "https://image.tmdb.org/t/p/w185" + profilePath;
            QNetworkRequest request(photoUrl);
            QNetworkReply* reply = m_networkManager->get(request);

            connect(reply, &QNetworkReply::finished, this, [photoLabel, reply]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QPixmap pixmap;
                    pixmap.loadFromData(reply->readAll());
                    if (!pixmap.isNull()) {
                        photoLabel->setPixmap(pixmap.scaled(124, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        photoLabel->setStyleSheet(R"(
                            QLabel {
                                border-radius: 8px;
                                border: 2px solid #8b5cf6;
                            }
                        )");
                    }
                }
                reply->deleteLater();
            });
        }

        col++;
        if (col >= 2) {
            col = 0;
            row++;
        }
    }
}

void SeriesWidget::onAddToFavorites() {
    if (m_selectedSeries.getId() == 0) return;

    bool isFavorite = m_favoritesManager->isSeriesInFavorites(m_selectedSeries.getId());
    
    if (isFavorite) {
        m_favoritesManager->removeSeriesFromFavorites(m_selectedSeries.getId());
        m_favoriteButton->setText("⭐ Favoris");
    } else {
        m_favoritesManager->addSeriesToFavorites(m_selectedSeries);
        m_favoriteButton->setText("❌ Retirer");
    }
}
