#ifndef SERIESWIDGET_H
#define SERIESWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include "api/TMDbApiClient.h"
#include "managers/FavoritesManager.h"
#include "models/Series.h"

class SeriesWidget : public QWidget {
    Q_OBJECT

public:
    explicit SeriesWidget(TMDbApiClient* apiClient, FavoritesManager* favoritesManager,
                         QWidget* parent = nullptr);

private slots:
    void onSearchClicked();
    void onSeriesFound(const QVector<Series>& series);
    void onAddToFavorites();
    void displaySeriesDetails(const Series& series);
    void onSeriesCreditsReceived(const QVector<QPair<QString, QString>>& cast);

private:
    void setupUI();

    TMDbApiClient* m_apiClient;
    FavoritesManager* m_favoritesManager;
    
    QLineEdit* m_searchEdit;
    QPushButton* m_searchButton;
    QGridLayout* m_resultsLayout;
    
    // Panneau de détails amélioré
    QScrollArea* m_detailsScrollArea;
    QWidget* m_detailsContent;
    QLabel* m_posterLabel;
    QLabel* m_titleLabel;
    QLabel* m_overviewLabel;
    QLabel* m_ratingLabel;
    QLabel* m_seasonsLabel;
    QLabel* m_genresLabel;
    QLabel* m_castTitleLabel;
    QGridLayout* m_castLayout;
    QPushButton* m_favoriteButton;
    
    QNetworkAccessManager* m_networkManager;

    QVector<Series> m_currentSeries;
    Series m_selectedSeries;
};

#endif // SERIESWIDGET_H
