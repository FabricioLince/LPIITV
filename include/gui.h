#ifndef GUI_LPIITV
#define GUI_LPIITV
#include "defines.h"
#include <allegro.h>
class Pos
{
    public:
        Pos();
        void copy(Pos*p);
        int x;
        int y;
        int w;
        int h;
};

class CaixaDeTexto
{
    public:
        void default_option();
        CaixaDeTexto();
        CaixaDeTexto(int mL, int bX, int bY);
        ~CaixaDeTexto();
        void drawTextBox();
        void is_password(bool b);
        void set_legenda(char*s, int i);
        char *text;
        int maxLenght;
        Pos pos;
        char *legenda;
        int posLegenda;
        int legColor;
        int verticalEdge;
        int horizontalEdge;
        int letterLenght;
        int defaultColor;
        int activeColor;
        int textColor;
        bool active;
        bool password;
        char* passwordText;
        int inTimer;
        int timeChange;
        int blinkColor;
        int borderColor;
        FONT*textFont;
        BITMAP*tela;
};

class Button
{
    public:
        void default_option();
        Button();
        virtual ~Button();
        Button(int x, int y, int w, int h, char*l, BITMAP*b);
        void draw();
        void click();
        int make_img();
        Pos pos;
        int color;
        int borderColor;
        int colorOnClick;
        char *legend;
        int textColor;
        int textOrientantion;
        int underLine;
        FONT *f;
        BITMAP* tela;
        BITMAP* bitmapIco;
        BITMAP* img;
        bool useBitmapIco;
};

class Selector: public Button
{
    public:
        Selector(int x, int y, int w, int h, char*l, BITMAP*b):Button(x, y, w, h, l, b)
        {
            highLighted = false;
            selectColor = WHITE;
        };
        ~Selector()
        {
            if (img) destroy_bitmap(img);
            img = NULL;
        }
        void set_highlight(bool b);
        int selectColor;
        bool highLighted;
};

class CaixaDeMensagem
{
    public:
        CaixaDeMensagem();
        CaixaDeMensagem(char*);  // Apenas o Conteudo da mensagem o botão de OK será escolhido
        CaixaDeMensagem(char*,char*,char*); // O conteudo da msg e os textos dos dois botões (e.g. OK e Cancel; Sim e Não)

        int show();

        void set_opt1(char*);
        void set_opt2(char*);

        Pos pos;
        BITMAP* tela;

        char *msg;
        char *opt1;
        char *opt2;

        int backgroundColor;
        int fontColor;
        int font;

};

class CheckBox
{
    public:
        CheckBox();
        CheckBox(char*);
        CheckBox(char*,BITMAP*);

        void draw();
        void check_click();

        Pos pos;
        bool state;
        char *text;
        int textColor;
        BITMAP*tela;
        FONT* textFont;
        bool click;

};

typedef struct S
{
    char sName[100];
    char sPass[100];
} save;

char *password_mask(char*s, char*p);

void show_load(int prog, int full, int w, int h);


#endif // GUI_LPIITV

