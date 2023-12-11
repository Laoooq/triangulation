#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

class CPeleng {
public:
    double E, B;
    double radianE, radianB;

    CPeleng(double a_E, double a_B) {
        E = a_E;
        B = a_B;
        radianE = M_PI / 180 * E;
        radianB = M_PI / 180 * B;
    }
};

class CCoord {
public:
    double x, y, h;

    CCoord(double a_x, double a_y, double a_h) {
        x = a_x;
        y = a_y;
        h = a_h;
    }
};

class CTriangulate {
public:
    CTriangulate(CPeleng peleng1, CPeleng peleng2, CCoord coord1, CCoord coord2, double degrees) {
        //step1
        double cosax = cos(peleng1.radianE) * cos(peleng1.radianB);
        double cosay = cos(peleng1.radianE) * sin(peleng1.radianB);
        double cosah = sin(peleng1.radianE);

        double cosbx = cos(peleng2.radianE) * cos(peleng2.radianB);
        double cosby = cos(peleng2.radianE) * sin(peleng2.radianB);
        double cosbh = sin(peleng2.radianE);

        //step 2
        double a1 = cosah * cosbx - cosax * cosbh;
        double a2 = cosay * cosbh - cosah * cosby;
        double a3 = cosax * cosby - cosay * cosbx;
        double b1 = cosax * (coord2.h - coord1.h) - cosah * (coord2.x - coord1.x);
        double b2 = cosah * (coord2.y - coord1.y) - cosay * (coord2.h - coord1.h);
        double b3 = cosay * (coord2.x - coord1.x) - cosax * (coord2.y - coord1.y);
        double t1 = -((a1 * b1 + a2 * b2 + a3 * b3) / (a1 * a1 + a2 * a2 + a3 * a3));

        //step3
        a1 = cosbh * cosax - cosbx * cosah;
        a2 = cosby * cosah - cosbh * cosay;
        a3 = cosbx * cosay - cosby * cosax;
        b1 = cosbx * (coord2.h - coord1.h) - cosbh * (coord2.x - coord1.x);
        b2 = cosbh * (coord2.y - coord1.y) - cosby * (coord2.h - coord1.h);
        b3 = cosby * (coord2.x - coord1.x) - cosbx * (coord2.y - coord1.y);
        double t2 = -((a1 * b1 + a2 * b2 + a3 * b3) / (a1 * a1 + a2 * a2 + a3 * a3));

        //step4
        double xt1 = coord1.x + t1 * cosax;
        double yt1 = coord1.y + t1 * cosay;
        double ht1 = coord1.h + t1 * cosah;

        double xt2 = coord2.x + t2 * cosbx;
        double yt2 = coord2.y + t2 * cosby;
        double ht2 = coord2.h + t2 * cosbh;

        //step 5
        double d = sqrt(pow(xt1 - xt2, 2) + pow(yt1 - yt2, 2) + pow(ht1 - ht2, 2));
        double deltaFi = M_PI / 180 * degrees;
        double dr = deltaFi * t1 + deltaFi * t2;
        bool target = (t1 > 0 && t2 > 0 && d < dr);

        //step 6
        if (target) {
            double xs = (xt1 * t1 + xt2 * t2) / (t1 + t2);
            double ys = (yt1 * t1 + yt2 * t2) / (t1 + t2);
            double hs = (ht1 * t1 + ht2 * t2) / (t1 + t2);

            std::cout << "x: " << xs << std::endl;
            std::cout << "y: " << ys << std::endl;
            std::cout << "h: " << hs << std::endl;
        }
        else {
            std::cout << "The goal is incompatible" << std::endl;
        }
    }
};

int main() {
    CPeleng peleng1(45, 0);
    CPeleng peleng2(0, 0);

    CCoord coord1(0, 0, 0);
    CCoord coord2(2, 0, 0);

    CTriangulate triangulation(peleng1, peleng2, coord1, coord2, 20);

    return 0;
}
