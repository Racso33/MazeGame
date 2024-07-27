#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "GameIO.h"

struct ImageInfo {
    const char* path;
    SDL_Texture* texture;
};

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* surface;
SDL_Texture* texture;
TTF_Font* font;
int windowId;
int windowWidth;
int windowHeight;
SDL_Event events[64];
int eventsCount;
bool quit;
ImageInfo loadedImages[64];
int loadedImagesCount;
int mouseX, mouseY;
bool mousePressed[3];
int mouseScroll;
bool keydown[255];
int bgcolor[3];

static void RectSetDims(SDL_Rect* r, int x, int y, int w, int h) {
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
}
static SDL_Texture* LoadImage(const char* path) {
    SDL_Surface* loadedImage;
    SDL_Texture* texture;

    loadedImage = SDL_LoadBMP(path);
    if (loadedImage != 0) {
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        loadedImages[loadedImagesCount].path = path;
        loadedImages[loadedImagesCount].texture = texture;
        loadedImagesCount++;
        SDL_FreeSurface(loadedImage);
        return texture;
    }
    return 0;
}

void IOInit() {
    SDL_Init(SDL_INIT_VIDEO);
    windowWidth = 640;
    windowHeight = 480;
    window = SDL_CreateWindow("hi", 50, 50, 640, 480, SDL_WINDOW_RESIZABLE);
    windowId = SDL_GetWindowID(window);
    renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Init();
    font = TTF_OpenFont("E:\\Programming Libraries\\Roboto\\Roboto-Medium.ttf", 24);
}
bool IOShouldClose() {
    return quit;
}
void IODrawText(const char* text, int x, int y) {
    SDL_Surface* s;
    SDL_Texture* t;
    SDL_Color c = { 30,30,30, 255 };
    SDL_Rect re = { x, y, 0, 0 };

    s = TTF_RenderText_Solid(font, text, c);
    re.w = s->w;
    re.h = s->h;
    t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_RenderCopy(renderer, t, 0, &re);

    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}
void IOFrame() {
    SDL_Event ev;
    int i;

    /*for (i = 0; i < 3; i++) {
        mousePressed[i] = false;
    }*/
    mouseScroll = 0;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_WINDOWEVENT:
            if (ev.window.windowID == windowId) {
                switch (ev.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    windowWidth = ev.window.data1;
                    windowHeight = ev.window.data2;
                    break;

                default:
                    break;
                }
            }
            break;

        case SDL_MOUSEMOTION:
            mouseX = ev.motion.x;
            mouseY = ev.motion.y;
            break;

        case SDL_MOUSEWHEEL:
            mouseScroll = ev.wheel.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mousePressed[ev.button.button - 1] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mousePressed[ev.button.button - 1] = false;
            break;

        case SDL_KEYDOWN:
            if(ev.key.keysym.sym >= 0 && ev.key.keysym.sym < 255)
                keydown[ev.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            if (ev.key.keysym.sym >= 0 && ev.key.keysym.sym < 255)
                keydown[ev.key.keysym.sym] = false;
            break;

        default:
            break;
        }
    }
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, bgcolor[0], bgcolor[1], bgcolor[2], 255);
    SDL_RenderClear(renderer);
}
void IODrawImage(const char* imagePath, int x, int y, int w, int h) {
    SDL_Texture* image;
    SDL_Rect rect;
    int i;

    image = 0;
    for (i = 0; i < loadedImagesCount; i++) {
        if (!strcmp(imagePath, loadedImages[i].path)) {
            image = loadedImages[i].texture;
        }
    }
    if (!image) {
        image = LoadImage(imagePath);
    }
    RectSetDims(&rect, x, y, w, h);
    SDL_RenderCopy(renderer, image, 0, &rect);
}
void IOGetWindowDims(int* x, int* y) {
    *x = windowWidth;
    *y = windowHeight;
}
void IODrawRect(int x, int y, int w, int h, int cr, int g, int b) {
    SDL_Rect r;

    RectSetDims(&r, x, y, w, h);
    SDL_SetRenderDrawColor(renderer, cr, g, b, 255);
    SDL_RenderFillRect(renderer, &r);
}
void IODrawBorderedRect(Rect r, Color inner, Color outer, int size) {
    IODrawRect(r.x, r.y, r.w, r.h, outer.r, outer.g, outer.b);
    r.x += size;
    r.y += size;
    r.w -= size * 2;
    r.h -= size * 2;
    IODrawRect(r.x, r.y, r.w, r.h, inner.r, inner.g, inner.b);
}
void IOGetMousePos(int* x, int* y) {
    *x = mouseX;
    *y = mouseY;
}
bool IOGetMousePressed(int button) {
    return mousePressed[button];
}
int IOGetMouseScroll() {
    return mouseScroll;
}
bool IOMouseWithinRect(Rect r) {
    int mx, my;

    IOGetMousePos(&mx, &my);
    return mx >= r.x && mx <= r.x + r.w
        && my >= r.y && my <= r.y + r.h;
}
void IODrawLine(Point p1, Point p2, int thick) {
    SDL_SetRenderDrawColor(renderer, 255, 30, 30, 255);
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}
bool IOKeyIsDown(int key) {
    return keydown[key];
}
void IOSetBgColor(int r, int g, int b) {
    bgcolor[0] = r;
    bgcolor[1] = g;
    bgcolor[2] = b;
}