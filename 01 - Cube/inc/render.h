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

int renderWithCamera(std::vector<std::vector<color>>& image, const object& obj, bool coordSystem);
int renderWithoutCamera(std::vector<std::vector<color>>& image, const object& obj, bool coordSystem);

#endif
