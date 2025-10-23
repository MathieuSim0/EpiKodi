#ifndef MOVIEWIDGET_H
#define MOVIEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include "api/TMDbApiClient.h"
#include "managers/FavoritesManager.h"
#include "models/Movie.h"

/**
 * @brief Widget de recherche et affichage de films avec grille moderne
 */
class MovieWidget : public QWidget {
    Q_OBJECT

public:
    explicit MovieWidget(TMDbApiClient* apiClient, FavoritesManager* favoritesManager,
                        QWidget* parent = nullptr);

signals:
    void playMedia(const QString& url, int mediaType);

private slots:
    void onSearchClicked();
    void onMoviesFound(const QVector<Movie>& movies);
    void onAddToFavorites();
    void displayMovieDetails(const Movie& movie);
    void displayMovieCredits(const QVector<QPair<QString, QString>>& cast);

private:
    void setupUI();

    TMDbApiClient* m_apiClient;
    FavoritesManager* m_favoritesManager;
    
    // UI Elements
    QLineEdit* m_searchEdit;
    QPushButton* m_searchButton;
    QGridLayout* m_resultsLayout;
    QLabel* m_posterLabel;
    QLabel* m_titleLabel;
    QLabel* m_overviewLabel;
    QLabel* m_ratingLabel;
    QLabel* m_genresLabel;
    QLabel* m_releaseDateLabel;
    QPushButton* m_favoriteButton;
    QPushButton* m_playTrailerButton;
    QWidget* m_castContainer;
    QGridLayout* m_castLayout;

    // Data
    QVector<Movie> m_currentMovies;
    Movie m_selectedMovie;
};

#endif // MOVIEWIDGET_H
