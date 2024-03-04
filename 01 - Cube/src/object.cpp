#include <cmath>

#include "object.h"

// Point move

point movePoint(const point& point, const double dx, const double dy, const double dz) {
    return {point.x + dx, point.y + dy, point.z + dz};
}

// Point rotate

point rotatePointX(const point& point, const double angleX) {
    const double radAngle = angleX * M_PI / 180, cosAngle = cos(radAngle), sinAngle = sin(radAngle);

    return {point.x, point.y * cosAngle - point.z * sinAngle, point.y * sinAngle + point.z * cosAngle};
}

point rotatePointY(const point& point, const double angleY) {
    const double radAngle = angleY * M_PI / 180, cosAngle = cos(radAngle), sinAngle = sin(radAngle);

    return {point.x * cosAngle + point.z * sinAngle, point.y, -point.x * sinAngle + point.z * cosAngle};
}

point rotatePointZ(const point& point, const double angleZ) {
    const double radAngle = angleZ * M_PI / 180, cosAngle = cos(radAngle), sinAngle = sin(radAngle);

    return {point.x * cosAngle - point.y * sinAngle, point.x * sinAngle + point.y * cosAngle, point.z};
}

point rotatePoint(const point& point, const double angleX, const double angleY, const double angleZ) {
    return rotatePointX(rotatePointY(rotatePointZ(point, angleZ), angleY), angleX);
}
