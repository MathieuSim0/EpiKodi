#include "models/Movie.h"

Movie::Movie(int id, const QString& title, const QString& overview, 
             const QString& posterPath, const QDate& releaseDate, double rating)
    : m_id(id)
    , m_title(title)
    , m_overview(overview)
    , m_posterPath(posterPath)
    , m_releaseDate(releaseDate)
    , m_rating(rating)
{
}

nlohmann::json Movie::toJson() const {
    return nlohmann::json{
        {"id", m_id},
        {"title", m_title.toStdString()},
        {"overview", m_overview.toStdString()},
        {"poster_path", m_posterPath.toStdString()},
        {"backdrop_path", m_backdropPath.toStdString()},
        {"release_date", m_releaseDate.toString(Qt::ISODate).toStdString()},
        {"rating", m_rating},
        {"runtime", m_runtime},
        {"imdb_id", m_imdbId.toStdString()},
        {"trailer_url", m_trailerUrl.toStdString()}
    };
}

Movie Movie::fromJson(const nlohmann::json& json) {
    Movie movie;
    movie.m_id = json.value("id", 0);
    movie.m_title = QString::fromStdString(json.value("title", ""));
    movie.m_overview = QString::fromStdString(json.value("overview", ""));
    movie.m_posterPath = QString::fromStdString(json.value("poster_path", ""));
    movie.m_backdropPath = QString::fromStdString(json.value("backdrop_path", ""));
    
    QString dateStr = QString::fromStdString(json.value("release_date", ""));
    movie.m_releaseDate = QDate::fromString(dateStr, Qt::ISODate);
    
    movie.m_rating = json.value("rating", 0.0);
    movie.m_trailerUrl = QString::fromStdString(json.value("trailer_url", ""));
    movie.m_runtime = json.value("runtime", 0);
    movie.m_imdbId = QString::fromStdString(json.value("imdb_id", ""));
    
    return movie;
}
