#ifndef FAVORITESMANAGER_H
#define FAVORITESMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <nlohmann/json.hpp>
#include "models/Movie.h"
#include "models/Series.h"
#include "models/Album.h"

/**
 * @brief Gestionnaire des favoris
 * Permet de sauvegarder et charger les favoris depuis un fichier JSON
 */
class FavoritesManager : public QObject {
    Q_OBJECT

public:
    explicit FavoritesManager(const QString& filePath, QObject* parent = nullptr);
    ~FavoritesManager() = default;

    // Gestion des films
    void addMovieToFavorites(const Movie& movie);
    void removeMovieFromFavorites(int movieId);
    bool isMovieInFavorites(int movieId) const;
    QVector<Movie> getFavoriteMovies() const { return m_favoriteMovies; }

    // Gestion des séries
    void addSeriesToFavorites(const Series& series);
    void removeSeriesFromFavorites(int seriesId);
    bool isSeriesInFavorites(int seriesId) const;
    QVector<Series> getFavoriteSeries() const { return m_favoriteSeries; }

    // Gestion des albums
    void addAlbumToFavorites(const Album& album);
    void removeAlbumFromFavorites(const QString& albumId);
    bool isAlbumInFavorites(const QString& albumId) const;
    QVector<Album> getFavoriteAlbums() const { return m_favoriteAlbums; }

    // Sauvegarde et chargement
    bool save();
    bool load();

signals:
    void favoritesChanged();
    void error(const QString& errorMessage);

private:
    QString m_filePath;
    QVector<Movie> m_favoriteMovies;
    QVector<Series> m_favoriteSeries;
    QVector<Album> m_favoriteAlbums;

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& json);
};

#endif // FAVORITESMANAGER_H
