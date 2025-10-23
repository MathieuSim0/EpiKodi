#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer>
#include "player/MediaPlayer.h"

class PlayerWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlayerWidget(MediaPlayer* mediaPlayer, QWidget* parent = nullptr);

public slots:
    void loadAndPlay(const QString& url, int mediaType);

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onVolumeChanged(int value);
    void onSeekPositionChanged(int value);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);

private:
    void setupUI();
    QString formatTime(qint64 milliseconds);

    MediaPlayer* m_mediaPlayer;
    
    QPushButton* m_playPauseButton;
    QPushButton* m_stopButton;
    QSlider* m_positionSlider;
    QSlider* m_volumeSlider;
    QLabel* m_timeLabel;
    QLabel* m_statusLabel;
    
    bool m_isUserSeeking;
};

#endif // PLAYERWIDGET_H
