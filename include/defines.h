#ifndef DEFINED_H
#define DEFINED_H

/** Sistem Defines */

#define END_STRING '\0'
#define STRING_NULL "NULL"
#define ERROR_FEEDBACK -1
#define SUCCESS_FEEDBACK 0
#define DUPLICATED_ITEM 255
#define TRUNCATE 256
#define NO_PASSWORD_DEFINED 257

#define COMMON_USER 258
#define ADM_USER 259

#define MOVIE 260
#define SERIE 261
#define DOCUMENTARY 262
#define SHOW 263

#define FLAG_DATABASE_NOT_FOUND 512

#define NAME_MAX_LENGHT 10
#define PASS_MAX_LENGHT 10
#define VNAME_MAX_LENGHT 20
#define IMGPATH_MAX_LENGHT 200
#define DESCRIPTION_MAX_LENGHT 50

#define VK_ENTER 13
#define VK_BACKSPACE 8

#define FLAG_MSG(s) (s==ERROR_FEEDBACK?"ERROR_FEEDBACK":(s==SUCCESS_FEEDBACK?"SUCCESS_FEEDBACK":(s==DUPLICATED_ITEM?"DUPLICATED_ITEM":(s==TRUNCATE?"TRUNCATE":"ELSE"))))
#define GETTYPE(s) (s==MOVIE?"Movie":(s==SERIE?"Serie":(s==DOCUMENTARY?"Documentary":(s==SHOW?"Show":"ELSE"))))
#define VALIDTYPE(s) (s==MOVIE?true:(s==SERIE?true:(s==DOCUMENTARY?true:(s==SHOW?true:false))))
#define GETSIZEOFTYPE(s) (s==MOVIE?sizeof(Movie):(s==SERIE?sizeof(Serie):(s==DOCUMENTARY?sizeof(Documentary):(s==SHOW?sizeof(Show):0))))
#define USERS_DATABASE "data/users.db"
#define VIDEOS_DATABASE "data/vdb.db"
#define TAGS_DATABASE "data/tags.db"
#define USERS_LIST "data/"
#define EXTENSION_DB ".db"

enum MovieGenre{None=0, Action=1, Fiction=2, Romance=3, Suspense=4};
#define GETMOVIEGENRE(s) (s==1?"Acao":s==2?"Ficcao":s==3?"Romance":s==4?"Suspense":"outro")
#define AMOUNT_OF_MOVIE_GENRES 4

/** Grafical Interface Defines */

#define BLACK 0
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0xFF00
#define BLUE 0xFF
#define YELLOW 0xFFFF00
#define TRANSPARENT 0-1
//#define GRAY 0xC0C0C0
#define GRAY(n) 0x101010 * n

#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP 4

#define TEXT_BOX_MAX_LENGHT 15

#define LOGIN 100
#define REGISTER 101

#define OPENING 100
#define CLOSING 101

#define RMB_FILE "rmb.dat"

#define TNR12 "fonts/TimesNewRoman_12.pcx"

#define ORIENTATION_CENTRE  100
#define ORIENTATION_LEFT    101
#define ORIENTATION_RIGHT   102

#define SORT_ID 0
#define SORT_NAME 1
#define SORT_RATE 2

typedef struct s
{
    int type;
    char s[100];
}File_Name;

#define TYPE_FILE       1
#define TYPE_FOLDER     2

#define MAXIMUM_FPS 100
#define MINIMUM_FPS 70

#define TTYPE int

#endif // DEFINED_H



