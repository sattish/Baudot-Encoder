#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QLineEdit;
class BaudotDisplay;
class QLabel;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void onTextChanged(const QString &text);

    QLineEdit *m_input;
    BaudotDisplay *m_display;
    QLabel *m_status;
};

#endif // MAINWINDOW_H
