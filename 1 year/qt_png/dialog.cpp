#include "dialog.h"
#include "ui_dialog.h"
#include <QtMath>
#include <vector>
#include <map>

Dialog::Dialog(std::map<int, int> &colors, QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    QPixmap pixmap(filename);// загружаем изображение
    ui->label->setPixmap(pixmap);// отображаем его на экране
    ui->label->setScaledContents( true );// позволяем масштабирование
    ui->label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    int size = colors.size();// получаем количество уникальных цветов
    int cols = static_cast<int>(qSqrt(size));// вычисляем необходимое количество столбцов
    int rows = cols + 1;// вычисляем необходимое количество строк

    ui->tableWidget->setColumnCount(cols);// устанавливаем необходимое количество столбцов в виджете
    ui->tableWidget->setRowCount(rows);// устанавливаем необходимое количество строк в виджете

    std::vector<std::pair<int, int>> pairs;
    for (auto itr = colors.begin(); itr != colors.end(); ++itr)// копируем цвета из map в вектор
        pairs.push_back(*itr);

    sort(pairs.begin(), pairs.end(), [=](std::pair<int, int>& a, std::pair<int, int>& b)// сортируем вектор по количеству вхождений цвета
                                        {
                                            return a.second > b.second;
                                        }
        );

    int row = 0;
    int col = 0;
    for (auto const &vk : pairs)// проходим по всем элементам вектора
    {
        ui->tableWidget->setItem(row, col, new QTableWidgetItem);// создаём ячейку в таблице
        // получаем отдельные компоненты цвета
        int r = (vk.first >> 16) & 0xff;
        int g = (vk.first >> 8) & 0xff;
        int b = vk.first & 0xff;

        QColor color(r, g, b);// создаем цвет
        QBrush brush(color);// создаем кисть

        ui->tableWidget->item(row, col)->setBackground(brush);// закрашиваем ячейку

        col += 1;// увеличиваем столбец на 1
        if (col == cols)// если столбец последний
        {
            row += 1;// увеличиваем строку
            col = 0;// столбец 0
        }
    }
}

Dialog::~Dialog()
{
    delete ui;
}
