#ifndef BAUDOTENCODER_H
#define BAUDOTENCODER_H

#include <QString>
#include <QVector>

// One transmitted 5-bit group. isShift == true means the group is a
// LTRS/FIGS shift code rather than a printable character.
struct BaudotFrame
{
    quint8 code = 0;     // 5-bit value, bit 4 is leftmost as displayed
    bool   isShift = false;
    bool   lettersShift = true; // only meaningful when isShift == true
    QChar  ch;           // character this frame represents (' ' for shifts)
};

class BaudotEncoder
{
public:
    // Converts text into a sequence of Baudot frames, inserting
    // LTRS/FIGS shift codes as required.
    static QVector<BaudotFrame> encode(const QString &text);

    // Returns the 5-bit code for a char in the given shift state,
    // or 0xFF if not representable.
    static quint8 codeFor(QChar ch, bool letters);

private:
    // ITA2 tables, indexed by code value (bit 4 = MSB).
    static const char *const kLetters[32];
    static const char *const kFigures[32];
};

#endif // BAUDOTENCODER_H