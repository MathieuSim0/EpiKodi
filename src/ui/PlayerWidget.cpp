#include "ui/PlayerWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>

PlayerWidget::PlayerWidget(MediaPlayer* mediaPlayer, QWidget* parent)
    : QWidget(parent)
    , m_mediaPlayer(mediaPlayer)
    , m_isUserSeeking(false)
{
    setupUI();
    
    // Connexions
    connect(m_mediaPlayer, &MediaPlayer::positionChanged, this, &PlayerWidget::onPositionChanged);
    connect(m_mediaPlayer, &MediaPlayer::durationChanged, this, &PlayerWidget::onDurationChanged);
    connect(m_mediaPlayer, &MediaPlayer::playbackStateChanged, this, &PlayerWidget::onPlaybackStateChanged);
    
    connect(m_playPauseButton, &QPushButton::clicked, this, &PlayerWidget::onPlayPauseClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &PlayerWidget::onStopClicked);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayerWidget::onVolumeChanged);
    connect(m_positionSlider, &QSlider::sliderPressed, [this]() { m_isUserSeeking = true; });
    connect(m_positionSlider, &QSlider::sliderReleased, [this]() { 
        m_isUserSeeking = false;
        onSeekPositionChanged(m_positionSlider->value());
    });
}

void PlayerWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Ligne de statut
    m_statusLabel = new QLabel("Aucun média chargé", this);
    mainLayout->addWidget(m_statusLabel);

    // Slider de position
    m_positionSlider = new QSlider(Qt::Horizontal, this);
    m_positionSlider->setRange(0, 0);
    mainLayout->addWidget(m_positionSlider);

    // Contrôles et temps
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    
    m_playPauseButton = new QPushButton("▶️ Lecture", this);
    m_stopButton = new QPushButton("⏹️ Stop", this);
    m_timeLabel = new QLabel("00:00 / 00:00", this);
    
    QLabel* volumeLabel = new QLabel("🔊", this);
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50);
    m_volumeSlider->setMaximumWidth(100);

    controlsLayout->addWidget(m_playPauseButton);
    controlsLayout->addWidget(m_stopButton);
    controlsLayout->addWidget(m_timeLabel);
    controlsLayout->addStretch();
    controlsLayout->addWidget(volumeLabel);
    controlsLayout->addWidget(m_volumeSlider);

    mainLayout->addLayout(controlsLayout);

    // Style
    setStyleSheet("PlayerWidget { background-color: #2c3e50; border-top: 2px solid #34495e; }"
                  "QPushButton { padding: 5px 10px; background-color: #3498db; color: white; border: none; border-radius: 3px; }"
                  "QPushButton:hover { background-color: #2980b9; }"
                  "QLabel { color: white; }");
}

void PlayerWidget::loadAndPlay(const QString& url, int mediaType) {
    m_mediaPlayer->loadMedia(url, static_cast<MediaPlayer::MediaType>(mediaType));
    m_mediaPlayer->play();
    
    QString typeStr = (mediaType == 0) ? "Vidéo" : "Audio";
    m_statusLabel->setText(QString("Lecture de %1: %2").arg(typeStr).arg(url));
}

void PlayerWidget::onPlayPauseClicked() {
    auto state = m_mediaPlayer->getState();
    
    if (state == QMediaPlayer::PlayingState) {
        m_mediaPlayer->pause();
        m_playPauseButton->setText("▶️ Lecture");
    } else {
        m_mediaPlayer->play();
        m_playPauseButton->setText("⏸️ Pause");
    }
}

void PlayerWidget::onStopClicked() {
    m_mediaPlayer->stop();
    m_playPauseButton->setText("▶️ Lecture");
    m_statusLabel->setText("Arrêté");
}

void PlayerWidget::onPositionChanged(qint64 position) {
    if (!m_isUserSeeking) {
        m_positionSlider->setValue(static_cast<int>(position));
    }
    
    qint64 duration = m_mediaPlayer->getDuration();
    m_timeLabel->setText(QString("%1 / %2")
        .arg(formatTime(position))
        .arg(formatTime(duration)));
}

void PlayerWidget::onDurationChanged(qint64 duration) {
    m_positionSlider->setRange(0, static_cast<int>(duration));
}

void PlayerWidget::onVolumeChanged(int value) {
    m_mediaPlayer->setVolume(value);
}

void PlayerWidget::onSeekPositionChanged(int value) {
    m_mediaPlayer->setPosition(value);
}

void PlayerWidget::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    switch (state) {
        case QMediaPlayer::PlayingState:
            m_playPauseButton->setText("⏸️ Pause");
            m_statusLabel->setText("Lecture en cours...");
            break;
        case QMediaPlayer::PausedState:
            m_playPauseButton->setText("▶️ Lecture");
            m_statusLabel->setText("En pause");
            break;
        case QMediaPlayer::StoppedState:
            m_playPauseButton->setText("▶️ Lecture");
            m_statusLabel->setText("Arrêté");
            break;
    }
}

QString PlayerWidget::formatTime(qint64 milliseconds) {
    qint64 seconds = milliseconds / 1000;
    qint64 minutes = seconds / 60;
    seconds = seconds % 60;
    
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}
