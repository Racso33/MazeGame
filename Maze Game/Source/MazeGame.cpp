#include "MazeGame.h"

extern int directions[4][2];
Point player;
int playerd;
double playerang;
double fov;
int raycount;
int timer;
double nplayerang;

int MinimapScale = 10;
int MinimapWallScale = 1;

Rect Minimap_GetVWall(int col, int y) {
    return { col * MinimapScale, y * MinimapScale, MinimapWallScale, MinimapScale };
}
Rect Minimap_GetHWall(int row, int x) {
    return { x * MinimapScale, row * MinimapScale, MinimapScale, MinimapWallScale };
}
Point Minimap_GetPoint(double x, double y) {
    return { x * MinimapScale, y * MinimapScale };
}
Point Minimap_GetPointRev(int x, int y) {
    return { (double)x / MinimapScale, (double)y / MinimapScale };
}

void GenerateMaze(Point origin);

int DirTurn(int i, char d) {
    if (d == 'r') i += 1;
    if (d == 'l') i -= 1;
    if (d == 'b') i += 2;
    if (i > 3) {
        i = 0 + i - 4;
    }
    if (i < 0) {
        i = 3 + i - -1;
    }
    return i;
}
double AngDirTurn(double i, double amm) {
    i += amm;
    if (i > M_PI) {
        i = -M_PI + (i - M_PI);
    }
    if (i < -M_PI) {
        i = M_PI + (i - -M_PI);
    }
    return i;
}
double ToAngle(double i) {
    return fmod(i, M_PI);
}
double AngAngDirTurn(double playerang, double nplayerang) {
    return -1;
}

void DrawMinimap() {
    bool mouse = false;
    int i, j;
    int hcellcount, vcellcount;
    GetMap(&hcellcount, &vcellcount);
    IODrawRect(0, 0, hcellcount * MinimapScale, vcellcount * MinimapScale, 30, 30, 30);
    /* vertical */
    for(j=0; j < vcellcount; j++) {
        for (i = 0; i < hcellcount + 1; i++) {
            Rect r = Minimap_GetVWall(i, j);
            int w = GetVWall(i, j);
            if(w) IODrawRect(r.x, r.y, r.w, r.h, 220, 220, 220);
        }
    }

    for (i = 0; i < hcellcount; i++) {
        for (j = 0; j < vcellcount + 1; j++) {
            Rect r = Minimap_GetHWall(j, i);
            int w = GetHWall(j, i);
            if(w) IODrawRect(r.x, r.y, r.w, r.h, 220, 220, 220);
        }
    }

    double outer, inner;
    /* draw field of vision */
    for (i = 0; i < raycount; i++) {
        double rang = ToAngle(playerang) - (((double)raycount*fov) / 2) + ((double)i*fov);
        if (i == 0) inner = rang;
        if (i == 89) outer = rang;
        Point p2 = Raycast(player, rang);
        //printf("%f %f\n", p2.x,p2.y);
        Point mp = Minimap_GetPoint(player.x, player.y);
        Point mp2 = Minimap_GetPoint(p2.x, p2.y);
        IODrawLine(mp, mp2, 1);
    }

    if (mouse) {
        int mx, my;
        IOGetMousePos(&mx, &my);
        Point p = Minimap_GetPointRev(mx, my);
        player = p;
    }
}
void DrawFirstPerson() {
    int i;
    int swidth, sheight;
    IOGetWindowDims(&swidth, &sheight);
    for (i = 0; i < raycount; i++) {
        double rang = ToAngle(playerang) - (((double)raycount * fov) / 2) + ((double)i * fov);
        Point p2 = Raycast(player, rang);
        bool colhit = false;
        if (p2.x - trunc(p2.x)) colhit = true;
        double h = hypot(p2.x - player.x, p2.y - player.y);
        int lh = sheight / h;
        int lw = (swidth-100) / raycount;
        IODrawRect(swidth/2 - (lw*raycount / 2) + i * lw, sheight / 2 - lh / 2, lw, lh, colhit ? 50 : 30, colhit ? 220 : 170, colhit ? 50 : 30);
    }
}

void GameInit() {
    int mapw = 16, maph = 16;
    SetMap(mapw, maph);
    int i, j;
    //for (i = 0; i < 8; i++) {
    //    SetVWall(1, 8, i);
    //}
    //for (i = 0; i < 8; i++) {
    //    SetHWall(1, 8, i);
    //}
    SetHWall(1, 5, 4);
    GenerateMaze({ (double)mapw-1,(double)maph-1});

    player = { 3.4,3.5 };
    playerang = 0;
    fov = 0.02;
    raycount = 90;
}
void GameLoop() {
    if (IOKeyIsDown(SDLK_j)) {
        playerang += 0.01;
        if (playerang > M_PI*2) playerang = 0;
    }
    if (nplayerang == 0 && timer % 75 == 0) {
        Point forward = { player.x + directions[playerd][0], player.y + directions[playerd][1] };
        Point back = { player.x + directions[DirTurn(playerd, 'b')][0], player.y + directions[DirTurn(playerd, 'b')][1] };
        Point left = { player.x + directions[DirTurn(playerd, 'l')][0], player.y + directions[DirTurn(playerd, 'l')][1] };
        Point right = { player.x + directions[DirTurn(playerd, 'r')][0], player.y + directions[DirTurn(playerd, 'r')][1] };
        if (!Intersection(player, left)) {
            playerd = DirTurn(playerd, 'l');
            nplayerang = -(M_PI/2);
        }
        else if (!Intersection(player, forward)) {
            (void)0;
        }
        else if (!Intersection(player, right)) { 
            playerd = DirTurn(playerd, 'r');
            nplayerang = (M_PI / 2);
        }
        else if (!Intersection(player, back)) {
            playerd = DirTurn(playerd, 'b');
            nplayerang = M_PI;
        }
        player.x += directions[playerd][0];
        player.y += directions[playerd][1];
    }
    double step = 0.01;
    if (nplayerang < 0) {
        playerang += -step;
        nplayerang += step;
    }
    if(nplayerang > 0) {
        playerang += step;
        nplayerang -= step;
    }
    if (abs(nplayerang) - 0.02 < 0) 
        nplayerang = 0;

    /* Draw minimap */
    DrawFirstPerson();
    DrawMinimap();
    timer++;
}

int main(int argc, char** argv) {
    IOInit();
    GameInit();
    while (!IOShouldClose()) {
        IOFrame();

        GameLoop();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
