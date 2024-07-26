#pragma once

struct Rect {
    int x, y, w, h;
};
struct Point {
    double x, y;
};
struct Color {
    int r, g, b, a;
};
#define ColorBlack { 30,30,30,255 }
#define ColorWhite { 255,255,255,255 }

bool IOShouldClose();
void IOInit();
void IODrawText(const char* text, int x, int y);
void IOFrame();
void IODrawImage(const char* imagePath, int x, int y, int w, int h);
void IOGetWindowDims(int* x, int* y);
void IODrawRect(int x, int y, int w, int h, int r, int g, int b);
void IODrawBorderedRect(Rect r, Color inner, Color outer, int size);
void IOGetMousePos(int* x, int* y);
bool IOGetMousePressed(int button);
int IOGetMouseScroll();
bool IOMouseWithinRect(Rect r);
void IODrawLine(Point p1, Point p2, int thick);
bool IOKeyIsDown(int key);