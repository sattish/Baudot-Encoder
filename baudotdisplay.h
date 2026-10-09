#ifndef BAUDOTDISPLAY_H
#define BAUDOTDISPLAY_H

#include <QWidget>
#include "baudotencoder.h"

class BaudotDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit BaudotDisplay(QWidget *parent = nullptr);

public slots:
    void setFrames(const QVector<BaudotFrame> &frames);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QSize frameExtent() const;
    void paintFrame(QPainter &p, const BaudotFrame &f, const QRect &rect);

    QVector<BaudotFrame> m_frames;

    static constexpr int kFrameW = 56;
    static constexpr int kFrameH = 130;
    static constexpr int kGap = 8;
};

#endif // BAUDOTDISPLAY_H