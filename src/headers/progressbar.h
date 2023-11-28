#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class ProgressBar;
}

class ProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressBar(QWidget *parent = nullptr);
    ~ProgressBar();

    void startProgress();
    void stopProgress();
    void setLightMode();
    void setDarkMode();

private slots:
    void updateProgress();

private:
    Ui::ProgressBar *ui;
    QTimer *progressTimer;
};

#endif // PROGRESSBAR_H
