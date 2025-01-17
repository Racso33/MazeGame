#include "MazeGame.h"

extern int directions[4][2];
extern int DirTurn(int i, char c);

void Player_PickDirection(Player* p) {
    Point forward = { p->pos.x + directions[p->dir][0], p->pos.y + directions[p->dir][1] };
    Point back = { p->pos.x + directions[DirTurn(p->dir, 'b')][0], p->pos.y + directions[DirTurn(p->dir, 'b')][1] };
    Point left = { p->pos.x + directions[DirTurn(p->dir, 'l')][0], p->pos.y + directions[DirTurn(p->dir, 'l')][1] };
    Point right = { p->pos.x + directions[DirTurn(p->dir, 'r')][0], p->pos.y + directions[DirTurn(p->dir, 'r')][1] };
    p->hasTarget = true;
    if (!Intersection(p->pos, left)) {
        p->dir = DirTurn(p->dir, 'l');
        p->tang -= M_PI / 2;
    }
    else if (!Intersection(p->pos, forward)) {
        p->dir = DirTurn(p->dir, 'f');
    }
    else if (!Intersection(p->pos, right)) {
        p->dir = DirTurn(p->dir, 'r');
        p->tang += M_PI / 2;
    }
    else if (!Intersection(p->pos, back)) {
        p->dir = DirTurn(p->dir, 'b');
        p->tang += M_PI;
    }
    p->target = { (int)p->pos.x + 0.5 + directions[p->dir][0],
                  (int)p->pos.y + 0.5 + directions[p->dir][1] };
    p->hasTarget = true;
}
void Player_Update(Player* p) {
    int mapw, maph;
    GetMap(&mapw, &maph);
    /* if j key, turn */
    if (IOKeyIsDown(SDLK_j)) {
        /* implement some time... */
    }
    /* if player has finished moving to target, pick a new one */
    if (!p->hasTarget && p->movecooldown < 0) {
        Player_PickDirection(p);
    }

    double d = AngleDiff(p->ang, p->tang);
    if (abs(d) - 0.04 >= 0) {
        double smoothf = 1.5;
        double turn = abs(d * p->turnspeed * smoothf);
        if (d > 0) {
            p->ang += turn;
        }
        else {
            p->ang -= turn;
        }
    }
    /* smoothly move pos to target */
    Point td = { p->target.x - p->pos.x, p->target.y - p->pos.y };
    double tdh = hypot(td.x, td.y);
    if (p->hasTarget && abs(tdh) - 0.2 < 0) {
        p->hasTarget = false;
        p->movecooldown = 0;
    }
    if (p->hasTarget) {
        p->pos.x += td.x / tdh * p->movespeed;
        p->pos.y += td.y / tdh * p->movespeed;
    }
    p->movecooldown -= 1;

    if ((int)p->pos.x == mapw - 1 && (int)p->pos.y == maph - 1) {
        NewMaze();
    }
}