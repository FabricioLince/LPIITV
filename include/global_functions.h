#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include "defines.h"
#include "gui.h"

void init();
void deinit();
void inputTextBox(char*box, int MAX_LENGHT);
bool testMousePos(int x1, int y1, int x2, int y2);
bool testMousePos(Pos*p);
bool testMousePos(Pos*p, int x, int y);
void change_screen(BITMAP*old, int division, BITMAP*newOne, int transition);

#endif // GLOBAL_FUNCTIONS_H
