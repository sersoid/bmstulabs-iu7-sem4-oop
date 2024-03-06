#include <cmath>

#include "render.h"

void drawLine(std::vector<std::vector<color>>& image, const point& point1, const point& point2, const color lineColor) {
    if (std::isinf(fabs(point1.x)) || std::isinf(fabs(point1.y)) || std::isinf(fabs(point2.x)) || std::isinf(fabs(point2.y)))
        return;

    int x1 = static_cast<int>(point1.x), y1 = static_cast<int>(point1.y);
    const int x2 = static_cast<int>(point2.x), y2 = static_cast<int>(point2.y);
    const int resolution = static_cast<int>(image.size());

    if (x1 < 0 && x2 < 0 || x1 >= resolution && x2 >= resolution || y1 < 0 && y2 < 0 || y1 >= resolution && y2 >= resolution)
        return;

    const int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int err = dx - dy;
    const int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;

    while (x1 != x2 || y1 != y2) {
        if (x1 < 0 && sx == -1 || x1 >= resolution && sx == 1 || y1 < 0 && sy == -1 || y1 >= resolution && sy == 1)
            break;

        if (x1 >= 0 && x1 < resolution && y1 >= 0 && y1 < resolution)
            image[x1][y1] = lineColor;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

point projectPoint(const point& targetPoint, const point& cameraPoint, const double dirX, const double dirY, const double dirZ, const int resolution) {
    point projectedPoint;

    const double dX = targetPoint.x - cameraPoint.x;
    const double dY = targetPoint.y - cameraPoint.y;
    const double dZ = targetPoint.z - cameraPoint.z;

    const double pointProduct = dX * dirX + dY * dirY + dZ * dirZ;

    if (pointProduct > 0) {
        projectedPoint.x = dX / pointProduct * resolution / 2 + static_cast<double>(resolution) / 2;
        projectedPoint.y = dY / pointProduct * resolution / 2 + static_cast<double>(resolution) / 2;
    }
    else
        projectedPoint = {INFINITY, INFINITY};

    return projectedPoint;
}

void initCoordSystemWithCamera(std::vector<std::vector<color>>& image, const point& camera, const double dirX, const double dirY, const double dirZ) {
    drawLine(image, projectPoint({-3, 0, 0}, camera, dirX, dirY, dirZ, static_cast<int>(image.size())), projectPoint({3, 0, 0}, camera, dirX, dirY, dirZ, static_cast<int>(image.size())), {255, 0 ,0});
    drawLine(image, projectPoint({0, -3, 0}, camera, dirX, dirY, dirZ, static_cast<int>(image.size())), projectPoint({0, 3, 0}, camera, dirX, dirY, dirZ, static_cast<int>(image.size())), {0, 255 ,0});
    drawLine(image, projectPoint({0, 0, -3}, camera, dirX, dirY, dirZ, static_cast<int>(image.size())), projectPoint({0, 0, 3}, camera, dirX, dirY, dirZ, static_cast<int>(image.size())), {0, 0 ,255});
}

void renderWithCamera(std::vector<std::vector<color>>& image, object& obj, const bool coordSystem, const int resolution) {
    constexpr point cameraFocus = {CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z}, camera = {CAMERA_X, CAMERA_Y, CAMERA_Z};

    double dirX = cameraFocus.x - camera.x;
    double dirY = cameraFocus.y - camera.y;
    double dirZ = cameraFocus.z - camera.z;

    const double dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

    dirX /= dirLength;
    dirY /= dirLength;
    dirZ /= dirLength;

    if (coordSystem)
        initCoordSystemWithCamera(image, camera, dirX, dirY, dirZ);

    for (auto [point1, point2] : obj.edges) {
        point1 = rotatePoint(point1, obj.rotation);
        point2 = rotatePoint(point2, obj.rotation);

        point1 = movePoint(point1, obj.center);
        point2 = movePoint(point2, obj.center);

        const point projectedPoint1 = projectPoint(point1, camera, dirX, dirY, dirZ, resolution);
        const point projectedPoint2 = projectPoint(point2, camera, dirX, dirY, dirZ, resolution);

        drawLine(image, projectedPoint1, projectedPoint2, {255, 255, 255});
    }
}

point projectPointWithoutCamera(const point& targetPoint, const int resolution) {
    point projectedPoint;

    projectedPoint.x = targetPoint.x * resolution / 4 + static_cast<double>(resolution) / 2;
    projectedPoint.y = targetPoint.y * resolution / 4 + static_cast<double>(resolution) / 2;

    return projectedPoint;
}

void initCoordSystemWithoutCamera(std::vector<std::vector<color>>& image) {
    drawLine(image, projectPointWithoutCamera({-3, 0, 0}, static_cast<int>(image.size())), projectPointWithoutCamera({3, 0, 0}, static_cast<int>(image.size())), {255, 0, 0});
    drawLine(image, projectPointWithoutCamera({0, -3, 0}, static_cast<int>(image.size())), projectPointWithoutCamera({0, 3, 0}, static_cast<int>(image.size())), {0, 255, 0});
}

void renderWithoutCamera(std::vector<std::vector<color>>& image, object& obj, const bool coordSystem, const int resolution) {
    if (coordSystem)
        initCoordSystemWithoutCamera(image);

    for (auto [point1, point2] : obj.edges) {
        point1 = rotatePoint(point1, obj.rotation);
        point2 = rotatePoint(point2, obj.rotation);

        point1 = movePoint(point1, obj.center);
        point2 = movePoint(point2, obj.center);

        const point projectedPoint1 = projectPointWithoutCamera(point1, resolution);
        const point projectedPoint2 = projectPointWithoutCamera(point2, resolution);

        drawLine(image, projectedPoint1, projectedPoint2, {255, 255, 255});
    }
}
