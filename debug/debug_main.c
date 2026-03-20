/*
Note to self: gcc -Isrc -Idebug debug/debug_main.c debug/debug_tests.c src/printer.c src/basic_shapes.c src/geometry.c -o debug/debug_test
Remember to do this from the ASCII_animation_stuff directory
*/
#include "config.h"
#ifdef DEBUG

#include "debug_tests.h"

int main(){
    PreviewShape(TETRAHEDRON);
}

#endif