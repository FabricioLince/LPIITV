#ifndef VIDEO_H
#define VIDEO_H

#include<stdio.h>
#include<conio.h>
#include<string.h>

#include "defines.h"

typedef struct Tag
{
    char name[16];
} Tag;

class Video
{
    public:
        Video();
        virtual ~Video();

        int copy(Video*);

        int set_name(char*);
        char* get_name();
        bool cmp_name(char*);
        int set_lenght(int);
        int get_lenght();
        int get_year();
        int set_year(int);
        int set_imgPath(char*);
        char* get_imgPath();

        void set_id(int i){id = i;}
        int get_id(){return id;}
        void set_videoType(int i){videoType=i;}
        int get_videoType(){return videoType;}

        int search_tag(char*);
        int add_tag(char*);
        int del_tag(char*);
        int del_tag(int);
        int get_tagAmount();
        Tag* get_tagList();
        int initTagList();

        int do_rate(int);
        float get_rate();

        void clean();

    protected:
        int id;
        char name[VNAME_MAX_LENGHT+1];
        int lenght;
        int year;
        char imgPath[IMGPATH_MAX_LENGHT+1];
        int videoType;

        int tagAmount;
        Tag *tagList;

        int views;
        float rate;
        float totalRate;

    private:
};

class Movie : public Video
{
    public:
        Movie()
        {
            strcpy(director, "");
            strcpy(cast, "");
            strcpy(description, "");
            genre = None;
            set_videoType(MOVIE);
            Video::Video();
        }

        int copy(Movie*v);

        char* get_director();
        int set_director(char*);
        char* get_cast();
        int set_cast(char*);
        char* get_description();
        int set_description(char*);
        MovieGenre get_genre();
        int set_genre(MovieGenre);

    protected:
        char director[NAME_MAX_LENGHT+1];
        char cast[DESCRIPTION_MAX_LENGHT+1];
        char description[DESCRIPTION_MAX_LENGHT+1];
        MovieGenre genre;

};

class Serie : public Movie
{
    public:
        Serie()
        {
            set_amountSeasons(0);
            set_amountEpis(0);
            set_videoType(SERIE);
            Movie::Movie();
        }

        int copy(Serie*v);

        int set_amountEpis(int);
        int get_amountEpis();
        int set_amountSeasons(int);
        int get_amountSeasons();

    protected:
        int amountEpis;
        int amountSeasons;
};

class Documentary : public Video
{
    public:
        Documentary()
        {
            strcpy(director, "");
            strcpy(description, "");
            Video::Video();
        }

        int copy(Documentary*v);

        int set_director(char*);
        char* get_director();
        int set_description(char*);
        char* get_description();

    protected:
        char director[NAME_MAX_LENGHT+1];
        char description[DESCRIPTION_MAX_LENGHT+1];

};

class Show : public Video
{
    public:
        Show()
        {
            strcpy(banda, "");
            strcpy(place, "");
            Video::Video();
        }

        int copy(Show*v);

        int set_banda(char*);
        char* get_banda();
        int set_place(char*);
        char* get_place();

    protected:
        char banda[DESCRIPTION_MAX_LENGHT+1];
        char place[DESCRIPTION_MAX_LENGHT+1];
};

class VideoHandle
{
    public:
        VideoHandle();
        VideoHandle(FILE*);
        ~VideoHandle();

        int insert(Video*);
        int search_video(char*);
        int search_video(int);
        int remove_video(char*);
        int remove_video(int);
        int modify_name(int, char*);
        int modify_lenght(int, int);
        int modify_imgPath(int, char*);
        int saveVideos();

        Video** get_list(){return list2;};
        int get_amount(){return videosAmount;};
        int get_lastId(){return lastId;};
        int get_flag(){return flag;};

        void showLista();
        void showListaFiltradaId(int*, int);
        void show_more_info(int);
        int edit_more_info(int);

        int* get_sugestionList(int*, int);

        void get_listaFiltradaId(int *id, int n);


    protected:
        void inicializador();
        char databasePath[50];
        int videosAmount;
        Video **list2;
        int lastId;
        int atualizaInfo();
        int flag;
        FILE* log;
        bool ownLog;
};

#endif // VIDEO_H
