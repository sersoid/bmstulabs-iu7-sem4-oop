#ifndef RENDER_H
#define RENDER_H

#include <QImage>
#include <QLabel>

#include "object.h"

#define CAMERA_X 3
#define CAMERA_Y 3
#define CAMERA_Z 4

#define CAMERA_TARGET_X 0
#define CAMERA_TARGET_Y 0
#define CAMERA_TARGET_Z 0

void drawLine(QImage& image, const point& point1, const point& point2, QColor color);

point projectPointWithCamera(const point& targetPoint, const point& cameraPoint, double dirX, double dirY, double dirZ, int resolution);
QImage initCoordSystemWithCamera(QImage& image, const point& camera, double dirX, double dirY, double dirZ);
void renderWithCamera(object& obj, QLabel& resultLabel, bool coordSystem, int resolution);

point projectPointWithoutCamera(const point& targetPoint, int resolution);
QImage initCoordSystemWithoutCamera(QImage& image);
void renderWithoutCamera(object& obj, QLabel& resultLabel, bool coordSystem, int resolution);

#endif
