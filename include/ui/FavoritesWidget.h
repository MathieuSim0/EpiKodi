#ifndef FAVORITESWIDGET_H
#define FAVORITESWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QLabel>
#include "managers/FavoritesManager.h"

class FavoritesWidget : public QWidget {
    Q_OBJECT

public:
    explicit FavoritesWidget(FavoritesManager* favoritesManager, QWidget* parent = nullptr);

private slots:
    void onFavoritesChanged();
    void onMovieSelected(QListWidgetItem* item);
    void onSeriesSelected(QListWidgetItem* item);
    void onAlbumSelected(QListWidgetItem* item);

private:
    void setupUI();
    void refreshFavorites();

    FavoritesManager* m_favoritesManager;
    
    QTabWidget* m_tabWidget;
    QGridLayout* m_moviesLayout;
    QGridLayout* m_seriesLayout;
    QListWidget* m_albumsListWidget;
};

#endif // FAVORITESWIDGET_H
