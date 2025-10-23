#ifndef ARTIST_H
#define ARTIST_H

#include <QString>
#include <QVector>
#include <nlohmann/json.hpp>

/**
 * @brief Modèle représentant un artiste musical
 */
class Artist {
public:
    Artist() = default;
    Artist(const QString& id, const QString& name, const QString& country);

    // Getters
    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getCountry() const { return m_country; }
    QString getType() const { return m_type; }
    QVector<QString> getGenres() const { return m_genres; }

    // Setters
    void setType(const QString& type) { m_type = type; }
    void setGenres(const QVector<QString>& genres) { m_genres = genres; }

    // Conversion JSON
    nlohmann::json toJson() const;
    static Artist fromJson(const nlohmann::json& json);

private:
    QString m_id;
    QString m_name;
    QString m_country;
    QString m_type;
    QVector<QString> m_genres;
};

#endif // ARTIST_H
