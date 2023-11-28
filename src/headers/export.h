#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>

namespace Ui {
class Export;
}

class Export : public QDialog
{
    Q_OBJECT

public:
    explicit Export(QWidget *parent = nullptr);
    ~Export();

private slots:
    void setup();

private:
    Ui::Export *ui;
};

#endif // EXPORT_H
