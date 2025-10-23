#include "managers/FavoritesManager.h"
#include <QFile>
#include <QJsonDocument>
#include <fstream>

FavoritesManager::FavoritesManager(const QString& filePath, QObject* parent)
    : QObject(parent)
    , m_filePath(filePath)
{
    load();
}

void FavoritesManager::addMovieToFavorites(const Movie& movie) {
    if (!isMovieInFavorites(movie.getId())) {
        m_favoriteMovies.append(movie);
        save();
        emit favoritesChanged();
    }
}

void FavoritesManager::removeMovieFromFavorites(int movieId) {
    for (int i = 0; i < m_favoriteMovies.size(); ++i) {
        if (m_favoriteMovies[i].getId() == movieId) {
            m_favoriteMovies.remove(i);
            save();
            emit favoritesChanged();
            break;
        }
    }
}

bool FavoritesManager::isMovieInFavorites(int movieId) const {
    for (const auto& movie : m_favoriteMovies) {
        if (movie.getId() == movieId) {
            return true;
        }
    }
    return false;
}

void FavoritesManager::addSeriesToFavorites(const Series& series) {
    if (!isSeriesInFavorites(series.getId())) {
        m_favoriteSeries.append(series);
        save();
        emit favoritesChanged();
    }
}

void FavoritesManager::removeSeriesFromFavorites(int seriesId) {
    for (int i = 0; i < m_favoriteSeries.size(); ++i) {
        if (m_favoriteSeries[i].getId() == seriesId) {
            m_favoriteSeries.remove(i);
            save();
            emit favoritesChanged();
            break;
        }
    }
}

bool FavoritesManager::isSeriesInFavorites(int seriesId) const {
    for (const auto& series : m_favoriteSeries) {
        if (series.getId() == seriesId) {
            return true;
        }
    }
    return false;
}

void FavoritesManager::addAlbumToFavorites(const Album& album) {
    if (!isAlbumInFavorites(album.getId())) {
        m_favoriteAlbums.append(album);
        save();
        emit favoritesChanged();
    }
}

void FavoritesManager::removeAlbumFromFavorites(const QString& albumId) {
    for (int i = 0; i < m_favoriteAlbums.size(); ++i) {
        if (m_favoriteAlbums[i].getId() == albumId) {
            m_favoriteAlbums.remove(i);
            save();
            emit favoritesChanged();
            break;
        }
    }
}

bool FavoritesManager::isAlbumInFavorites(const QString& albumId) const {
    for (const auto& album : m_favoriteAlbums) {
        if (album.getId() == albumId) {
            return true;
        }
    }
    return false;
}

bool FavoritesManager::save() {
    try {
        nlohmann::json json = toJson();
        
        std::ofstream file(m_filePath.toStdString());
        if (!file.is_open()) {
            emit error("Impossible d'ouvrir le fichier de favoris en écriture");
            return false;
        }
        
        file << json.dump(4);
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        emit error(QString("Erreur lors de la sauvegarde des favoris: %1").arg(e.what()));
        return false;
    }
}

bool FavoritesManager::load() {
    try {
        std::ifstream file(m_filePath.toStdString());
        if (!file.is_open()) {
            // Fichier n'existe pas encore, ce n'est pas une erreur
            return true;
        }
        
        nlohmann::json json;
        file >> json;
        file.close();
        
        fromJson(json);
        
        return true;
    } catch (const std::exception& e) {
        emit error(QString("Erreur lors du chargement des favoris: %1").arg(e.what()));
        return false;
    }
}

nlohmann::json FavoritesManager::toJson() const {
    nlohmann::json moviesJson = nlohmann::json::array();
    for (const auto& movie : m_favoriteMovies) {
        moviesJson.push_back(movie.toJson());
    }
    
    nlohmann::json seriesJson = nlohmann::json::array();
    for (const auto& series : m_favoriteSeries) {
        seriesJson.push_back(series.toJson());
    }
    
    nlohmann::json albumsJson = nlohmann::json::array();
    for (const auto& album : m_favoriteAlbums) {
        albumsJson.push_back(album.toJson());
    }
    
    return nlohmann::json{
        {"movies", moviesJson},
        {"series", seriesJson},
        {"albums", albumsJson}
    };
}

void FavoritesManager::fromJson(const nlohmann::json& json) {
    m_favoriteMovies.clear();
    m_favoriteSeries.clear();
    m_favoriteAlbums.clear();
    
    if (json.contains("movies") && json["movies"].is_array()) {
        for (const auto& movieJson : json["movies"]) {
            m_favoriteMovies.append(Movie::fromJson(movieJson));
        }
    }
    
    if (json.contains("series") && json["series"].is_array()) {
        for (const auto& seriesJson : json["series"]) {
            m_favoriteSeries.append(Series::fromJson(seriesJson));
        }
    }
    
    if (json.contains("albums") && json["albums"].is_array()) {
        for (const auto& albumJson : json["albums"]) {
            m_favoriteAlbums.append(Album::fromJson(albumJson));
        }
    }
}
