#include <cmath>

#include "render.h"

// Drawing line

void drawLine(std::vector<std::vector<color>>& image, const edge& targetEdge, const color lineColor) {
    if (std::isinf(fabs(targetEdge.point1.x)) || std::isinf(fabs(targetEdge.point1.y)) || std::isinf(fabs(targetEdge.point2.x)) || std::isinf(fabs(targetEdge.point2.y)))
        return;

    int x1 = static_cast<int>(std::round(targetEdge.point1.x)), y1 = static_cast<int>(std::round(targetEdge.point1.y));
    const int x2 = static_cast<int>(std::round(targetEdge.point2.x)), y2 = static_cast<int>(std::round(targetEdge.point2.y));
    const int resolution = static_cast<int>(image.size());

    if (x1 < 0 && x2 < 0 || x1 >= resolution && x2 >= resolution || y1 < 0 && y2 < 0 || y1 >= resolution && y2 >= resolution)
        return;

    const int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int err = dx - dy;
    const int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;

    while ((x1 != x2 || y1 != y2) && ! (x1 < 0 && sx == -1 || x1 >= resolution && sx == 1 || y1 < 0 && sy == -1 || y1 >= resolution && sy == 1)) {
        if (x1 >= 0 && x1 < resolution && y1 >= 0 && y1 < resolution)
            image[x1][y1] = lineColor;

        const int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    if (x1 >= 0 && x1 < resolution && y1 >= 0 && y1 < resolution)
        image[x1][y1] = lineColor;
}

// Render with camera

point projectPointWithCamera(const point& targetPoint, const point& cameraPoint, const std::tuple<double, double, double>& dir, const int resolution) {
    point projectedPoint;

    const double dX = targetPoint.x - cameraPoint.x;
    const double dY = targetPoint.y - cameraPoint.y;
    const double dZ = targetPoint.z - cameraPoint.z;

    const double pointProduct = dX * std::get<0>(dir) + dY * std::get<1>(dir) + dZ * std::get<2>(dir);

    if (pointProduct > 0) {
        projectedPoint.x = dX / pointProduct * resolution / 2 + static_cast<double>(resolution) / 2;
        projectedPoint.y = dY / pointProduct * resolution / 2 + static_cast<double>(resolution) / 2;
    }
    else
        projectedPoint = {INFINITY, INFINITY};

    return projectedPoint;
}

void initCoordSystemWithCamera(std::vector<std::vector<color>>& image, const point& camera, const std::tuple<double, double, double>& dir) {
    const int resolution = static_cast<int>(image.size());

    drawLine(image, {projectPointWithCamera({-3, 0, 0}, camera, dir, resolution), projectPointWithCamera({3, 0, 0}, camera, dir, resolution)}, {255, 0 ,0});
    drawLine(image, {projectPointWithCamera({0, -3, 0}, camera, dir, resolution), projectPointWithCamera({0, 3, 0}, camera, dir, resolution)}, {0, 255 ,0});
    drawLine(image, {projectPointWithCamera({0, 0, -3}, camera, dir, resolution), projectPointWithCamera({0, 0, 3}, camera, dir, resolution)}, {0, 0 ,255});
}

void renderWithCamera(std::vector<std::vector<color>>& image, object& obj, const bool coordSystem) {
    const int resolution = static_cast<int>(image.size());
    constexpr point cameraFocus = {CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z}, camera = {CAMERA_X, CAMERA_Y, CAMERA_Z};

    constexpr double dirX = cameraFocus.x - camera.x;
    constexpr double dirY = cameraFocus.y - camera.y;
    constexpr double dirZ = cameraFocus.z - camera.z;

    const double dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    const std::tuple dir = {dirX / dirLength, dirY / dirLength, dirZ / dirLength};

    if (coordSystem)
        initCoordSystemWithCamera(image, camera, dir);

    for (auto [point1, point2] : obj.edges) {
        const edge projectedEdge = {
            projectPointWithCamera(movePoint(rotatePoint(point1, obj.rotation), obj.center), camera, dir, resolution),
            projectPointWithCamera(movePoint(rotatePoint(point2, obj.rotation), obj.center), camera, dir, resolution)
        };

        drawLine(image, projectedEdge, {255, 255, 255});
    }
}

// Render without camera

point projectPointWithoutCamera(const point& targetPoint, const int resolution) {
    point projectedPoint;

    projectedPoint.x = targetPoint.x * resolution / 4 + static_cast<double>(resolution) / 2;
    projectedPoint.y = targetPoint.y * resolution / 4 + static_cast<double>(resolution) / 2;

    return projectedPoint;
}

void initCoordSystemWithoutCamera(std::vector<std::vector<color>>& image) {
    const int resolution = static_cast<int>(image.size());

    drawLine(image, {projectPointWithoutCamera({-3, 0, 0}, resolution), projectPointWithoutCamera({3, 0, 0}, static_cast<int>(image.size()))}, {255, 0, 0});
    drawLine(image, {projectPointWithoutCamera({0, -3, 0}, resolution), projectPointWithoutCamera({0, 3, 0}, static_cast<int>(image.size()))}, {0, 255, 0});
}

void renderWithoutCamera(std::vector<std::vector<color>>& image, object& obj, const bool coordSystem) {
    const int resolution = static_cast<int>(image.size());

    if (coordSystem)
        initCoordSystemWithoutCamera(image);

    for (auto [point1, point2] : obj.edges) {
        const edge projectedEdge = {
            projectPointWithoutCamera(movePoint(rotatePoint(point1, obj.rotation), obj.center), resolution),
            projectPointWithoutCamera(movePoint(rotatePoint(point2, obj.rotation), obj.center), resolution)
        };

        drawLine(image, projectedEdge, {255, 255, 255});
    }
}
