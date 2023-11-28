#ifndef INTRO_OUTRO_H
#define INTRO_OUTRO_H

#include <QDialog>
#include <QFontDialog>
#include <QLabel>
#include <QColorDialog>
#include <QPainter>
#include <QTemporaryFile>
#include <QProcess>
#include <QStandardPaths>

class MainWindow;

namespace Ui {
class Intro_Outro;
}

class Intro_Outro : public QDialog
{
    Q_OBJECT

public:
    explicit Intro_Outro(bool isIntro, QWidget *parent = nullptr);
    ~Intro_Outro();

private slots:
    void setup();
    void on_pushButton_Font_clicked();
    void on_pushButton_Text_Color_clicked();
    void on_pushButton_Background_clicked();
    void updatePreview(QLabel *label, const QString &text, const QFont &font, const QColor &textColor, const QColor &backgroundColor);
    void on_buttonBox_accepted();

private:
    Ui::Intro_Outro *ui;
    MainWindow *mainWin;
    QFont font;
    QColor textColor = Qt::black;
    QColor backgroundColor = Qt::white;
    bool isIntro;
};

#endif // INTRO_OUTRO_H
