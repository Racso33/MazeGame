#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>

struct Mouse {
    int x, y;
    bool wasClicked;
};
struct Cell {
    int x, y;
    bool visited;
    bool walls[4];
};
struct Rect {
    double x, y, w, h;

    bool overlapsPoint(double x2, double y2) {
        return x2 >= x && x2 <= x + w
            && y2 >= y && y2 <= y + h;
    }
};
int directions[4][2] = { { 1,0 }, { 0,1}, {-1,0}, {0,-1} };
void drawText(const char* text, int x, int y, int w, int h, int r, int g, int b);
void sdlMessages();
void gameLoop();
void gameSetup();
void getCellAt(Cell** res, int x, int y);
void generateMaze(Cell* cell);
int reverseDirection(int n);
void getAllRectangles(std::vector<Rect>* res);
double raycast(double x, double y, double angle);
void drawCell(Cell* c);
bool isJunction(int x, int y);
bool canTraverse(int x1, int y1, int x2, int y2);
int getDirection(int x, int y);
void initCells();

/* sdl */
SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* surface;
SDL_Texture* texture;
TTF_Font* font;
Mouse mouse;
int windowWidth, windowHeight;
int windowId;
bool quit;
/* game */
#define MAZEWIDTH 8
#define MAZEHEIGHT 8
Cell cells[MAZEHEIGHT][MAZEHEIGHT];
int cellSize;
double px = 0.5, py = 0.5;
bool pHasTarget;
int pDir;

int main(int argc, char **argv) {
    std::srand(std::time(0));

    gameSetup();

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("hi", 50, 50, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    windowId = SDL_GetWindowID(window);
    renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Init();
    font = TTF_OpenFont("E:\\Programming Libraries\\Roboto\\Roboto-Medium.ttf", 24);

    while (!quit) {
        // sdl stuff
        sdlMessages();

        /* game loop */
        gameLoop();
    }

    return 0;
}

void drawText(const char* text, int x, int y, int w, int h, int r, int g, int b) {
    SDL_Surface* s;
    SDL_Texture* t;
    SDL_Color c = { r, g, b, 255 };
    SDL_Rect re = { x, y, w, h };

    s = TTF_RenderText_Solid(font, text, c);
    t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_RenderCopy(renderer, t, 0, &re);

    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

void sdlMessages() {
    SDL_Event event;
    mouse.wasClicked = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;

            /* window events */
        case SDL_WINDOWEVENT:
            if (event.window.windowID == windowId) {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    break;

                default:
                    break;
                }
            }
            break;

        case SDL_MOUSEMOTION:
            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouse.wasClicked = true;
            break;

        case SDL_KEYDOWN:
            px += 1;
            break;

        default:
            break;
        }
    }
}

void gameSetup() {
    windowWidth = 1280;
    windowHeight = 960;
    cellSize = 56;
    pDir = 0;
    initCells();
    /* generate maze */
    generateMaze(&cells[rand() % MAZEHEIGHT][rand() % MAZEWIDTH]);
}

void gameLoop() {
    /* sdl */
    SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
    SDL_RenderClear(renderer);

    /* do stuff */
    for (int j = 0; j < MAZEHEIGHT; j++) {
        for (int i = 0; i < MAZEWIDTH; i++) {
            drawCell(&cells[j][i]);
        }
    }
    if (canTraverse(px,py, px+directions[pDir][0],py+directions[pDir][1])) {
        px += directions[pDir][0];
        py += directions[pDir][1];
    }
    if(!canTraverse(px, py, px + directions[pDir][0], py + directions[pDir][1]) || isJunction(px,py)) {
        pDir = rand() % 4;
    }
    if ((int)px == MAZEWIDTH-1 && (int)py == MAZEHEIGHT-1) {
        initCells();
        generateMaze(&cells[rand() % MAZEHEIGHT][rand() % MAZEWIDTH]);
        px = 0.5;
        py = 0.5;
    }
    SDL_Rect r = { px * cellSize, py * cellSize, 30,30 };
    SDL_SetRenderDrawColor(renderer, 255, 120, 120, 255);
    SDL_RenderFillRect(renderer, &r);

    /* sdl */
    SDL_RenderPresent(renderer);
}
void getCellAt(Cell** res, int x, int y) {
    if (x < 0 || x >= MAZEWIDTH || y < 0 || y >= MAZEHEIGHT) {
        *res = 0;
        return;
    }

    *res = &cells[y][x];
}
void generateMaze(Cell* cell) {
    cell->visited = true;
    /* recursing */
    while (true) {
        /* break out if no neighbours remaining */
        bool hasUnvisitedNeighbours = false;
        for (int i = 0; i < 4; i++) {
            Cell* neighbour;
            getCellAt(&neighbour, cell->x + directions[i][0], cell->y + directions[i][1]);
            if (neighbour && !neighbour->visited) {
                hasUnvisitedNeighbours = true;
            }
        }
        if (!hasUnvisitedNeighbours) {
            break;
        }

        /* choose neighbour */ 
        int neighbournr = rand() % 4;
        Cell* neighbour;
        getCellAt(&neighbour, cell->x + directions[neighbournr][0], cell->y + directions[neighbournr][1]);
        if (neighbour && !neighbour->visited) {
            /* remove walls */
            cell->walls[neighbournr] = false;
            neighbour->walls[reverseDirection(neighbournr)] = false;
            /* recurse */
            generateMaze(neighbour);
        }
    }
}
int reverseDirection(int n) {
    for (int i = 2; i > 0; i--) {
        n--;
        if (n == 4) {
            n = 0;
        }
        if (n == -1) {
            n = 3;
        }
    }
    return n;
}

double raycast(double x, double y, double angle) {
    double mx = cos(angle) * 0.1, my = sin(angle) * 0.1;
    double ix = x, iy = y;
    while (true) {
        /* check for hits */
        bool hit = true;
        std::vector<Rect> rects;
        getAllRectangles(&rects);
        for (int i = 0; i < rects.size(); i++) {
            if (rects[i].overlapsPoint(ix,iy)) {
                hit = false;
            }
        }
        if (hit) {
            break;
        }

        ix += mx;
        iy += my;
    }
    return hypot(ix-x, iy-y);
}

void getAllRectangles(std::vector<Rect>* res) {
    for (int j = 0; j < MAZEHEIGHT; j++) {
        for (int i = 0; i < MAZEWIDTH; i++) {
            Rect r;
            /* cell core */
            r.x = cells[j][i].x + 0.25;
            r.y = cells[j][i].y + 0.25;
            r.w = 0.5;
            r.h = 0.5;
            res->push_back(r);
            /* walls */
            for (int m = 0; m < 4; m++) {
                if (!cells[j][i].walls[m]) {
                    continue;
                }
                switch (m) {
                case 0:
                    r.x = cells[j][i].x + 0.5 + + 0.25;
                    r.y = cells[j][i].y + 0.25;
                    r.w = 0.25;
                    r.h = 0.5;
                    break;
                case 1:
                    r.x = cells[j][i].x + 0.25;
                    r.y = cells[j][i].y + 0.5 + 0.25;
                    r.w = 0.5;
                    r.h = 0.25;
                    break;
                case 2:
                    r.x = cells[j][i].x;
                    r.y = cells[j][i].y + 0.25;
                    r.w = 0.25;
                    r.h = 0.5;
                    break;
                default:
                    r.x = cells[j][i].x + 0.25;
                    r.y = cells[j][i].y;
                    r.w = 0.5;
                    r.h = 0.25;
                    break;
                }
                res->push_back(r);
            }
        }
    }
}
void drawCell(Cell* c) {
    SDL_Rect r = { c->x * cellSize, c->y * cellSize, cellSize,cellSize };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &r);
    /* walls */
    SDL_SetRenderDrawColor(renderer, 255, 30, 30, 255);
    SDL_Rect w;
    if (c->walls[2]) {
        w.x = r.x;
        w.w = cellSize / 10;
        w.y = r.y;
        w.h = r.h;
        SDL_RenderFillRect(renderer, &w);
    }
    if (c->walls[0]) {
        w.x = r.x + cellSize - (cellSize / 10);
        w.w = cellSize / 10;
        w.y = r.y;
        w.h = r.h;
        SDL_RenderFillRect(renderer, &w);
    }
    if (c->walls[3]) {
        w.x = r.x;
        w.w = r.w;
        w.y = r.y;
        w.h = cellSize / 10;
        SDL_RenderFillRect(renderer, &w);
    }
    if (c->walls[1]) {
        w.x = r.x;
        w.w = r.w;
        w.y = r.y + cellSize - (cellSize / 10);
        w.h = cellSize / 10;
        SDL_RenderFillRect(renderer, &w);
    }
}
bool isJunction(int x, int y) {
    int openWalls = 0;
    Cell* c = &cells[y][x];
    for (int i = 0; i < 4; i++) {
        if (!c->walls[i]) {
            openWalls++;
        }
    }
    return openWalls >= 3;
}
bool canTraverse(int x1, int y1, int x2, int y2) {
    Cell* c = &cells[y1][x1];
    int d = getDirection(x2 - x1, y2 - y1);
    return !c->walls[d];
}
int getDirection(int x, int y) {
    for (int i = 0; i < 4; i++) {
        if (x == directions[i][0] && y == directions[i][1]) {
            return i;
        }
    }
    return -1;
}
void initCells() {
    for (int j = 0; j < MAZEHEIGHT; j++) {
        for (int i = 0; i < MAZEWIDTH; i++) {
            cells[j][i].x = i;
            cells[j][i].y = j;
            cells[j][i].visited = false;
            for (int m = 0; m < 4; m++) {
                cells[j][i].walls[m] = true;
            }
        }
    }
}