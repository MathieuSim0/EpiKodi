#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include "api/TMDbApiClient.h"
#include "api/MusicBrainzApiClient.h"
#include "managers/FavoritesManager.h"
#include "player/MediaPlayer.h"

// Forward declarations
class MovieWidget;
class SeriesWidget;
class MusicWidget;
class FavoritesWidget;
class PlayerWidget;

/**
 * @brief Fenêtre principale de l'application EpiKodi
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void loadConfiguration();
    void createMenuBar();
    void createStatusBar();

    // Widgets principaux
    QTabWidget* m_tabWidget;
    MovieWidget* m_movieWidget;
    SeriesWidget* m_seriesWidget;
    MusicWidget* m_musicWidget;
    FavoritesWidget* m_favoritesWidget;
    PlayerWidget* m_playerWidget;

    // Services
    TMDbApiClient* m_tmdbClient;
    MusicBrainzApiClient* m_musicBrainzClient;
    FavoritesManager* m_favoritesManager;
    MediaPlayer* m_mediaPlayer;

    // Configuration
    QString m_tmdbApiKey;
    QString m_musicBrainzUserAgent;
    QString m_favoritesFilePath;
};

#endif // MAINWINDOW_H
