#include "../include/global_functions.h"
#include <string.h>

void init()
{
	int depth, res;

	allegro_init();

	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);

	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 500, 0, 0);
	if (res != 0)
	{
	    allegro_message(allegro_error);
            exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();

/*
	FONT* padrao = font;
    font = load_font(TNR12, NULL, NULL);
    int card;
    int resW;
    int resH;
	gfx_mode_select(&card, &resW, &resH);
	res = set_gfx_mode(card, resW, resH, 0, 0);
	if (res != 0)
	{
	    allegro_message(allegro_error);
        res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 500, 0, 0);
	}
    destroy_font(font);
	font = padrao;
	*/
	/* add other initializations here */
}

void deinit()
{
	clear_keybuf();
	/* add other deinitializations here */
}

bool testMousePos(int x1, int y1, int x2, int y2)
{
    //textprintf_ex(screen, font, 200, 200, WHITE, BLACK, "%d, %d, %d, %d", x1, y1, x2, y2);
    if (mouse_x > x1 && mouse_y > y1 && mouse_x < x2 && mouse_y < y2)
    {
        return true;
    }
    return false;
}

bool testMousePos(Pos*p)
{
    return testMousePos(p->x, p->y, p->x+p->w, p->y+p->h);
}

bool testMousePos(Pos*p, int x, int y)
{
    return testMousePos(p->x+x, p->y+y, p->x+p->w+x, p->y+p->h+y);
}

void inputTextBox(char*box, int MAX_LENGHT)
{
    int timeKey = 0;
    int timeToRepeat = 100;
    for (int input = KEY_A; input <= KEY_Z; input++)
    {
        if (key[input])
        {
            if (strlen(box) < MAX_LENGHT)
            {
                box[strlen(box)+1] = 0;
                if (key_shifts == KB_SHIFT_FLAG || key_shifts == KB_CAPSLOCK_FLAG)
                    box[strlen(box)] = input + 'A' - 1;
                else
                    box[strlen(box)] = input + 'a' - 1;
            }
            while (key[input]);
        }
    }
    for (int input = KEY_0; input <= KEY_9; input++)
    {
        if (key[input])
        {
            if (strlen(box) < MAX_LENGHT)
            {
                box[strlen(box)+1] = 0;
                box[strlen(box)] = input + KEY_0 - 6;
            }
        }
        while (key[input]);
    }
    if (key[KEY_SPACE])
    {
        box[strlen(box)+1] = 0;
        box[strlen(box)] = ' ';
        while(key[KEY_SPACE] && timeKey < timeToRepeat)
        {
            timeKey++;
            rest(1);
        }
    }
    if (key[KEY_BACKSPACE])
    {
        box[strlen(box)-1] = 0;
        while(key[KEY_BACKSPACE] && timeKey < timeToRepeat)
        {
            timeKey++;
            rest(1);
        }
    }
}

void pos()
{
    BITMAP* tela;
    Pos RA;
    RA.x = 100;
    RA.y = 100;
    RA.w = 200;
    RA.h = 200;
    fixed arc1 = 0;
    fixed arc2 = 0;

    {
        //rect(tela, RA.x, RA.y, RA.x+RA.w, RA.y+RA.h, BLACK);
        hline(tela, RA.x+5, RA.y, RA.x+RA.w-5, BLACK);
        hline(tela, RA.x+5, RA.y+RA.h, RA.x+RA.w-5, BLACK);
        //circle(tela, RA.x+5, RA.y+5, 5, BLACK);
        arc(tela, RA.x+5, RA.y+5, arc1, arc2, 5, BLACK);
        circle(tela, RA.x+RA.w-5, RA.y+5, 5, BLACK);
        circle(tela, RA.x+5, RA.y+RA.h-5, 5, BLACK);
        circle(tela, RA.x+RA.w-5, RA.y+RA.h-5, 5, BLACK);

        arc2++;
        if (arc2 % 100 == 0) arc1++;

    }
}

void change_screen(BITMAP*old, int division, BITMAP*newOne, int transition)
{
    BITMAP* tela = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* out_screen1 = create_bitmap(division, SCREEN_H);
    BITMAP* out_screen2 = create_bitmap(SCREEN_W - division, SCREEN_H);
    BITMAP* screen_behind = create_bitmap(SCREEN_W, SCREEN_H);

    if (transition == OPENING)
    {
        if (newOne == NULL)
        {
            clear_to_color(screen_behind, BLUE);
        }
        else draw_sprite(screen_behind, newOne, 0, 0);

        blit(old, out_screen1, 0, 0, 0, 0, division, SCREEN_H);
        blit(old, out_screen2, division, 0, 0, 0, SCREEN_W - division, SCREEN_H);
        int x1 = SCREEN_W;
        int x2 = 0;
        int frames = 50;
        for (int i = 1; i<frames; i++)
        {
            x1 += i;
            x2 += i;
            draw_sprite(tela, screen_behind, 0, 0);
            draw_sprite(tela, out_screen1, - x1 + SCREEN_W, 0);
            draw_sprite(tela, out_screen2, division + x2, 0);
            draw_sprite(screen, tela, 0, 0);
            rest(5);
        }
    }
    else if (transition == CLOSING)
    {
        // No Closing a tela de trás é a tela antiga
        draw_sprite(screen_behind, old, 0, 0);
        blit(newOne, out_screen1, 0, 0, 0, 0, division, SCREEN_H); // parte da esquerda
        blit(newOne, out_screen2, division, 0, 0, 0, SCREEN_W - division, SCREEN_H); // parte da direita

        int x1 = -division;
        int x2 = SCREEN_W;
        int frames = 50;

        for (int i = 1; i<frames; i++)
        {
            x1 += i;
            if (x1>0) x1 = 0;
            x2 -= i;
            if (x2<division) x2 = division;
            draw_sprite(tela, screen_behind, 0, 0);
            draw_sprite(tela, out_screen1, x1, 0);
            draw_sprite(tela, out_screen2, x2, 0);
            draw_sprite(screen, tela, 0, 0);
            rest(5);
        }
    }

    destroy_bitmap(tela);
    destroy_bitmap(out_screen1);
    destroy_bitmap(out_screen2);
    destroy_bitmap(screen_behind);
}


