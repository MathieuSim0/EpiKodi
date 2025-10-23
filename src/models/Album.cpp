#include "models/Album.h"

Album::Album(const QString& id, const QString& title, const QString& artistName,
             const QDate& releaseDate)
    : m_id(id)
    , m_title(title)
    , m_artistName(artistName)
    , m_releaseDate(releaseDate)
{
}

nlohmann::json Album::toJson() const {
    return nlohmann::json{
        {"id", m_id.toStdString()},
        {"title", m_title.toStdString()},
        {"artist_name", m_artistName.toStdString()},
        {"artist_id", m_artistId.toStdString()},
        {"release_date", m_releaseDate.toString(Qt::ISODate).toStdString()},
        {"cover_art_url", m_coverArtUrl.toStdString()},
        {"track_count", m_trackCount}
    };
}

Album Album::fromJson(const nlohmann::json& json) {
    Album album;
    album.m_id = QString::fromStdString(json.value("id", ""));
    album.m_title = QString::fromStdString(json.value("title", ""));
    album.m_artistName = QString::fromStdString(json.value("artist_name", ""));
    album.m_artistId = QString::fromStdString(json.value("artist_id", ""));
    
    QString dateStr = QString::fromStdString(json.value("release_date", ""));
    album.m_releaseDate = QDate::fromString(dateStr, Qt::ISODate);
    
    album.m_coverArtUrl = QString::fromStdString(json.value("cover_art_url", ""));
    album.m_trackCount = json.value("track_count", 0);
    
    return album;
}
