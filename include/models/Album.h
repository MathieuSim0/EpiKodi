#ifndef ALBUM_H
#define ALBUM_H

#include <QString>
#include <QDate>
#include <QVector>
#include <nlohmann/json.hpp>

/**
 * @brief Modèle représentant un album musical
 */
class Album {
public:
    Album() = default;
    Album(const QString& id, const QString& title, const QString& artistName,
          const QDate& releaseDate);

    // Getters
    QString getId() const { return m_id; }
    QString getTitle() const { return m_title; }
    QString getArtistName() const { return m_artistName; }
    QString getArtistId() const { return m_artistId; }
    QDate getReleaseDate() const { return m_releaseDate; }
    QString getCoverArtUrl() const { return m_coverArtUrl; }
    int getTrackCount() const { return m_trackCount; }

    // Setters
    void setArtistId(const QString& id) { m_artistId = id; }
    void setCoverArtUrl(const QString& url) { m_coverArtUrl = url; }
    void setTrackCount(int count) { m_trackCount = count; }

    // Conversion JSON
    nlohmann::json toJson() const;
    static Album fromJson(const nlohmann::json& json);

private:
    QString m_id;
    QString m_title;
    QString m_artistName;
    QString m_artistId;
    QDate m_releaseDate;
    QString m_coverArtUrl;
    int m_trackCount{0};
};

#endif // ALBUM_H
