#include "../include/usuario.h"
#include "../include/defines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <allegro.h>

User::User()
{
    id = 0;
    name[0] = 0;
    userType = COMMON_USER;
    password[0] = 0;
    videosWatched = 0;
    watchedListId = NULL;
}
User::User(char*s)
{
    if (!s) return;
    strcpy(name, s);
    name[NAME_MAX_LENGHT] = END_STRING;
}

void User::copy(User*u)
{
    id = u->id;
    strcpy(name, u->name);
    strcpy(password, u->password);
    userType = u->userType;
    videosWatched = u->videosWatched;
    watchedListId = u->watchedListId;
}

int User::get_id()
{
    return id;
}
void User::set_id(int i)
{
    id = i;
}
char* User::get_name()
{
    return name;
}
int User::set_name(char*s)
{
    strcpy(name, s);
    if (strlen(s)>NAME_MAX_LENGHT)
    {
        name[NAME_MAX_LENGHT] = END_STRING;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
int User::set_password(char*s)
{
    strcpy(password, s);
    if (strlen(s)>PASS_MAX_LENGHT)
    {
        password[PASS_MAX_LENGHT] = END_STRING;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* User::get_password()
{
    return password;
}
bool User::cmp_password(char*s)
{
    if (!strcmp(password, s)) return true;
    return false;
}
bool User::cmp_name(char*n)
{
    if (!strcmp(name, n)) return true;
    return false;
}
int User::set_userType(int i)
{
    userType = i;
    if (userType!=ADM_USER || userType!=COMMON_USER) return ERROR_FEEDBACK;
    return SUCCESS_FEEDBACK;
}
int User::get_userType()
{
    return userType;
}

int User::insertVideo(int v)
{
    // Tenho que verificar que v corresponde a um ID de video válido antes
    if (wasWatched(v)) return DUPLICATED_ITEM; // Procurar se o Video já não está na lista
    videosWatched++;  // Aumenta a quantidade de videos assistidos
    watchedListId = (int *) realloc(watchedListId, sizeof(int) * videosWatched); // realoca a lista para caber a quantidade nova
    watchedListId[videosWatched-1] = v;
    return SUCCESS_FEEDBACK;

}
int *User::get_watchedListId()
{
    return watchedListId;
}
bool User::wasWatched(int v)
{
    if (watchedListId == NULL) return false;
    for (int i = 0; i<videosWatched; i++)
    {
        //printf("z.%d\n",i);
        if (watchedListId[i] == v)
        {
            return true;
        }
    }
    return false;
}
int User::get_videosWatched()
{
    return videosWatched;
}
void User::set_videosWatched(int i )
{
    videosWatched = i;
}
void User::eraseList()
{
    watchedListId = NULL;
}
/* **************************************************** */

UserHandle::UserHandle(FILE*f)
{
    strcpy(databasePath, USERS_DATABASE);
    log = f;
    fprintf(log, "Inicializando UserHandle...\n");
    ownLog = false;
    inicializador();
}

UserHandle::UserHandle()
{
    strcpy(databasePath, USERS_DATABASE);
    log = fopen("UserHandle_log.txt", "w");
    fprintf(log, "Inicializando UserHandle...\n");
    ownLog = true;
    inicializador();
}

void UserHandle::inicializador()
{
    FILE *database;
    if (database = fopen(databasePath, "rb"))
    {
        fread(&usersAmount, sizeof(int), 1, database);
        fread(&lastId, sizeof(int), 1, database);
        list = (User*) calloc(sizeof(User), usersAmount);
        fread(list, sizeof(User), usersAmount, database);
        fclose(database);
        fprintf(log, "Database encontrada\n");
        fprintf(log, "usersAmount: %d\n", usersAmount);
        fprintf(log, "lastId: %d\n", lastId);
        for (int i = 0; i<usersAmount; i++) // recuperar a lista de videos assistidos
        {
            /* Cria o nome arquivo que irá recuperar a lista de videos assitidos no formato id_do_usuario.db  */
            char temp[10];
            char *path = (char*) calloc(sizeof(char), strlen(USERS_LIST) + strlen(EXTENSION_DB) + strlen(itoa(list[i].get_id(), temp, 10)));
            strcpy(path, USERS_LIST);
            strcat(path, temp);
            strcat(path, EXTENSION_DB);

            database = fopen(path, "rb");
            int tempVideoId; // Reserva um variavel para colocar os IDs do videos para ser adicionado ao usuario
            int tempQtde = list[i].get_videosWatched();
            list[i].eraseList();
            list[i].set_videosWatched(0);

            for (int j = 0; j < tempQtde; j++)
            {
                fread(&(tempVideoId), sizeof(int), 1, database);
                list[i].insertVideo(tempVideoId);
            }
            fprintf(log, "Usuario #%d assistiu %d video(s) e foi recuperado de %s\n", i, list[i].get_videosWatched(), path);
            free(path);
            fclose(database);
        }
    }
    else
    {
        usersAmount = 0;
        list = NULL;
        lastId = 0;
        flag = FLAG_DATABASE_NOT_FOUND;
        fprintf(log, "Database não encontrada\n");
    }
    //alert("INICI USER", NULL, NULL, "OK", NULL, 'o', 0);
}

int UserHandle::insert(User*u)
{
    fprintf(log, "Inserindo Usuario\n");
    if (!u) return ERROR_FEEDBACK; // Se o apontador for NULL retorna ERRO
    fprintf(log, "Nome: %s\nPassword: %s\n", u->get_name(), u->get_password());
    if (u->cmp_name("")) return ERROR_FEEDBACK; // Se não existir nome retorna ERRO
    if (search_user(u->get_name())!=ERROR_FEEDBACK) return DUPLICATED_ITEM;
    usersAmount++;  // Aumenta a quantidade de Usuarios
    lastId++;
    list = (User*) realloc(list, sizeof(User) * usersAmount);   // realoca a lista para caber a quantidade nova
    list[usersAmount-1].copy(u);    //Copia o item novo para o ultimo item da lista;
    return atualizaInfo();
}

int UserHandle::search_user(char*n)
{
    fprintf(log, "Procurando Usuario: %s\n", n);
    for (int i = 0; i < usersAmount; i++)
    {
        if (list[i].cmp_name(n))
        {
            return i;
        }
    }
    return ERROR_FEEDBACK;
}

int UserHandle::remove_user(char *s)
{
    fprintf(log, "Removendo Usuario: %s\n", s);
    int tempI = search_user(s);
    if (tempI == ERROR_FEEDBACK)
    {
        return ERROR_FEEDBACK;
    }
    else
    {
        if (remove_user(tempI)==ERROR_FEEDBACK) return ERROR_FEEDBACK;
    }
    return atualizaInfo();
}

int UserHandle::remove_user(int x)
{
    fprintf(log, "Removendo Usuario: %d\n", x);
    if (x >= usersAmount) return ERROR_FEEDBACK;
    for (int i=x; i<usersAmount-1; i++)
    {
        list[i].copy(&(list[i+1]));
    }
    usersAmount--;
    list = (User*) realloc(list, sizeof(User) * usersAmount);
    fprintf(log, "Removido Usuario: %d\n", x);
    return SUCCESS_FEEDBACK;
}

int UserHandle::modify_name(int i, char*s)
{
    fprintf(log, "Modificando nome para: %s\n", s);
    if (i>=usersAmount) return ERROR_FEEDBACK;
    fprintf(log, "Nome antigo: %s\n", list[i].get_name());
    return list[i].set_name(s);
}
int UserHandle::modify_password(int i, char*s)
{
    fprintf(log, "Modificando password para: %s\n", s);
    if (i>=usersAmount) return ERROR_FEEDBACK;
    fprintf(log, "Password antigo: %s\n", list[i].get_password());
    return list[i].set_password(s);
}

int UserHandle::saveUsers()
{
    atualizaInfo();
    fprintf(log, "Salvando Usuarios\n");
    if (!usersAmount) return SUCCESS_FEEDBACK; // Se não existirem usuarios retorna Sucesso
    FILE *file = fopen(USERS_DATABASE,"ab");
    if (!file) return ERROR_FEEDBACK;
    fwrite(list, sizeof(User), usersAmount, file);  // Salva a lista de usuarios no BD
    fclose(file);
    fprintf(log, "Usuarios Salvos em %s\n", databasePath);
    for (int i = 0; i<usersAmount; i++) // Salva a lista de videos assistidos por cada usuario
    {
        /* Cria o arquivo que irá salvar a lista de videos assitidos no formato id_do_usuario.db  */
        char temp[10];
        char *path = (char*) calloc(sizeof(char), strlen(USERS_LIST) + strlen(EXTENSION_DB) + strlen(itoa(list[i].get_id(), temp, 10)));
        strcpy(path, USERS_LIST);
        strcat(path, temp);
        strcat(path, EXTENSION_DB);

        file = fopen(path, "wb");
        for (int j = 0; j<list[i].get_videosWatched(); j++)
        {
            int tempId = list[i].get_watchedListId()[j];
            fwrite(&(tempId), sizeof(int), 1, file);
            fprintf(log, "Salvado video #%d para usuario %s\n", tempId, list[i].get_name());
        }
        //fwrite((list[i].get_watchedListId()), sizeof(int), list[i].get_videosWatched(), file);
        fprintf(log, "Usuario #%d assistiu %d video(s) e foi salvo em %s\n", i, list[i].get_videosWatched(), path);
        free(path);
    }
    return SUCCESS_FEEDBACK;
}

int UserHandle::login(User*u)
{
    fprintf(log, "Fazendo login: %s; %s\n", u->get_name(), u->get_password());
    int idSearch = search_user(u->get_name());
    fprintf(log, "Resultado: %d\n", idSearch);
    if (idSearch == ERROR_FEEDBACK) return ERROR_FEEDBACK;
    if (list[idSearch].cmp_password(u->get_password()))
    {
        fprintf(log, "Logado\n");
        return idSearch;
    }
    else
    {
        fprintf(log, "Não logado\n");
        return ERROR_FEEDBACK;
    }
}

int UserHandle::atualizaInfo()
{
    fprintf(log, "Atualizando Info\n");
    FILE *aux = fopen(USERS_DATABASE, "wb");
    if (!aux) return ERROR_FEEDBACK;
    fwrite(&usersAmount, sizeof(int), 1, aux);
    fwrite(&lastId, sizeof(int), 1, aux);
    fclose(aux);
    fprintf(log, "usersAmount: %d\n", usersAmount);
    fprintf(log, "lastId: %d\n", lastId);
    fprintf(log, "Atualizado em %s\n", databasePath);
    return SUCCESS_FEEDBACK;
}

void UserHandle::showLista()
{
    fprintf(log, "Mostrando lista de usuarios\n");
    for (int i = 0; i<usersAmount; i++)
    {
        printf("%d: ID %d; Name: ", i, list[i].get_id());
        puts(list[i].get_name());
    }
}

UserHandle::~UserHandle()
{
    fprintf(log, "Finalizando VideoHandle...\n");
    if (ownLog) fclose(log);
    free(list);
}


