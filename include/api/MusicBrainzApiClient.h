#ifndef MUSICBRAINZAPICLIENT_H
#define MUSICBRAINZAPICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QVector>
#include "models/Artist.h"
#include "models/Album.h"

/**
 * @brief Client API pour MusicBrainz et Cover Art Archive
 * Permet de rechercher des artistes et albums
 */
class MusicBrainzApiClient : public QObject {
    Q_OBJECT

public:
    explicit MusicBrainzApiClient(const QString& userAgent, QObject* parent = nullptr);
    ~MusicBrainzApiClient() = default;

    // Recherche d'artistes
    void searchArtists(const QString& query);
    
    // Recherche d'albums
    void searchAlbums(const QString& query);
    
    // Récupérer les albums d'un artiste
    void getArtistAlbums(const QString& artistId);
    
    // Récupérer la pochette d'un album
    void getAlbumCoverArt(const QString& albumId);

signals:
    // Signaux pour les résultats
    void artistsFound(const QVector<Artist>& artists);
    void albumsFound(const QVector<Album>& albums);
    void coverArtReceived(const QString& albumId, const QString& coverUrl);
    
    // Signaux d'erreur
    void error(const QString& errorMessage);

private slots:
    void onSearchArtistsFinished();
    void onSearchAlbumsFinished();
    void onArtistAlbumsFinished();
    void onCoverArtFinished();

private:
    QNetworkAccessManager* m_networkManager;
    QString m_userAgent;
    QString m_baseUrl;
    QString m_coverArtUrl;

    QVector<Artist> parseArtistsResponse(const QByteArray& data);
    QVector<Album> parseAlbumsResponse(const QByteArray& data);
    QString parseCoverArtResponse(const QByteArray& data);
    
    void rateLimitDelay(); // MusicBrainz a un rate limit
};

#endif // MUSICBRAINZAPICLIENT_H
