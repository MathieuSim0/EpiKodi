#include "api/OMDbApiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>

OMDbApiClient::OMDbApiClient(const QString& apiKey, QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_apiKey(apiKey)
    , m_baseUrl("http://www.omdbapi.com/")
{
}

void OMDbApiClient::searchMovies(const QString& query) {
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("apikey", m_apiKey);
    urlQuery.addQueryItem("s", query);
    // Ne pas filtrer par type pour avoir films ET séries
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &OMDbApiClient::onSearchFinished);
}

void OMDbApiClient::searchSeries(const QString& query) {
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("apikey", m_apiKey);
    urlQuery.addQueryItem("s", query);
    urlQuery.addQueryItem("type", "series");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &OMDbApiClient::onSearchFinished);
}

void OMDbApiClient::getMovieDetails(const QString& imdbId) {
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("apikey", m_apiKey);
    urlQuery.addQueryItem("i", imdbId);
    urlQuery.addQueryItem("plot", "full");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &OMDbApiClient::onDetailsFinished);
}

void OMDbApiClient::getSeriesDetails(const QString& imdbId) {
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("apikey", m_apiKey);
    urlQuery.addQueryItem("i", imdbId);
    urlQuery.addQueryItem("plot", "full");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &OMDbApiClient::onDetailsFinished);
}

void OMDbApiClient::onSearchFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if (obj["Response"].toString() == "True") {
            // Séparer films et séries selon le champ Type
            QVector<Movie> movies;
            QVector<Series> series;
            parseSearchResultsByType(obj, movies, series);
            
            // Émettre les signaux appropriés
            if (!movies.isEmpty()) {
                emit moviesFound(movies);
            }
            if (!series.isEmpty()) {
                emit seriesFound(series);
            }
            if (movies.isEmpty() && series.isEmpty()) {
                emit error("Aucun résultat trouvé");
            }
        } else {
            emit error("Aucun résultat: " + obj["Error"].toString());
        }
    } else {
        emit error("Erreur réseau: " + reply->errorString());
    }

    reply->deleteLater();
}

void OMDbApiClient::onDetailsFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if (obj["Response"].toString() == "True") {
            if (obj["Type"].toString() == "movie") {
                Movie movie = parseMovie(obj);
                emit movieDetailsReceived(movie);
            } else if (obj["Type"].toString() == "series") {
                Series series = parseSeries(obj);
                emit seriesDetailsReceived(series);
            }
        } else {
            emit error("Erreur: " + obj["Error"].toString());
        }
    } else {
        emit error("Erreur réseau: " + reply->errorString());
    }

    reply->deleteLater();
}

void OMDbApiClient::parseSearchResultsByType(const QJsonObject& json, QVector<Movie>& movies, QVector<Series>& series) {
    QJsonArray results = json["Search"].toArray();

    for (const QJsonValue& value : results) {
        QJsonObject itemObj = value.toObject();
        QString type = itemObj["Type"].toString();
        
        if (type == "movie") {
            // C'est un film
            Movie movie;
            QString imdbId = itemObj["imdbID"].toString();
            movie.setImdbId(imdbId);
            movie.setId(imdbId.mid(2).toInt());  // Convertir tt1234567 -> 1234567
            movie.setTitle(itemObj["Title"].toString());
            movie.setPosterPath(itemObj["Poster"].toString());
            
            // Parser l'année
            QString year = itemObj["Year"].toString();
            if (!year.isEmpty()) {
                QDate releaseDate(year.left(4).toInt(), 1, 1);
                movie.setReleaseDate(releaseDate);
            }
            
            // Note: Les résultats de recherche n'incluent pas Plot, il faudra le charger avec getMovieDetails
            movie.setOverview("");  // Sera chargé plus tard
            
            movies.append(movie);
            
        } else if (type == "series") {
            // C'est une série
            Series seriesItem;
            QString imdbId = itemObj["imdbID"].toString();
            seriesItem.setId(imdbId.mid(2).toInt());  // Convertir tt1234567 -> 1234567
            seriesItem.setName(itemObj["Title"].toString());
            seriesItem.setPosterPath(itemObj["Poster"].toString());
            
            // Parser l'année
            QString year = itemObj["Year"].toString();
            if (!year.isEmpty()) {
                QDate firstAirDate(year.left(4).toInt(), 1, 1);
                seriesItem.setFirstAirDate(firstAirDate);
            }
            
            seriesItem.setOverview("");  // Sera chargé plus tard
            
            series.append(seriesItem);
        }
    }
}

QVector<Movie> OMDbApiClient::parseSearchResults(const QJsonObject& json) {
    QVector<Movie> movies;
    QJsonArray results = json["Search"].toArray();

    for (const QJsonValue& value : results) {
        QJsonObject movieObj = value.toObject();
        
        Movie movie;
        movie.setId(movieObj["imdbID"].toString().toInt()); // Note: OMDb uses string IDs
        movie.setTitle(movieObj["Title"].toString());
        movie.setPosterPath(movieObj["Poster"].toString());
        
        // Parser l'année
        QString year = movieObj["Year"].toString();
        if (!year.isEmpty()) {
            QDate releaseDate(year.left(4).toInt(), 1, 1);
            movie.setReleaseDate(releaseDate);
        }
        
        movies.append(movie);
    }

    return movies;
}

Movie OMDbApiClient::parseMovie(const QJsonObject& json) {
    Movie movie;
    
    // Stocker l'IMDb ID comme string puis convertir
    QString imdbId = json["imdbID"].toString();
    movie.setImdbId(imdbId);
    movie.setId(imdbId.remove("tt").toInt());  // Convertir tt1234567 -> 1234567
    
    movie.setTitle(json["Title"].toString());
    movie.setOverview(json["Plot"].toString());  // Utiliser Plot pour la description
    movie.setPosterPath(json["Poster"].toString());
    
    // Parser l'année de sortie
    QString released = json["Released"].toString();
    QDate releaseDate = QDate::fromString(released, "dd MMM yyyy");
    if (!releaseDate.isValid()) {
        QString year = json["Year"].toString();
        if (!year.isEmpty()) {
            releaseDate = QDate(year.left(4).toInt(), 1, 1);
        }
    }
    movie.setReleaseDate(releaseDate);
    
    // Parser la note IMDb
    QString rating = json["imdbRating"].toString();
    movie.setRating(rating.toDouble());
    
    // Parser la durée
    QString runtime = json["Runtime"].toString();
    runtime.replace(" min", "");
    movie.setRuntime(runtime.toInt());
    
    // Parser les genres
    QString genresStr = json["Genre"].toString();
    QStringList genres = genresStr.split(", ");
    movie.setGenres(genres.toVector());
    
    return movie;
}

Series OMDbApiClient::parseSeries(const QJsonObject& json) {
    Series series;
    
    series.setId(json["imdbID"].toString().toInt());
    series.setName(json["Title"].toString());
    series.setOverview(json["Plot"].toString());
    series.setPosterPath(json["Poster"].toString());
    
    // Parser la première diffusion
    QString released = json["Released"].toString();
    QDate firstAirDate = QDate::fromString(released, "dd MMM yyyy");
    if (!firstAirDate.isValid()) {
        QString year = json["Year"].toString();
        if (!year.isEmpty()) {
            firstAirDate = QDate(year.left(4).toInt(), 1, 1);
        }
    }
    series.setFirstAirDate(firstAirDate);
    
    // Parser la note
    QString rating = json["imdbRating"].toString();
    series.setRating(rating.toDouble());
    
    // Parser les saisons
    QString totalSeasons = json["totalSeasons"].toString();
    series.setNumberOfSeasons(totalSeasons.toInt());
    
    // Parser les genres
    QString genresStr = json["Genre"].toString();
    QStringList genres = genresStr.split(", ");
    series.setGenres(genres.toVector());
    
    return series;
}
