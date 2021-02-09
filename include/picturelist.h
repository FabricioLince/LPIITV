#ifndef PICTURELIST_H
#define PICTURELIST_H

#include <allegro.h>
#include "gui.h"

class Picture
{
    public:
        Picture();
        ~Picture();
        void copy(Picture*);
        void draw(BITMAP*b, int x, int y);
        int set_img(char*s);
        int color;
        bool useBitmap;
        char* path;
        BITMAP*img;
};

class PictureList
{
    public:
        PictureList();
        virtual ~PictureList();

        void draw();
        void move();
        void inertia_move();
        void mouse_movement();

        void delete_picture(int);

        void set_amount(int i);

        void inicializar_paths(int, int);

        int is_showing(int);

        int sort(int*);

    //protected:
        int amount;
        Picture *colorList;
        Picture *selected;
        int selectedId;
        Pos pos;
        int backgroundColor;
        int picWidth;
        int picHeight;
        int spaceAbovePics;
        int spaceBetweenPics;
        int roll;
        int rollMax;

        int inertia;
        int maxVelocity;
        int mouseDx;
        int mouseDy;
        int mouseDz;
        bool mouseB1;
        int maxDistance;
        bool onFocus;

        bool useMouseWheel;
        bool horizontal;
        bool canTeleport;

        int selectStyle;

        BITMAP* tela;
        BITMAP* list;

};


class ThingList : public PictureList
{
    public:
        ThingList();
        ~ThingList();

        PictureList** picList;

        void draw();
        void move();
        int insert(PictureList*);

};

class PosList : public PictureList
{
    public:
        PosList();
        ~PosList();

        Pos** posList;

        void draw();
        void move();

        int insert(Pos*);

        int is_showing(int);
};

class Folder
{
    public:
        Folder(int, int, int, int, FONT*);
        ~Folder();

        int insert(char*);

        void draw();
        void mouse_action();

        Pos pos;

        int amount;
        int showing;

        int backgroundColor;
        int edgeColor;

        char** caption;
        BITMAP** pageList;

        FONT* captionFont;

        BITMAP* tela;

};

#endif // PICTURELIST_H
