#include "baudotdisplay.h"
#include <QPainter>

namespace {
inline bool bitAt(quint8 code, int bit)
{
    return (code >> bit) & 1;
}
}

BaudotDisplay::BaudotDisplay(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(kFrameH + 2 * kGap);
}

void BaudotDisplay::setFrames(const QVector<BaudotFrame> &frames)
{
    m_frames = frames;
    // Grow width so the parent QScrollArea gives us scrollbars.
    const int perRow = qMax(1, (width() - kGap) / (kFrameW + kGap));
    const int rows = (m_frames.size() + perRow - 1) / perRow;
    setMinimumHeight(qMax(kFrameH + 2 * kGap, rows * (kFrameH + kGap) + kGap));
    update();
}

QSize BaudotDisplay::frameExtent() const
{
    return {kFrameW, kFrameH};
}

void BaudotDisplay::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const int availW = width() - kGap;
    const int perRow = qMax(1, availW / (kFrameW + kGap));

    for (int i = 0; i < m_frames.size(); ++i) {
        const int row = i / perRow;
        const int col = i % perRow;
        const QRect rect(kGap + col * (kFrameW + kGap),
                         kGap + row * (kFrameH + kGap),
                         kFrameW, kFrameH);
        paintFrame(p, m_frames.at(i), rect);
    }
}

void BaudotDisplay::paintFrame(QPainter &p, const BaudotFrame &f, const QRect &rect)
{
    // Card background
    QColor bg = f.isShift
                    ? (f.lettersShift ? QColor(210, 235, 210) : QColor(235, 215, 210))
                    : QColor(245, 245, 245);
    p.setPen(Qt::lightGray);
    p.setBrush(bg);
    p.drawRoundedRect(rect, 6, 6);

    if (f.isShift) {
        p.setPen(Qt::darkGreen);
        QFont fnt = font();
        fnt.setBold(true);
        fnt.setPointSize(9);
        p.setFont(fnt);
        p.drawText(rect, Qt::AlignCenter, f.lettersShift ? "LTRS" : "FIGS");
        return;
    }

    // Five bit circles, bit 4 (MSB) at the top.
    const int circleR = 9;
    const int spacing = (rect.height() - 36) / 5;
    const int cx = rect.center().x();

    for (int bit = 4; bit >= 0; --bit) {
        const bool on = bitAt(f.code, bit);
        const int cy = rect.top() + 10 + (4 - bit) * spacing + circleR;

        p.setPen(Qt::darkGray);
        p.setBrush(on ? QColor(40, 90, 160) : Qt::white);
        p.drawEllipse(QPoint(cx, cy), circleR, circleR);
    }

    // Character label
    p.setPen(Qt::black);
    QFont fnt = font();
    fnt.setBold(true);
    fnt.setPointSize(11);
    p.setFont(fnt);
    QRect labelRect = rect.adjusted(0, rect.height() - 26, 0, 0);
    p.drawText(labelRect, Qt::AlignCenter,
               f.ch == '\n' ? "\\n" : f.ch == '\r' ? "\\r" : QString(f.ch));

    // Bit pattern (e.g. "10010")
    p.setPen(Qt::gray);
    fnt.setBold(false);
    fnt.setPointSize(7);
    p.setFont(fnt);
    QString bits;
    for (int bit = 4; bit >= 0; --bit)
        bits += bitAt(f.code, bit) ? '1' : '0';
    p.drawText(QRect(rect.x(), rect.y() + rect.height() + 1,
                     rect.width(), 12), Qt::AlignCenter, bits);
}