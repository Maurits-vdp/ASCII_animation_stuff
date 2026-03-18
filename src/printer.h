#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int cols;
    int rows;
    char* pImage;
} ImageBuffer;

void DrawVertices(ImageBuffer* image, Vec4* vertices, int* indices);
void InsertBrensenhamLine(Vec2* pPointI, Vec2* pPointF, ImageBuffer* image, char character);
void ClearImageBuffer(ImageBuffer* buffer);
void PrintImage(ImageBuffer* pImageBuf);