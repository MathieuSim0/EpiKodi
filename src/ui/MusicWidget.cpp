#include "ui/MusicWidget.h"
#include <QHBoxLayout>
#include <QSplitter>

MusicWidget::MusicWidget(MusicBrainzApiClient* apiClient, FavoritesManager* favoritesManager,
                         QWidget* parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_favoritesManager(favoritesManager)
{
    setupUI();
    connect(m_apiClient, &MusicBrainzApiClient::artistsFound, this, &MusicWidget::onArtistsFound);
    connect(m_apiClient, &MusicBrainzApiClient::albumsFound, this, &MusicWidget::onAlbumsFound);
    connect(m_searchButton, &QPushButton::clicked, this, &MusicWidget::onSearchClicked);
    connect(m_albumsListWidget, &QListWidget::itemClicked, this, &MusicWidget::onAlbumSelected);
    connect(m_favoriteButton, &QPushButton::clicked, this, &MusicWidget::onAddToFavorites);
}

void MusicWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Rechercher un artiste ou un album...");
    m_searchButton = new QPushButton("🔍 Rechercher", this);
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchButton);
    mainLayout->addLayout(searchLayout);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // Tabs pour artistes et albums
    m_resultsTabWidget = new QTabWidget(this);
    m_artistsListWidget = new QListWidget(this);
    m_albumsListWidget = new QListWidget(this);
    m_resultsTabWidget->addTab(m_artistsListWidget, "Artistes");
    m_resultsTabWidget->addTab(m_albumsListWidget, "Albums");
    splitter->addWidget(m_resultsTabWidget);

    // Panneau de détails
    QWidget* detailsWidget = new QWidget(this);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsWidget);

    m_coverLabel = new QLabel(this);
    m_coverLabel->setFixedSize(300, 300);
    m_coverLabel->setScaledContents(true);
    m_coverLabel->setStyleSheet("border: 2px solid #ccc; background-color: #f0f0f0;");
    
    m_albumInfoLabel = new QLabel("<h2>Sélectionnez un album</h2>", this);
    m_albumInfoLabel->setWordWrap(true);

    m_favoriteButton = new QPushButton("⭐ Ajouter aux favoris", this);
    m_favoriteButton->setEnabled(false);

    detailsLayout->addWidget(m_coverLabel);
    detailsLayout->addWidget(m_albumInfoLabel);
    detailsLayout->addWidget(m_favoriteButton);
    detailsLayout->addStretch();

    splitter->addWidget(detailsWidget);
    mainLayout->addWidget(splitter);
    
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MusicWidget::onSearchClicked);
}

void MusicWidget::onSearchClicked() {
    QString query = m_searchEdit->text().trimmed();
    if (!query.isEmpty()) {
        m_artistsListWidget->clear();
        m_albumsListWidget->clear();
        
        // Rechercher à la fois artistes et albums
        m_apiClient->searchArtists(query);
        m_apiClient->searchAlbums(query);
    }
}

void MusicWidget::onArtistsFound(const QVector<Artist>& artists) {
    m_currentArtists = artists;
    m_artistsListWidget->clear();

    for (const auto& artist : artists) {
        QString displayText = QString("%1 (%2)")
            .arg(artist.getName())
            .arg(artist.getCountry());
        
        QListWidgetItem* item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, artist.getId());
        m_artistsListWidget->addItem(item);
    }

    if (artists.isEmpty()) {
        m_artistsListWidget->addItem("Aucun artiste trouvé");
    }
}

void MusicWidget::onAlbumsFound(const QVector<Album>& albums) {
    m_currentAlbums = albums;
    m_albumsListWidget->clear();

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
        m_albumsListWidget->addItem("Aucun album trouvé");
    }
}

void MusicWidget::onAlbumSelected(QListWidgetItem* item) {
    QString albumId = item->data(Qt::UserRole).toString();
    
    for (const auto& album : m_currentAlbums) {
        if (album.getId() == albumId) {
            m_selectedAlbum = album;
            displayAlbumDetails(album);
            
            // Récupérer la pochette
            m_apiClient->getAlbumCoverArt(albumId);
            
            break;
        }
    }
}

void MusicWidget::displayAlbumDetails(const Album& album) {
    m_albumInfoLabel->setText(QString("<h2>%1</h2>"
                                      "<b>Artiste:</b> %2<br>"
                                      "<b>Date de sortie:</b> %3<br>"
                                      "<b>Pistes:</b> %4")
        .arg(album.getTitle())
        .arg(album.getArtistName())
        .arg(album.getReleaseDate().toString("dd/MM/yyyy"))
        .arg(album.getTrackCount()));

    m_coverLabel->setText("🎵\n" + album.getTitle());

    bool isFavorite = m_favoritesManager->isAlbumInFavorites(album.getId());
    m_favoriteButton->setText(isFavorite ? "❌ Retirer des favoris" : "⭐ Ajouter aux favoris");
    m_favoriteButton->setEnabled(true);
}

void MusicWidget::onAddToFavorites() {
    if (m_selectedAlbum.getId().isEmpty()) return;

    bool isFavorite = m_favoritesManager->isAlbumInFavorites(m_selectedAlbum.getId());
    
    if (isFavorite) {
        m_favoritesManager->removeAlbumFromFavorites(m_selectedAlbum.getId());
        m_favoriteButton->setText("⭐ Ajouter aux favoris");
    } else {
        m_favoritesManager->addAlbumToFavorites(m_selectedAlbum);
        m_favoriteButton->setText("❌ Retirer des favoris");
    }
}
