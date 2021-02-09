#include "../include/video.h"
#include "../include/defines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <allegro.h>

Video::Video()
{
    name[0] = 0;
    year = 2013;
    lenght = 0;
    imgPath[0] = 0;
    tagAmount = 0;
    tagList = NULL;
    rate = 2.5;
    views = 0;
    totalRate = 0;
}

Video::~Video()
{
    free(tagList);
}

int Video::copy(Video*v)
{
    alert("COPYING", NULL, NULL, "OK", NULL, 'o', 0);
    id = v->get_id();
    strcpy(name, v->get_name());
    lenght = v->get_lenght();
    year = v->get_year();
    strcpy(imgPath, v->get_imgPath());
    videoType = v->get_videoType();
    tagAmount = v->get_tagAmount();
    tagList = v->get_tagList();
    rate = v->rate;
    views = v->views;
    totalRate = v->totalRate;
    return SUCCESS_FEEDBACK;
}

int Video::set_name(char*s)
{
    strcpy(name, s);
    if (strlen(s) > VNAME_MAX_LENGHT)
    {
        name[VNAME_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* Video::get_name()
{
    return name;
}
int Video::set_lenght(int i)
{
    lenght = i;
    return SUCCESS_FEEDBACK;
}
int Video::get_lenght()
{
    return lenght;
}
int Video::get_year()
{
    return year;
}
int Video::set_year(int i)
{
    year = i;
    return SUCCESS_FEEDBACK;
}
int Video::set_imgPath(char*s)
{
    strcpy(imgPath, s);
    if (strlen(s) > IMGPATH_MAX_LENGHT)
    {
        imgPath[IMGPATH_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* Video::get_imgPath()
{
    return imgPath;
}
bool Video::cmp_name(char*n)
{
    if (!strcmp(name, n)) return true;
    return false;
}

int Video::search_tag(char*s)
{
    if (!s) return ERROR_FEEDBACK;
    for (int i = 0; i<tagAmount; i++)
    {
        if (strcmp(tagList[i].name, s) == 0) return i;
    }
    return ERROR_FEEDBACK;
}
int Video::add_tag(char*s)
{
    if (!s) return ERROR_FEEDBACK;
    if (search_tag(s) != ERROR_FEEDBACK) return ERROR_FEEDBACK;
    tagAmount++;
    tagList = (Tag*) realloc(tagList, sizeof(Tag) * tagAmount);
    if (!tagList) {tagAmount--; return ERROR_FEEDBACK;}
    strcpy(tagList[tagAmount-1].name, s);
    return SUCCESS_FEEDBACK;
}
int Video::del_tag(char*s)
{
    if (!s) return ERROR_FEEDBACK;
    int tagPos = search_tag(s);
    if (tagPos == ERROR_FEEDBACK) return ERROR_FEEDBACK;
    return del_tag(tagPos);
}
int Video::del_tag(int tagPos)
{
    if (tagPos >= tagAmount) return ERROR_FEEDBACK;
    for (int i = tagPos; i<tagAmount-1; i++)
    {
        strcpy(tagList[i].name, tagList[i+1].name);
    }
    tagAmount--;
    return SUCCESS_FEEDBACK;
}
int Video::get_tagAmount()
{
    return tagAmount;
}
Tag* Video::get_tagList()
{
    return tagList;
}
int Video::initTagList()
{
    tagList = NULL;
    tagAmount = 0;
    return SUCCESS_FEEDBACK;
}

int Video::do_rate(int i)
{
    if (i<1 || i>5) return ERROR_FEEDBACK;
    views++;
    totalRate+=i;
    rate = totalRate/views;
    return i;
}
float Video::get_rate()
{
    return rate;
}
void Video::clean()
{
    Movie* tempMovie;
    Serie* tempSerie;
    Documentary* tempDoc;
    Show* tempShow;
    switch(videoType)
    {
        case MOVIE:
            tempMovie = (Movie*) this;
            tempMovie->set_director("");
            tempMovie->set_description("");
            tempMovie->set_cast("");
            break;
        case SERIE:
            tempSerie = (Serie*) this;
            tempSerie->set_director("");
            tempSerie->set_description("");
            tempSerie->set_cast("");
            tempSerie->set_amountEpis(0);
            tempSerie->set_amountSeasons(0);
            break;
        case DOCUMENTARY:
            tempDoc = (Documentary*) this;
            tempDoc->set_description("");
            tempDoc->set_director("");
            break;
        case SHOW:
            tempShow = (Show*) this;
            tempShow->set_banda("");
            tempShow->set_place("");
            break;
    }
}

int Movie::copy(Movie*v)
{
    puts("a");
    puts(v->get_director());
    printf("Endereco de Diretor: %d\n", (int)director);
    printf("%d\n", set_director(v->get_director()));
    //strcpy(director, v->get_director());
    puts("b");
    strcpy(cast, v->get_cast());
    puts("c");
    strcpy(description, v->get_description());
    puts("d");
    return Video::copy(v);
}

char* Movie::get_director()
{
    return director;
}
int Movie::set_director(char* s)
{
    puts("01");
    //memset(director, 0, sizeof(director));
    puts("02");
    //strcpy(director, s);
    for (int i=0; s[i]!=0; i++)
    {
        printf("laco %d\n", i);
        director[i+1] = 0;
        director[i] = s[i];
    }

    puts("03");
    if (strlen(s) > NAME_MAX_LENGHT)
    {
        puts("04");
        director[NAME_MAX_LENGHT] = 0;
        puts("05");
        return TRUNCATE;
    }
    puts("06");
    return SUCCESS_FEEDBACK;
}
char* Movie::get_cast()
{
    return cast;
}
int Movie::set_cast(char* s)
{
    memset(cast, 0, sizeof(cast));
    strcpy(cast, s);
    if (strlen(s) > DESCRIPTION_MAX_LENGHT)
    {
        cast[DESCRIPTION_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* Movie::get_description()
{
    return description;
}
int Movie::set_description(char* s)
{
    memset(description, 0, sizeof(description));
    strcpy(description, s);
    if (strlen(s) > DESCRIPTION_MAX_LENGHT)
    {
        description[DESCRIPTION_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
MovieGenre Movie::get_genre()
{
    return genre;
}
int Movie::set_genre(MovieGenre mg)
{
    if (mg > 0 && mg < AMOUNT_OF_MOVIE_GENRES)
        genre = mg;
    else
        return ERROR_FEEDBACK;
    return SUCCESS_FEEDBACK;
}

int Serie::set_amountEpis(int i)
{
    amountEpis = i;
    return SUCCESS_FEEDBACK;
}
int Serie::get_amountEpis()
{
    return amountEpis;
}
int Serie::set_amountSeasons(int i)
{
    amountSeasons = i;
    return SUCCESS_FEEDBACK;
}
int Serie::get_amountSeasons()
{
    return amountSeasons;
}

int Documentary::copy(Documentary*v)
{
    set_director(v->get_director());
    set_description(v->get_description());
    return Video::copy(v);
}
char* Documentary::get_director()
{
    return director;
}
int Documentary::set_director(char* s)
{
    memset(director, 0, sizeof(director));
    strcpy(director, s);
    if (strlen(s) > NAME_MAX_LENGHT)
    {
        director[NAME_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* Documentary::get_description()
{
    return description;
}
int Documentary::set_description(char* s)
{
    memset(description, 0, sizeof(description));
    strcpy(description, s);
    if (strlen(s) > DESCRIPTION_MAX_LENGHT)
    {
        description[DESCRIPTION_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}

int Show::set_banda(char*s)
{
    memset(banda, 0, sizeof(banda));
    strcpy(banda, s);
    if (strlen(s) > DESCRIPTION_MAX_LENGHT)
    {
        banda[DESCRIPTION_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* Show::get_banda()
{
    return banda;
}
int Show::set_place(char*s)
{
    memset(place, 0, sizeof(place));
    strcpy(place, s);
    if (strlen(s) > DESCRIPTION_MAX_LENGHT)
    {
        place[DESCRIPTION_MAX_LENGHT] = 0;
        return TRUNCATE;
    }
    return SUCCESS_FEEDBACK;
}
char* Show::get_place()
{
    return place;
}

/* **************************************************** */

VideoHandle::VideoHandle(FILE*f)
{
    log = f;
    fprintf(log, "Inicializando VideoHandle...\n");
    ownLog = false;
    strcpy(databasePath, VIDEOS_DATABASE);
    inicializador();
}

VideoHandle::VideoHandle()
{
    strcpy(databasePath, USERS_DATABASE);
    log = fopen("VideoHandle_log.txt", "w");
    fprintf(log, "Inicializando VideoHandle...\n");
    ownLog = true;
    strcpy(databasePath, VIDEOS_DATABASE);
    inicializador();
}

void VideoHandle::inicializador() // usando list2
{
    //alert("Inicializando videoHandle", NULL, NULL, "OK", NULL, 'o', 0);
    FILE *database = fopen(databasePath, "rb");
    FILE *tagFile = fopen(TAGS_DATABASE, "rb");
    if (database && tagFile)
    {
        fread(&videosAmount, sizeof(int), 1, database);
        fread(&lastId, sizeof(int), 1, database);
        int listaTipos[videosAmount]; // Lista dos tipos de videos que serão carregados
        fread(listaTipos, sizeof(int), videosAmount, database);
        list2 = (Video**) calloc(sizeof(Video*), videosAmount);
        for (int i = 0; i<videosAmount; i++)
        {
            Movie *tempMovie;
            Serie *tempSerie;
            Documentary *tempDocumentary;
            Show *tempShow;
            switch(listaTipos[i])
            {
                case MOVIE:
                    tempMovie = (Movie*) malloc(sizeof(Movie));
                    fread(tempMovie, sizeof(Movie), 1, database);
                    list2[i] = tempMovie;
                    break;
                case SERIE:
                    tempSerie = (Serie*) malloc(sizeof(Serie));
                    fread(tempSerie, sizeof(Serie), 1, database);
                    list2[i] = tempSerie;
                    break;
                case DOCUMENTARY:
                    tempDocumentary = (Documentary*) malloc(sizeof(Documentary));
                    fread(tempDocumentary, sizeof(Documentary), 1, database);
                    list2[i] = tempDocumentary;
                    break;
                case SHOW:
                    tempShow = (Show*) malloc(sizeof(Show));
                    fread(tempShow, sizeof(Show), 1, database);
                    list2[i] = tempShow;
                    break;
            }
            // Carregando Tags
            int tempQtde = list2[i]->get_tagAmount();
            list2[i]->initTagList();
            for (int j = 0; j<tempQtde; j++)
            {
                Tag* tempTag = (Tag*) calloc(sizeof(Tag), 1);
                fread(tempTag, sizeof(Tag), 1, tagFile);
                puts(tempTag->name);
                printf("%d\n", list2[i]->add_tag(tempTag->name));
                free(tempTag);
            }
            fprintf(log, "Recuperado %s; tagA: %d\n", list2[i]->get_name(), list2[i]->get_tagAmount());
        }
        fclose(database);
        fclose(tagFile);
        fprintf(log, "Database Encontrada\n");
        fprintf(log, "videosAmount: %d\n", videosAmount);
        fprintf(log, "lastId: %d\n", lastId);
    }
    else
    {
        videosAmount = 0;
        list2 = NULL;
        lastId = 0;
        flag = FLAG_DATABASE_NOT_FOUND;
        fprintf(log, "Database não encontrada\n");
    }
    //alert("INICI USER", NULL, NULL, "OK", NULL, 'o', 0);
}

int VideoHandle::insert(Video*v) // usando list2
{
    fprintf(log, "Inserindo Video\n");
    fprintf(log, "Nome: %s\nLenght: %d\nYear:%d\nType: %s\n", v->get_name(), v->get_lenght(), v->get_year(),GETTYPE(v->get_videoType()));
    if (!v) return ERROR_FEEDBACK; // Se o apontador for NULL retorna ERRO
    if (search_video(v->get_name())!=ERROR_FEEDBACK) return DUPLICATED_ITEM;
    videosAmount++;  // Aumenta a quantidade de Videos
    lastId++;
    list2 = (Video**) realloc(list2, sizeof(Video*) * videosAmount);   // realoca a lista para caber a quantidade nova
    list2[videosAmount-1] = v; // e o apontador para o conteúdo é colocado na list2
    return atualizaInfo();
}

int VideoHandle::search_video(char*n) //Usando list2
{
    fprintf(log, "Procurando Video: %s\n", n);
    for (int i = 0; i < videosAmount; i++)
    {
        if (list2[i]->cmp_name(n))
        {
            fprintf(log, "Encontrado na posição: %d\n", i);
            return i;
        }
    }
    fprintf(log, "Não encontrado\n");
    return ERROR_FEEDBACK;
}
int VideoHandle::search_video(int id) //Usando list2
{
    fprintf(log, "Procurando Video (id): %d\n", id);
    for (int i = 0; i < videosAmount; i++)
    {
        if (list2[i]->get_id() == id)
        {
            fprintf(log, "Encontrado na posição: %d\n", i);
            return i;
        }
    }
    fprintf(log, "Não encontrado\n");
    return ERROR_FEEDBACK;
}

int VideoHandle::remove_video(char *s) // OK
{
    fprintf(log, "Removendo Video: %s\n", s);
    int tempI = search_video(s);
    if (tempI == ERROR_FEEDBACK)
    {
        fprintf(log, "Video não removido: %s\n", s);
        return ERROR_FEEDBACK;
    }
    else
    {
        fprintf(log, "Video removido: %s\n", s);
        if (remove_video(tempI)==ERROR_FEEDBACK) return ERROR_FEEDBACK;
    }
    return atualizaInfo();
}

int VideoHandle::remove_video(int x) // Usando list2
{
    fprintf(log, "Removendo Video: %d\n", x);
    if (x >= videosAmount) return ERROR_FEEDBACK;
    for (int i=x; i<videosAmount-1; i++)
    {
        list2[i] = list2[i+1];
    }
    videosAmount--;
    list2 = (Video**) realloc(list2, sizeof(Video*) * videosAmount);   // realoca a lista para caber a quantidade nova
    fprintf(log, "Video removido: %d\n", x);
    return SUCCESS_FEEDBACK;
}

int VideoHandle::modify_name(int i, char*s) // Usando list2
{
    fprintf(log, "Modificando nome: %d, %s\n", i, s);
    if (i>=videosAmount) return ERROR_FEEDBACK;
    return list2[i]->set_name(s);
}
int VideoHandle::modify_lenght(int i, int i2) // Usando list2
{
    fprintf(log, "Modificando duração: %d, %d\n", i, i2);
    if (i>=videosAmount) return ERROR_FEEDBACK;
    return list2[i]->set_lenght(i2);
}
int VideoHandle::modify_imgPath(int i, char*s) // Usando list2
{
    fprintf(log, "Modificando imagem: %d, %s\n", i, s);
    if (i>=videosAmount) return ERROR_FEEDBACK;
    return list2[i]->set_imgPath(s);
}

int VideoHandle::saveVideos() // Usando list2
{
    atualizaInfo();
    fprintf(log, "Salvando videos\n");
    if (!videosAmount) return SUCCESS_FEEDBACK; // Se não existirem videos retorna Sucesso
    FILE *file = fopen(databasePath, "ab");
    if (!file) return ERROR_FEEDBACK; // Se não conseguir abrir o arquivo retorna erro
    FILE *tagFile = fopen(TAGS_DATABASE, "wb");
    if (!tagFile) return ERROR_FEEDBACK;
    for (int i = 0; i<videosAmount; i++) // Passa video por video verificando seu tipo para salvar apropriadamente
    {
        fprintf(log, "Salvando %s; tagA: %d\n", list2[i]->get_name(), list2[i]->get_tagAmount());
        fwrite(list2[i]->get_tagList(), sizeof(Tag), list2[i]->get_tagAmount(), tagFile);
        switch(list2[i]->get_videoType())
        {
            case MOVIE:
                fwrite(list2[i], sizeof(Movie), 1, file);
                break;
            case SERIE:
                fwrite(list2[i], sizeof(Serie), 1, file);
                break;
            case DOCUMENTARY:
                fwrite(list2[i], sizeof(Documentary), 1, file);
                break;
            case SHOW:
                fwrite(list2[i], sizeof(Show), 1, file);
                break;
        }
    }
    if (ferror(file))
    {
        fprintf(log, "Erro ao salvar Videos");
        fclose(file);
        fclose(tagFile);
        return ERROR_FEEDBACK;
    }
    fclose(file);
    fclose(tagFile);
    fprintf(log, "Videos salvos em %s\n", databasePath);
    return SUCCESS_FEEDBACK;
}

int VideoHandle::atualizaInfo()
{
    remove(databasePath);
    fprintf(log, "Atualizando Info em %s\n", databasePath);
    FILE *aux = fopen(databasePath, "wb");
    if (!aux) return ERROR_FEEDBACK;
    fwrite(&videosAmount, sizeof(int), 1, aux);
    fprintf(log, "videosAmount: %d\n", videosAmount);
    fwrite(&lastId, sizeof(int), 1, aux);
    fprintf(log, "lastId: %d\n", lastId);
    for (int i = 0; i<videosAmount; i++) // Cria lista com os tipos de video
    {
        int tempI = list2[i]->get_videoType();
        fwrite(&tempI, sizeof(int), 1, aux);
        fprintf(log, "Video #%d: %s\n", i, GETTYPE(tempI));
    }
    if (ferror(aux))
    {
        fclose(aux);
        return ERROR_FEEDBACK;
    }
    fclose(aux);
    return SUCCESS_FEEDBACK;
}

void VideoHandle::showLista() // Usando list2
{
    fprintf(log, "Mostrando Lista de Videos\n");
    for (int i = 0; i<videosAmount; i++)
    {
        printf("%d: ID %d; Name: %s; Lenght %d; Type %s;\n", i, list2[i]->get_id(), list2[i]->get_name(), list2[i]->get_lenght(), GETTYPE(list2[i]->get_videoType()));
    }
}

void VideoHandle::showListaFiltradaId(int *id, int n)
{
    fprintf(log, "Mostrando Lista de Videos filtrados por id \n");
    for (int i = 0; i<videosAmount; i++)
    {
        bool show = false;
        for (int j=0; j<n; j++)
            if (id[j] == list2[i]->get_id())
                show=true;
        if (show)
        printf("%d: ID %d; Name: %s; Lenght %d; Type %s\n", i, list2[i]->get_id(), list2[i]->get_name(), list2[i]->get_lenght(), GETTYPE(list2[i]->get_videoType()));
        //puts(list[i].get_name());
    }
}

void VideoHandle::get_listaFiltradaId(int *id, int n)
{
    fprintf(log, "Mostrando Lista de Videos filtrados por id \n");
    for (int i = 0; i<videosAmount; i++)
    {
        bool show = false;
        for (int j=0; j<n; j++)
            if (id[j] == list2[i]->get_id())
                show=true;
        if (show)
        printf("%d: ID %d; Name: %s; Lenght %d; Type %s\n", i, list2[i]->get_id(), list2[i]->get_name(), list2[i]->get_lenght(), GETTYPE(list2[i]->get_videoType()));
        //puts(list[i].get_name());
    }
}

void VideoHandle::show_more_info(int id)
{

    Movie* tempMovie;
    Serie* tempSerie;
    Documentary* tempDoc;
    Show* tempShow;
    printf("Ano: %d\n", list2[id]->get_year());
    printf("Tipo: %d\n", list2[id]->get_videoType());
    switch(list2[id]->get_videoType())
    {
        case MOVIE:
            tempMovie = (Movie*) list2[id];
            printf("Diretor: %s\n", tempMovie->get_director());
            printf("Elenco: %s\n", tempMovie->get_cast());
            printf("Descricao: %s\n", tempMovie->get_description());
            printf("Genero: %s\n", GETMOVIEGENRE(tempMovie->get_genre()));
            break;
        case SERIE:
            tempSerie = (Serie*) list2[id];
            printf("Diretor: %s\n", tempSerie->get_director());
            printf("Elenco: %s\n", tempSerie->get_cast());
            printf("Descricao: %s\n", tempSerie->get_description());
            printf("Genero: %s\n", GETMOVIEGENRE(tempSerie->get_genre()));
            printf("Qtde de Temporadas: %d\n", tempSerie->get_amountSeasons());
            printf("Qtde de Episodios: %d\n", tempSerie->get_amountEpis());
            break;
        case DOCUMENTARY:
            tempDoc = (Documentary*) list2[id];
            printf("Diretor: %s\n", tempDoc->get_director());
            printf("Descricao: %s\n", tempDoc->get_description());
            break;
        case SHOW:
            tempShow = (Show*) list2[id];
            printf("Banda: %s\n", tempShow->get_banda());
            printf("Lugar: %s\n", tempShow->get_place());
            break;
        default:
            printf("ERRO!\n");
    }
    for (int i = 0; i<list2[id]->get_tagAmount(); i++)
    {
        printf("\t%s\n", list2[id]->get_tagList()[i].name);
    }
}
MovieGenre choose_genre()
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
        printf("%s", GETMOVIEGENRE(select));
        switch(getch())
        {
            case 72: //UP KEY
                    select--;
                    if (select<0) select=4;
                    break;
            case 80: //DOWN KEY
                    select++;
                    if (select>4) select=0;
                    break;
            case '\r': //ENTER
                    ok=true;
                    break;
        }
    }
    return (MovieGenre) select;
    //MovieGenre{None=0, Action=1, Fiction=2, Romance=3, Suspense=4};
}
int VideoHandle::edit_more_info(int id)
{
    Movie* tempMovie;
    Serie* tempSerie;
    Documentary* tempDoc;
    Show* tempShow;
    int tempI;
    char s[100];
    printf("\nDigite Ano: ");
    scanf("%d", &tempI);
    list2[id]->set_year(tempI);
    switch(list2[id]->get_videoType())
    {
        case MOVIE:
            tempMovie = (Movie*) list2[id];
            printf("\nDigite Diretor: ");
            fflush(stdin);
            gets(s);
            tempMovie->set_director(s);
            printf("\nDigite Elenco: ");
            fflush(stdin);
            gets(s);
            tempMovie->set_cast(s);
            printf("\nDigite Descricao:");
            fflush(stdin);
            gets(s);
            tempMovie->set_description(s);
            printf("\nEscolha Genero:\n");
            tempMovie->set_genre(choose_genre());
            break;
        case SERIE:
            tempSerie = (Serie*) list2[id];
            printf("\nDigite Diretor: ");
            fflush(stdin);
            gets(s);
            tempSerie->set_director(s);
            printf("\nDigite Elenco: ");
            fflush(stdin);
            gets(s);
            tempSerie->set_cast(s);
            printf("\nDigite Descricao:");
            fflush(stdin);
            gets(s);
            tempSerie->set_description(s);
            printf("\nEscolha Genero:\n");
            tempMovie->set_genre(choose_genre());
            printf("\nDigite Qtde de Temporadas: ");
            scanf("%d", &tempI);
            tempSerie->set_amountSeasons(tempI);
            printf("\nDigite Qtde de Episodios: ");
            scanf("%d", &tempI);
            tempSerie->set_amountEpis(tempI);
            break;
        case DOCUMENTARY:
            tempDoc = (Documentary*) list2[id];
            printf("\nDigite Diretor: ");
            fflush(stdin);
            gets(s);
            tempDoc->set_director(s);
            printf("\nDigite Descricao: ");
            fflush(stdin);
            gets(s);
            tempDoc->set_description(s);
            break;
        case SHOW:
            tempShow = (Show*) list2[id];
            printf("\nDigite Banda: ");
            fflush(stdin);
            gets(s);
            tempShow->set_banda(s);
            printf("\nDigite Lugar: ");
            fflush(stdin);
            gets(s);
            tempShow->set_place(s);
            break;
        default:
            printf("ERRO!\n");
    }

    return SUCCESS_FEEDBACK;
}

int* VideoHandle::get_sugestionList(int* idList, int n)
{
    int* sugestionList = NULL;
    int sugestionAmount = 0;
    Tag* tagList = NULL;
    int tagAmount = 0;

    /** Criar lista unica de tags */
    {
        for (int i = 0; i<n; i++) // Passando video por video da lista idList
        {
            int videoPos = search_video(idList[i]); // Procura a posição do Video a partir do ID dado
            if (videoPos != ERROR_FEEDBACK)
            {
                int newAmount = list2[videoPos]->get_tagAmount();
                int oldAmount = tagAmount;
                tagAmount += newAmount;
                tagList = (Tag*) realloc(tagList, sizeof(Tag) * tagAmount);
                for (int j = 0; j<newAmount; j++)
                {
                    strcpy(tagList[oldAmount].name, list2[videoPos]->get_tagList()[j].name);
                    oldAmount++;
                }
            }
        }
        // Excluir Tags repetidas
        Tag tempTag;
        for (int i = 0; i<tagAmount; i++)
        {
            strcpy(tempTag.name, tagList[i].name);
            for (int j = i+1; j<tagAmount; j++)
            {
                if (strcmpi(tempTag.name, tagList[j].name) == 0) //
                {
                    for (int k = j; k<tagAmount-1; k++)
                    {
                        strcpy(tagList[k].name, tagList[k+1].name);
                    }
                    tagAmount--;
                }
            }
        }
        printf("Qtde de Tags: %d\n", tagAmount);
        for (int i = 0; i<tagAmount; i++)
        {
            puts(tagList[i].name);
        }
    }

    /** Procurar por videos que possuam aquela Tag e que não estejam na idList */
    {
        for (int i = 0; i<videosAmount; i++) // Passa video por video procurando as Tags
        {
            for (int j = 0; j<tagAmount; j++)
            {
                if (list2[i]->search_tag(tagList[j].name) != ERROR_FEEDBACK)
                {   // Se o video possuir umas das Tags
                    j = tagAmount;
                    // Verifica se o ID do Video está em idList
                    bool passa = false; // se no final do laço seguinte passa == true; o ID do Video está na lista idList
                    for (int k = 0; k<n; k++)
                    {
                        if (list2[i]->get_id() == idList[k]) passa = true;
                    }
                    if (!passa) // se o ID do Video não estiver na lista idList
                    {
                        sugestionAmount++;
                        sugestionList = (int*) realloc(sugestionList, sizeof(int) * sugestionAmount);
                        sugestionList[sugestionAmount-1] = list2[i]->get_id();
                    }
                }
            }

        }
    }
    sugestionAmount++;
    sugestionList = (int*) realloc(sugestionList, sizeof(int) * sugestionAmount);
    sugestionList[sugestionAmount-1] = -1;
    return sugestionList;
}

VideoHandle::~VideoHandle()
{
    fprintf(log, "Finalizando VideoHandle...\n");
    for (int i = 0; i<videosAmount; i++)
    {
        fprintf(log, "Liberando video #%d: %s\n", i, list2[i]->get_name());
        free(list2[i]);
    }
    if (ownLog) fclose(log);
    free(list2);
}


