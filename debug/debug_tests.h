#include "config.h"

#ifdef DEBUG
#ifndef DEBUG_SHAPES
#define DEBUG
#include <Windows.h>
#include <time.h>
#include <math.h>
#include "printer.h"
#include "basic_shapes.h"

#define PI 3.1415926535

#define WIDTH 200
#define HEIGHT 100 //Must be half of width for a square aspect ratio if SQUISH_Y is defined
#define DegToRad(theta) theta*(PI)/180

void PreviewShape(enum ReferenceShapes shapeID);

#endif
#endif