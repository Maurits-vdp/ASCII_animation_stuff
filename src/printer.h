#ifndef PRINTER_H
#define PRINTER_H

#include "config.h"
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef SQUISH_Y
#define SQUISH_FACTOR 1.0f
#else
#define SQUISH_FACTOR 0.5f
#endif

#ifndef ENABLE_DIAGONALS
#define DIAGS_ENABLED 0
#else
#define DIAGS_ENABLED 1
#endif

typedef struct {
    int cols;
    int rows;
    char* pImage;
} ImageBuffer;

void DrawVertices(ImageBuffer* image, Vec4* vertices, int* indices, int numIndices);
void InsertBrensenhamLine(Vec2* pPointI, Vec2* pPointF, ImageBuffer* image, char character);
void ClearImageBuffer(ImageBuffer* buffer);
void PrintImage(ImageBuffer* pImageBuf);

#endif