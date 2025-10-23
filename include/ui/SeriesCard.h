#ifndef SERIESCARD_H
#define SERIESCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "models/Series.h"

/**
 * @brief Widget carte moderne pour afficher une série
 */
class SeriesCard : public QWidget {
    Q_OBJECT

public:
    explicit SeriesCard(const Series& series, QWidget* parent = nullptr);
    Series getSeries() const { return m_series; }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void setupUI();
    void loadPoster();
    void onPosterDownloaded();

    Series m_series;
    QLabel* m_posterLabel;
    QLabel* m_titleLabel;
    QLabel* m_descriptionLabel;
    QLabel* m_ratingLabel;
    QLabel* m_yearLabel;
    
    QNetworkAccessManager* m_networkManager;
    QPixmap m_posterPixmap;
    bool m_hovered;
};

#endif // SERIESCARD_H
