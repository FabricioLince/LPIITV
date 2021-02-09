#include "../include/gui.h"
#include "../include/defines.h"
#include "../include/global_functions.h"
#include<string.h>

char *password_mask(char*s, char*p)
{
    for (int i=0; i<strlen(s); i++)
    {
        p[i] = '*';
    }
    p[strlen(s)] = 0;
    return p;
}

void show_load(int prog, int full, int w, int h)
{
    BITMAP* load = create_bitmap(w, h);
    clear_to_color(load, WHITE);

    int percent = prog*w/full;
    for (int i = 0; i<percent; i++)
    {
        line(load, i, 0, i, h, BLACK);
        textprintf_ex(screen, font, SCREEN_W/2, SCREEN_H/2+10, BLACK, WHITE, "%d", i);
        draw_sprite(screen, load, SCREEN_W/2, SCREEN_H/2);
    }

    destroy_bitmap(load);
}

Pos::Pos()
{
    x = y = w = h = 0;
}

void Pos::copy(Pos*p)
{
    x = p->x;
    y = p->y;
    w = p->w;
    h = p->h;
}

void CaixaDeTexto::default_option()
{
    textFont = font;
    pos.h = textFont->height*2;
    letterLenght = 8;
    verticalEdge = 1;
    horizontalEdge = 1;
    defaultColor = 0xD0D0D0;
    activeColor = WHITE;
    textColor = BLACK;
    active = false;
    password = false;
    passwordText = NULL;
    inTimer = 0;
    timeChange = 30;
    blinkColor = BLACK;
    borderColor = BLACK;
    textFont = font;
    legenda = NULL;
    legColor = WHITE;
}

CaixaDeTexto::CaixaDeTexto()
{
    default_option();
    text = NULL;
};

CaixaDeTexto::CaixaDeTexto(int mL, int bX, int bY)
{
    maxLenght = mL;
    pos.w = 8*maxLenght+8;
    pos.x = bX;
    pos.y = bY;
    text = (char*) calloc(sizeof(char), maxLenght+1);
    text[0] = 0;
    default_option();
}

CaixaDeTexto::~CaixaDeTexto()
{
    if (text) free(text);
    if (passwordText) free(passwordText);
    if (legenda) free(legenda);
}

void CaixaDeTexto::drawTextBox()
{
    if (legenda)
        if (posLegenda==LEFT)
            textprintf_ex(tela, textFont, pos.x-(strlen(legenda)*letterLenght), pos.y, legColor, -1, "%s", legenda);
        else if (posLegenda==RIGHT)
            textprintf_ex(tela, textFont, pos.x+pos.w+1, pos.y, legColor, -1, "%s", legenda);
        else if (posLegenda==UP)
            textprintf_ex(tela, textFont, pos.x, pos.y-textFont->height-2, legColor, -1, "%s", legenda);
        else if (posLegenda==DOWN)
            textprintf_ex(tela, textFont, pos.x-(strlen(legenda)*letterLenght), pos.y, legColor, -1, "%s", legenda);

    rectfill(tela, pos.x-horizontalEdge, pos.y-verticalEdge, pos.x+pos.w+horizontalEdge, pos.y+pos.h+verticalEdge, borderColor);
    rectfill(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, (active?activeColor:defaultColor));

    if (password)
        textprintf_ex(tela, textFont, pos.x, pos.y+3, textColor, -1, "%s", password_mask(text, passwordText));
    else
        textprintf_ex(tela, textFont, pos.x, pos.y+3, textColor, -1, "%s", text);

    if (timeChange == 0) return;
    if (active)
        rectfill(tela, pos.x+1+(strlen(text)*letterLenght), pos.y+1, pos.x+2+(strlen(text)*letterLenght), pos.y+pos.h-1, blinkColor);
    inTimer++;
    if (inTimer>timeChange)
    {
        inTimer = 0;
        if (blinkColor == BLACK)
            blinkColor = activeColor;
        else
            blinkColor = BLACK;
    }
}

void CaixaDeTexto::is_password(bool b)
{
    if (b)
    {
        password = true;
        passwordText = (char*)calloc(sizeof(char), maxLenght+1);
    }
    else
    {
        password = false;
        if (passwordText) free(passwordText);
        passwordText = NULL;
    }
}

void CaixaDeTexto::set_legenda(char*s, int i)
{
    if (legenda) free(legenda);
    legenda = (char*) malloc(sizeof(char) * (strlen(s)+1));
    strcpy(legenda, s);
    posLegenda = i;
}

void Button::default_option()
{
    f = font;
    color = 0xE0E0E0;
    colorOnClick = 0xA0A0A0;
    borderColor = color;
    textColor = BLACK;
    textOrientantion = ORIENTATION_LEFT;
    useBitmapIco = false;
    bitmapIco = NULL;
}

Button::Button()
{
    legend = NULL;
    default_option();
    img = NULL;
}

Button::~Button()
{
    if (img) destroy_bitmap(img);
    img = NULL;
}

Button::Button(int x, int y, int w, int h, char*l, BITMAP*b)
{
    default_option();
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
    legend = (char*) malloc(sizeof(char) * (strlen(l)+1));
    strcpy(legend, l);
    tela = b;
    img = NULL;
    make_img();

}

int Button::make_img()
{
    if (img) destroy_bitmap(img);
    img = create_bitmap(pos.w, pos.h);
    clear_to_color(img, 0xFF00FF);
    //rectfill(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, color);
    //rect(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, borderColor);
    int borderSize = 3;
    hline(img, borderSize,  0,          pos.w-1-borderSize, borderColor);   // Cima
    hline(img, borderSize,  pos.h-1,    pos.w-1-borderSize, borderColor); // Baixo
    vline(img, 0,           borderSize, pos.h-1-borderSize, borderColor);  //Esquerda
    vline(img, pos.w-1,     borderSize, pos.h-1-borderSize, borderColor); //Direita
    int i=1;
    arc(img, borderSize, borderSize, itofix(64*i), itofix(i*64+64), borderSize, borderColor);
    i++;
    arc(img, borderSize, pos.h-1-borderSize, itofix(64*i), itofix(i*64+64), borderSize, borderColor);
    i++;
    arc(img, pos.w-1-borderSize, pos.h-1-borderSize, itofix(64*i), itofix(i*64+64), borderSize, borderColor);
    i++;
    arc(img, pos.w-1-borderSize, borderSize, itofix(64*i), itofix(i*64+64), borderSize, borderColor);
    floodfill(img, img->h/2, img->h/2, color);

    // Procurando Por &
    {
        underLine = -1;
        for (int i = 0; i<strlen(legend); i++)
        {
            if (legend[i] == '&')
            {
                i++;
                if (legend[i] == '&')
                {
                    i++;
                    for (int j=i-2; j<strlen(legend); j++)
                    {
                        legend[j] = legend[j+1];
                    }
                }
                else
                {
                    underLine = i-1;
                    for (i--;i<strlen(legend);i++)
                    {
                        legend[i] = legend[i+1];
                    }
                }
            }
        }

    }
}

void Button::draw()
{
    //rectfill(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, color);
    //rect(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, borderColor);
    //vline(tela, pos.x+pos.w/2, pos.y, pos.y+pos.h, BLACK);
    //hline(tela, pos.x, pos.y+pos.h/2, pos.x+pos.w, BLACK);
    draw_sprite(tela, img, pos.x, pos.y);
    floodfill(img, img->h/2, img->h/2, color);
    if (useBitmapIco)
    {
        if (bitmapIco)
            blit(bitmapIco, tela, 0, 0, pos.x, pos.y, pos.w, pos.h);
        if (textOrientantion == ORIENTATION_LEFT)
            textprintf_ex(tela, f, pos.x+10, (pos.y+pos.h+pos.y)/2-4, textColor, -1, "%s", legend);
        else if (textOrientantion == ORIENTATION_CENTRE)
            textprintf_centre_ex(tela, f, (pos.x+pos.x+pos.w)/2, pos.y+pos.h-f->height, textColor, -1, "%s", legend);
    }
    else
    {

        int y = (pos.y+pos.y+pos.h)/2 - f->height/2;
        if (textOrientantion == ORIENTATION_LEFT)
            textprintf_ex(tela, f, pos.x+10, y, textColor, -1, "%s", legend);
        else if (textOrientantion == ORIENTATION_CENTRE)
            textprintf_centre_ex(tela, f, (pos.x+pos.x+pos.w)/2, y, textColor, -1, "%s", legend);
        if (underLine != -1)
        {
            if (textOrientantion == ORIENTATION_LEFT)
                hline(tela, pos.x+10+underLine*11, y+f->height-3, pos.x+10+(underLine+1)*11-2, textColor);
                //textprintf_ex(tela, f, pos.x+10, y, textColor, -1, "%s", legend);
            else if (textOrientantion == ORIENTATION_CENTRE)
                hline(tela, (pos.x+pos.x+pos.w)/2+(underLine-1)*11, y+f->height-3, (pos.x+pos.x+pos.w)/2+underLine*11-2, textColor);
                //textprintf_centre_ex(tela, f, (pos.x+pos.x+pos.w)/2, y, textColor, -1, "%s", legend);
        }
    }


}

void Button::click()
{
    floodfill(img, img->h/2, img->h/2, colorOnClick);
    int temp = color;
    color = colorOnClick;
    draw();
    color = temp;
}

void Selector::set_highlight(bool b)
{
    if (b!=highLighted)
    {
        int temp = selectColor;
        selectColor = color;
        color = temp;
    }
    highLighted = b;
}

CheckBox::CheckBox()
{
    state = false;
    textFont = font;
}

CheckBox::CheckBox(char*s)
{
    text = (char*) calloc(sizeof(char), strlen(s)+1);
    strcpy(text, s);
    state = false;
    textFont = font;
}

void CheckBox::draw()
{
    rectfill(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, GRAY(2));
    rectfill(tela, pos.x+1, pos.y+1, pos.x+pos.w-1, pos.y+pos.h-1, GRAY(15));
    if (state)
    {
        line(tela, pos.x, pos.y, pos.x+pos.w, pos.y+pos.h, BLACK);
        line(tela, pos.x, pos.y+pos.h, pos.x+pos.w, pos.y, BLACK);
    }
    textprintf_ex(tela, textFont, pos.x+pos.w+5, pos.y+1, textColor, TRANSPARENT, text);
}

void CheckBox::check_click()
{
    if (testMousePos(&pos) && !click && mouse_b == 1)
    {
        state = !state;
        click = true;
    }
    else if (mouse_b != 1)
    {
        click = false;
    }
}
