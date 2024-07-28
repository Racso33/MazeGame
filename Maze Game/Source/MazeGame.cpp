#include "MazeGame.h"

Player player;
double fov;
int timer;
bool colors[3];
bool bg[3];
int solved;

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
    double r = fmod(i, M_PI*2);
    if (r < 0) {
        r = M_PI * 2 + r;
    }
    return r;
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

    /* draw field of vision */
    int swidth, sheight;
    IOGetWindowDims(&swidth, &sheight);
    int raycount = swidth/(swidth/16);
    for (i = 0; i < raycount; i++) {
        double rang = ToAngle(player.ang - (fov / 2) + i * (fov / raycount));
        Point p2 = Raycast(player.pos, rang);
        Point mp = Minimap_GetPoint(player.pos.x, player.pos.y);
        Point mp2 = Minimap_GetPoint(p2.x, p2.y);
        IODrawLine(mp, mp2, 1);
    }

    if (mouse) {
        int mx, my;
        IOGetMousePos(&mx, &my);
        Point p = Minimap_GetPointRev(mx, my);
        player.pos = p;
    }
}
void DrawFirstPerson() {
    int i;
    int swidth, sheight;
    IOGetWindowDims(&swidth, &sheight);
    int raycount = swidth/4;
    double range = raycount * fov;
    for (i = 0; i < raycount; i++) {
        double rang = ToAngle(player.ang - (fov / 2) + i * (fov/raycount));
        Point p2 = Raycast(player.pos, rang);
        bool colhit = false;
        if (p2.x - trunc(p2.x)) colhit = true;
        double h = hypot(p2.x - player.pos.x, p2.y - player.pos.y) * cos(rang - player.ang);
        double lh = (sheight/3) / h;
        double lw = (double)swidth / raycount;
        int r = colors[0] ? 230 : 170;
        if (colhit) r *= 0.7;
        int g = colors[1] ? 230 : 170;
        if (colhit) g *= 0.7;
        int b = colors[2] ? 230 : 170;
        if (colhit) b *= 0.7;
        IODrawRect(swidth/2.0f - (lw*raycount / 2) + i*lw, sheight / 2.0f - lh / 2, lw+1, lh, r,g,b);
    }
}

void NewMaze() {
    int mapw, maph;
    GetMap(&mapw, &maph);
    GenerateMaze({ (double)mapw - 1, (double)maph - 1 });
    player.pos.x = player.pos.y = player.ang = player.dir = player.tang = 0;
    player.hasTarget = false;
    player.pos.x = 0.5; player.pos.y = 0.5;
    int r = rand() % 7;
    colors[0] = r & 0b001;
    colors[1] = r & 0b010;
    colors[2] = r & 0b100;
    int r2 = rand() % 7;
    bg[0] = r2 & 0b001;
    bg[1] = r2 & 0b010;
    bg[2] = r2 & 0b100;
    IOSetBgColor(bg[0] ? 70 : 40, bg[1] ? 70 : 40, bg[2] ? 70 : 40);
    solved++;
}
void OnHelloWorldEvent(void* p) {
    player.pos.x = rand() % 8 + 0.5;
    player.pos.y = rand() % 8 + 0.5;
}
void GameInit() {
    InitButton();

    int mapw = 8, maph = 8;
    SetMap(mapw, maph);
    int i, j;
    //for (i = 0; i < 8; i++) {
    //    SetVWall(1, 8, i);
    //}
    //for (i = 0; i < 8; i++) {
    //    SetHWall(1, 8, i);
    //}
    memset(&player, 0, sizeof(Player));
    fov = 1.57; /* 90 degrees */
    player.movespeed = 0.03;
    player.turnspeed = 0.05;
    NewMaze();
    solved = 0;

    Button* b = (Button*)InstantiateHudObject("Button");
    HudObject_SetPosition((HudObject*)b, 0, 0, 50, 30);
    b->label = "Bogos Binted";
}
void Player_Update(Player* p);
void GameLoop() {
    int mapw, maph;
    GetMap(&mapw, &maph);

    /* if menu is open do this */
    /* do click callbacks on hudobjects */
    /* draw hudobjects */

    /* if menu is open dont do this*/

    if (!IOGetMousePressed(2) && IOGetMouseScroll() == -1) fov += 0.1;
    if (!IOGetMousePressed(2) && IOGetMouseScroll() == 1) fov -= 0.1;
    Player_Update(&player);

    /* Draw minimap */
    DrawFirstPerson();
    DrawMinimap();
    char text[32] = {};
    sprintf_s(text, "Solved: %d", solved);
    IODrawText(text, MinimapScale * mapw + 7, 0);

    if (timer % 50 == 0 && rand() % 5 == 0) {
        ProgramEvent_Send("Hello World");
    }

    if (IOGetMousePressed(0)) ProgramEvent_Send("Click");

    ProgramEvent_Send("DrawHud");

    timer++;
}

int main(int argc, char** argv) {
    std::srand(std::time(0));
    IOInit();
    GameInit();
    while (!IOShouldClose()) {
        IOFrame();

        GameLoop();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
