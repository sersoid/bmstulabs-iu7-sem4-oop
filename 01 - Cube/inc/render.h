#ifndef __H_RENDER
#define __H_RENDER

#include <QImage>

#include "object.h"
#include "ui.h"

#define CAMERA_X 3
#define CAMERA_Y 3
#define CAMERA_Z 3

#define CAMERA_TARGET_X 0
#define CAMERA_TARGET_Y 0
#define CAMERA_TARGET_Z 0

#define CAMERA_FOV 90

void drawLine(QImage& image, const point& point1, const point& point2, QColor color);
point projectPoint(const point& targetPoint, const point& cameraPoint, double dirX, double dirY, double dirZ, int resolution);
QImage initCoordSystem(QImage& image, const point& camera, double dirX, double dirY, double dirZ);
void render(object& obj, QLabel& resultLabel, bool coordSystem, int resolution);

#endif
