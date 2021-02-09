#include "../include/picturelist.h"
#include "../include/defines.h"
#include"../include/global_functions.h"
#include <string.h>

Picture::Picture()
{
    path = NULL;
    img = NULL;
    useBitmap = false;
}
Picture::~Picture()
{
    if (path) free(path);
    if (img) destroy_bitmap(img);
}
void Picture::copy(Picture*source)
{
    //alert("copying...", NULL, NULL, "OK", NULL, 'o', 0);
    color = source->color;
    //alert("source color", itoa(color, new char[10], 16), NULL, "OK", NULL, 'o', 0);
    useBitmap = source->useBitmap;
    //alert("useBitmap", (useBitmap?"1":"0"), NULL, "OK", NULL, 'o', 0);
    if (path) free(path);
    if (img) destroy_bitmap(img);
    path = NULL;
    img = NULL;
    //alert("Finishing basics...", NULL, NULL, "OK", NULL, 'o', 0);
    {
        if (source->path)
        {
            //alert("Trying to load path", itoa((int)source->path, new char[10], 16), NULL, "OK", NULL, 'o', 0);
            BITMAP* temp = load_bitmap(source->path, NULL);
            //if (!temp) alert("There is no path", NULL, NULL, "OK", NULL, 'o', 0);
            //else alert("There is a path", NULL, NULL, "OK", NULL, 'o', 0);
            if (!temp) source->path = NULL;
            else destroy_bitmap(temp);
        }
    }

    //if (source->path) alert("There is a path", NULL, NULL, "OK", NULL, 'o', 0);;
    if (source->path) set_img(source->path);
    else if (source->img)
    {
        //alert("Copiou com img","mas sem path", NULL, "OK", NULL, 'o', 0);
        img = create_bitmap(source->img->w, source->img->h);
        draw_sprite(img, source->img, 0, 0);
        useBitmap = true;
    }
    //alert("Finished Copying", NULL, NULL, "OK", NULL, 'o', 0);
}

void Picture::draw(BITMAP*b, int x, int y)
{
    draw_sprite(b, img, x, y);
}

int Picture::set_img(char*s)
{
    if (!s) return -1;
    path = (char*) calloc(1, sizeof(char)*(strlen(s)+1));
    strcpy(path, s);
    img = load_bitmap(path, NULL);
    if (!img)
    {
        img = create_bitmap(1, 1);
        clear_to_color(img, 0xFF00FF);
    }
    return 0;
}

PictureList::PictureList()
{
    amount = 0;
    colorList = NULL;
    selected = NULL;
    selectedId = -1;
    backgroundColor = GRAY(13);
    picWidth = picHeight = 100;
    pos.x = pos.y = 0;
    pos.w = pos.h = 120;
    spaceAbovePics = 10;
    spaceBetweenPics = 10;
    roll = 0;
    inertia = 0;
    maxVelocity = 40;
    mouseDx = 0;
    mouseDy = 0;
    mouseDz = 0;
    maxDistance = 15;
    tela = NULL;
    list = create_bitmap(pos.w, pos.h);
    selectStyle = 0;
    useMouseWheel = false;
    horizontal = true;
    onFocus = false;
    canTeleport = true;
}

void PictureList::set_amount(int i)
{
    amount = i;
    colorList = (Picture*) realloc(colorList, sizeof(Picture)*amount);
}

void PictureList::delete_picture(int pos)
{
    if (pos >= amount) return;
    for (int i = pos; i<amount-1; i++)
    {
        colorList[i].copy(&(colorList[i+1]));
    }
    amount--;
    colorList = (Picture*) realloc(colorList, sizeof(Picture)*amount);
}

void PictureList::inertia_move()
{
    roll += inertia/20;
}

void PictureList::move()
{
    if (inertia>0) inertia--;
    if (inertia<0) inertia++;
    if (inertia > maxVelocity) inertia -=2;
    if (inertia < -maxVelocity) inertia +=2;
    if (horizontal)
        rollMax = (spaceBetweenPics)*(amount+1) + (picWidth*(amount)) - pos.w;
    else
        rollMax = (spaceBetweenPics)*(amount+1) + (picHeight*(amount)) - pos.h;
    if (canTeleport)
    {
        if (roll < -((horizontal?pos.w:pos.h))) roll = rollMax+(horizontal?pos.w:pos.h); // SCROLL TELEPORT
        if (roll > rollMax+(horizontal?pos.w:pos.h)) roll = -((horizontal?pos.w:pos.h)); // SCROLL TELEPORT
        if (roll > rollMax) roll-=2;
        if (roll < 0) roll+=2;
    }
    else
    {
        if (roll > rollMax) roll = rollMax;
        if (roll < 0) roll = 0;
    }
}

void PictureList::draw()
{
    move();
    if (!(pos.w == list->w && pos.h == list->h)) // Se as dimensões da lista forem modificadas: refazer o BITMAP
    {
        destroy_bitmap(list);
        list = create_bitmap(pos.w, pos.h);
    }
    clear_to_color(list, backgroundColor);

    int rollerPos;
    if (horizontal) rollerPos = roll*(pos.w)/rollMax;
    else rollerPos = roll*(pos.h)/rollMax;
    bool showGradBar = false;
    bool showSingleBar = true;
    if (showGradBar)
    {
        rectfill(list, 0, 0, pos.w, 3, WHITE);
        for (int i = 0; i<pos.w/2; i++)
        {
            int temp = i*255/(pos.w/2-1);
            rectfill(list, -i+rollerPos, 0, -i+rollerPos, 3, makecol(temp, temp, 255));
            temp = 255-temp;
            rectfill(list, -i+rollerPos+(pos.w/2), 0, -i+rollerPos+(pos.w/2), 3, makecol(temp, temp, 255));
        }
    }
    if (showSingleBar)
    {
        int rollerColor = ABS(inertia*255/300);
        if (rollerColor > 255 ) rollerColor = 255;
        rollerColor = 255 - rollerColor;
        int makedColor = makecol(rollerColor, rollerColor, rollerColor);
        while (makedColor > backgroundColor) makedColor-=0xF;
        if (horizontal)
            rectfill(list, rollerPos, 0, rollerPos+1, 4, makedColor);
        else
            rectfill(list, 0, rollerPos, 4, rollerPos+1, makedColor);
    }

    for (int i=0; i<amount; i++) // Desenhar Pictures (Quadrados Coloridos)
    {
        if (horizontal)
        {
            rectfill(list,  (spaceBetweenPics)*(i+1) + (picWidth*i) - roll,
                        spaceAbovePics,
                        (spaceBetweenPics)*(i+1) + (picWidth*(i+1)) - roll,
                        picHeight+spaceAbovePics,
                        colorList[i].color);
            if (colorList[i].useBitmap)
            {
                colorList[i].draw(list, (spaceBetweenPics)*(i+1) + (picWidth*i) - roll, spaceAbovePics);
            }
        }
        else
        {
            rectfill(list, spaceAbovePics,
                        (spaceBetweenPics)*(i+1) + (picHeight*i) - roll,
                        picWidth+spaceAbovePics,
                        (spaceBetweenPics)*(i+1) + (picHeight*(i+1)) - roll,
                        colorList[i].color);
            if (colorList[i].useBitmap)
            {
                colorList[i].draw(list, spaceAbovePics, (spaceBetweenPics)*(i+1) + (picHeight*i) - roll);
            }
        }

    }

    switch (selectStyle) // Desenhar Selecionador
    {
        case 1: // Quadrado Gradiente
            for (int i=0; i<5 && selectedId!=-1; i++)
                rect(list,  (spaceBetweenPics)*(selectedId+1) + (picWidth*selectedId) - roll - i, // x1
                        spaceAbovePics - i, // y1
                        (spaceBetweenPics)*(selectedId+1) + (picWidth*(selectedId+1)) - roll + i, // x2
                        picHeight+spaceAbovePics + i,   //y2
                        GRAY((i*2)) + GRAY(3)); //color

            break;
        case 2: // Linha Gradiente
            for(int i = 2; i<5 && selectedId!=-1; i++)
                hline(list,  (spaceBetweenPics)*(selectedId+1) + (picWidth*selectedId) - roll - i, // x1
                            picHeight+spaceAbovePics + i, // y
                            (spaceBetweenPics)*(selectedId+1) + (picWidth*(selectedId+1)) - roll + i, // x2
                            GRAY((i*2)) + GRAY(3)); //color

            break;
        case 3: // Quadrado Cor estatica
            for(int i = 2; i<5 && selectedId!=-1; i++)
                rect(list,  (spaceBetweenPics)*(selectedId+1) + (picWidth*selectedId) - roll - i, // x1
                            spaceAbovePics - i, // y1
                            (spaceBetweenPics)*(selectedId+1) + (picWidth*(selectedId+1)) - roll + i, // x2
                            picHeight+spaceAbovePics + i,   //y2
                            GRAY(10)); //color

        case 4: // Linha Cor Estatica
            for(int i = 2; i<5 && selectedId!=-1; i++)
                hline(list,  (spaceBetweenPics)*(selectedId+1) + (picWidth*selectedId) - roll - i, // x1
                        picHeight+spaceAbovePics + i, // y
                        (spaceBetweenPics)*(selectedId+1) + (picWidth*(selectedId+1)) - roll + i, // x2
                        GRAY(10)); //color
            break;
    }

    //textprintf_ex(list, font, 0, 0, BLACK, backgroundColor, "%d : %d : %X : %d", roll, inertia, (selected?selected->color:0), selectedId);
    //textprintf_ex(list, font, 0, pos.h-9, BLACK, backgroundColor, "%X : %d : %d",0, mouseDx, ABS(mouseDx-mouse_x));

    draw_sprite(tela, list, pos.x, pos.y);
}

void PictureList::mouse_movement()
{
    if (mouse_b == 1)
    {
        if (testMousePos(&(pos)) && !mouseB1)
        {
            mouseB1 = true;
            mouseDx = mouse_x;
            mouseDy = mouse_y;
        }
        else if (mouseB1)
        {
            if (horizontal)
            {
                inertia = mouseDx - mouse_x;
                int d = (mouseDx - mouse_x)/maxDistance;
                roll +=d;
                mouseDx -=d;
            }
            else
            {
                inertia = mouseDy - mouse_y;
                int d = (mouseDy - mouse_y)/maxDistance;
                roll +=d;
                mouseDy -=d;
            }
        }
    }
    else if (mouse_b != 1 && mouseB1)
    {
        if (maxDistance > ABS(mouseDx - mouse_x) && maxDistance > ABS(mouseDy - mouse_y)) // Selecionando
        {
            int lista;
            if (horizontal) lista = (mouse_x+roll-pos.x)/(picWidth+spaceBetweenPics);
            else lista = (mouse_y+roll-pos.y)/(picHeight+spaceBetweenPics);

            if (lista < 0 || lista >= amount)
            {
                selected =  NULL;
                selectedId = -1;
            }
            else
            {
                selected = &(colorList[lista]);
                selectedId = lista;
            }

        }
        mouseB1 = false;
    }
    if (!mouse_b && !mouseB1 || mouse_b == 1 && !testMousePos(&(pos)) && !mouseB1) // Soltou o mouse_b para inercia agir
    {
        inertia_move();
    }
    if (testMousePos(&(pos)) && !onFocus) // Wheel Mouse Movement
    {
        mouseDz = mouse_z;
        onFocus = true;
    }
    else if (!testMousePos(&(pos))) onFocus = false;
    if (onFocus && useMouseWheel)
    {
        if (horizontal)
        {
            inertia += (mouse_z - mouseDz)*maxDistance;
            //roll += (mouse_z - mouseDz)*maxDistance;
            mouseDz = mouse_z;
            if (inertia > 300) inertia = 300;
            if (inertia < -300) inertia = -300;
        }
        else
        {
            inertia -= (mouse_z - mouseDz)*maxDistance;
            //roll -= (mouse_z - mouseDz)*maxDistance;
            mouseDz = mouse_z;
            if (inertia > 300) inertia = 300;
            if (inertia < -300) inertia = -300;
        }
    }
}

void PictureList::inicializar_paths(int ini, int end)
{
    for (int i=ini; i<end; i++)
    {
        colorList[i].path = NULL;
        colorList[i].img = NULL;
    }
}

int PictureList::is_showing(int id)
{
    if (id > amount) return 0;
    if (horizontal)
    {
        int x = (spaceBetweenPics)*(id+1) + (picWidth*id) - roll;
        int x2 = (spaceBetweenPics)*(id+1) + (picWidth*(id+1)) - roll;
        if ( x < 0 )
        {
            return -1;
        }
        if ( x2 > pos.w)
        {
            return 1;
        }
    }
    else
    {
        int y = (spaceBetweenPics)*(id+1) + (picHeight*id) - roll;
        int y2 = (spaceBetweenPics)*(id+1) + (picHeight*(id+1)) - roll;
        if ( y < 0)
        {
            return -1;
        }
        if ( y2 > pos.h)
        {
            return 1;
        }
    }
    return 0;
}

int PictureList::sort(int*v)
{
    if (!v) return ERROR_FEEDBACK;
    //alert("There is a vector", NULL, NULL, "OK", NULL, 'o', 0);
    for (int i = 0; i<amount; i++)
    {
        //alert("#", itoa(v[i], new char[10], 10), NULL, "OK", NULL, 'o', 0);
        if (v[i] > amount-1 || v[i] < 0) return ERROR_FEEDBACK;
    }
    //alert("The vector is alright", NULL, NULL, "OK", NULL, 'o', 0);
    Picture* tempList = (Picture*) calloc(sizeof(Picture), amount);
    if (!tempList) return ERROR_FEEDBACK;
    //alert("Memory succesfully allocated", NULL, NULL, "OK", NULL, 'o', 0);
    for (int i = 0; i<amount; i++)
    {
        //alert("Copying", itoa(i, new char[10], 10), itoa(v[i], new char[10], 10), "OK", NULL, 'o', 0);
        tempList[i].copy(&(colorList[v[i]]));
    }
    //alert("New vector is sorted", NULL, NULL, "OK", NULL, 'o', 0);
    free(colorList);
    colorList = tempList;

    return SUCCESS_FEEDBACK;
}

PictureList::~PictureList()
{
    if (colorList) free(colorList);
}

ThingList::ThingList()
{
    picList = NULL;
}
ThingList::~ThingList()
{
    free(picList);
}

void ThingList::move()
{
    if (inertia>0) inertia--;
    if (inertia<0) inertia++;
    if (inertia > maxVelocity) inertia -=2;
    if (inertia < -maxVelocity) inertia +=2;
    if (horizontal)
        rollMax = (spaceBetweenPics)*(amount+1) + (picWidth*(amount)) - pos.w;
    else
        rollMax = (spaceBetweenPics)*(amount+1) + (picHeight*(amount)) - pos.h;

    if (roll < -((horizontal?pos.w:pos.h))) roll = rollMax+(horizontal?pos.w:pos.h); // SCROLL TELEPORT
    if (roll > rollMax+(horizontal?pos.w:pos.h)) roll = -((horizontal?pos.w:pos.h)); // SCROLL TELEPORT

    if (roll > rollMax) roll = rollMax;
    if (roll < 0) roll = 0;
}

void ThingList::draw()
{
    move();

    int rollerPos;
    if (horizontal) rollerPos = roll*(pos.w)/rollMax;
    else rollerPos = roll*(pos.h)/rollMax;

    for (int i=0; i<amount; i++) // Desenhar Pictures (Quadrados Coloridos)
    {
        if (horizontal)
        {
            picList[i]->pos.x = (spaceBetweenPics)*(i+1) + (picWidth*i) - roll;
        }
        else
        {
            picList[i]->pos.y = (spaceBetweenPics)*(i+1) + (picHeight*i) - roll;
        }
    }

    textprintf_ex(list, font, 0, 0, BLACK, backgroundColor, "%X : %X", (picList!=NULL?picList[0]->tela:0), list);
    textprintf_ex(list, font, 0, pos.h-9, BLACK, backgroundColor, "%X : %d : %d",0, mouseDx, ABS(mouseDx-mouse_x));

    //draw_sprite(tela, list, pos.x, pos.y);
}

int ThingList::insert(PictureList*pl)
{
    if (!pl) return -1;
    amount++;
    picList = (PictureList**) realloc(picList, sizeof(PictureList*) * amount);
    picList[amount-1] = pl;
    return 0;
}

PosList::PosList()
{
    posList = NULL;
    PictureList::PictureList();
}
PosList::~PosList()
{
    if (posList) free(posList);
}

void PosList::move()
{
    if (inertia>0) inertia--;
    if (inertia<0) inertia++;
    if (inertia > maxVelocity) inertia -=2;
    if (inertia < -maxVelocity) inertia +=2;
    if (horizontal)
        rollMax = (spaceBetweenPics)*(amount+1) + (picWidth*(amount)) - pos.w;
    else
        rollMax = (spaceBetweenPics)*(amount+1) + (picHeight*(amount)) - pos.h;

    if (roll < -((horizontal?pos.w:pos.h))) roll = rollMax+(horizontal?pos.w:pos.h); // SCROLL TELEPORT
    if (roll > rollMax+(horizontal?pos.w:pos.h)) roll = -((horizontal?pos.w:pos.h)); // SCROLL TELEPORT

    if (roll > rollMax) roll = rollMax;
    if (roll < 0) roll = 0;
}

void PosList::draw()
{
    move();

    int rollerPos;
    if (horizontal) rollerPos = roll*(pos.w)/rollMax;
    else rollerPos = roll*(pos.h)/rollMax;

    for (int i=0; i<amount; i++) // Mudar Pos associadas
    {
        if (horizontal)
        {
            posList[i]->x = (spaceBetweenPics)*(i+1) + (picWidth*i) - roll;
        }
        else
        {
            posList[i]->y = (spaceBetweenPics)*(i+1) + (picHeight*i) - roll;
        }
    }

    //textprintf_ex(list, font, 0, 0, BLACK, backgroundColor, "%X : %X", (picList!=NULL?picList[0]->tela:0), list);
    //textprintf_ex(list, font, 0, pos.h-9, BLACK, backgroundColor, "%X : %d : %d",0, mouseDx, ABS(mouseDx-mouse_x));

    return;
    if (!(pos.w == list->w && pos.h == list->h)) // Se as dimensões da lista forem modificadas: refazer o BITMAP
    {
        destroy_bitmap(list);
        list = create_bitmap(pos.w, pos.h);
        clear_to_color(list, backgroundColor);
    }
    draw_sprite(tela, list, pos.x, pos.y);
}

int PosList::insert(Pos* p)
{
    if (!p) return -1;
    amount++;
    posList = (Pos**) realloc(posList, sizeof(Pos*) * amount);
    posList[amount-1] = p;
    return 0;
}

int PosList::is_showing(int id)
{
    if (id > amount) return 0;
    if (horizontal)
    {
        int x = (spaceBetweenPics)*(id+1) + (picWidth*id) - roll;
        int x2 = (spaceBetweenPics)*(id+1) + (picWidth*(id+1)) - roll;
        if ( x < 0 )
        {
            return -1;
        }
        if ( x2 > pos.w)
        {
            return 1;
        }
    }
    else
    {
        int y = (spaceBetweenPics)*(id+1) + (picHeight*id) - roll;
        int y2 = (spaceBetweenPics)*(id+1) + (picHeight*(id+1)) - roll;
        if ( y < 0 )
        {
            return -1;
        }
        if ( y2 > pos.h)
        {
            return 1;
        }
    }
    return 0;
}

Folder::Folder(int x, int y, int w, int h, FONT*f)
{
    amount = 0;
    showing = 0;
    pageList = NULL;
    caption = NULL;
    backgroundColor = GRAY(12);
    edgeColor = GRAY(4);
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
    captionFont = f;
}

Folder::~Folder()
{
    for (int i = 0; i<amount; i++)
    {
        //alert("Liberando", caption[i], NULL, "OK", NULL, 'o', 0);
        destroy_bitmap(pageList[i]);
        free(caption[i]);
    }
}

int Folder::insert(char* s)
{
    BITMAP* temp = create_bitmap(pos.w, pos.h - captionFont->height - 2);
    if (!temp) return -1;
    clear_to_color(temp, backgroundColor);

    amount++;
    pageList = (BITMAP**) realloc(pageList, sizeof(BITMAP*) * amount);
    if (!pageList) return -1;
    pageList[amount-1] = temp;

    char* tempS = (char*) calloc(sizeof(char), strlen(s)+1);
    strcpy(tempS, s);
    caption = (char**) realloc(caption, sizeof(char*) * amount);
    if (!caption) return -1;
    caption[amount-1] = tempS;

    return 0;
}

void Folder::draw()
{
    if (amount == 0) return;
    draw_sprite(tela, pageList[showing], pos.x, pos.y + captionFont->height + 2);
    for (int i = 0; i<amount; i++)
    {
        int tabW = pos.w/amount;

        rectfill(tela, pos.x+tabW*i, pos.y, pos.x+tabW*i, pos.y+captionFont->height, edgeColor);
        if (i != showing)
        {
            rectfill(tela, pos.x+tabW*i, pos.y, pos.x+tabW*(i+1), pos.y+captionFont->height, backgroundColor-GRAY(1));
            rectfill(tela, pos.x+tabW*i, pos.y+captionFont->height-1, pos.x+tabW*(i+1), pos.y+captionFont->height, edgeColor);
        }
        else
            rectfill(tela, pos.x+tabW*i, pos.y, pos.x+tabW*(i+1), pos.y+captionFont->height, backgroundColor);
        textprintf_ex(tela, captionFont, 1+pos.x+tabW*i, pos.y, edgeColor, -1, "%s", caption[i]);
    }
}

void Folder::mouse_action()
{
    int tabW = pos.w/amount;
    for (int i = 0; i<amount; i++)
    {
        if (testMousePos(pos.x+tabW*i, pos.y, pos.x+tabW*(i+1), pos.y+captionFont->height))
        {
            if (mouse_b == 1)
                showing = i;
        }
    }

}
