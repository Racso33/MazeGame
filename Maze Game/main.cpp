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
Cell cells[16][16];
int cellSize;
int cellCoreSize;
double px = 0.5, py = 0.5;

int main(int argc, char **argv) {
    std::srand(std::time(0));

    gameSetup();

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("hi", 50, 50, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    windowId = SDL_GetWindowID(window);
    renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Init();
    font = TTF_OpenFont("D:\\Programming Libraries\\Roboto\\Roboto-Medium.ttf", 24);

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
    cellCoreSize = cellSize / 2;
    /* initialize cells */
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            cells[j][i].x = i;
            cells[j][i].y = j;
            cells[j][i].visited = false;
            for (int m = 0; m < 4; m++) {
                cells[j][i].walls[m] = true;
            }
        }
    }
    /* generate maze */
    generateMaze(&cells[0][0]);
}

void gameLoop() {
    /* sdl */
    SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
    SDL_RenderClear(renderer);

    /* do stuff */
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            /* cell core */ {
                SDL_Rect r = { cellSize * i + cellCoreSize/2
                             , cellSize * j + cellCoreSize/2
                             , cellCoreSize, cellCoreSize };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &r);
            }
            /* cell walls */
            for (int m = 0; m < 4; m++) {
                if (cells[j][i].walls[m]) {
                    continue;
                }
                SDL_Rect r;
                switch (m) {
                case 0:
                    r.x = (cellSize * i) + cellSize / 2 + cellSize / 4;
                    r.y = (cellSize * j) + cellSize / 4;
                    r.w = cellSize / 4;
                    r.h = cellSize / 2;
                    break;
                case 1:
                    r.x = (cellSize * i) + cellSize / 4;
                    r.y = (cellSize * j) + cellSize / 2 + cellSize/4;
                    r.w = cellSize / 2;
                    r.h = cellSize / 4;
                    break;
                case 2:
                    r.x = (cellSize * i);
                    r.y = (cellSize * j) + cellSize / 4;
                    r.w = cellSize / 4;
                    r.h = cellSize / 2;
                    break;
                default:
                    r.x = (cellSize * i) + cellSize / 4;
                    r.y = (cellSize * j);
                    r.w = cellSize / 2;
                    r.h = cellSize / 4;
                    break;
                }
                //SDL_Rect r = { (cellSize*1 * i) + (cellSize/2 * mx)
                //             , (cellSize * 1 * j) + (cellSize / 2 * my)
                //             , cellSize / 2, cellSize / 2};
                SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
                SDL_RenderFillRect(renderer, &r);
            }

            /* raycast */ {
                //double d = raycast(px,py, 0);
                //SDL_SetRenderDrawColor(renderer, 255, 0,0, 255);
                //SDL_RenderDrawLine(renderer, px*cellSize, py*cellSize, px * cellSize, (py + d) * cellSize);
            }
        }
    }

    /* sdl */
    SDL_RenderPresent(renderer);
}
void getCellAt(Cell** res, int x, int y) {
    if (x < 0 || x >= 16 || y < 0 || y >= 16) {
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
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
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