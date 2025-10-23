#include "api/MusicBrainzApiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>
#include <QThread>

MusicBrainzApiClient::MusicBrainzApiClient(const QString& userAgent, QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_userAgent(userAgent)
    , m_baseUrl("https://musicbrainz.org/ws/2")
    , m_coverArtUrl("https://coverartarchive.org")
{
}

void MusicBrainzApiClient::searchArtists(const QString& query) {
    QUrl url(m_baseUrl + "/artist");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("query", query);
    urlQuery.addQueryItem("fmt", "json");
    urlQuery.addQueryItem("limit", "20");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    QNetworkReply* reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MusicBrainzApiClient::onSearchArtistsFinished);
}

void MusicBrainzApiClient::searchAlbums(const QString& query) {
    QUrl url(m_baseUrl + "/release");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("query", query);
    urlQuery.addQueryItem("fmt", "json");
    urlQuery.addQueryItem("limit", "20");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    QNetworkReply* reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MusicBrainzApiClient::onSearchAlbumsFinished);
}

void MusicBrainzApiClient::getArtistAlbums(const QString& artistId) {
    QUrl url(m_baseUrl + "/release");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("artist", artistId);
    urlQuery.addQueryItem("fmt", "json");
    urlQuery.addQueryItem("limit", "50");
    urlQuery.addQueryItem("type", "album");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    QNetworkReply* reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MusicBrainzApiClient::onArtistAlbumsFinished);
}

void MusicBrainzApiClient::getAlbumCoverArt(const QString& albumId) {
    QUrl url(m_coverArtUrl + "/release/" + albumId);
    
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    QNetworkReply* reply = m_networkManager->get(request);
    
    // Stocker l'ID de l'album dans la propriété de la réponse
    reply->setProperty("albumId", albumId);
    
    connect(reply, &QNetworkReply::finished, this, &MusicBrainzApiClient::onCoverArtFinished);
}

void MusicBrainzApiClient::onSearchArtistsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QVector<Artist> artists = parseArtistsResponse(data);
        emit artistsFound(artists);
    } else {
        emit error("Erreur lors de la recherche d'artistes: " + reply->errorString());
    }

    reply->deleteLater();
    rateLimitDelay();
}

void MusicBrainzApiClient::onSearchAlbumsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QVector<Album> albums = parseAlbumsResponse(data);
        emit albumsFound(albums);
    } else {
        emit error("Erreur lors de la recherche d'albums: " + reply->errorString());
    }

    reply->deleteLater();
    rateLimitDelay();
}

void MusicBrainzApiClient::onArtistAlbumsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QVector<Album> albums = parseAlbumsResponse(data);
        emit albumsFound(albums);
    } else {
        emit error("Erreur lors de la récupération des albums: " + reply->errorString());
    }

    reply->deleteLater();
    rateLimitDelay();
}

void MusicBrainzApiClient::onCoverArtFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QString albumId = reply->property("albumId").toString();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QString coverUrl = parseCoverArtResponse(data);
        emit coverArtReceived(albumId, coverUrl);
    } else {
        // Pas d'erreur si pas de pochette trouvée
        emit coverArtReceived(albumId, QString());
    }

    reply->deleteLater();
}

QVector<Artist> MusicBrainzApiClient::parseArtistsResponse(const QByteArray& data) {
    QVector<Artist> artists;
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray artistsArray = obj["artists"].toArray();

    for (const QJsonValue& value : artistsArray) {
        QJsonObject artistObj = value.toObject();
        
        QString id = artistObj["id"].toString();
        QString name = artistObj["name"].toString();
        QString country = artistObj["country"].toString();
        
        Artist artist(id, name, country);
        artist.setType(artistObj["type"].toString());
        
        artists.append(artist);
    }

    return artists;
}

QVector<Album> MusicBrainzApiClient::parseAlbumsResponse(const QByteArray& data) {
    QVector<Album> albums;
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray releasesArray = obj["releases"].toArray();

    for (const QJsonValue& value : releasesArray) {
        QJsonObject albumObj = value.toObject();
        
        QString id = albumObj["id"].toString();
        QString title = albumObj["title"].toString();
        QString dateStr = albumObj["date"].toString();
        
        // Récupérer le nom de l'artiste
        QString artistName;
        QJsonArray artistCredits = albumObj["artist-credit"].toArray();
        if (!artistCredits.isEmpty()) {
            artistName = artistCredits[0].toObject()["artist"].toObject()["name"].toString();
        }
        
        QDate releaseDate = QDate::fromString(dateStr, Qt::ISODate);
        
        Album album(id, title, artistName, releaseDate);
        
        // Récupérer le nombre de pistes
        if (albumObj.contains("track-count")) {
            album.setTrackCount(albumObj["track-count"].toInt());
        }
        
        albums.append(album);
    }

    return albums;
}

QString MusicBrainzApiClient::parseCoverArtResponse(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray imagesArray = obj["images"].toArray();

    // Récupérer la première image (généralement la pochette principale)
    if (!imagesArray.isEmpty()) {
        QJsonObject imageObj = imagesArray[0].toObject();
        return imageObj["image"].toString();
    }

    return QString();
}

void MusicBrainzApiClient::rateLimitDelay() {
    // MusicBrainz demande 1 requête par seconde maximum
    QThread::msleep(1000);
}
