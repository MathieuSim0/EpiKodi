#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>

/**
 * @brief Lecteur multimédia pour audio et vidéo
 * Utilise QtMultimedia pour lire des fichiers locaux ou URLs
 */
class MediaPlayer : public QObject {
    Q_OBJECT

public:
    enum MediaType {
        Video,
        Audio
    };

    explicit MediaPlayer(QObject* parent = nullptr);
    ~MediaPlayer() = default;

    // Contrôles de base
    void play();
    void pause();
    void stop();
    void setVolume(int volume); // 0-100
    void setPosition(qint64 position);

    // Chargement de média
    void loadMedia(const QString& url, MediaType type);
    
    // Getters
    QMediaPlayer* getMediaPlayer() { return m_mediaPlayer; }
    QMediaPlayer::PlaybackState getState() const;
    qint64 getDuration() const;
    qint64 getPosition() const;
    int getVolume() const;
    bool isMuted() const;

    // Mute
    void setMuted(bool muted);

signals:
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void error(const QString& errorMessage);

private slots:
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onErrorOccurred(QMediaPlayer::Error error, const QString& errorString);

private:
    QMediaPlayer* m_mediaPlayer;
    QAudioOutput* m_audioOutput;
    MediaType m_currentMediaType;
};

#endif // MEDIAPLAYER_H
