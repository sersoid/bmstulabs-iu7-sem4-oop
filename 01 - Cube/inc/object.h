#ifndef __H_OBJECT
#define __H_OBJECT

#include <list>

union point {
    struct {
        double x;
        double y;
        double z;
    };
    double coords[3] = {0, 0, 0};
};

struct edge {
    point point1;
    point point2;
};

struct object {
    std::list<edge> edges;
    double dx = 0;
    double dy = 0;
    double dz = 0;
    double rx = 0;
    double ry = 0;
    double rz = 0;
};

// Point offset

point movePoint(const point& point, double dx, double dy, double dz);

// Point rotate

point rotatePointX(const point& point, double angleX);
point rotatePointY(const point& point, double angleY);
point rotatePointZ(const point& point, double angleZ);
point rotatePoint(const point& point, double angleX, double angleY, double angleZ);

#endif
