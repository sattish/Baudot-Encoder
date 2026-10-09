#include "baudotencoder.h"
#include <QHash>

static const QHash<QChar, quint8> &lettersTable()
{
    static const QHash<QChar, quint8> t = {
        {'A', 0b00011}, {'B', 0b11001}, {'C', 0b01110}, {'D', 0b01001},
        {'E', 0b00001}, {'F', 0b01101}, {'G', 0b11010}, {'H', 0b10100},
        {'I', 0b00110}, {'J', 0b01011}, {'K', 0b01111}, {'L', 0b10010},
        {'M', 0b11100}, {'N', 0b01100}, {'O', 0b11000}, {'P', 0b10110},
        {'Q', 0b10111}, {'R', 0b01010}, {'S', 0b00101}, {'T', 0b10000},
        {'U', 0b00111}, {'V', 0b11110}, {'W', 0b10011}, {'X', 0b11101},
        {'Y', 0b10101}, {'Z', 0b10001},
        {' ', 0b00100}, {'\n', 0b00010}, {'\r', 0b01000}
    };
    return t;
}

static const QHash<QChar, quint8> &figuresTable()
{
    static const QHash<QChar, quint8> t = {
        {'1', 0b10111}, {'2', 0b10011}, {'3', 0b00001}, {'4', 0b01010},
        {'5', 0b10000}, {'6', 0b10101}, {'7', 0b00111}, {'8', 0b00110},
        {'9', 0b11000}, {'0', 0b10110},
        {' ', 0b00100}, {'\n', 0b00010}, {'\r', 0b01000},
        {'-', 0b00011}, {'?', 0b11001}, {':', 0b01110}, {'$', 0b01001},
        {',', 0b01100}, {'(', 0b01111}, {')', 0b10010}, {'.', 0b11100},
        {'\'', 0b00101}, {'=', 0b10011}, {'/', 0b11101}, {'"', 0b10001},
        {'&', 0b11010}, {'!', 0b01101}, {';', 0b11110}, {'#', 0b10100}
    };
    return t;
}

quint8 BaudotEncoder::codeFor(QChar ch, bool letters)
{
    const QChar upper = ch.toUpper();
    const auto &table = letters ? lettersTable() : figuresTable();
    return table.contains(upper) ? table.value(upper) : 0xFF;
}

QVector<BaudotFrame> BaudotEncoder::encode(const QString &text)
{
    QVector<BaudotFrame> frames;
    bool currentLetters = true; // teleprinters start in letters case

    for (const QChar ch : text) {
        const quint8 lettersCode = codeFor(ch, true);
        const quint8 figuresCode = codeFor(ch, false);

        // Character exists in both shifts with the same code
        // (space, CR, LF): transmit as-is, no shift needed.
        if (lettersCode != 0xFF && lettersCode == figuresCode) {
            frames.append({lettersCode, false, true, ch});
            continue;
        }

        if (lettersCode != 0xFF) {
            if (!currentLetters) {
                frames.append({0b11111, true, true, QChar()}); // LTRS
                currentLetters = true;
            }
            frames.append({lettersCode, false, true, ch.toUpper()});
        } else if (figuresCode != 0xFF) {
            if (currentLetters) {
                frames.append({0b11011, true, false, QChar()}); // FIGS
                currentLetters = false;
            }
            frames.append({figuresCode, false, false, ch});
        }
        // else: character not representable in ITA2 — silently skip
    }

    // Tidy ending: always return to letters shift.
    if (!frames.isEmpty() && !currentLetters)
        frames.append({0b11111, true, true, QChar()});

    return frames;
}