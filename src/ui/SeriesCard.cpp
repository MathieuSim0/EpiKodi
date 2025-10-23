#include "ui/SeriesCard.h"
#include <QPainter>
#include <QMouseEvent>

SeriesCard::SeriesCard(const Series& series, QWidget* parent)
    : QWidget(parent)
    , m_series(series)
    , m_hovered(false)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setFixedSize(220, 400);  // Même taille que MovieCard
    setupUI();
    loadPoster();
    setCursor(Qt::PointingHandCursor);
}

void SeriesCard::setupUI() {
    setStyleSheet(R"(
        SeriesCard {
            background-color: #1e293b;
            border-radius: 12px;
            border: 2px solid #334155;
        }
        SeriesCard:hover {
            background-color: #334155;
            border: 2px solid #ec4899;
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
    m_posterLabel->setFixedSize(204, 290);  // TAILLE FIXE
    m_posterLabel->setScaledContents(false);  // NE PAS étirer l'image
    m_posterLabel->setAlignment(Qt::AlignCenter);  // Centrer l'image dans le label
    m_posterLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 8px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(236, 72, 153, 0.15),
                                        stop:1 rgba(139, 92, 246, 0.15));
            border: 2px solid #334155;
        }
    )");
    m_posterLabel->setText("📺");
    m_posterLabel->setStyleSheet(m_posterLabel->styleSheet() + "font-size: 36pt; color: #64748b;");

    // Container pour les infos en dessous du poster
    QWidget* infoContainer = new QWidget(this);
    infoContainer->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoContainer);
    infoLayout->setContentsMargins(4, 4, 4, 4);
    infoLayout->setSpacing(6);

    // Titre
    m_titleLabel = new QLabel(m_series.getName(), this);
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
    
    m_ratingLabel = new QLabel(QString("⭐ %1/10").arg(m_series.getRating(), 0, 'f', 1), this);
    m_ratingLabel->setStyleSheet(R"(
        QLabel {
            font-size: 9pt; 
            font-weight: 600;
            color: #fbbf24;
            padding: 2px;
            background: transparent;
        }
    )");
    
    m_yearLabel = new QLabel(m_series.getFirstAirDate().toString("yyyy"), this);
    m_yearLabel->setStyleSheet(R"(
        QLabel {
            font-size: 9pt; 
            font-weight: 600;
            color: #ec4899;
            padding: 2px;
            background: transparent;
        }
    )");
    
    ratingYearLayout->addWidget(m_ratingLabel);
    ratingYearLayout->addStretch();
    ratingYearLayout->addWidget(m_yearLabel);

    // Description courte
    m_descriptionLabel = new QLabel(this);
    QString description = m_series.getOverview();
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

void SeriesCard::loadPoster() {
    QString posterPath = m_series.getPosterPath();
    
    if (posterPath.isEmpty() || posterPath == "N/A") {
        return;
    }

    // Construire l'URL complète TMDb
    QString posterUrl = "https://image.tmdb.org/t/p/w500" + posterPath;

    QNetworkRequest request(posterUrl);
    QNetworkReply* reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, &SeriesCard::onPosterDownloaded);
}

void SeriesCard::onPosterDownloaded() {
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

void SeriesCard::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void SeriesCard::enterEvent(QEnterEvent* event) {
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void SeriesCard::leaveEvent(QEvent* event) {
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}

void SeriesCard::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    
    if (m_hovered) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Bordure brillante au survol
        QPen pen;
        pen.setWidth(3);
        
        // Dégradé pour la bordure (rose/violet pour séries)
        QLinearGradient gradient(0, 0, width(), height());
        gradient.setColorAt(0.0, QColor(236, 72, 153));   // Rose
        gradient.setColorAt(0.5, QColor(139, 92, 246));   // Violet
        gradient.setColorAt(1.0, QColor(6, 182, 212));    // Cyan
        
        pen.setBrush(QBrush(gradient));
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 12, 12);
    }
}
