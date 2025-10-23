#ifndef OMDBAPICLIENT_H
#define OMDBAPICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include "models/Movie.h"
#include "models/Series.h"

/**
 * @brief Client API pour OMDb (Open Movie Database)
 */
class OMDbApiClient : public QObject {
    Q_OBJECT

public:
    explicit OMDbApiClient(const QString& apiKey, QObject* parent = nullptr);

    // Recherche
    void searchMovies(const QString& query);
    void searchSeries(const QString& query);
    
    // Détails
    void getMovieDetails(const QString& imdbId);
    void getSeriesDetails(const QString& imdbId);

signals:
    void moviesFound(const QVector<Movie>& movies);
    void seriesFound(const QVector<Series>& series);
    void movieDetailsReceived(const Movie& movie);
    void seriesDetailsReceived(const Series& series);
    void error(const QString& errorMessage);

private slots:
    void onSearchFinished();
    void onDetailsFinished();

private:
    Movie parseMovie(const QJsonObject& json);
    Series parseSeries(const QJsonObject& json);
    QVector<Movie> parseSearchResults(const QJsonObject& json);
    void parseSearchResultsByType(const QJsonObject& json, QVector<Movie>& movies, QVector<Series>& series);

    QNetworkAccessManager* m_networkManager;
    QString m_apiKey;
    QString m_baseUrl;
};

#endif // OMDBAPICLIENT_H
