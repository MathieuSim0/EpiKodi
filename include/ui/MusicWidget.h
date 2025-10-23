#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QTabWidget>
#include "api/MusicBrainzApiClient.h"
#include "managers/FavoritesManager.h"
#include "models/Artist.h"
#include "models/Album.h"

class MusicWidget : public QWidget {
    Q_OBJECT

public:
    explicit MusicWidget(MusicBrainzApiClient* apiClient, FavoritesManager* favoritesManager,
                        QWidget* parent = nullptr);

signals:
    void playMedia(const QString& url, int mediaType);

private slots:
    void onSearchClicked();
    void onArtistsFound(const QVector<Artist>& artists);
    void onAlbumsFound(const QVector<Album>& albums);
    void onAlbumSelected(QListWidgetItem* item);
    void onAddToFavorites();

private:
    void setupUI();
    void displayAlbumDetails(const Album& album);

    MusicBrainzApiClient* m_apiClient;
    FavoritesManager* m_favoritesManager;
    
    QLineEdit* m_searchEdit;
    QPushButton* m_searchButton;
    QTabWidget* m_resultsTabWidget;
    QListWidget* m_artistsListWidget;
    QListWidget* m_albumsListWidget;
    QLabel* m_coverLabel;
    QLabel* m_albumInfoLabel;
    QPushButton* m_favoriteButton;

    QVector<Artist> m_currentArtists;
    QVector<Album> m_currentAlbums;
    Album m_selectedAlbum;
};

#endif // MUSICWIDGET_H
