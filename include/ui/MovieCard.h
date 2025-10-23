#ifndef MOVIECARD_H
#define MOVIECARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMouseEvent>
#include "models/Movie.h"

/**
 * @brief Widget représentant une carte de film moderne
 */
class MovieCard : public QWidget {
    Q_OBJECT

public:
    explicit MovieCard(const Movie& movie, QWidget* parent = nullptr);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onPosterDownloaded();

private:
    void setupUI();
    void loadPoster();

    Movie m_movie;
    QLabel* m_posterLabel;
    QLabel* m_titleLabel;
    QLabel* m_ratingLabel;
    QLabel* m_yearLabel;
    QLabel* m_descriptionLabel;
    QPixmap m_posterPixmap;
    bool m_hovered;
    QNetworkAccessManager* m_networkManager;
};

#endif // MOVIECARD_H
