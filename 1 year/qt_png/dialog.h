#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(std::map<int, int> &colors, QString filename, QWidget *parent = nullptr);
    ~Dialog();

private slots:

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
