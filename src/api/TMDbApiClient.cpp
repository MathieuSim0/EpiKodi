#include "api/TMDbApiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>

TMDbApiClient::TMDbApiClient(const QString& apiKey, QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_apiKey(apiKey)
    , m_baseUrl("https://api.themoviedb.org/3")
    , m_imageBaseUrl("https://image.tmdb.org/t/p/w500")
{
}

void TMDbApiClient::searchMovies(const QString& query) {
    QUrl url(m_baseUrl + "/search/movie");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    urlQuery.addQueryItem("query", query);
    urlQuery.addQueryItem("language", "fr-FR");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onSearchMoviesFinished);
}

void TMDbApiClient::searchSeries(const QString& query) {
    QUrl url(m_baseUrl + "/search/tv");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    urlQuery.addQueryItem("query", query);
    urlQuery.addQueryItem("language", "fr-FR");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onSearchSeriesFinished);
}

void TMDbApiClient::getPopularMovies() {
    QUrl url(m_baseUrl + "/movie/popular");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    urlQuery.addQueryItem("language", "fr-FR");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onPopularMoviesFinished);
}

void TMDbApiClient::getMovieDetails(int movieId) {
    QUrl url(m_baseUrl + QString("/movie/%1").arg(movieId));
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    urlQuery.addQueryItem("language", "fr-FR");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onMovieDetailsFinished);
}

void TMDbApiClient::getSeriesDetails(int seriesId) {
    QUrl url(m_baseUrl + QString("/tv/%1").arg(seriesId));
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    urlQuery.addQueryItem("language", "fr-FR");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onSeriesDetailsFinished);
}

void TMDbApiClient::getMovieTrailers(int movieId) {
    QUrl url(m_baseUrl + QString("/movie/%1/videos").arg(movieId));
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onTrailersFinished);
}

void TMDbApiClient::getMovieCredits(int movieId) {
    QUrl url(m_baseUrl + QString("/movie/%1/credits").arg(movieId));
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onMovieCreditsFinished);
}

void TMDbApiClient::getSeriesCredits(int seriesId) {
    QUrl url(m_baseUrl + QString("/tv/%1/credits").arg(seriesId));
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("api_key", m_apiKey);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &TMDbApiClient::onSeriesCreditsFinished);
}

void TMDbApiClient::onSearchMoviesFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QVector<Movie> movies = parseMoviesResponse(data);
        emit moviesFound(movies);
    } else {
        emit error("Erreur lors de la recherche de films: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onSearchSeriesFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QVector<Series> series = parseSeriesResponse(data);
        emit seriesFound(series);
    } else {
        emit error("Erreur lors de la recherche de séries: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onMovieDetailsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        Movie movie = parseMovieDetails(data);
        emit movieDetailsReceived(movie);
    } else {
        emit error("Erreur lors de la récupération des détails: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onSeriesDetailsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        Series series = parseSeriesDetails(data);
        emit seriesDetailsReceived(series);
    } else {
        emit error("Erreur lors de la récupération des détails: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onTrailersFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QString trailerUrl = parseTrailerUrl(data);
        emit trailerUrlReceived(trailerUrl);
    } else {
        emit error("Erreur lors de la récupération des trailers: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onPopularMoviesFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QVector<Movie> movies = parseMoviesResponse(data);
        emit popularMoviesFound(movies);
    } else {
        emit error("Erreur lors de la récupération des films populaires: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onMovieCreditsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        QJsonArray cast = obj["cast"].toArray();

        QVector<QPair<QString, QString>> actors;
        
        // Prendre les 10 premiers acteurs
        for (int i = 0; i < qMin(10, cast.size()); ++i) {
            QJsonObject actorObj = cast[i].toObject();
            QString name = actorObj["name"].toString();
            QString profilePath = actorObj["profile_path"].toString();
            actors.append(qMakePair(name, profilePath));
        }
        
        emit movieCreditsReceived(actors);
    } else {
        emit error("Erreur lors de la récupération des acteurs: " + reply->errorString());
    }

    reply->deleteLater();
}

void TMDbApiClient::onSeriesCreditsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        QJsonArray cast = obj["cast"].toArray();

        QVector<QPair<QString, QString>> actors;
        
        // Prendre les 10 premiers acteurs
        for (int i = 0; i < qMin(10, cast.size()); ++i) {
            QJsonObject actorObj = cast[i].toObject();
            QString name = actorObj["name"].toString();
            QString profilePath = actorObj["profile_path"].toString();
            actors.append(qMakePair(name, profilePath));
        }
        
        emit seriesCreditsReceived(actors);
    } else {
        emit error("Erreur lors de la récupération des acteurs: " + reply->errorString());
    }

    reply->deleteLater();
}

QVector<Movie> TMDbApiClient::parseMoviesResponse(const QByteArray& data) {
    QVector<Movie> movies;
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray results = obj["results"].toArray();

    for (const QJsonValue& value : results) {
        QJsonObject movieObj = value.toObject();
        
        int id = movieObj["id"].toInt();
        QString title = movieObj["title"].toString();
        QString overview = movieObj["overview"].toString();
        QString posterPath = movieObj["poster_path"].toString();
        QString releaseDateStr = movieObj["release_date"].toString();
        double rating = movieObj["vote_average"].toDouble();

        QDate releaseDate = QDate::fromString(releaseDateStr, Qt::ISODate);
        
        Movie movie(id, title, overview, posterPath, releaseDate, rating);
        movie.setBackdropPath(movieObj["backdrop_path"].toString());
        
        movies.append(movie);
    }

    return movies;
}

QVector<Series> TMDbApiClient::parseSeriesResponse(const QByteArray& data) {
    QVector<Series> seriesList;
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray results = obj["results"].toArray();

    for (const QJsonValue& value : results) {
        QJsonObject seriesObj = value.toObject();
        
        int id = seriesObj["id"].toInt();
        QString name = seriesObj["name"].toString();
        QString overview = seriesObj["overview"].toString();
        QString posterPath = seriesObj["poster_path"].toString();
        QString firstAirDateStr = seriesObj["first_air_date"].toString();
        double rating = seriesObj["vote_average"].toDouble();

        QDate firstAirDate = QDate::fromString(firstAirDateStr, Qt::ISODate);
        
        Series series(id, name, overview, posterPath, firstAirDate, rating);
        series.setBackdropPath(seriesObj["backdrop_path"].toString());
        
        seriesList.append(series);
    }

    return seriesList;
}

Movie TMDbApiClient::parseMovieDetails(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    
    int id = obj["id"].toInt();
    QString title = obj["title"].toString();
    QString overview = obj["overview"].toString();
    QString posterPath = obj["poster_path"].toString();
    QString releaseDateStr = obj["release_date"].toString();
    double rating = obj["vote_average"].toDouble();

    QDate releaseDate = QDate::fromString(releaseDateStr, Qt::ISODate);
    
    Movie movie(id, title, overview, posterPath, releaseDate, rating);
    movie.setBackdropPath(obj["backdrop_path"].toString());
    
    // Parse genres
    QVector<QString> genres;
    QJsonArray genresArray = obj["genres"].toArray();
    for (const QJsonValue& genreValue : genresArray) {
        genres.append(genreValue.toObject()["name"].toString());
    }
    movie.setGenres(genres);
    
    return movie;
}

Series TMDbApiClient::parseSeriesDetails(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    
    int id = obj["id"].toInt();
    QString name = obj["name"].toString();
    QString overview = obj["overview"].toString();
    QString posterPath = obj["poster_path"].toString();
    QString firstAirDateStr = obj["first_air_date"].toString();
    double rating = obj["vote_average"].toDouble();

    QDate firstAirDate = QDate::fromString(firstAirDateStr, Qt::ISODate);
    
    Series series(id, name, overview, posterPath, firstAirDate, rating);
    series.setBackdropPath(obj["backdrop_path"].toString());
    series.setNumberOfSeasons(obj["number_of_seasons"].toInt());
    series.setNumberOfEpisodes(obj["number_of_episodes"].toInt());
    
    // Parse genres
    QVector<QString> genres;
    QJsonArray genresArray = obj["genres"].toArray();
    for (const QJsonValue& genreValue : genresArray) {
        genres.append(genreValue.toObject()["name"].toString());
    }
    series.setGenres(genres);
    
    return series;
}

QString TMDbApiClient::parseTrailerUrl(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray results = obj["results"].toArray();

    // Chercher le premier trailer YouTube
    for (const QJsonValue& value : results) {
        QJsonObject videoObj = value.toObject();
        if (videoObj["site"].toString() == "YouTube" && 
            videoObj["type"].toString() == "Trailer") {
            QString key = videoObj["key"].toString();
            return "https://www.youtube.com/watch?v=" + key;
        }
    }

    return QString();
}
