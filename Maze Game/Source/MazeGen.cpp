#include "MazeGame.h"

struct Cell {
    bool Visited;
    int x, y;
};
Cell* cells;
int directions[4][2] = { { 1,0 }, { 0,1}, {-1,0}, {0,-1} };

void InitCells(int w, int h) {
    cells = (Cell*)malloc(sizeof(Cell) * w * h);
    memset(cells, 0, sizeof(Cell) * w * h);
    for(int j=0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            Cell* c = &cells[i + j * w];
            c->x = i;
            c->y = j;
        }
    }
}
Cell* GetCell(int x, int y) {
    int mapw, maph;
    GetMap(&mapw, &maph);
    if (!(x >= 0 && x < mapw && y >= 0 && y < maph)) return 0;
    return &cells[x + y * mapw];
}

static void GenerateMazeGo(Cell* c) {
    c->Visited = true;
    /* recursing */
    while (true) {
        /* break out if no neighbours remaining */
        bool hasUnvisitedNeighbours = false;
        for (int i = 0; i < 4; i++) {
            Cell* neighbour;
            neighbour = GetCell(c->x + directions[i][0], c->y + directions[i][1]);
            if (neighbour && !neighbour->Visited) {
                hasUnvisitedNeighbours = true;
            }
        }
        if (!hasUnvisitedNeighbours) {
            break;
        }

        /* choose neighbour */
        int neighbournr = rand() % 4;
        Cell* neighbour;
        neighbour = GetCell(c->x + directions[neighbournr][0], c->y + directions[neighbournr][1]);
        if (neighbour && !neighbour->Visited) {
            /* Finding the wall 
               Removing it 
            */
            if (neighbournr == 0 || neighbournr == 2) {
                /* Vertical walls */
                int col = c->x + (neighbournr == 0 ? 1 : 0);
                int y = c->y;
                SetVWall(0, col, y);
            }
            else {
                /* Horizontal walls*/
                int row = c->y + (neighbournr == 1 ? 1 : 0);
                int x = c->x;
                SetHWall(0, row, x);
            }
            /* recurse */
            GenerateMazeGo(neighbour);
        }
    }
}
static void PrepMap() {
    int mapw, maph;
    GetMap(&mapw, &maph);
    for (int r = 0; r < maph+1; r++) {
        for (int c = 0; c < mapw+1; c++) {
            if (r < maph) SetVWall(1, c, r);
            if (c < mapw) SetHWall(1, r, c);
        }
    }
}
void GenerateMaze(Point origin) {
    int mapw, maph; 
    GetMap(&mapw, &maph);
    PrepMap();
    InitCells(mapw, maph);
    GenerateMazeGo(GetCell(origin.x, origin.y));
}

