#include <allegro.h>
#include <string.h>
#include <stdio.h>
#include"include/gui.h"
#include"include/picturelist.h"
#include"include/global_functions.h"
#include "include/usuario.h"
#include "include/video.h"
#include "include/lpiitv.h"

void menu_user_window(char*,BITMAP*old, int division); // quando associar o sistema ao db, o 1st paramentro derр trocado por id do user
int menu_adm_window(char*,BITMAP*old, int division);
int menu_video_handle_window(char*,BITMAP*old, int division);
int FirstWindow();
int okButtonCode(char* s1, char* s2, int op);
void change_screen(BITMAP*old, int division, BITMAP*newOne, int transition);
int conversor();

int main()
{
    LPIITV *tv = new LPIITV;

    init();

    while (tv->first_window());

    delete tv;

    set_gfx_mode(GFX_TEXT, 640, 480, 0, 0);

	return 0;
}
END_OF_MAIN()
