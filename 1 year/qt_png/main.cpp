#include <iostream>
#include <map>
#include <png++/png.hpp>
#include <png++/image_info.hpp>

#include <QApplication>
#include "dialog.h"

/* Демо-версия проекта без оболочки QT.
 * на ввод подается изображение, после выводит политру цветов
 * в форматe png(0-255,0-255,0-255)*/

int main(int argc, char *argv[])
{
    char *image_file = "rgb.png";

    if(argc < 2)
    {
        std::cout << "No PNG found.\n";

        png::image< png::rgb_pixel > image(128, 128);

        for (png::uint_32 y = 0; y < image.get_height(); ++y)
        {
            for (png::uint_32 x = 0; x < image.get_width(); ++x)
            {
                image[y][x] = png::rgb_pixel(x, y, (x + y) % 512);
            }
        }

        image.write(image_file);
    }
    else
    {
        image_file = argv[1];
    }

    std::map<int, int> colorTable;

    png::image< png::rgb_pixel > image2(image_file);

    for (png::uint_32 y = 0; y < image2.get_height(); ++y)
    {
        for (png::uint_32 x = 0; x < image2.get_width(); ++x)
        {
            png::rgb_pixel p = image2.get_pixel(x, y);

            int rgb = (p.red << 16) + (p.green << 8) + p.blue;

            colorTable[rgb] += 1;
        }
    }

    std::cout << "Colors count:" << std::endl;

    std::map<int, int>::iterator it;

    for (it = colorTable.begin(); it != colorTable.end(); it++ )
    {
        std::cout << "rgb(" << ((it->first >> 16) & 0xff) << ", " << ((it->first >> 8) & 0xff) << ", " << (it->first & 0xff) << ") = " << it->second << std::endl;
    }

    QApplication a(argc, argv);// создаём экземпляр графического приложения Qt

    Dialog d(colorTable, image_file);// создаём диалог
    d.show();// отображаем диалог

    return a.exec();
}
