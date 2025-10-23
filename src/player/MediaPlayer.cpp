#include "player/MediaPlayer.h"
#include <QUrl>

MediaPlayer::MediaPlayer(QObject* parent)
    : QObject(parent)
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(nullptr)
    , m_currentMediaType(MediaType::Video)
{
    // Créer l'audio output seulement si nécessaire
    try {
        m_audioOutput = new QAudioOutput(this);
        m_mediaPlayer->setAudioOutput(m_audioOutput);
        // Volume par défaut à 50%
        setVolume(50);
    } catch (...) {
        // Si échec, continuer sans audio
        qWarning("Impossible d'initialiser l'audio output");
    }
    
    // Connexion des signaux
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged,
            this, &MediaPlayer::onPlaybackStateChanged);
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &MediaPlayer::onMediaStatusChanged);
    connect(m_mediaPlayer, &QMediaPlayer::errorOccurred,
            this, &MediaPlayer::onErrorOccurred);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
            this, &MediaPlayer::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged,
            this, &MediaPlayer::durationChanged);
}

void MediaPlayer::play() {
    m_mediaPlayer->play();
}

void MediaPlayer::pause() {
    m_mediaPlayer->pause();
}

void MediaPlayer::stop() {
    m_mediaPlayer->stop();
}

void MediaPlayer::setVolume(int volume) {
    if (!m_audioOutput) {
        return; // Pas d'audio output disponible
    }
    
    // Clamp volume entre 0 et 100
    volume = qBound(0, volume, 100);
    
    // Convertir en valeur linéaire (0.0 - 1.0)
    qreal linearVolume = volume / 100.0;
    m_audioOutput->setVolume(linearVolume);
}

void MediaPlayer::setPosition(qint64 position) {
    m_mediaPlayer->setPosition(position);
}

void MediaPlayer::loadMedia(const QString& url, MediaType type) {
    m_currentMediaType = type;
    
    QUrl mediaUrl(url);
    m_mediaPlayer->setSource(mediaUrl);
}

QMediaPlayer::PlaybackState MediaPlayer::getState() const {
    return m_mediaPlayer->playbackState();
}

qint64 MediaPlayer::getDuration() const {
    return m_mediaPlayer->duration();
}

qint64 MediaPlayer::getPosition() const {
    return m_mediaPlayer->position();
}

int MediaPlayer::getVolume() const {
    return static_cast<int>(m_audioOutput->volume() * 100);
}

bool MediaPlayer::isMuted() const {
    return m_audioOutput->isMuted();
}

void MediaPlayer::setMuted(bool muted) {
    m_audioOutput->setMuted(muted);
}

void MediaPlayer::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    emit playbackStateChanged(state);
}

void MediaPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    emit mediaStatusChanged(status);
    
    if (status == QMediaPlayer::InvalidMedia) {
        emit error("Média invalide ou impossible à charger");
    }
}

void MediaPlayer::onErrorOccurred(QMediaPlayer::Error error, const QString& errorString) {
    Q_UNUSED(error);
    emit this->error("Erreur du lecteur: " + errorString);
}
