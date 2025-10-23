#ifndef SERIES_H
#define SERIES_H

#include <QString>
#include <QDate>
#include <QVector>
#include <nlohmann/json.hpp>

/**
 * @brief Modèle représentant une série TV
 */
class Series {
public:
    Series() = default;
    Series(int id, const QString& name, const QString& overview, 
           const QString& posterPath, const QDate& firstAirDate, double rating);

    // Getters
    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getOverview() const { return m_overview; }
    QString getPosterPath() const { return m_posterPath; }
    QString getBackdropPath() const { return m_backdropPath; }
    QDate getFirstAirDate() const { return m_firstAirDate; }
    double getRating() const { return m_rating; }
    int getNumberOfSeasons() const { return m_numberOfSeasons; }
    int getNumberOfEpisodes() const { return m_numberOfEpisodes; }
    QVector<QString> getGenres() const { return m_genres; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setOverview(const QString& overview) { m_overview = overview; }
    void setPosterPath(const QString& path) { m_posterPath = path; }
    void setBackdropPath(const QString& path) { m_backdropPath = path; }
    void setFirstAirDate(const QDate& date) { m_firstAirDate = date; }
    void setRating(double rating) { m_rating = rating; }
    void setNumberOfSeasons(int seasons) { m_numberOfSeasons = seasons; }
    void setNumberOfEpisodes(int episodes) { m_numberOfEpisodes = episodes; }
    void setGenres(const QVector<QString>& genres) { m_genres = genres; }

    // Conversion JSON
    nlohmann::json toJson() const;
    static Series fromJson(const nlohmann::json& json);

private:
    int m_id{0};
    QString m_name;
    QString m_overview;
    QString m_posterPath;
    QString m_backdropPath;
    QDate m_firstAirDate;
    double m_rating{0.0};
    int m_numberOfSeasons{0};
    int m_numberOfEpisodes{0};
    QVector<QString> m_genres;
};

#endif // SERIES_H
