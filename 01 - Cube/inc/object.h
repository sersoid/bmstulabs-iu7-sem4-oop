#ifndef OBJECT_H
#define OBJECT_H

#include <list>

union point {
    struct {
        double x;
        double y;
        double z;
    };
    double values[3] = {0, 0, 0};
};

struct edge {
    point point1;
    point point2;
};

struct object {
    std::list<edge> edges;
    point center;
    point rotation;
};

// Point move

point movePointX(const point& targetPoint, double dX);
point movePointY(const point& targetPoint, double dY);
point movePointZ(const point& targetPoint, double dZ);
point movePoint(const point& targetPoint, const point& objCenter);

// Point rotate

point rotatePointX(const point& targetPoint, double angleX);
point rotatePointY(const point& targetPoint, double angleY);
point rotatePointZ(const point& targetPoint, double angleZ);
point rotatePoint(const point& targetPoint, const point& rotation);

#endif
