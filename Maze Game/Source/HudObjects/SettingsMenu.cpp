#include "..\MazeGame.h"

/* draw background, reposition button when resized */

void SettingsMenuLoadFields(SettingsMenu* s) {
    HudObjTextBox* fovfield = (HudObjTextBox*)GetHudObj(s->fovfield);
    HudObjTextBox* movfield = (HudObjTextBox*)GetHudObj(s->movspeedfield);
    HudObjTextBox* rotfield = (HudObjTextBox*)GetHudObj(s->rotspeedfield);
    HudObjTextBox* wallhfield = (HudObjTextBox*)GetHudObj(s->wallhfield);
    
    sprintf_s(fovfield->buffer, "%.3f", fov);
    sprintf_s(movfield->buffer, "%.3f", player.movespeed);
    sprintf_s(rotfield->buffer, "%.3f", player.turnspeed);
    sprintf_s(wallhfield->buffer, "%.3f", wallheight);
}

static void ApplyButton(HudObjButton* b) {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(settingsmenu);
    HudObjTextBox* fovfield = (HudObjTextBox*)GetHudObj(s->fovfield);
    HudObjTextBox* movfield = (HudObjTextBox*)GetHudObj(s->movspeedfield);
    HudObjTextBox* rotfield = (HudObjTextBox*)GetHudObj(s->rotspeedfield);
    HudObjTextBox* wallhfield = (HudObjTextBox*)GetHudObj(s->wallhfield);

    fov = atof(fovfield->buffer);
    player.movespeed = atof(movfield->buffer);
    player.turnspeed = atof(rotfield->buffer);
    wallheight = atof(wallhfield->buffer);
}
static void NextButton(HudObjButton* b) {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(settingsmenu);
    NewMaze();
}


static void OnCreate(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    s->applybutton = InstantiateHudObj(HudObjTypeButton);
    s->nextbutton = InstantiateHudObj(HudObjTypeButton);
    s->fovfield = InstantiateHudObj(HudObjTypeTextBox);
    s->rotspeedfield = InstantiateHudObj(HudObjTypeTextBox);
    s->movspeedfield = InstantiateHudObj(HudObjTypeTextBox);
    s->wallhfield = InstantiateHudObj(HudObjTypeTextBox);
    s->fovlabel = InstantiateHudObj(HudObjTypeLabel);
    s->rotspeedlabel = InstantiateHudObj(HudObjTypeLabel);
    s->movspeedlabel = InstantiateHudObj(HudObjTypeLabel);
    s->wallhlabel = InstantiateHudObj(HudObjTypeLabel);

    HudObjRef buttons[] = { s->applybutton, s->nextbutton };
    const char* buttonlabels[] = {"Apply", "Go Next"};
    HudObjButtonCallback* buttoncallbacks[] = { ApplyButton, NextButton };
    for (int i = 0; i < 2; i++) {
        HudObjButtonSet((HudObjButton*)GetHudObj(buttons[i]), buttoncallbacks[i], buttonlabels[i]);
    }


    HudObjRef labels[] = { s->fovlabel, s->rotspeedlabel, s->movspeedlabel, s->wallhlabel };
    const char* labelss[] = { "Fov:", "Rotation:", "Move:", "WallHeight:"};
    for (int i = 0; i < 4; i++) {
        HudObjLabel* l = (HudObjLabel*)GetHudObj(labels[i]);
        strcpy_s(l->buffer, labelss[i]);
    }
}
static void OnResize(HudObj* o) {
    /* hard code stuff.
       Nice way to hard code stuff.
       Arrays.
    */
    SettingsMenu* s = (SettingsMenu*)o;
    int labelstart = s->base.x + 5;
    int labelstarty = s->base.y + 5;
    int fieldwidth = s->base.w / 2;
    int fieldheight = 20;
    int fieldstart = s->base.x + s->base.w - fieldwidth - 5;
    int buttonwidth = 100, buttonheight = 30;
    HudObjRef refs[] = {
        s->movspeedlabel, s->movspeedfield,
        s->rotspeedlabel, s->rotspeedfield,
        s->fovlabel, s->fovfield,
        s->wallhlabel,s->wallhfield,
        s->nextbutton, s->applybutton
    };
    Rect rects[] = {
        { labelstart, labelstarty, 0,0 },
        { fieldstart, labelstarty, fieldwidth, fieldheight, },

        { labelstart, labelstarty + fieldheight * 1, 0,0 },
        { fieldstart, labelstarty + fieldheight * 1, fieldwidth, fieldheight, },

        { labelstart, labelstarty + fieldheight * 2, 0,0 },
        { fieldstart, labelstarty + fieldheight * 2, fieldwidth, fieldheight, },

        { labelstart, labelstarty + fieldheight * 3, 0,0 },
        { fieldstart, labelstarty + fieldheight * 3, fieldwidth, fieldheight, },

        { s->base.x + s->base.w - buttonwidth*2 - 10, s->base.y + s->base.h - buttonheight - 5, buttonwidth, buttonheight, },
        { s->base.x + s->base.w - buttonwidth - 5, s->base.y + s->base.h - buttonheight - 5, buttonwidth, buttonheight, }
    };

    for (int i = 0; i < 10; i++) {
        HudObj* obj = GetHudObj(refs[i]);
        HudObjSetPosition(obj, rects[i].x, rects[i].y, rects[i].w, rects[i].h);
    }

    /*HudObjButton* b = (HudObjButton*)GetHudObj(s->applybutton);
    HudObjButton* b2 = (HudObjButton*)GetHudObj(s->nextbutton);
    HudObjTextBox* fov = (HudObjTextBox*)GetHudObj(s->fovtext);
    
    int bw = 100, bh = 30;
    HudObjSetPosition((HudObj*)b, s->base.x + s->base.w - (bw + 5), s->base.y + s->base.h - (bh + 5), bw, bh);
    HudObjSetPosition((HudObj*)b2, s->base.x + s->base.w - (bw + 5)*2, s->base.y + s->base.h - (bh + 5), bw, bh);

    HudObjSetPosition((HudObj*)fov, s->base.x+5, s->base.y+1, s->base.w-10, 50);*/

}
static void OnDraw(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    IODrawRect(s->base.x, s->base.y, s->base.w, s->base.h, 200,200,180);
}

void SettingsMenuInit() {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(RegisterHudObj(HudObjTypeSettingsMenu, sizeof(SettingsMenu)));
    HudObjRegEvent((HudObj*)s, HudObjEventCreate, OnCreate);
    HudObjRegEvent((HudObj*)s, HudObjEventResize, OnResize);
    HudObjRegEvent((HudObj*)s, HudObjEventDraw, OnDraw);
}