#ifndef USUARIO_H
#define USUARIO_H
#include<string.h>
#include<stdio.h>
#include"defines.h"
#include"video.h"

class User
{
    public:
        User();
        User(char*);
        bool cmp_name(char*);
        void copy(User*u);

        int get_id();
        void set_id(int i);
        char* get_name();
        int set_name(char*);
        int set_password(char*);
        char* get_password();
        bool cmp_password(char*);
        int set_userType(int);
        int get_userType();

        int insertVideo(int);
        int *get_watchedListId();
        bool wasWatched(int);
        int get_videosWatched();
        void set_videosWatched(int);
        void eraseList();

    protected:
        int id;
        char name[NAME_MAX_LENGHT+1];
        char password[PASS_MAX_LENGHT+1];
        int userType;
        int videosWatched;
        int *watchedListId;
};

class UserHandle
{
    public:
        UserHandle();
        UserHandle(FILE*);
        ~UserHandle();

        int insert(User*u);
        int search_user(char*);
        int remove_user(char*);
        int remove_user(int);
        int modify_name(int, char*);
        int modify_password(int, char*);
        int saveUsers();

        int login(User*);

        User* get_list(){return list;};
        int get_amount(){return usersAmount;};
        int get_lastId(){return lastId;};
        int get_flag(){return flag;};

        void showLista();

    protected:
        void inicializador();
        int usersAmount;
        User *list;
        int lastId;
        int atualizaInfo();
        int flag;
        char databasePath[50];
        FILE* log;
        bool ownLog;
};


#endif // USUARIO_H
