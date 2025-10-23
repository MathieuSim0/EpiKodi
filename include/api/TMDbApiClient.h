#ifndef TMDBAPICLIENT_H
#define TMDBAPICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QVector>
#include "models/Movie.h"
#include "models/Series.h"

/**
 * @brief Client API pour TheMovieDB
 * Permet de rechercher des films et séries
 */
class TMDbApiClient : public QObject {
    Q_OBJECT

public:
    explicit TMDbApiClient(const QString& apiKey, QObject* parent = nullptr);
    ~TMDbApiClient() = default;

    // Recherche de films
    void searchMovies(const QString& query);
    
    // Recherche de séries
    void searchSeries(const QString& query);
    // Récupérer les films populaires
    void getPopularMovies();
    
    // Détails d'un film
    void getMovieDetails(int movieId);
    
    // Récupérer les acteurs d'un film
    void getMovieCredits(int movieId);
    
    // Détails d'une série
    void getSeriesDetails(int seriesId);
    
    // Récupérer les acteurs d'une série
    void getSeriesCredits(int seriesId);
    
    // Récupérer les trailers
    void getMovieTrailers(int movieId);

signals:
    // Signaux pour les résultats
    void moviesFound(const QVector<Movie>& movies);
    void seriesFound(const QVector<Series>& series);
    void movieDetailsReceived(const Movie& movie);
    void seriesDetailsReceived(const Series& series);
    void popularMoviesFound(const QVector<Movie>& movies);
    void trailerUrlReceived(const QString& trailerUrl);
    void movieCreditsReceived(const QVector<QPair<QString, QString>>& cast); // (name, profile_path)
    void seriesCreditsReceived(const QVector<QPair<QString, QString>>& cast); // (name, profile_path)
    
    // Signaux d'erreur
    void error(const QString& errorMessage);

private slots:
    void onSearchMoviesFinished();
    void onSearchSeriesFinished();
    void onPopularMoviesFinished();
    void onMovieDetailsFinished();
    void onSeriesDetailsFinished();
    void onTrailersFinished();
    void onMovieCreditsFinished();
    void onSeriesCreditsFinished();

private:
    QNetworkAccessManager* m_networkManager;
    QString m_apiKey;
    QString m_baseUrl;
    QString m_imageBaseUrl;

    QVector<Movie> parseMoviesResponse(const QByteArray& data);
    QVector<Series> parseSeriesResponse(const QByteArray& data);
    Movie parseMovieDetails(const QByteArray& data);
    Series parseSeriesDetails(const QByteArray& data);
    QString parseTrailerUrl(const QByteArray& data);
};

#endif // TMDBAPICLIENT_H
