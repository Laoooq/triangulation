#include <iostream>
#include <cmath>
#include <cassert>
#include <iomanip>
#include "test.h"

using namespace std;

class CCoord
{
public:
    double x, y, h;

public:
    CCoord(){};
    CCoord(double a_x, double a_y, double a_h)
    { // Конструктор задает 2 пеленгатора
        x = a_x;
        y = a_y;
        h = a_h;
    }
    void pelengatorsInput()
    {
        // Ввод первого пеленгатора пользователем
        std::cout << "Please enter three direction finder coordinates (x, y, h): " << std::endl;
        std::cout << "x = ";
        std::cin >> x;
        std::cout << "y = ";
        std::cin >> y;
        std::cout << "h = ";
        std::cin >> h;
    }

    void f_Print()
    {
        cout << endl;
        cout << "x: " << x << endl;
        cout << "y: " << y << endl;
        cout << "h: " << h << endl;
    }
};

class CPelengs
{
public:
    double E, B;
    double radiane_E, radiane_B;

public:
    CPelengs(double a_E, double a_B)
    { // Вводятся азимуты и углы места
        E = a_E;
        B = a_B;
        radiane_E = M_PI / 180 * E;
        radiane_B = M_PI / 180 * B;
    }

    void azimuthInput()
    {
        // Ввод первого азимута пользователем
        std::cout << "Please enter two bearings in degrees: " << std::endl;
        std::cout << "E = ";
        std::cin >> E;
        std::cout << "B = ";
        std::cin >> B;
    }

    void f_Print()
    {
        cout << endl;
        cout << "E - " << E << endl;
        cout << "B - " << B << endl;
        cout << endl;
    }
};

class CStep1 //correct
{
public:
    double cos_ax, cos_ay, cos_ah, cos_bx, cos_by, cos_bh;
    CStep1(CPelengs peleng_1, CPelengs peleng_2)
    {   // косинусы первого пеленга
        cos_ax = cos(peleng_1.radiane_E) * cos(peleng_1.radiane_B);
        cos_ay = cos(peleng_1.radiane_E) * sin(peleng_1.radiane_B);
        cos_ah = sin(peleng_1.radiane_E);
        // косинусы второго пеленга
        cos_bx = cos(peleng_2.radiane_E) * cos(peleng_2.radiane_B);
        cos_by = cos(peleng_2.radiane_E) * sin(peleng_2.radiane_B);
        cos_bh = sin(peleng_2.radiane_E);
    }
};

class CStep2 //correct
{
public:
    double a1, a2, a3, b1, b2, b3;
    double t1;
    // определение расстояния точки t1 от точки стояния пеленгатора P1
    CStep2(CStep1 cstep1, CCoord coord, CCoord coord2)
    {
        a1 = cstep1.cos_ah * cstep1.cos_bx - cstep1.cos_ax * cstep1.cos_bh;
        a2 = cstep1.cos_ay * cstep1.cos_bh - cstep1.cos_ah * cstep1.cos_by;
        a3 = cstep1.cos_ax * cstep1.cos_by - cstep1.cos_ay * cstep1.cos_bx;
        b1 = cstep1.cos_ax * (coord2.h - coord.h) - cstep1.cos_ah * (coord2.x - coord.x);
        b2 = cstep1.cos_ah * (coord2.y - coord.y) - cstep1.cos_ay * (coord2.h - coord.h);
        b3 = cstep1.cos_ay * (coord2.x - coord.x) - cstep1.cos_ax * (coord2.y - coord.y);
        t1 = -((a1 * b1 + a2 * b2 + a3 * b3) / (pow(a1, 2) + pow(a2, 2) + pow(a3, 2)));
    }
};

class CStep3
{
public:
    double a1, a2, a3, b1, b2, b3;
    double t2;
    CStep3(CStep1 cstep1, CCoord coord, CCoord coord2)
    { // определение расстояния точки t2 от точки стояния пеленгатора P2
        a1 = cstep1.cos_bh * cstep1.cos_ax - cstep1.cos_bx * cstep1.cos_ah;
        a2 = cstep1.cos_by * cstep1.cos_ah - cstep1.cos_bh * cstep1.cos_ay;
        a3 = cstep1.cos_bx * cstep1.cos_ay - cstep1.cos_by * cstep1.cos_ax;
        b1 = cstep1.cos_bx * (coord2.h - coord.h) - cstep1.cos_bh * (coord2.x - coord.x);
        b2 = cstep1.cos_bh * (coord2.y - coord.y) - cstep1.cos_by * (coord2.h - coord.h);
        b3 = cstep1.cos_by * (coord2.x - coord.x) - cstep1.cos_bx * (coord2.y - coord.y);
        t2 = -((a1 * b1 + a2 * b2 + a3 * b3) / (pow(a1, 2) + pow(a2, 2) + pow(a3, 2)));
    }
};

class CStep4
{
public:
    CCoord coord_t1, coord_t2;
public:
    CStep4(CStep2 cstep2, CCoord coord, CCoord coord2, CStep1 cstep1, CStep3 cstep3)
    { // определяются координаты точки Pt1 и точки Рt 2
        // определяются координаты Pt1
        coord_t1.x = coord.x + cstep2.t1 * cstep1.cos_ax;
        coord_t1.y = coord.y + cstep2.t1 * cstep1.cos_ay;
        coord_t1.h = coord.h + cstep2.t1 * cstep1.cos_ah;
        // определяются координаты Pt2
        coord_t2.x = coord2.x + cstep3.t2 * cstep1.cos_bx;
        coord_t2.y = coord2.y + cstep3.t2 * cstep1.cos_by;
        coord_t2.h = coord2.h + cstep3.t2 * cstep1.cos_bh;
    }
};

class CStep5
{
public:
    double d, dr;
    bool target;
    double deltaFi;

public:
    CStep5(CStep2 cstep2, CStep3 cstep3, CStep4 cstep4, double degrees)
    {
        deltaFi = M_PI / 180 * degrees;
        d = sqrt(pow(cstep4.coord_t1.x - cstep4.coord_t2.x, 2) + pow(cstep4.coord_t1.y - cstep4.coord_t2.y, 2) + pow(cstep4.coord_t1.h - cstep4.coord_t2.h, 2));             // расстояние между точками Рt1 и Pt2
        dr = deltaFi * cstep2.t1 + deltaFi + cstep3.t2;                                                                            // максимальное возможное расстояние между пеленгами p1 и р2

        if (cstep2.t1 > 0 && cstep3.t2 > 0 && d < dr)
        {
            target = true;
        }
        else
        {
            target = false;
        }
    }
};

// определяются выходные данные - координаты точки Ps на отрезке Рt1 Рt2
class CStep6
{
public:
    CCoord step6;

public:
    CStep6(){};
    CStep6(CCoord coord, CStep2 cstep2, CStep3 cstep3, CStep4 cstep4)
    {
        step6.x = (cstep4.coord_t1.x * cstep2.t1 + cstep4.coord_t2.x * cstep3.t2) / (cstep2.t1 + cstep3.t2);
        step6.y = (cstep4.coord_t1.y * cstep2.t1 + cstep4.coord_t2.y * cstep3.t2) / (cstep2.t1 + cstep3.t2);
        step6.h = (cstep4.coord_t1.h * cstep2.t1 + cstep4.coord_t2.h * cstep3.t2) / (cstep2.t1 + cstep3.t2);
    }
};

// Класс создан для вычисления всех шагов триангуляции
// Последовательное создание объектов классов каждого шага
// И передача данных от одного объекта в другой
// по результатам проведенных выч   ислений определяют координаты цели
// и ставят цель на сопровождение
class CTriangulate
{
public:
    CCoord triang;

    CTriangulate(){};
    CTriangulate(CCoord coord, CCoord coord2, CPelengs peleng_1, CPelengs peleng_2, double degrees)
    {
        CStep1 cstep1 = CStep1(peleng_1, peleng_2);

        CStep2 cstep2 = CStep2(cstep1, coord, coord2);

        CStep3 cstep3 = CStep3(cstep1, coord, coord2);

        CStep4 cstep4 = CStep4(cstep2, coord, coord2, cstep1, cstep3);

        CStep5 cstep5 = CStep5(cstep2, cstep3, cstep4, degrees);

        CStep6 cstep6 = CStep6(coord, cstep2, cstep3, cstep4);

        triang.x = cstep6.step6.x;
        triang.y = cstep6.step6.y;
        triang.h = cstep6.step6.h;
    }
};


// Функция проверки правильности работы триангуляции
void f_Test()
{
    CCoord coords(0, 0, 0);
    CPelengs peleng_1(45, 0);

    CCoord coords2(2, 0, 0);
    CPelengs peleng_2(0, 0);
    // Вычисления
    CTriangulate triangulation = CTriangulate(coords, coords2, peleng_1, peleng_2, 15);

    // Проверка результатов
    assert(std::fabs(triangulation.triang.x - 2) < std::numeric_limits<float>::epsilon());
    assert(std::fabs(triangulation.triang.y - 2) < std::numeric_limits<float>::epsilon());
    assert(std::fabs(triangulation.triang.h - 2) < std::numeric_limits<float>::epsilon());
}


void f_Test2()
{
    CCoord coords(55.4, 76, 16);
    CPelengs peleng_1(32, 17.5);

    CCoord coords2(30, 25, 11);
    CPelengs peleng_2(12, 21.8);
    // Вычисления
    CTriangulate triangulation = CTriangulate(coords, coords2, peleng_1, peleng_2, 2);

    // Проверка результатов
    assert(std::fabs(triangulation.triang.x - (-46.7064)) < std::numeric_limits<float>::epsilon());
    assert(std::fabs(triangulation.triang.y - 195.725) < std::numeric_limits<float>::epsilon());
    assert(std::fabs(triangulation.triang.h - (-147.155)) < std::numeric_limits<float>::epsilon());
}

void print_res(){
    CTriangulate triangulation;

    double x = triangulation.triang.x;
    double h = triangulation.triang.y;
    double y = triangulation.triang.h;

    std::cout << setprecision(13);
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "h: " << h << std::endl;
}

int main()
{
    // Input
    CCoord coord(55.4, 76, 16); // Ввод первых пеленгаторов
    CPelengs cpeleng(1, 32); // Ввод первых пеленгов

    coord.pelengatorsInput();
    cpeleng.azimuthInput();

    CCoord coord2(32, 40, 11); // Ввод вторых пеленгаторов
    CPelengs cpeleng2(2, 41); // Ввод вторых пеленгов

    coord2.pelengatorsInput();
    cpeleng2.azimuthInput();

    cout << "------" << endl;
    coord.f_Print();
    cpeleng.f_Print();
    cout << "------" << endl;
    coord2.f_Print();
    cpeleng2.f_Print();
    cout << "------" << endl;

    // Computing
    CTriangulate triangulation = CTriangulate(coord, coord2, cpeleng, cpeleng2, 20); // 40 градусов

    print_res(); // Вывод результатов

    // Tests
    f_Test();
    //f_Test2();
    return 0;
}
