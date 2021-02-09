#ifndef LPIITV_H
#define LPIITV_H

#include <allegro.h>

#include"usuario.h"
#include"video.h"
#include"defines.h"

class LPIITV
{
    public:
        LPIITV();
        virtual ~LPIITV();

        int first_window();

    protected:

        void first_menu();
        void users_menu();
        void video_menu();

        UserHandle* usersHandle;
        User* tempUser;
        VideoHandle* videoHandle;
        Video* tempVideo;

    private:
        void fazer_login();
        void fazer_cadastro();

        void adm_menu(int);
        void commom_user_menu(int userCode);

        void mostrar_lista_videos(int);
        void mostrar_lista_disponivel(int);
        void mostrar_lista_sugestao(int);
        void assistir_video(int);

        void novo_usuario();
        int procurar_usuario();
        void remover_usuario();
        void modificar_usuario();

        void inserir_video();
        int procurar_video();
        void remover_video();
        void modificar_video();
        void more_info_video();
        int ins_basic_info(Video*);

        int okButtonCode(char* s1, char* s2, int op);
        int menu_video_handle_window(char*,BITMAP*old, int division);
        int menu_user_window(int, BITMAP*old, int division);
        void menu_editUser_window(int userCode);
        int select_type();
        int watch_video(int videoId, int);
        int tag_cloud(int userCode);
        int rate_video();

        int tempI;
        int waitTime;

        int login;
        FILE* log;

};

#endif // LPIITV_H
