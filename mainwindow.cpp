#include "mainwindow.h"
#include "baudotencoder.h"
#include "baudotdisplay.h"

#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("Type text here - it becomes Baudot below");
    m_input->setFont(QFont("Monospace", 12));

    m_display = new BaudotDisplay(this);

    auto *scroll = new QScrollArea(this);
    scroll->setWidget(m_display);
    scroll->setWidgetResizable(true);
    scroll->setBackgroundRole(QPalette::Base);

    m_status = new QLabel("0 characters, 0 bits", this);

    layout->addWidget(m_input);
    layout->addWidget(scroll, 1);
    layout->addWidget(m_status);

    connect(m_input, &QLineEdit::textChanged,
            this, &MainWindow::onTextChanged);
}

void MainWindow::onTextChanged(const QString &text)
{
    const auto frames = BaudotEncoder::encode(text);
    m_display->setFrames(frames);

    const int chars = std::count_if(frames.cbegin(), frames.cend(),
                                    [](const BaudotFrame &f){ return !f.isShift; });
    m_status->setText(QString("%1 character(s), %2 frame(s), %3 bits")
                          .arg(chars).arg(frames.size()).arg(frames.size() * 5));
}