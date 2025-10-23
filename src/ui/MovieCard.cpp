#include "ui/MovieCard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

MovieCard::MovieCard(const Movie& movie, QWidget* parent)
    : QWidget(parent)
    , m_movie(movie)
    , m_hovered(false)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setFixedSize(220, 400);  // Plus haut pour contenir poster + infos en dessous
    setupUI();
    loadPoster();
    setCursor(Qt::PointingHandCursor);
}

void MovieCard::setupUI() {
    setStyleSheet(R"(
        MovieCard {
            background-color: #1e293b;
            border-radius: 12px;
            border: 2px solid #334155;
        }
        MovieCard:hover {
            background-color: #334155;
            border: 2px solid #6366f1;
        }
        QLabel {
            background: transparent;
            color: #f1f5f9;
        }
    )");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // Poster en haut
    m_posterLabel = new QLabel(this);
    m_posterLabel->setFixedSize(204, 290);  // Poster rectangulaire - TAILLE FIXE
    m_posterLabel->setScaledContents(false);  // NE PAS étirer l'image
    m_posterLabel->setAlignment(Qt::AlignCenter);  // Centrer l'image dans le label
    m_posterLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 8px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(99, 102, 241, 0.15),
                                        stop:1 rgba(6, 182, 212, 0.15));
            border: 2px solid #334155;
        }
    )");
    m_posterLabel->setText("🎬");
    m_posterLabel->setStyleSheet(m_posterLabel->styleSheet() + "font-size: 36pt; color: #64748b;");

    // Container pour les infos en dessous du poster
    QWidget* infoContainer = new QWidget(this);
    infoContainer->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoContainer);
    infoLayout->setContentsMargins(4, 4, 4, 4);
    infoLayout->setSpacing(6);

    // Titre
    m_titleLabel = new QLabel(m_movie.getTitle(), this);
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setFixedHeight(36);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            font-weight: 700; 
            font-size: 10pt; 
            color: #f1f5f9;
            padding: 2px;
            background: transparent;
        }
    )");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // Note et année
    QHBoxLayout* ratingYearLayout = new QHBoxLayout();
    ratingYearLayout->setSpacing(10);
    
    m_ratingLabel = new QLabel(QString("⭐ %1/10").arg(m_movie.getRating(), 0, 'f', 1), this);
    m_ratingLabel->setStyleSheet(R"(
        QLabel {
            font-size: 9pt; 
            font-weight: 600;
            color: #fbbf24;
            padding: 2px;
            background: transparent;
        }
    )");
    
    m_yearLabel = new QLabel(m_movie.getReleaseDate().toString("yyyy"), this);
    m_yearLabel->setStyleSheet(R"(
        QLabel {
            font-size: 9pt; 
            font-weight: 600;
            color: #06b6d4;
            padding: 2px;
            background: transparent;
        }
    )");
    
    ratingYearLayout->addWidget(m_ratingLabel);
    ratingYearLayout->addStretch();
    ratingYearLayout->addWidget(m_yearLabel);

    // Description courte sous le titre
    m_descriptionLabel = new QLabel(this);
    QString description = m_movie.getOverview();
    if (description.length() > 60) {
        description = description.left(57) + "...";
    }
    if (description.isEmpty() || description == "N/A") {
        description = "Cliquez pour détails";
    }
    m_descriptionLabel->setText(description);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setFixedHeight(36);
    m_descriptionLabel->setStyleSheet(R"(
        QLabel {
            font-size: 8pt; 
            color: #94a3b8;
            padding: 2px;
            background: transparent;
        }
    )");
    m_descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    infoLayout->addWidget(m_titleLabel);
    infoLayout->addLayout(ratingYearLayout);
    infoLayout->addWidget(m_descriptionLabel);

    layout->addWidget(m_posterLabel, 0, Qt::AlignTop);
    layout->addWidget(infoContainer, 0, Qt::AlignTop);
    layout->addStretch();
}

void MovieCard::loadPoster() {
    QString posterPath = m_movie.getPosterPath();
    
    if (posterPath.isEmpty() || posterPath == "N/A") {
        return;
    }

    // Construire l'URL complète TMDb
    QString posterUrl = "https://image.tmdb.org/t/p/w500" + posterPath;

    QNetworkRequest request(posterUrl);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &MovieCard::onPosterDownloaded);
}

void MovieCard::onPosterDownloaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        m_posterPixmap.loadFromData(imageData);
        
        // Redimensionner l'image pour qu'elle tienne dans le label SANS déborder
        QPixmap scaledPixmap = m_posterPixmap.scaled(
            204, 290,  // Taille max du label
            Qt::KeepAspectRatio,  // Garder les proportions
            Qt::SmoothTransformation  // Lissage
        );
        
        m_posterLabel->setPixmap(scaledPixmap);
        m_posterLabel->setStyleSheet(R"(
            QLabel {
                border-radius: 8px;
                border: 2px solid #475569;
                background-color: #0f172a;
            }
        )");
    }

    reply->deleteLater();
}

void MovieCard::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void MovieCard::enterEvent(QEnterEvent* event) {
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void MovieCard::leaveEvent(QEvent* event) {
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}

void MovieCard::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    
    if (m_hovered) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Bordure brillante au survol
        QPen pen;
        pen.setWidth(3);
        
        // Créer un dégradé pour la bordure
        QLinearGradient gradient(0, 0, width(), height());
        gradient.setColorAt(0.0, QColor(99, 102, 241));   // Violet
        gradient.setColorAt(0.5, QColor(236, 72, 153));   // Rose
        gradient.setColorAt(1.0, QColor(6, 182, 212));    // Cyan
        
        pen.setBrush(QBrush(gradient));
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 12, 12);
    }
}
