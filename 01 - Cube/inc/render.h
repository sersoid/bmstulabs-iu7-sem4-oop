#ifndef RENDER_H
#define RENDER_H

#include <vector>

#include "object.h"

#define CAMERA_X 3
#define CAMERA_Y 3
#define CAMERA_Z 4

#define CAMERA_TARGET_X 0
#define CAMERA_TARGET_Y 0
#define CAMERA_TARGET_Z 0

union color {
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    unsigned char values[3] = {0, 0, 0};
};

void drawLine(std::vector<std::vector<color>>& image, const point& point1, const point& point2, color lineColor);

point projectPointWithCamera(const point& targetPoint, const point& cameraPoint, double dirX, double dirY, double dirZ, int resolution);
void initCoordSystemWithCamera(std::vector<std::vector<color>>& image, const point& camera, double dirX, double dirY, double dirZ);
void renderWithCamera(std::vector<std::vector<color>>& image, object& obj, bool coordSystem, int resolution);

point projectPointWithoutCamera(const point& targetPoint, int resolution);
void initCoordSystemWithoutCamera(std::vector<std::vector<color>>& image);
void renderWithoutCamera(std::vector<std::vector<color>>& image, object& obj, bool coordSystem, int resolution);

#endif
