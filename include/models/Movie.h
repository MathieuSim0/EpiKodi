#ifndef MOVIE_H
#define MOVIE_H

#include <QString>
#include <QDate>
#include <QVector>
#include <nlohmann/json.hpp>

/**
 * @brief Modèle représentant un film
 */
class Movie {
public:
    Movie() = default;
    Movie(int id, const QString& title, const QString& overview, 
          const QString& posterPath, const QDate& releaseDate, double rating);

    // Getters
    int getId() const { return m_id; }
    QString getImdbId() const { return m_imdbId; }
    QString getTitle() const { return m_title; }
    QString getOverview() const { return m_overview; }
    QString getPosterPath() const { return m_posterPath; }
    QString getBackdropPath() const { return m_backdropPath; }
    QDate getReleaseDate() const { return m_releaseDate; }
    double getRating() const { return m_rating; }
    int getRuntime() const { return m_runtime; }
    QVector<QString> getGenres() const { return m_genres; }
    QString getTrailerUrl() const { return m_trailerUrl; }

    // Setters
    void setId(int id) { m_id = id; }
    void setTitle(const QString& title) { m_title = title; }
    void setOverview(const QString& overview) { m_overview = overview; }
    void setBackdropPath(const QString& path) { m_backdropPath = path; }
    void setGenres(const QVector<QString>& genres) { m_genres = genres; }
    void setTrailerUrl(const QString& url) { m_trailerUrl = url; }
    void setPosterPath(const QString& path) { m_posterPath = path; }
    void setReleaseDate(const QDate& date) { m_releaseDate = date; }
    void setRating(double rating) { m_rating = rating; }
    void setRuntime(int runtime) { m_runtime = runtime; }
    void setImdbId(const QString& imdbId) { m_imdbId = imdbId; }

    // Conversion JSON
    nlohmann::json toJson() const;
    static Movie fromJson(const nlohmann::json& json);

private:
    int m_id{0};
    int m_runtime{0};
    QString m_title;
    QString m_overview;
    QString m_posterPath;
    QString m_backdropPath;
    QString m_imdbId;
    QDate m_releaseDate;
    double m_rating{0.0};
    QVector<QString> m_genres;
    QString m_trailerUrl;
};

#endif // MOVIE_H
