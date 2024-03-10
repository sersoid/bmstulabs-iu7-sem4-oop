#include <cmath>

#include "error.h"
#include "render.h"

// Drawing line

int drawLine(std::vector<std::vector<color>>& image, const edge& targetEdge, const color lineColor) {
    int rc = OK;

    if (std::isinf(fabs(targetEdge.point1.x)) || std::isinf(fabs(targetEdge.point1.y)) || std::isinf(fabs(targetEdge.point2.x)) || std::isinf(fabs(targetEdge.point2.y)))
        rc |= EDGE_OUT_OF_VIEW;

    int x1 = static_cast<int>(std::round(targetEdge.point1.x)), y1 = static_cast<int>(std::round(targetEdge.point1.y));
    const int x2 = static_cast<int>(std::round(targetEdge.point2.x)), y2 = static_cast<int>(std::round(targetEdge.point2.y));
    const int resolution = static_cast<int>(image.size());

    if (x1 < 0 && x2 < 0 || x1 >= resolution && x2 >= resolution || y1 < 0 && y2 < 0 || y1 >= resolution && y2 >= resolution)
        rc |= EDGE_OUT_OF_VIEW;

    if (rc == OK) {
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

    return rc;
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

std::vector<int> initCoordSystemWithCamera(std::vector<std::vector<color>>& image, const point& camera, const std::tuple<double, double, double>& dir) {
    std::vector<int> rc;
    const int resolution = static_cast<int>(image.size());

    rc.push_back(drawLine(image, {projectPointWithCamera({-3, 0, 0}, camera, dir, resolution), projectPointWithCamera({3, 0, 0}, camera, dir, resolution)}, {255, 0 ,0}));
    rc.push_back(drawLine(image, {projectPointWithCamera({0, -3, 0}, camera, dir, resolution), projectPointWithCamera({0, 3, 0}, camera, dir, resolution)}, {0, 255 ,0}));
    rc.push_back(drawLine(image, {projectPointWithCamera({0, 0, -3}, camera, dir, resolution), projectPointWithCamera({0, 0, 3}, camera, dir, resolution)}, {0, 0 ,255}));

    return rc;
}

std::map<std::string, std::vector<int>> renderWithCamera(std::vector<std::vector<color>>& image, const object& obj, const bool coordSystem) {
    std::map<std::string, std::vector<int>> rc;
    std::vector<int> rcEdges;

    const int resolution = static_cast<int>(image.size());
    constexpr point cameraFocus = {CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z}, camera = {CAMERA_X, CAMERA_Y, CAMERA_Z};

    constexpr double dirX = cameraFocus.x - camera.x;
    constexpr double dirY = cameraFocus.y - camera.y;
    constexpr double dirZ = cameraFocus.z - camera.z;

    const double dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    const std::tuple dir = {dirX / dirLength, dirY / dirLength, dirZ / dirLength};

    if (coordSystem)
        rc["axis"] = initCoordSystemWithCamera(image, camera, dir);

    for (const edge targetEdge : obj.edges) {
        const edge projectedEdge = {
            projectPointWithCamera(movePoint(rotatePoint(targetEdge.point1, obj.rotation), obj.center), camera, dir, resolution),
            projectPointWithCamera(movePoint(rotatePoint(targetEdge.point2, obj.rotation), obj.center), camera, dir, resolution)
        };

        rcEdges.push_back(drawLine(image, projectedEdge, {255, 255, 255}));
    }

    rc["edges"] = rcEdges;

    return rc;
}

// Render without camera

point projectPointWithoutCamera(const point& targetPoint, const int resolution) {
    const point projectedPoint = {
        targetPoint.x * resolution / 4 + static_cast<double>(resolution) / 2,
        targetPoint.y * resolution / 4 + static_cast<double>(resolution) / 2
    };

    return projectedPoint;
}

std::vector<int> initCoordSystemWithoutCamera(std::vector<std::vector<color>>& image) {
    std::vector<int> rc;
    const int resolution = static_cast<int>(image.size());

    rc.push_back(drawLine(image, {projectPointWithoutCamera({-3, 0, 0}, resolution), projectPointWithoutCamera({3, 0, 0}, resolution)}, {255, 0, 0}));
    rc.push_back(drawLine(image, {projectPointWithoutCamera({0, -3, 0}, resolution), projectPointWithoutCamera({0, 3, 0}, resolution)}, {0, 255, 0}));

    return rc;
}

std::map<std::string, std::vector<int>> renderWithoutCamera(std::vector<std::vector<color>>& image, const object& obj, const bool coordSystem) {
    std::map<std::string, std::vector<int>> rc;
    std::vector<int> rcEdges;
    const int resolution = static_cast<int>(image.size());

    if (coordSystem)
        rc["axis"] = initCoordSystemWithoutCamera(image);

    for (const edge targetEdge : obj.edges) {
        const edge projectedEdge = {
            projectPointWithoutCamera(movePoint(rotatePoint(targetEdge.point1, obj.rotation), obj.center), resolution),
            projectPointWithoutCamera(movePoint(rotatePoint(targetEdge.point2, obj.rotation), obj.center), resolution)
        };

        rcEdges.push_back(drawLine(image, projectedEdge, {255, 255, 255}));
    }

    rc["edges"] = rcEdges;

    return rc;
}
