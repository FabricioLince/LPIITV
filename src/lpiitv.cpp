#include "../include/lpiitv.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

char* inputPassword(char *pass, int lngt)
{
    memset(pass, 0, sizeof(pass));
    bool ready=false;
    int i=0;
    while (!ready)
    {
        switch(char c = getch())
        {
            case '\b': //backspace '\b' 8
                if (i==0) break;
                i--;
                pass[i]=0;
                printf("\b \b");
                break;
            case '\r': //return '\r' 13
                ready=true;
                break;
            case 27: //esc
                break;
            default:
                if (i==lngt) break;
                pass[i]=c;
                i++;
                pass[i]=0;
                printf("*");
                break;
        }
    }
/*    printf("\n\"%s\"\n", pass);
    for (int i=0; i<strlen(pass); i++)
    printf("%d; ", pass[i]);
*/
    return pass;
}

char* criar_logName(char*logName2)
{
    char logName[20];
    strcpy(logName2, "log\\");
    strcpy(logName, itoa(time(NULL), logName, 10));
    strcat(logName, "log.txt");
    strcat(logName2, logName);
    return logName2;
}

LPIITV::LPIITV()
{
    // Criando as pastas necessárias
    system("mkdir data");
    system("mkdir log");

    // Criando arquivo de LOG
    char logName[30];
    log = fopen(criar_logName(logName), "w");

    if (log)
    {
        usersHandle = new UserHandle(log);
        videoHandle = new VideoHandle(log);
    }
    else
    {
        usersHandle = new UserHandle;
        videoHandle = new VideoHandle;
    }


    tempUser = new User;
    tempVideo = new Video;

    if (usersHandle->get_flag() == FLAG_DATABASE_NOT_FOUND)
    {   // Caso não encontre um database, cria um usuario administrador padrão
        tempUser->set_id(0);
        tempUser->set_name("adm");
        tempUser->set_password("adm");
        tempUser->set_userType(ADM_USER);
        usersHandle->insert(tempUser);
    }

    waitTime = 10;
}

LPIITV::~LPIITV()
{
    delete usersHandle;
    delete tempUser;
    delete videoHandle;
    delete tempVideo;
    fclose(log);
}

void LPIITV::first_menu()
{
    bool sair=false;

    while (!sair)
    {
        system("cls");
        printf("\n\n\tMenu Inicial\n");
        printf("\n\n\n\t1 - Fazer Login\n");
        printf("\n\t2 - Fazer Cadastro\n");
        printf("\n\tesc - Sair\n");

        switch(getch())
        {
            case '1':
                    fazer_login();
                    break;
            case '2':
                    novo_usuario();
                    break;
            case 27 :
                    sair = true;
                    break;
        }
        system("cls");
    } // While (!sair)

    if (usersHandle->saveUsers()==ERROR_FEEDBACK)
    {
        printf("ERROR in Saving...\n");
        getch();
    }
    if (videoHandle->saveVideos()==ERROR_FEEDBACK)
    {
        printf("ERROR in Saving...\n");
        getch();
    }

}

void LPIITV::fazer_login()
{
    char n[100];
    printf("\n\n\tInsira nome: ");
    fflush(stdin);
    gets(n);
    tempUser->set_name(n);
    printf("\n\tInsira senha: ");
    inputPassword(n, PASS_MAX_LENGHT+1);
    tempUser->set_password(n);
    tempI = usersHandle->login(tempUser);
    if (tempI == ERROR_FEEDBACK)
    {
        printf("\n\n\tNOME OU SENHA INVALIDOS\n");
        getch();
    }
    else
    {
        if (usersHandle->get_list()[tempI].get_userType()==ADM_USER)
        {
            system("cls");
            adm_menu(tempI);
        }
        else
        {
            commom_user_menu(tempI);
        }
    }
}

void LPIITV::adm_menu(int admCode)
{
    bool sair=false;

    while (!sair)
    {
        system("cls");
        printf("\n\n\tMenu Administrador - %s\n", usersHandle->get_list()[admCode].get_name());
        printf("\n\n\n\t1 - Lista de Usuarios\n");
        printf("\n\t2 - Lista de Videos\n");
        printf("\n\tesc - Sair\n");

        switch(getch())
        {
            case '1':
                    users_menu();
                    break;
            case '2':
                    video_menu();
                    break;
            case 27 :
                    sair = true;
                    break;
        }
        system("cls");
    } // While (!sair)

}

void LPIITV::commom_user_menu(int userCode)
{
    int* sugestionList = NULL;
    int endLista = 0;

    bool sair=false;

    while (!sair)
    {
        system("cls");
        printf("\n\n\tMenu Usuario - %s\n", usersHandle->get_list()[userCode].get_name());
        printf("\n\n\n\t1 - Lista de videos ja assistidos - %d\n", usersHandle->get_list()[userCode].get_videosWatched());
        printf("\n\t2 - Lista de Videos disponiveis\n");
        printf("\n\t3 - Lista de Sugestoes\n");
        printf("\n\t4 - Modificar minhas informacoes\n");
        printf("\n\t5 - Assistir Video\n");
        printf("\n\tesc - Sair\n");

        switch(getch())
        {
            case '1':
                    mostrar_lista_videos(userCode);
                    break;
            case '2':
                    mostrar_lista_disponivel(userCode);
                    break;
            case '5':
                    assistir_video(userCode);
                    break;
            case '6':
                    sugestionList = videoHandle->get_sugestionList(usersHandle->get_list()[userCode].get_watchedListId(), usersHandle->get_list()[userCode].get_videosWatched());
                    while (sugestionList[endLista] != -1) endLista++;
                    videoHandle->showListaFiltradaId(sugestionList, endLista);
                    getch();
                    break;
            case 27 :
                    sair = true;
                    break;
        }
        system("cls");
    } // While (!sair)
}

void LPIITV::mostrar_lista_videos(int id)
{
    videoHandle->showListaFiltradaId(usersHandle->get_list()[id].get_watchedListId(), usersHandle->get_list()[id].get_videosWatched());
    getch();
}

void LPIITV::mostrar_lista_disponivel(int id)
{
    system("cls");
    videoHandle->showLista();
    getch();
}

void LPIITV::mostrar_lista_sugestao(int id)
{
    system("cls");

}

void LPIITV::assistir_video(int id)
{
    char tempS[NAME_MAX_LENGHT+1];
    printf("\n\tDigite o Nome do video: ");
    fflush(stdin);
    gets(tempS);

    int tempI = videoHandle->search_video(tempS);
    if (tempI == ERROR_FEEDBACK)
    {
        printf("\n\tNome invalido!\n");
        getch();
        return;
    }
    int tempId = videoHandle->get_list()[tempI]->get_id();
    if (usersHandle->get_list()[id].insertVideo(tempId) == DUPLICATED_ITEM)
    {
        printf("\n\tVideo ja assistido!\n");
        getch();
        return;
    }
    else
    {
        printf("\n\tVideo assistido com sucesso!\n");
        getch();
        return;
    }
}

void LPIITV::users_menu()
{
    bool sair=false;

    while (!sair)
    {
        system("cls");
        puts("");
        usersHandle->showLista();
        printf("\n\t1 - Inserir\n");
        printf("\t2 - Procurar\n");
        printf("\t3 - Remover\n");
        printf("\t4 - Modificar\n");
        printf("\tesc - Sair\n");

        switch(getch())
        {
            case '1':
                    novo_usuario();
                    break;
            case '2':
                    procurar_usuario();
                    break;
            case '3':
                    remover_usuario();
                    break;
            case 27 :
                    sair = true;
                    break;
            case '4':
                    modificar_usuario();
                    break;

        }
    } // While (!sair)


}

void LPIITV::novo_usuario()
{
    char n[100];
    int tempI;
    printf("\n\n\tInsira nome: ");
    fflush(stdin);
    gets(n);
    if (!strcmp(n, "0")) return;
    if (usersHandle->search_user(n)!=ERROR_FEEDBACK)
    {   // Já existe um usuario com esse nome
        printf("Nome de usuario ja usado!");
        getch();
        return;
    }
    tempUser->set_id(usersHandle->get_lastId()+1);
    if (tempUser->set_name(n) == TRUNCATE)
    {
        printf("Nome truncado para \"%s\"!\n", tempUser->get_name());
        getch();
    }
    printf("\n\tInsira senha: ");
    fflush(stdin);
    inputPassword(n, PASS_MAX_LENGHT+1);
    if (tempUser->set_password(n)==TRUNCATE)
    {
        printf("Senha truncada para \"%s\"!\n", tempUser->get_password());
        getch();
    }
    tempUser->set_userType(COMMON_USER);
    tempI = usersHandle->insert(tempUser);
    if (tempI == SUCCESS_FEEDBACK)
    {
        puts("\n\tUsuario criado com sucesso!");
    }
    else if (tempI == ERROR_FEEDBACK)
    {
        puts("\n\tErro ao criar usuario!");
    }
    else if (tempI == DUPLICATED_ITEM)
    {
        puts("\n\tUsuario ja existe!");
    }
    getch();
}

int LPIITV::procurar_usuario()
{
    char n[100];
    int tempI;
    printf("Insira nome: ");
    fflush(stdin);
    gets(n);
    tempI = usersHandle->search_user(n);
    if (tempI == ERROR_FEEDBACK)
    {
        printf("Nao encontrado!\n");
        getch();
    }
    else
    {
        printf("Encontrado na posicao %d\n", tempI);
        getch();
    }
    return tempI;
}

void LPIITV::remover_usuario()
{
    char n[100];
    printf("Insira nome: ");
    fflush(stdin);
    gets(n);
    usersHandle->remove_user(n);
}

void LPIITV::modificar_usuario()
{
    char n[100];
    login = procurar_usuario();
    if (login == ERROR_FEEDBACK)
    {
        printf("Não encontrado!\n");
        getch();
        return;
    }
    printf("Insira novo nome: ");
    fflush(stdin);
    gets(n);
    if (usersHandle->modify_name(login, n)==TRUNCATE)
    {
        printf("Nome truncado!\n");
        getch();
    }
    printf("Insira nova senha: ");
    fflush(stdin);
    gets(n);
    if (usersHandle->modify_password(login, n)==TRUNCATE)
    {
        printf("Senha truncada para %s!\n", usersHandle->get_list()[login].get_password());
        getch();
    }
}

int choose_type()
{
    bool ok=false;
    char choosen[12];
    int select=0;
    memset(choosen, 0, sizeof(choosen));
    while(!ok)
    {
        for (int i=0; i<12; i++) printf("\b");
        for (int i=0; i<12; i++) printf(" ");
        for (int i=0; i<12; i++) printf("\b");
        printf("%s", GETTYPE(select+MOVIE));
        switch(getch())
        {
            case 72: //UP KEY
                    select--;
                    if (select<0) select=3;
                    break;
            case 80: //DOWN KEY
                    select++;
                    if (select>3) select=0;
                    break;
            case '\r': //ENTER
                    ok=true;
                    break;
        }
    }
    return select+MOVIE;
    /*
    MOVIE 260
    SERIE 261
    DOCUMENTARY 262
    SHOW 263 */
}

int LPIITV::ins_basic_info(Video*v)
{
    char n[100];
    int i;
    do
    {
        printf("\nInsira nome: ");
        fflush(stdin);
        gets(n);
        if (!strcmp(n, "0")) return ERROR_FEEDBACK;
        if (v->set_name(n) == TRUNCATE)
        {
            printf("Nome truncado para \"%s\"!\n", tempVideo->get_name());
            getch();
        }
        i = videoHandle->search_video(n);
        if (i!=ERROR_FEEDBACK)
        {
            printf("Nome ja registrado!\n");
        }
    } while(i!=ERROR_FEEDBACK);

    v->set_id(videoHandle->get_lastId()+1);
    printf("Insira Duracao: ");
    scanf("%d",&tempI);
    v->set_lenght(tempI);
    printf("Insira Ano: ");
    scanf("%d",&tempI);
    v->set_year(tempI);

    return SUCCESS_FEEDBACK;
}

void LPIITV::inserir_video()
{
    Movie* tempMovie = NULL;
    Serie* tempSerie = NULL;
    Documentary* tempDoc = NULL;
    Show* tempShow = NULL;
    switch(choose_type())
    {
        case MOVIE:
            tempMovie = new Movie;
            tempMovie->set_videoType(MOVIE);
            if (ins_basic_info(tempMovie)==ERROR_FEEDBACK)
                break;
            //printf("\n Nome: %s\ndur.: %d\nAno: %d\n", tempMovie->get_name(), tempMovie->get_lenght(), tempMovie->get_year());
            videoHandle->insert(tempMovie);
            break;
        case SERIE:
            tempSerie = new Serie;
            tempSerie->set_videoType(SERIE);
            ins_basic_info(tempSerie);
            videoHandle->insert(tempSerie);
            break;
        case DOCUMENTARY:
            tempDoc = new Documentary;
            tempDoc->set_videoType(DOCUMENTARY);
            ins_basic_info(tempDoc);
            videoHandle->insert(tempDoc);
            break;
        case SHOW:
            tempShow = new Show;
            tempShow->set_videoType(SHOW);
            ins_basic_info(tempShow);
            videoHandle->insert(tempShow);
            break;
        default:
            printf("ERRO!\n");
    }
}

int LPIITV::procurar_video()
{
    char n[100];
    printf("Insira nome: ");
    fflush(stdin);
    gets(n);
    tempI = videoHandle->search_video(n);
    if (tempI == ERROR_FEEDBACK)
    {
        printf("Nao encontrado!\n");
        getch();
        return ERROR_FEEDBACK;
    }
    else
    {
        printf("Encontrado na posicao %d\n", tempI);
        getch();
        return tempI;
    }
}

void LPIITV::remover_video()
{
    char n[100];
    printf("Insira nome: ");
    fflush(stdin);
    gets(n);
    videoHandle->remove_video(n);
}

void LPIITV::modificar_video()
{
    char n[100];
    int tempI2;
    printf("Insira nome: ");
    fflush(stdin);
    gets(n);
    int tempI = videoHandle->search_video(n);
    if (tempI == ERROR_FEEDBACK)
    {
        printf("Nao encontrado!\n");
        getch();
        return;
    }
    printf("Insira novo nome: ");
    fflush(stdin);
    gets(n);
    if (videoHandle->modify_name(tempI, n)==TRUNCATE)
    {
        printf("Nome truncado!\n");
        getch();
    }
    printf("Insira nova duracao: ");
    scanf("%d",&tempI2);
    videoHandle->modify_lenght(tempI, tempI2);
}

void LPIITV::more_info_video()
{
    char n[100];
    printf("\n\tDigite o nome do Video: ");
    fflush(stdin);
    gets(n);
    int idVideo = videoHandle->search_video(n);
    if (idVideo == ERROR_FEEDBACK)
    {
        printf("\tVideo nao encontrado");
        getch();
        return;
    }
    int posVideo = idVideo;
    idVideo = videoHandle->get_list()[idVideo]->get_id();

    bool sair = false;

    while (!sair)
    {
        system("cls");
        printf("Id %d\n", idVideo);
        videoHandle->showListaFiltradaId(&idVideo, 1);
        videoHandle->show_more_info(posVideo);
        printf("\n1 - Editar info");
        printf("\nesc - Voltar");

        char newTag[16];

        switch(getch())
        {
            case '1':
                    videoHandle->edit_more_info(idVideo);
                    break;
            case '2':
                    printf("Digite a Tag: ");
                    fflush(stdin);
                    gets(newTag);
                    if (videoHandle->get_list()[posVideo]->add_tag(newTag) == ERROR_FEEDBACK)
                    {
                        puts("ERRO!");
                    }
                    else printf("%s adicianado!\n", newTag);
                    getch();
                    break;
            case 27:    //ESC
                    return;

        }

    }

}

void LPIITV::video_menu()
{
    bool sair=false;
    int selector = 0;
    char select[4];
    strcpy(select, ">  ");

    while (!sair)
    {
        system("cls");
        printf("Lista de Videos: %d\n",videoHandle->get_amount());
        videoHandle->showLista();
        printf("\n\t%s1 - Inserir\n", (selector==0?select:""));
        printf("\n\t%s2 - Procurar\n",(selector==1?select:""));
        printf("\n\t%s3 - Remover\n",(selector==2?select:""));
        printf("\n\t%s4 - Modificar\n", (selector==3?select:""));
        printf("\n\t%s5 - Mais Info\n", (selector==3?select:""));
        printf("\n\t%sesc - Sair\n", (selector==4?select:""));

        switch(getch())
        {
            case '1':
                    inserir_video();
                    break;
            case '2':
                    procurar_video();
                    break;
            case '3':
                    remover_video();
                    break;
            case 27 :
                    sair = true;
                    break;
            case '4':
                    modificar_video();
                    break;
            case '5':
                    more_info_video();
                    break;
            /*
            case 72:    //UP KEY
                    selector--;
                    if (selector<0)selector=4;
                    break;
            case 80:    //DOWN KEY
                    selector++;
                    if (selector>4)selector=0;
                    break;
            */
        }
        system("cls");
    } // While (!sair)


}



