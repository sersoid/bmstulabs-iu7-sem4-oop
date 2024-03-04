#include <thread>

#include <QPixmap>

#include "render.h"

void drawLine(QImage& image, const point& point1, const point& point2, const QColor color) {
    if (std::isinf(fabs(point1.x)) || std::isinf(fabs(point1.y)) || std::isinf(fabs(point2.x)) || std::isinf(fabs(point2.y)))
        return;

    int x1 = static_cast<int>(point1.x), y1 = static_cast<int>(point1.y);
    const int x2 = static_cast<int>(point2.x), y2 = static_cast<int>(point2.y);
    const int resolution = image.width();

    if (x1 < 0 && x2 < 0 || x1 >= resolution && x2 >= resolution || y1 < 0 && y2 < 0 || y1 >= resolution && y2 >= resolution)
        return;

    int dx = abs(x2 - x1), dy = abs(y2 - y1), err = dx - dy;
    const int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;

    while (x1 != x2 || y1 != y2) {
        if (x1 < 0 && sx == -1 || x1 >= resolution && sx == 1 || y1 < 0 && sy == -1 || y1 >= resolution && sy == 1)
            break;

        if (x1 >= 0 && x1 < resolution && y1 >= 0 && y1 < resolution)
            image.setPixel(x1, y1, color.rgb());

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

    const double dx = targetPoint.x - cameraPoint.x;
    const double dy = targetPoint.y - cameraPoint.y;
    const double dz = targetPoint.z - cameraPoint.z;

    const double pointProduct = dx * dirX + dy * dirY + dz * dirZ;

    if (pointProduct > 0) {
        projectedPoint.x = dx / pointProduct * resolution / 2 + resolution / 2;
        projectedPoint.y = dy / pointProduct * resolution / 2 + resolution / 2;
    }
    else
        projectedPoint = {INFINITY, INFINITY};

    return projectedPoint;
}

QImage initCoordSystem(QImage& image, const point& camera, const double dirX, const double dirY, const double dirZ) {
    drawLine(image, projectPoint({-3, 0, 0}, camera, dirX, dirY, dirZ, image.height()), projectPoint({3, 0, 0}, camera, dirX, dirY, dirZ, image.height()), Qt::red);
    drawLine(image, projectPoint({0, -3, 0}, camera, dirX, dirY, dirZ, image.height()), projectPoint({0, 3, 0}, camera, dirX, dirY, dirZ, image.height()), Qt::green);
    drawLine(image, projectPoint({0, 0, -3}, camera, dirX, dirY, dirZ, image.height()), projectPoint({0, 0, 3}, camera, dirX, dirY, dirZ, image.height()), Qt::blue);

    return image;
}

void render(object& obj, QLabel& resultLabel, const bool coordSystem, const int resolution) {
    constexpr point cameraFocus = {CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z}, camera = {CAMERA_X, CAMERA_Y, CAMERA_Z};

    double dirX = cameraFocus.x - camera.x;
    double dirY = cameraFocus.y - camera.y;
    double dirZ = cameraFocus.z - camera.z;

    const double dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

    if (dirLength == 0)
        return;

    QImage image(resolution, resolution, QImage::Format_RGB32);
    image.fill(Qt::black);

    dirX /= dirLength;
    dirY /= dirLength;
    dirZ /= dirLength;

    if (coordSystem)
        initCoordSystem(image, camera, dirX, dirY, dirZ);

    for (auto [point1, point2] : obj.edges) {
        point1 = rotatePoint(point1, obj.rx, obj.ry, obj.rz);
        point2 = rotatePoint(point2, obj.rx, obj.ry, obj.rz);

        point1 = movePoint(point1, obj.dx, obj.dy, obj.dz);
        point2 = movePoint(point2, obj.dx, obj.dy, obj.dz);

        const point projectedPoint1 = projectPoint(point1, camera, dirX, dirY, dirZ, resolution);
        const point projectedPoint2 = projectPoint(point2, camera, dirX, dirY, dirZ, resolution);

        drawLine(image, projectedPoint1, projectedPoint2, Qt::white);
    }

    resultLabel.setPixmap(QPixmap::fromImage(image).scaled(resultLabel.width(), resultLabel.height(), Qt::KeepAspectRatio));
}
