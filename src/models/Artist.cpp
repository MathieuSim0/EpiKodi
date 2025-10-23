#include "models/Artist.h"

Artist::Artist(const QString& id, const QString& name, const QString& country)
    : m_id(id)
    , m_name(name)
    , m_country(country)
{
}

nlohmann::json Artist::toJson() const {
    nlohmann::json genresJson = nlohmann::json::array();
    for (const auto& genre : m_genres) {
        genresJson.push_back(genre.toStdString());
    }

    return nlohmann::json{
        {"id", m_id.toStdString()},
        {"name", m_name.toStdString()},
        {"country", m_country.toStdString()},
        {"type", m_type.toStdString()},
        {"genres", genresJson}
    };
}

Artist Artist::fromJson(const nlohmann::json& json) {
    Artist artist;
    artist.m_id = QString::fromStdString(json.value("id", ""));
    artist.m_name = QString::fromStdString(json.value("name", ""));
    artist.m_country = QString::fromStdString(json.value("country", ""));
    artist.m_type = QString::fromStdString(json.value("type", ""));
    
    if (json.contains("genres") && json["genres"].is_array()) {
        for (const auto& genre : json["genres"]) {
            artist.m_genres.append(QString::fromStdString(genre.get<std::string>()));
        }
    }
    
    return artist;
}
