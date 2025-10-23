#include "models/Series.h"

Series::Series(int id, const QString& name, const QString& overview, 
               const QString& posterPath, const QDate& firstAirDate, double rating)
    : m_id(id)
    , m_name(name)
    , m_overview(overview)
    , m_posterPath(posterPath)
    , m_firstAirDate(firstAirDate)
    , m_rating(rating)
{
}

nlohmann::json Series::toJson() const {
    return nlohmann::json{
        {"id", m_id},
        {"name", m_name.toStdString()},
        {"overview", m_overview.toStdString()},
        {"poster_path", m_posterPath.toStdString()},
        {"backdrop_path", m_backdropPath.toStdString()},
        {"first_air_date", m_firstAirDate.toString(Qt::ISODate).toStdString()},
        {"rating", m_rating},
        {"number_of_seasons", m_numberOfSeasons},
        {"number_of_episodes", m_numberOfEpisodes}
    };
}

Series Series::fromJson(const nlohmann::json& json) {
    Series series;
    series.m_id = json.value("id", 0);
    series.m_name = QString::fromStdString(json.value("name", ""));
    series.m_overview = QString::fromStdString(json.value("overview", ""));
    series.m_posterPath = QString::fromStdString(json.value("poster_path", ""));
    series.m_backdropPath = QString::fromStdString(json.value("backdrop_path", ""));
    
    QString dateStr = QString::fromStdString(json.value("first_air_date", ""));
    series.m_firstAirDate = QDate::fromString(dateStr, Qt::ISODate);
    
    series.m_rating = json.value("rating", 0.0);
    series.m_numberOfSeasons = json.value("number_of_seasons", 0);
    series.m_numberOfEpisodes = json.value("number_of_episodes", 0);
    
    return series;
}
