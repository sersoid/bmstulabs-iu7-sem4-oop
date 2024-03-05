#include <cmath>

#include "object.h"

// Point move

point movePointX(const point& targetPoint, const double dX) {
    return {targetPoint.x + dX, targetPoint.y, targetPoint.z};
}

point movePointY(const point& targetPoint, const double dY) {
    return {targetPoint.x, targetPoint.y + dY, targetPoint.z};
}

point movePointZ(const point& targetPoint, const double dZ) {
    return {targetPoint.x, targetPoint.y, targetPoint.z + dZ};
}

point movePoint(const point& targetPoint, const point& objCenter) {
    return movePointX(movePointY(movePointZ(targetPoint, objCenter.z), objCenter.y), objCenter.x);
}

// Point rotate

point rotatePointX(const point& targetPoint, const double angleX) {
    const double radAngle = angleX * M_PI / 180, cosAngle = cos(radAngle), sinAngle = sin(radAngle);

    return {targetPoint.x, targetPoint.y * cosAngle - targetPoint.z * sinAngle, targetPoint.y * sinAngle + targetPoint.z * cosAngle};
}

point rotatePointY(const point& targetPoint, const double angleY) {
    const double radAngle = angleY * M_PI / 180, cosAngle = cos(radAngle), sinAngle = sin(radAngle);

    return {targetPoint.x * cosAngle + targetPoint.z * sinAngle, targetPoint.y, -targetPoint.x * sinAngle + targetPoint.z * cosAngle};
}

point rotatePointZ(const point& targetPoint, const double angleZ) {
    const double radAngle = angleZ * M_PI / 180, cosAngle = cos(radAngle), sinAngle = sin(radAngle);

    return {targetPoint.x * cosAngle - targetPoint.y * sinAngle, targetPoint.x * sinAngle + targetPoint.y * cosAngle, targetPoint.z};
}

point rotatePoint(const point& targetPoint, const point& rotation) {
    return rotatePointX(rotatePointY(rotatePointZ(targetPoint, rotation.z), rotation.y), rotation.x);
}
