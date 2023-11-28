#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class Tutorial;
}

class Tutorial : public QDialog
{
    Q_OBJECT

public:
    explicit Tutorial(QWidget *parent = nullptr);
    ~Tutorial();

private slots:
    void setup();
    void onFeatureSelected(QListWidgetItem *current);
    void on_pushButton_Close_clicked();

private:
    Ui::Tutorial *ui;
};

#endif // TUTORIAL_H
