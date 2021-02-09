#include "../include/lpiitv.h"
#include "../include/gui.h"
#include "../include/picturelist.h"
#include "../include/global_functions.h"
#include <allegro.h>
#include <dirent.h>

int LPIITV::first_window()
{
    int backgroundColor = GRAY(8);
    BITMAP* tela = create_bitmap(SCREEN_W, SCREEN_H);
    clear_to_color(tela, GRAY(12));
    BITMAP* windowsLogin = create_bitmap(SCREEN_W-200, SCREEN_H);
    BITMAP* title = load_bitmap("imgs/title.bmp", NULL);
    if (!title)
    {
        title = create_bitmap(1, 1);
        clear_to_color(title, 0xFF00FF);
    }
    clear_to_color(windowsLogin, backgroundColor);
    FONT* timesNewRoman12 = load_font(TNR12, NULL, NULL);
    if (!timesNewRoman12)
    {
        allegro_message("Não foi possivel carregar a fonte!\nTimesNewRoman_12.pcx");
        exit(1);
    }

    int MAX_LENGHT = 10;
    char box[2][MAX_LENGHT+1];
    memset(box, 0, sizeof(box));
    char passwordBox[MAX_LENGHT+1];

    CaixaDeTexto *tBox1 = new CaixaDeTexto(15, 160, 125);
    CaixaDeTexto *tBox2 = new CaixaDeTexto(15, 160, 150);
    tBox2->is_password(true);
    tBox1->set_legenda("Nome: ", LEFT);
    tBox2->set_legenda("Senha: ", LEFT);
    tBox1->tela = tBox2->tela = windowsLogin;

    CheckBox *remember = new CheckBox("Lembrar Login e Senha");
    {
        remember->tela = windowsLogin;
        remember->textColor = GRAY(12);
        remember->pos.x = 160;
        remember->pos.y = 175;
        remember->pos.w = 10;
        remember->pos.h = 10;
    }

    FONT* padrao = font;
    font = timesNewRoman12;

    Button *okButton = new Button(300, 125, 40, 40, "&OK", windowsLogin);
    Selector *loginButton = new Selector(SCREEN_W-200, 120, 198, 20, "&LOGIN", tela);
    Selector *registerButton = new Selector(SCREEN_W-200, 140, 198, 20, "&CADASTRAR", tela);
    Selector *exitButton = new Selector(SCREEN_W-200, 160, 198, 20, "SAIR", tela);
    {
        okButton->textOrientantion = ORIENTATION_CENTRE;
        okButton->f = timesNewRoman12;
        okButton->colorOnClick = okButton->color - GRAY(3);
        //okButton->color = GRAY(4);
        okButton->colorOnClick = GRAY(6);
        okButton->textColor = backgroundColor;

        loginButton->selectColor = loginButton->color;
        loginButton->color = loginButton->borderColor = GRAY(12);

        registerButton->selectColor = registerButton->color;
        registerButton->color = registerButton->borderColor = GRAY(12);

        exitButton->selectColor = exitButton->color;
        exitButton->color = exitButton->borderColor = GRAY(12);
        loginButton->textOrientantion = registerButton->textOrientantion = exitButton->textOrientantion = ORIENTATION_LEFT;
    }

    int activeTextBox = 0;
    bool sair = false;

    int option = LOGIN;
    int optionAnim = -50; // Variavel que controla a posição do texto que diz a option escolhida (LOGIN/REGISTER)

    PictureList* ad = new PictureList;
    {
        ad->tela = windowsLogin;
        ad->pos.y = SCREEN_H - ad->pos.h - 10;
        ad->pos.w = SCREEN_W-200;
        ad->set_amount(videoHandle->get_amount());
        ad->selectStyle = 0;
        ad->useMouseWheel = true;
        ad->inicializar_paths(0, ad->amount);
        for (int i = 0; i<ad->amount; i++)
        {
            ad->colorList[i].color = makecol(rand()%255, rand()%255, rand()%255);
            if (strlen(videoHandle->get_list()[i]->get_imgPath()) > 0)
            {
                ad->colorList[i].set_img(videoHandle->get_list()[i]->get_imgPath());
                ad->colorList[i].useBitmap = true;
            }
            else
            {
                ad->colorList[i].img = create_bitmap(100, 100);
                clear_to_color(ad->colorList[i].img, makecol(rand()%255, rand()%255, rand()%255));
                textprintf_ex(ad->colorList[i].img, timesNewRoman12, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
                ad->colorList[i].useBitmap = true;
            }
        }

        //alert("1",NULL,NULL,"OK",NULL,'o',0);
    }
    if (videoHandle->get_amount())
        textprintf_ex(windowsLogin, timesNewRoman12, 0, ad->pos.y-timesNewRoman12->height, WHITE, -1, "Em Cartaz");
    int credit_height = SCREEN_H;
    BITMAP* credit = create_bitmap(190, 100);
    {
        clear_to_color(credit, BLACK);
        rectfill(credit, 3, 3, credit->w-3, credit->h-3, GRAY(13));
        textout_centre_ex(credit, timesNewRoman12, "CREDITS", credit->w/2, 14, BLACK, -1);
        textout_centre_ex(credit, padrao, "Code by Mr_Lawboy", credit->w/2, 44, BLACK, -1);
        textout_centre_ex(credit, padrao, "fabricio.1337@gmail.com", credit->w/2, 54, BLACK, -1);
        textout_centre_ex(credit, padrao, "Powered by Allegro", credit->w/2, 74, BLACK, -1);
        textout_centre_ex(credit, padrao, ALLEGRO_VERSION_STR, credit->w/2, 84, BLACK, -1);
    }

    gui_fg_color = GRAY(2);
    gui_bg_color = GRAY(12);

    show_mouse(screen);

    int fps = MAXIMUM_FPS;
    int atualSec = time(NULL);
    int frames = 0;

    // Recuperando dados caso estejam salvos
    {
        FILE*file = fopen(RMB_FILE, "rb");
        if (file)
        {
            remember->state = true;
            save s;
            fread(&s, sizeof(s), 1, file);
            s.sName[tBox1->maxLenght] = '\0';
            s.sPass[tBox2->maxLenght] = '\0';
            strcpy(tBox1->text, s.sName);
            strcpy(tBox2->text, s.sPass);
            fclose(file);
        }

    }

    while(!sair)
    {
        //Draw Login Windows
        tBox1->drawTextBox();
        tBox2->drawTextBox();
        remember->draw();
        remember->check_click();
        okButton->draw();
        loginButton->draw();
        registerButton->draw();
        exitButton->draw();
        ad->draw();
        ad->mouse_movement();

        if (testMousePos(&(loginButton->pos)))
            loginButton->set_highlight(true);
        else loginButton->set_highlight(false);
        if (testMousePos(&(registerButton->pos)))
            registerButton->set_highlight(true);
        else registerButton->set_highlight(false);
        if (testMousePos(&(exitButton->pos)))
            exitButton->set_highlight(true);
        else exitButton->set_highlight(false);

        if (key[KEY_ENTER])
        {
            //position_mouse(okButton->pos.x+okButton->pos.w/2, okButton->pos.y+okButton->pos.h/2);
            mouse_b = 1;
            mouse_x = okButton->pos.x+okButton->pos.w/2;
            mouse_y = okButton->pos.y+okButton->pos.h/2;
        }

        if (mouse_b == 1) // Clique do mouse com botão esquerdo
        {
            if (testMousePos(&(tBox1->pos))) // Ativa caixa de texto de Nome
            {
                activeTextBox = 0;
            }
            else if (testMousePos(&(tBox2->pos))) // Ativa caixa de texto de Senha
            {
                activeTextBox = 1;
            }
            else // Para os Buttons
            {
                if (testMousePos(&(okButton->pos)))
                {
                    if (testMousePos(&(okButton->pos)))
                    {
                        if (!strlen(tBox1->text))
                            alert("Preencha o Campo", "Usuario", NULL,"&OK", NULL, 'o', 0);
                        else if (!strlen(tBox2->text))
                            alert("Preencha o Campo", "Senha", NULL,"&OK", NULL, 'o', 0);
                        else
                        {
                            if (remember->state)
                            {
                                FILE*file = fopen(RMB_FILE, "wb");
                                save s;
                                strcpy(s.sName, tBox1->text);
                                strcpy(s.sPass, tBox2->text);
                                fwrite(&s, sizeof(save), 1, file);
                                fclose(file);
                            }
                            else
                                remove(RMB_FILE);
                            User* temp = new User;
                            temp->set_name(tBox1->text);
                            temp->set_password(tBox2->text);
                            if (option == LOGIN)
                            {
                                int tempI = usersHandle->login(temp);
                                if (tempI == ERROR_FEEDBACK)
                                {
                                    alert("Login Falhou!", NULL, NULL, "&OK", NULL, 'o', 0);
                                }
                                else if (usersHandle->get_list()[tempI].get_userType()==ADM_USER)
                                {
                                    menu_video_handle_window(tBox1->text, screen, SCREEN_W-200);
                                }
                                else
                                {
                                    menu_user_window(tempI, screen, SCREEN_W-200);
                                }
                                key[KEY_ESC] = 0;
                            }
                            if (option == REGISTER)
                            {
                                switch (usersHandle->insert(temp))
                                {
                                    case SUCCESS_FEEDBACK:
                                        alert("Cadastro Realizado", "Com Sucesso", NULL, "&OK", NULL, 'o', 0);
                                        break;
                                    case DUPLICATED_ITEM:
                                        if (alert("Cliente Ja Cadastrado", NULL, NULL, "&OK", "&Esqueci Minha Senha", 'o', 0) == 2)
                                        {
                                            alert("Senha para Cliente:", temp->get_name(),
                                                  usersHandle->get_list()[usersHandle->search_user(temp->get_name())].get_password(), "&OK", NULL, 'o', 0);
                                        }
                                        break;
                                    case ERROR_FEEDBACK:
                                        alert("ERRO Ao realizar Cadastro", NULL, NULL, "&OK", NULL, 'o', 0);
                                        break;
                                }

                            }
                            delete temp;
                        }
                    }

                }
                else if (testMousePos(&(loginButton->pos)))
                {
                    loginButton->click();
                    if (option!=LOGIN)
                    {
                        rectfill(windowsLogin, 0, 40, optionAnim+200, 60, getpixel(windowsLogin, windowsLogin->w-1, windowsLogin->h-1));
                        option = LOGIN;
                        optionAnim = 0;
                    }
                }
                else if (testMousePos(&(registerButton->pos)))
                {
                    registerButton->click();
                    if (option!=REGISTER)
                    {
                        rectfill(windowsLogin, 0, 40, optionAnim+200, 60, getpixel(windowsLogin, windowsLogin->w-1, windowsLogin->h-1));
                        option = REGISTER;
                        optionAnim = 0;
                    }
                }
                activeTextBox = -1;
            }
        }

        if (activeTextBox != -1) // Se existe uma caixa de texto ativa
        {
            if (activeTextBox == 0) // Caixa de texto 1 (tBox1)
            {
                inputTextBox(tBox1->text, tBox1->maxLenght);
                tBox1->active = true;
                tBox2->active = false;
            }
            else if (activeTextBox == 1) // Caixa de texto 2 (tBox2)
            {
                inputTextBox(tBox2->text, tBox2->maxLenght);
                tBox2->active = true;
                tBox1->active = false;
            }
        }
        else    // Se NÃO existe uma caixa de texto ativa
        {
            tBox1->active = false;
            tBox2->active = false;
        }

        if (key[KEY_TAB])
        {
            if (activeTextBox == 0) activeTextBox = 1;
            if ((key[KEY_LSHIFT] || key[KEY_RSHIFT]) && activeTextBox == 1)
            {
                activeTextBox = 0;
            }
            if (activeTextBox != 0 && activeTextBox != 1) activeTextBox = 0;
        }

        if ((testMousePos(&(exitButton->pos)) && mouse_b & 1)||(key[KEY_ESC]))
        {
            exitButton->click();
            sair = true;
        }

        if (key[KEY_S])
        {
            int v[ad->amount];
            for (int i = 0; i< ad->amount; i++)
                v[i] = ad->amount-i-1;
            ad->sort(v);
            for (int i = 0; i< ad->amount; i++)
            {
                if (strlen(videoHandle->get_list()[i]->get_imgPath()) == 0)
                textprintf_ex(ad->colorList[i].img, timesNewRoman12, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
            }
        }
        while(key[KEY_S]);

        /** Movimentação dos CREDITOS */
        if (testMousePos(SCREEN_W-200, credit_height, SCREEN_W, SCREEN_H)) credit_height-=5;
        else credit_height+=5;
        if (credit_height < SCREEN_H-credit->h) credit_height = SCREEN_H-credit->h;
        if (credit_height > SCREEN_H-40) credit_height = SCREEN_H-40;
        rectfill(tela, SCREEN_W-credit->w-5, credit_height-5, SCREEN_W-5, credit_height, getpixel(tela, SCREEN_W-1, SCREEN_H-1));
        draw_sprite(tela, credit, SCREEN_W-credit->w-5, credit_height);

        rectfill(windowsLogin, 0, 40, optionAnim+200, 60, WHITE);
        textprintf_ex(windowsLogin, timesNewRoman12, optionAnim-40, 40, BLACK, -1, "%s", (option==LOGIN?"LOGIN":"CADASTRAR"));
        if (optionAnim<200) optionAnim+=10;

        draw_sprite(tela, title, SCREEN_W - 175, 10);
        draw_sprite(tela, windowsLogin, 0, 0);
        //textprintf_ex(tela, font, 0, 0, BLACK, -1, "%d x %d", fps, waitTime);
        draw_sprite(screen, tela, 0, 0);

        rest(waitTime);

        frames++;
        if (atualSec != time(NULL))
        {
            fps = frames;
            frames = 0;
            atualSec = time(NULL);
            if (fps > MAXIMUM_FPS) waitTime++;
            if (fps < MINIMUM_FPS) waitTime--;
        }
    }   //WHILE(!sair)
    while(key[KEY_ESC]);
    while(mouse_b == 1);

    if (!remember->state) remove(RMB_FILE);

    if (usersHandle->saveUsers()==ERROR_FEEDBACK)
    {
        alert("ERROR in Saving...", NULL, NULL, "&OK", NULL, 'o', 0);
    }
    if (videoHandle->saveVideos()==ERROR_FEEDBACK)
    {
        alert("ERROR in Saving...", NULL, NULL, "&OK", NULL, 'o', 0);
    }

    font = padrao;

    delete tBox1;
    delete tBox2;
    delete okButton;
    delete loginButton;
    delete registerButton;
    delete exitButton;
    delete remember;
    delete ad;

    destroy_bitmap(windowsLogin);
    destroy_bitmap(tela);
    destroy_bitmap(title);
    destroy_bitmap(credit);
    destroy_font(timesNewRoman12);

    return 0;
}

int LPIITV::okButtonCode(char* s1, char* s2, int op)
{
    if (op == LOGIN)
    {
        FILE* file;
        char t[strlen(s1)+10];
        strcpy(t, "data/");
        strcat(t, s1);
        char temp[100];
        if ((file = fopen(strcat(t, ".txt"), "r")) != NULL)
        {
            fscanf(file, "%s", temp);
            fscanf(file, "%s", temp);
            fclose(file);
            if (strcmp(temp, s2) == 0)
                return 0;
            else return -1;
        }
        else return -1;
    }
    else
    {
        FILE* file;
        char t[strlen(s1)+10];
        strcpy(t, "data/");
        strcat(t, s1);
        if ((file = fopen(strcat(t, ".txt"), "w")) == NULL) return -1;
        fprintf(file, "%s\n%s", s1, s2);
        fclose(file);
    }
    return 0;
}

int LPIITV::menu_video_handle_window(char*name, BITMAP*old, int division)
{
    BITMAP* tela = create_bitmap(SCREEN_W, SCREEN_H);

    FONT* timesNewRoman12 = load_font(TNR12, NULL, NULL);

    FONT* padrao = font;
    font = timesNewRoman12;

    PictureList* videoList = new PictureList;
    {
        videoList->tela = tela;
        videoList->pos.x = 30;
        videoList->pos.y = 10;
        videoList->pos.w = SCREEN_W-60;
        videoList->pos.h = 200;
        videoList->picHeight = timesNewRoman12->height+2;
        videoList->picWidth = videoList->pos.w - videoList->spaceAbovePics*2;
        videoList->horizontal = false;
        videoList->set_amount(videoHandle->get_amount());
        videoList->selectStyle = 0;
        videoList->useMouseWheel = true;
        videoList->canTeleport = false;
        for (int i = 0; i<videoList->amount; i++)
        {
            videoList->colorList[i].color = makecol(rand()%255, rand()%255, rand()%255);
            BITMAP* img = create_bitmap(videoList->picWidth, videoList->picHeight);
            clear_to_color(img, WHITE);
            textprintf_ex(img, font, 0, 0, BLACK, -1, "%d: %s : %d", videoHandle->get_list()[i]->get_id(), videoHandle->get_list()[i]->get_name(), videoHandle->get_list()[i]->get_year());
            videoList->colorList[i].img = img;
            videoList->colorList[i].useBitmap = true;
        }
    }
    BITMAP* rollBar = create_bitmap(10, videoList->pos.h);

    BITMAP* sbIco = create_bitmap(20, 20);
    {   /** Desenhando Icone de procura */
        clear_to_color(sbIco, WHITE);
        circlefill(sbIco, 8, 8, 5, GRAY(11));
        circlefill(sbIco, 8, 8, 3, WHITE);
        line(sbIco, 12, 10, 17, 15, GRAY(11));
        line(sbIco, 10, 12, 15, 17, GRAY(11));
        line(sbIco, 17, 15, 15, 17, GRAY(11));
    }
    int sbX = SCREEN_W; // search box position X
    CaixaDeTexto* searchBox = new CaixaDeTexto(TEXT_BOX_MAX_LENGHT, sbX, videoList->pos.y);
    searchBox->tela = tela;
    searchBox->textFont = padrao;
    searchBox->pos.h = 20;
    bool searchResult = false;
    int searchResultPos = -1;

    int shownId = -1;
    CaixaDeTexto* videoNameBox = new CaixaDeTexto(TEXT_BOX_MAX_LENGHT, 150, 220+timesNewRoman12->height);
    CaixaDeTexto* videoYearBox = new CaixaDeTexto(5, 300, 220+timesNewRoman12->height);
    CaixaDeTexto* videoLenghtBox = new CaixaDeTexto(10, 370, 220+timesNewRoman12->height);
    {
        videoNameBox->set_legenda("Nome:", UP);
        videoNameBox->textFont = timesNewRoman12;

        videoYearBox->set_legenda("Ano:", UP);
        videoYearBox->textFont = timesNewRoman12;

        videoLenghtBox->set_legenda("Duracao:", UP);
        videoLenghtBox->textFont = timesNewRoman12;
        videoNameBox->tela = videoYearBox->tela = videoLenghtBox->tela = tela;
        videoNameBox->pos.h = videoYearBox->pos.h = videoLenghtBox->pos.h = timesNewRoman12->height + 2;
        videoNameBox->timeChange = videoYearBox->timeChange = videoLenghtBox->timeChange = 0;
    }

    Folder* videoClassifier = new Folder(150, 265, 420, 220, timesNewRoman12);
    {
        videoClassifier->tela = tela;
        if (videoClassifier->insert("Filme") == -1) return -1;
        if (videoClassifier->insert("Serie") == -1) return -1;
        if (videoClassifier->insert("Documentario") == -1) return -1;
        if (videoClassifier->insert("Show") == -1) return -1;
    }

    /** Caixas de Texto para Movies */
    CaixaDeTexto* movieDirectorBox = new CaixaDeTexto(TEXT_BOX_MAX_LENGHT, 5, timesNewRoman12->height);
    CaixaDeTexto* movieCastBox = new CaixaDeTexto(50, 5, timesNewRoman12->height*3 + 3);
    CaixaDeTexto* movieDescriptionBox = new CaixaDeTexto(50, 5, timesNewRoman12->height*5 + 6);
    {
        movieDirectorBox->set_legenda("Diretor:", UP);
        movieDirectorBox->textFont = timesNewRoman12;
        movieDirectorBox->pos.h = movieDirectorBox->textFont->height + 2;

        movieCastBox->set_legenda("Elenco:", UP);
        movieCastBox->textFont = timesNewRoman12;
        movieCastBox->pos.h = movieCastBox->textFont->height + 2;

        movieDescriptionBox->set_legenda("Descricao:", UP);
        movieDescriptionBox->textFont = timesNewRoman12;
        movieDescriptionBox->pos.h = movieDescriptionBox->textFont->height + 2;
        movieDirectorBox->tela = movieCastBox->tela = movieDescriptionBox->tela = videoClassifier->pageList[0];
        movieDirectorBox->timeChange = movieCastBox->timeChange = movieDescriptionBox->timeChange = 0;
    }

    /** Caixas de Texto para Series */
    CaixaDeTexto* serieDirectorBox = new CaixaDeTexto(TEXT_BOX_MAX_LENGHT, 5, timesNewRoman12->height);
    CaixaDeTexto* serieCastBox = new CaixaDeTexto(50, 5, timesNewRoman12->height*3 + 3);
    CaixaDeTexto* serieDescriptionBox = new CaixaDeTexto(50, 5, timesNewRoman12->height*5 + 6);
    CaixaDeTexto* serieSeasonBox = new CaixaDeTexto(4, 5, timesNewRoman12->height*7 + 12);
    CaixaDeTexto* serieEpisodeBox = new CaixaDeTexto(4, 120, timesNewRoman12->height*7 + 12);
    {
        serieDirectorBox->set_legenda("Diretor:", UP);
        serieDirectorBox->textFont = timesNewRoman12;
        serieDirectorBox->pos.h = serieDirectorBox->textFont->height + 2;

        serieCastBox->set_legenda("Elenco:", UP);
        serieCastBox->textFont = timesNewRoman12;
        serieCastBox->pos.h = serieCastBox->textFont->height + 2;

        serieDescriptionBox->set_legenda("Descricao:", UP);
        serieDescriptionBox->textFont = timesNewRoman12;
        serieDescriptionBox->pos.h = serieDescriptionBox->textFont->height + 2;
        serieDirectorBox->tela = serieCastBox->tela = serieDescriptionBox->tela = videoClassifier->pageList[1];
        serieDirectorBox->timeChange = serieCastBox->timeChange = serieDescriptionBox->timeChange = 0;

        serieSeasonBox->set_legenda("N. Temporadas", UP);
        serieSeasonBox->textFont = serieEpisodeBox->textFont = timesNewRoman12;
        serieSeasonBox->pos.h = serieSeasonBox->textFont->height + 2;
        serieEpisodeBox->set_legenda("N. Episodios", UP);
        serieEpisodeBox->pos.h = serieEpisodeBox->textFont->height + 2;
        serieSeasonBox->tela = serieEpisodeBox->tela = videoClassifier->pageList[1];
        serieSeasonBox->timeChange = serieEpisodeBox->timeChange = 0;
    }

    /** Caixas de Texto para Documentarios */
    CaixaDeTexto* documentDirectorBox = new CaixaDeTexto(TEXT_BOX_MAX_LENGHT, 5, timesNewRoman12->height);
    CaixaDeTexto* documentDescriptionBox = new CaixaDeTexto(50, 5, timesNewRoman12->height*3 + 3);
    {
        documentDirectorBox->set_legenda("Diretor:", UP);
        documentDirectorBox->textFont = timesNewRoman12;
        documentDirectorBox->pos.h = documentDirectorBox->textFont->height + 2;

        documentDescriptionBox->set_legenda("Descricao:", UP);
        documentDescriptionBox->textFont = timesNewRoman12;
        documentDescriptionBox->pos.h = documentDescriptionBox->textFont->height + 2;
        documentDirectorBox->tela = documentDescriptionBox->tela = videoClassifier->pageList[2];
        documentDirectorBox->timeChange = documentDescriptionBox->timeChange = 0;
    }

    /** Caixas de Texto para Shows */
    CaixaDeTexto* showBandBox = new CaixaDeTexto(50, 5, timesNewRoman12->height);
    CaixaDeTexto* showPlaceBox = new CaixaDeTexto(50, 5, timesNewRoman12->height*3 + 3);
    {
        showBandBox->set_legenda("Banda:", UP);
        showBandBox->textFont = timesNewRoman12;
        showBandBox->pos.h = showBandBox->textFont->height + 2;

        showPlaceBox->set_legenda("Local:", UP);
        showPlaceBox->textFont = timesNewRoman12;
        showPlaceBox->pos.h = showPlaceBox->textFont->height + 2;
        showPlaceBox->tela = showBandBox->tela = videoClassifier->pageList[3];
        showPlaceBox->timeChange = showBandBox->timeChange = 0;
    }

    CaixaDeTexto** tagBoxList;
    int tempTagAmount = 20;

    BITMAP* tagListTela;
    PosList* tagList = new PosList;
    {
        tagList->pos.x = 590;
        tagList->pos.y = 260;
        tagList->pos.w = 150;
        tagList->pos.h = 200;
        tagList->picHeight = 20;
        tagList->spaceBetweenPics = 1;
        tagList->horizontal = false;
        tagList->set_amount(0);
        tagList->selectStyle = 0;
        tagList->useMouseWheel = true;
        tagList->canTeleport = false;

        tagListTela = create_bitmap(tagList->pos.w, tagList->pos.h);
        clear_to_color(tagListTela, GRAY(6));

        tagBoxList = (CaixaDeTexto**) calloc(sizeof(CaixaDeTexto*), tempTagAmount);
        for (int i = 0; i<tempTagAmount; i++) // Criando TextBoxes
        {
            CaixaDeTexto* temp = new CaixaDeTexto(15, 10, 0);
            temp->tela = tagListTela;
            temp->textFont = padrao;
            temp->pos.h = padrao->height*2;
            temp->pos.y = temp->pos.h*i;
            tagBoxList[i] = temp;
        }
        for (int i = 0; i<tempTagAmount; i++) // Inserindo TextBoxes na Lista de Pos
        {
            tagList->insert(&(tagBoxList[i]->pos));
        }
    }

    int buttonListY = 250;
    int buttonListDist = 40;
    Button* saveButton = new Button(770, buttonListY, 100, 30, "Salvar", tela);
    Button* cancelButton = new Button(770, buttonListY+buttonListDist, 100, 30, "Cancelar", tela);
    Button* newButton = new Button(770, buttonListY+buttonListDist*2, 100, 30, "Novo", tela);
    Button* delButton = new Button(770, buttonListY+buttonListDist*3, 100, 30, "Deletar", tela);
    Button* infoButton = new Button(770, buttonListY+buttonListDist*4, 100, 30, "MudarInfo.", tela);
    Button* exitButton = new Button(770, buttonListY+buttonListDist*5, 100, 30, "Voltar", tela);
    {
        saveButton->textOrientantion = ORIENTATION_CENTRE;
        saveButton->f = timesNewRoman12;

        cancelButton->textOrientantion = ORIENTATION_CENTRE;
        cancelButton->f = timesNewRoman12;

        newButton->textOrientantion = ORIENTATION_CENTRE;
        newButton->f = timesNewRoman12;

        delButton->textOrientantion = ORIENTATION_CENTRE;
        delButton->f = timesNewRoman12;

        infoButton->textOrientantion = ORIENTATION_CENTRE;
        infoButton->f = timesNewRoman12;

        exitButton->textOrientantion = ORIENTATION_CENTRE;
        exitButton->f = timesNewRoman12;
    }

    BITMAP* shownColor = create_bitmap(100, 100);
    clear_to_color(shownColor, WHITE);
    BITMAP* sPicture = NULL;
    char sPath[IMGPATH_MAX_LENGHT];
    strcpy(sPath, "");
    Pos videoPicture;
    videoPicture.w = 100;
    videoPicture.h = 100;
    videoPicture.x = 30;
    videoPicture.y = 220;

    BITMAP*oldOne = create_bitmap(SCREEN_W, SCREEN_H);
    draw_sprite(oldOne, old, 0, 0);
    /** Mostrar animacao de entrada */
    {
        clear_to_color(tela, GRAY(8));
        clear_to_color(tagListTela, GRAY(6));
        videoList->draw();
        tagList->draw();
        draw_sprite(tela, tagListTela, tagList->pos.x, tagList->pos.y);
        {
            clear_to_color(rollBar, GRAY(11));
            int y = (videoList->roll*rollBar->h/videoList->rollMax); //regra de três simples
            rectfill(rollBar, 0, y-10, rollBar->w, y+10, GRAY(5));
            draw_sprite(tela, rollBar, 20, 10);
        }

        draw_sprite(tela, sbIco, videoList->pos.x+videoList->pos.w+5, videoList->pos.y);
        saveButton->draw();
        cancelButton->draw();
        newButton->draw();
        delButton->draw();
        infoButton->draw();
        exitButton->draw();
        {   /** Draws relacionados aos detalhes do video*/
            draw_sprite(tela, shownColor, 30, 220);
            videoNameBox->drawTextBox();
            videoYearBox->drawTextBox();
            videoLenghtBox->drawTextBox();
            movieDirectorBox->drawTextBox();
            movieCastBox->drawTextBox();
            movieDescriptionBox->drawTextBox();
            serieDirectorBox->drawTextBox();
            serieCastBox->drawTextBox();
            serieDescriptionBox->drawTextBox();
            serieSeasonBox->drawTextBox();
            serieEpisodeBox->drawTextBox();
            documentDirectorBox->drawTextBox();
            documentDescriptionBox->drawTextBox();
            showBandBox->drawTextBox();
            showPlaceBox->drawTextBox();

            videoClassifier->draw();
            videoClassifier->mouse_action();
        }

        change_screen(old, SCREEN_W - 200, tela, OPENING);
    }

    int fps = 0;
    int atualSec = time(NULL);
    int frames = 0;

    bool sair = false;
    while(!sair)
    {
        clear_to_color(tela, GRAY(8));
        clear_to_color(tagListTela, GRAY(6));

        videoList->draw();
        videoList->mouse_movement();
        tagList->draw();
        tagList->mouse_movement();

        for (int i = 0; i<tempTagAmount; i++)
        {
            tagBoxList[i]->drawTextBox();
            if (shownId != -1)
            {
                if (mouse_b == 1)
                {
                    if (testMousePos(&(tagBoxList[i]->pos), tagList->pos.x, tagList->pos.y) && tagList->is_showing(i) == 0)
                    {
                        for (int j = 0; j<tempTagAmount; j++)
                        {
                            if (i == j)
                                tagBoxList[j]->active = true;
                            else
                                tagBoxList[j]->active = false;
                        }
                    }
                    else
                    {
                        for (int j = 0; j<tempTagAmount; j++)
                        {
                            if (i == j)
                                tagBoxList[j]->active = false;
                        }
                    }
                }
                if (tagBoxList[i]->active)
                {
                    inputTextBox(tagBoxList[i]->text, tagBoxList[i]->maxLenght);
                }
            }
        }
        draw_sprite(tela, tagListTela, tagList->pos.x, tagList->pos.y);

        if (videoList->selectedId != -1) /** Selecionando Video Na Lista */
        {
            if (videoList->selectedId != shownId)
            {
                shownId = videoList->selectedId;
                videoList->selectedId = -1;
                clear_to_color(shownColor, videoList->colorList[shownId].color);

                strcpy(sPath, videoHandle->get_list()[shownId]->get_imgPath());
                if (strlen(sPath) > 0)
                {
                    sPicture = load_bitmap(sPath, NULL);
                    if (!sPicture)
                    {
                        sPicture = create_bitmap(1, 1);
                        clear_to_color(sPicture, 0xFF00FF);
                    }
                }
                else
                {
                    if (sPicture) destroy_bitmap(sPicture);
                    sPicture = NULL;
                }
                strcpy(videoNameBox->text, videoHandle->get_list()[shownId]->get_name());
                itoa(videoHandle->get_list()[shownId]->get_year(), videoYearBox->text, 10);
                itoa(videoHandle->get_list()[shownId]->get_lenght(), videoLenghtBox->text, 10);
                videoClassifier->showing = videoHandle->get_list()[shownId]->get_videoType() - MOVIE;
                mouse_b = 1;
                mouse_x = cancelButton->pos.x + 1;
                mouse_y = cancelButton->pos.y + 1;
            }
        }

        {   /** Calcular posição da barra de rolagem */
            clear_to_color(rollBar, GRAY(11));
            int y = (videoList->roll*rollBar->h/videoList->rollMax); //regra de três simples
            rectfill(rollBar, 0, y-10, rollBar->w, y+10, GRAY(5));
            draw_sprite(tela, rollBar, 20, 10);
            if (testMousePos(20, 10, 20+rollBar->w, 10+rollBar->h))
            {
                if (mouse_b == 1)
                {
                    int mouseY = (mouse_y-10)*videoList->rollMax/rollBar->h; //regra de três simples
                    int y2 = y*videoList->rollMax/rollBar->h; //regra de três simples
                    if (y2 < mouseY)
                    {
                        y2 += (mouseY-y2)/10;
                    }
                    else if (y2 > mouseY)
                    {
                        y2 -= (y2-mouseY)/10;
                    }
                    videoList->roll = y2;
                }

            }
        }

        {   /** Operações com a Caixa de Pesquisa */
            if (key[KEY_S] && (key[KEY_RCONTROL] || key[KEY_LCONTROL]))
            {
                position_mouse(videoList->pos.x+videoList->pos.w+5 - (SCREEN_W-sbX) + 2, videoList->pos.y+2);
            }
            searchBox->drawTextBox();
            searchBox->pos.x = sbX;
            if (testMousePos(videoList->pos.x+videoList->pos.w+5 - (SCREEN_W-sbX), videoList->pos.y,
                             videoList->pos.x+videoList->pos.w+5+sbIco->w, videoList->pos.y+sbIco->h))
            {
                sbX-=10;
                searchBox->active = true;
                if (sbX < SCREEN_W - searchBox->pos.w - 25) sbX = SCREEN_W - searchBox->pos.w - 25;

            }
            else if (sbX < SCREEN_W)
            {
                sbX+=10;
                searchBox->active = false;
                strcpy(searchBox->text, "");
            }
            draw_sprite(tela, sbIco, videoList->pos.x+videoList->pos.w+5, videoList->pos.y);
            if (searchBox->active)
            {
                inputTextBox(searchBox->text, TEXT_BOX_MAX_LENGHT);
                if ((key[KEY_ENTER] || mouse_b == 1)&&(strlen(searchBox->text))) // Realizar Pesquisa
                {
                    int tempI = videoHandle->search_video(searchBox->text);
                    if (tempI == ERROR_FEEDBACK)
                    {
                        alert("Nao encontrado", NULL, searchBox->text, "&OK", NULL, 'o', 0);
                    }
                    else
                    {
                        searchResultPos = videoList->selectedId = tempI;
                        searchResult = true;
                    }

                    key[KEY_ENTER] = 0;
                }
            }
            if (searchResult)
            {
                int toMove = videoList->is_showing(searchResultPos);
                if (toMove != 0)
                {
                    videoList->inertia += toMove*4;
                } else searchResult = false;
            }
        }

        {   /** Botões de Controle */
            saveButton->draw();
            cancelButton->draw();
            newButton->draw();
            delButton->draw();
            infoButton->draw();
            exitButton->draw();
            if (testMousePos(&(saveButton->pos)))
            {
                if (mouse_b == 1)   /** Clique no Botao "SALVAR" */
                {
                    saveButton->click();
                    if (shownId != -1)
                    {
                        videoHandle->get_list()[shownId]->set_name(videoNameBox->text);
                        videoHandle->get_list()[shownId]->set_year(atoi(videoYearBox->text));
                        videoHandle->get_list()[shownId]->set_lenght(atoi(videoLenghtBox->text));
                        videoHandle->get_list()[shownId]->set_imgPath(sPath);
                        Movie* tempMovie;
                        Serie* tempSerie;
                        Documentary* tempDoc;
                        Show* tempShow;
                        switch(videoHandle->get_list()[shownId]->get_videoType())
                        {
                            case MOVIE:
                                tempMovie = (Movie*) videoHandle->get_list()[shownId];
                                tempMovie->set_director(movieDirectorBox->text);
                                tempMovie->set_description(movieDescriptionBox->text);
                                tempMovie->set_cast(movieCastBox->text);
                                break;
                            case SERIE:
                                tempSerie = (Serie*) videoHandle->get_list()[shownId];
                                tempSerie->set_director(serieDirectorBox->text);
                                tempSerie->set_description(serieDescriptionBox->text);
                                tempSerie->set_cast(serieCastBox->text);
                                tempSerie->set_amountEpis(atoi(serieEpisodeBox->text));
                                tempSerie->set_amountSeasons(atoi(serieSeasonBox->text));
                                break;
                            case DOCUMENTARY:
                                tempDoc = (Documentary*) videoHandle->get_list()[shownId];
                                tempDoc->set_description(documentDescriptionBox->text);
                                tempDoc->set_director(documentDirectorBox->text);
                                break;
                            case SHOW:
                                tempShow = (Show*) videoHandle->get_list()[shownId];
                                tempShow->set_banda(showBandBox->text);
                                tempShow->set_place(showPlaceBox->text);
                                break;
                        }
                        clear_to_color(videoList->colorList[shownId].img, WHITE);
                        textprintf_ex(videoList->colorList[shownId].img, font, 0, 0, BLACK, -1,
                                "%d: %s : %d", videoHandle->get_list()[shownId]->get_id(), videoHandle->get_list()[shownId]->get_name(), videoHandle->get_list()[shownId]->get_year());
                        // Salvando Tags
                        videoHandle->get_list()[shownId]->initTagList();
                        for (int i = 0; i < tempTagAmount; i++)
                        {
                            if(strlen(tagBoxList[i]->text) > 0)
                            {
                                videoHandle->get_list()[shownId]->add_tag(tagBoxList[i]->text);
                            }
                        }
                        alert("Registro Salvo!", NULL, NULL, "OK", NULL, 'o', 0);
                    }
                }
            }
            if (testMousePos(&(cancelButton->pos)))
            {
                if (mouse_b == 1)   /** Clique no Botao "CANCELAR */
                {
                    cancelButton->click();
                    if (shownId != -1)
                    {
                        Movie* tempMovie;
                        Serie* tempSerie;
                        Documentary* tempDoc;
                        Show* tempShow;
                        switch(videoHandle->get_list()[shownId]->get_videoType())
                        {
                            case MOVIE:
                                tempMovie = (Movie*) videoHandle->get_list()[shownId];
                                strcpy(movieDirectorBox->text, tempMovie->get_director());
                                strcpy(movieDescriptionBox->text, tempMovie->get_description());
                                strcpy(movieCastBox->text, tempMovie->get_cast());
                                break;
                            case SERIE:
                                tempSerie = (Serie*) videoHandle->get_list()[shownId];
                                strcpy(serieDirectorBox->text, tempSerie->get_director());
                                strcpy(serieDescriptionBox->text, tempSerie->get_description());
                                strcpy(serieCastBox->text, tempSerie->get_cast());
                                strcpy(serieEpisodeBox->text, itoa(tempSerie->get_amountEpis(), serieEpisodeBox->text, 10));
                                strcpy(serieSeasonBox->text, itoa(tempSerie->get_amountSeasons(), serieSeasonBox->text, 10));
                                break;
                            case DOCUMENTARY:
                                tempDoc = (Documentary*) videoHandle->get_list()[shownId];
                                strcpy(documentDescriptionBox->text, tempDoc->get_description());
                                strcpy(documentDirectorBox->text, tempDoc->get_director());
                                break;
                            case SHOW:
                                tempShow = (Show*) videoHandle->get_list()[shownId];
                                strcpy(showBandBox->text, tempShow->get_banda());
                                strcpy(showPlaceBox->text, tempShow->get_place());
                                break;
                        }
                        // Pegando Tags
                        for (int i = 0; i< tempTagAmount; i++)
                            strcpy(tagBoxList[i]->text, "");
                        for (int i = 0; i < videoHandle->get_list()[shownId]->get_tagAmount(); i++)
                        {
                            strcpy(tagBoxList[i]->text, videoHandle->get_list()[shownId]->get_tagList()[i].name);
                        }
                    }

                }
            }
            if (testMousePos(&(newButton->pos)))
            {
                if (mouse_b == 1)   /** Clique no Botao "NOVO" */
                {
                    newButton->click();
                    int tipoToAdd = select_type();
                    if (tipoToAdd)
                    {
                        BITMAP* img; // Usado para colocar no videoList
                        /** Adicionando ao VideoHandle */
                        Video* tempVideo;
                        Movie* tempMovie;
                        Serie* tempSerie;
                        Documentary* tempDoc;
                        Show* tempShow;
                        switch (tipoToAdd)
                        {
                            case MOVIE:
                                tempMovie = new Movie;
                                tempVideo = tempMovie;
                                break;
                            case SERIE:
                                tempSerie = new Serie;
                                tempVideo = tempSerie;
                                break;
                            case DOCUMENTARY:
                                tempDoc = new Documentary;
                                tempVideo = tempDoc;
                                break;
                            case SHOW:
                                tempShow = new Show;
                                tempVideo = tempShow;
                                break;
                        }
                        tempVideo->set_videoType(tipoToAdd);
                        char tempS[100];
                        strcpy(tempS, "Registro ");
                        tempVideo->set_name(strcat(tempS, itoa(videoHandle->get_lastId()+1, new char[10], 10)));
                        tempVideo->set_id(videoHandle->get_lastId()+1);
                        switch(videoHandle->insert(tempVideo))
                        {
                            case SUCCESS_FEEDBACK:
                                //alert("SUCESSO", GETTYPE(tempVideo->get_videoType()), NULL, "OK", NULL, 'o', 0);
                                /** Adicionando ao VideoList */
                                videoList->set_amount(videoList->amount+1);
                                videoList->colorList[videoList->amount-1].color = makecol(rand()%255, rand()%255, rand()%255);
                                videoList->inicializar_paths(videoList->amount-1, videoList->amount);
                                img = create_bitmap(videoList->picWidth, videoList->picHeight);
                                clear_to_color(img, WHITE);
                                textprintf_ex(img, font, 0, 0, BLACK, -1, "%s : %d",
                                              videoHandle->get_list()[videoList->amount-1]->get_name(),
                                              videoHandle->get_list()[videoList->amount-1]->get_year());
                                videoList->colorList[videoList->amount-1].img = img;
                                videoList->colorList[videoList->amount-1].path = NULL;
                                videoList->colorList[videoList->amount-1].useBitmap = true;
                                while (int temp = videoList->is_showing(videoList->amount-1))
                                {
                                    videoList->roll += temp*2;
                                }
                                videoList->inertia = 0;
                                shownId = videoList->amount-1;
                                videoClassifier->showing = videoHandle->get_list()[shownId]->get_videoType() - MOVIE;
                                mouse_b = 1;
                                mouse_x = cancelButton->pos.x + 1;
                                mouse_y = cancelButton->pos.y + 1;
                                break;
                            case DUPLICATED_ITEM:
                                alert("Registro Duplicado", itoa(videoHandle->get_amount(), new char[10], 10), NULL, "OK", NULL, 'o', 0);
                                break;
                            case ERROR_FEEDBACK:
                                alert("ERROR", itoa(videoHandle->get_amount(), new char[10], 10), NULL, "OK", NULL, 'o', 0);
                                break;
                        }
                    }

                    rest(100);
                }
            }
            if (testMousePos(&(delButton->pos)))
            {
                if (mouse_b == 1)   /** Clique no Botao "DELETAR" */
                {
                    delButton->click();
                    if (shownId != -1)
                    switch(alert("Deseja realmente deletar", NULL, NULL, "&Sim", "&Nao", 's', 'n'))
                    {
                        case 1:
                                videoHandle->remove_video(shownId);
                                videoList->delete_picture(shownId);
                                shownId = -1;
                                break;
                        case 2:
                                break;
                    }

                }
            }
            if (testMousePos(&(infoButton->pos)))
            {
                if (mouse_b == 1)
                {
                    infoButton->click();
                    menu_editUser_window(0);
                }
            }
            if (testMousePos(&(exitButton->pos)))
            {
                if (mouse_b == 1)   /** Clique no Botao "VOLTAR" */
                {
                    exitButton->click();
                    rest(waitTime);
                    sair = true;
                }
            }
        }

        {   /** Draws relacionados aos detalhes do video*/
            draw_sprite(tela, shownColor, videoPicture.x, videoPicture.y);
            if (sPicture) draw_sprite(tela, sPicture, videoPicture.x, videoPicture.y);
            videoNameBox->drawTextBox();
            videoYearBox->drawTextBox();
            videoLenghtBox->drawTextBox();
            movieDirectorBox->drawTextBox();
            movieCastBox->drawTextBox();
            movieDescriptionBox->drawTextBox();
            serieDirectorBox->drawTextBox();
            serieCastBox->drawTextBox();
            serieDescriptionBox->drawTextBox();
            serieSeasonBox->drawTextBox();
            serieEpisodeBox->drawTextBox();
            documentDirectorBox->drawTextBox();
            documentDescriptionBox->drawTextBox();
            showBandBox->drawTextBox();
            showPlaceBox->drawTextBox();

            videoClassifier->draw();
            //videoClassifier->mouse_action();
        }

        if (shownId != -1) // Trabalhar com caixas de Texto e Video Img
        {
            {   /** Para as caixas de texto comuns */
                if (mouse_b == 1)
                {
                    if (testMousePos(&(videoNameBox->pos)))
                    {
                        videoNameBox->active = true;
                    }
                    else videoNameBox->active = false;
                    if (testMousePos(&(videoYearBox->pos)))
                    {
                        videoYearBox->active = true;
                    }
                    else videoYearBox->active = false;
                    if (testMousePos(&(videoLenghtBox->pos)))
                    {
                        videoLenghtBox->active = true;
                    }
                    else videoLenghtBox->active = false;
                }

                if (videoNameBox->active)
                {
                    inputTextBox(videoNameBox->text, videoNameBox->maxLenght);
                }
                if (videoYearBox->active)
                {
                    inputTextBox(videoYearBox->text, videoYearBox->maxLenght);
                }
                if (videoLenghtBox->active)
                {
                    inputTextBox(videoLenghtBox->text, videoLenghtBox->maxLenght);
                }
            }

            if (videoClassifier->showing == 0) /** Movies */
            {
                if (mouse_b == 1)
                {
                    if (testMousePos(&(movieDirectorBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        movieDirectorBox->active = true;
                    }
                    else movieDirectorBox->active = false;
                    if (testMousePos(&(movieCastBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        movieCastBox->active = true;
                    }
                    else movieCastBox->active = false;
                    if (testMousePos(&(movieDescriptionBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        movieDescriptionBox->active = true;
                    }
                    else movieDescriptionBox->active = false;
                }
                if (movieDirectorBox->active)
                {
                    inputTextBox(movieDirectorBox->text, movieDirectorBox->maxLenght);
                }
                else if (movieCastBox->active)
                {
                    inputTextBox(movieCastBox->text, movieCastBox->maxLenght);
                }
                else if (movieDescriptionBox->active)
                {
                    inputTextBox(movieDescriptionBox->text, movieDescriptionBox->maxLenght);
                }
            }
            if (videoClassifier->showing == 1) /** Series */
            {
                if (mouse_b == 1)
                {
                    if (testMousePos(&(serieDirectorBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        serieDirectorBox->active = true;
                    }
                    else serieDirectorBox->active = false;
                    if (testMousePos(&(serieCastBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        serieCastBox->active = true;
                    }
                    else serieCastBox->active = false;
                    if (testMousePos(&(serieDescriptionBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        serieDescriptionBox->active = true;
                    }
                    else serieDescriptionBox->active = false;
                    if (testMousePos(&(serieSeasonBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        serieSeasonBox->active = true;
                    }
                    else serieSeasonBox->active = false;
                    if (testMousePos(&(serieEpisodeBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        serieEpisodeBox->active = true;
                    }
                    else serieEpisodeBox->active = false;
                }
                if (serieDirectorBox->active)
                {
                    inputTextBox(serieDirectorBox->text, serieDirectorBox->maxLenght);
                }
                else if (serieCastBox->active)
                {
                    inputTextBox(serieCastBox->text, serieCastBox->maxLenght);
                }
                else if (serieDescriptionBox->active)
                {
                    inputTextBox(serieDescriptionBox->text, serieDescriptionBox->maxLenght);
                }
                else if (serieEpisodeBox->active)
                {
                    inputTextBox(serieEpisodeBox->text, serieEpisodeBox->maxLenght);
                }
                else if (serieSeasonBox->active)
                {
                    inputTextBox(serieSeasonBox->text, serieSeasonBox->maxLenght);
                }
            }
            if (videoClassifier->showing == 2) /** Documentaries */
            {
                if (mouse_b == 1)
                {
                    if (testMousePos(&(documentDirectorBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        documentDirectorBox->active = true;
                    }
                    else documentDirectorBox->active = false;
                    if (testMousePos(&(documentDescriptionBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        documentDescriptionBox->active = true;
                    }
                    else documentDescriptionBox->active = false;
                }
                if (documentDirectorBox->active)
                {
                    inputTextBox(documentDirectorBox->text, documentDirectorBox->maxLenght);
                }
                else if (documentDescriptionBox->active)
                {
                    inputTextBox(documentDescriptionBox->text, documentDescriptionBox->maxLenght);
                }
            }
            if (videoClassifier->showing == 3) /** Shows */
            {
                if (mouse_b == 1)
                {
                    if (testMousePos(&(showBandBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        showBandBox->active = true;
                    }
                    else showBandBox->active = false;
                    if (testMousePos(&(showPlaceBox->pos), videoClassifier->pos.x, videoClassifier->pos.y + timesNewRoman12->height))
                    {
                        showPlaceBox->active = true;
                    }
                    else showPlaceBox->active = false;
                }
                if (showBandBox->active)
                {
                    inputTextBox(showBandBox->text, showBandBox->maxLenght);
                }
                else if (showPlaceBox->active)
                {
                    inputTextBox(showPlaceBox->text, showPlaceBox->maxLenght);
                }
            }

            /** Mouse Sobre shownColor */
            if (testMousePos(videoPicture.x, videoPicture.y, videoPicture.x + videoPicture.w, videoPicture.y + videoPicture.h))
            {
                textprintf_centre_ex(tela, timesNewRoman12, 31 + shownColor->w/2, 241 + shownColor->h/2, WHITE, -1, "Carregar Pic.");
                textprintf_centre_ex(tela, timesNewRoman12, 30 + shownColor->w/2, 240 + shownColor->h/2, BLACK, -1, "Carregar Pic.");
                hline(tela, 35, 241 + shownColor->h/2 + timesNewRoman12->height, 25 + shownColor->w, BLACK);
                if (mouse_b == 1)
                {
                    if (file_select_ex("Escolha uma imagem (100x100)", sPath, "BMP", 200, 400, 400))
                    {
                        if (sPicture) destroy_bitmap(sPicture);
                        sPicture = load_bitmap(sPath, NULL);
                        if (!sPicture) strcpy(sPath, "");
                        else if (sPicture->w > 100 || sPicture->h > 100)
                        {
                            BITMAP* tempB;
                            char pathToSave[100];
                            switch(alert("Imagem maior do que", "100 x 100", NULL, "&Recortar", "&Anular", 'r', 'a'))
                            {
                                case 1:
                                    tempB = create_bitmap(100, 100);
                                    clear_to_color(tempB, 0xFF00FF);
                                    draw_sprite(tempB, sPicture, 0, 0);
                                    destroy_bitmap(sPicture);
                                    sPicture = tempB;
                                    strcpy(pathToSave, sPath);
                                    strcat(pathToSave, "_resized.bmp");
                                    save_bitmap(pathToSave, tempB, NULL);
                                    strcpy(sPath, pathToSave);
                                    break;
                                case 2:
                                    destroy_bitmap(sPicture);
                                    sPicture = NULL;
                                    strcpy(sPath, "");
                                    break;
                            }
                        }
                    }
                    while(key[KEY_ESC]);
                    while(mouse_b == 1);
                }
            }
            if (sPicture) // Se há uma picture carregada para poder desassociar do registro
            {
                textprintf_centre_ex(tela, font, videoPicture.x+videoPicture.w/2, videoPicture.y+videoPicture.h+2, BLACK, -1, "Remover Imagem");
                if (testMousePos(videoPicture.x, videoPicture.y+videoPicture.h, videoPicture.x+videoPicture.w, videoPicture.y+videoPicture.h+timesNewRoman12->height))
                {
                    hline(tela, videoPicture.x-10, videoPicture.y+videoPicture.h + timesNewRoman12->height, videoPicture.x+videoPicture.w+10, BLACK);
                    if (mouse_b == 1)
                    {
                        destroy_bitmap(sPicture);
                        sPicture = NULL;
                        strcpy(sPath, "");
                    }
                }
            }
        }
        textprintf_centre_ex(tela, timesNewRoman12, tagList->pos.x+tagList->pos.w/2, tagList->pos.y - timesNewRoman12->height, WHITE, -1, "Tags");
        //textprintf_ex(tela, font, 0, 0, BLACK, -1, "%dx%d;  %d;  %d", mouse_x, mouse_y, shownId, 1);

        //textprintf_ex(tela, font, 0, 0, BLACK, -1, "%d x %d", fps, waitTime);
        draw_sprite(screen, tela, 0, 0);

        rest(waitTime);
        frames++;
        if (atualSec != time(NULL))
        {
            fps = frames;
            frames = 0;
            atualSec = time(NULL);
            if (fps > MAXIMUM_FPS) waitTime++;
            if (fps < MINIMUM_FPS) waitTime--;
        }

        if (key[KEY_ESC]) sair = true;
    } // While (!sair)
    while(key[KEY_ESC]);
    while(mouse_b == 1);

    change_screen(tela, SCREEN_W - 200, oldOne, CLOSING);

    font = padrao;

    delete tagList;
    destroy_bitmap(tagListTela);
    for (int i = 0; i<tempTagAmount; i++)
    {
        delete tagBoxList[i];
    }
    free (tagBoxList);

    delete saveButton;
    delete cancelButton;
    delete newButton;
    delete delButton;
    delete infoButton;
    delete exitButton;

    delete videoList;
    delete searchBox;
    delete videoClassifier;

    delete videoNameBox;
    delete videoYearBox;
    delete videoLenghtBox;

    delete movieDirectorBox;
    delete movieCastBox;
    delete movieDescriptionBox;

    delete serieDirectorBox;
    delete serieCastBox;
    delete serieDescriptionBox;
    delete serieSeasonBox;
    delete serieEpisodeBox;

    delete documentDirectorBox;
    delete documentDescriptionBox;

    delete showBandBox;
    delete showPlaceBox;

    destroy_bitmap(tela);
    destroy_bitmap(oldOne);
    destroy_bitmap(shownColor);
    if (sPicture) destroy_bitmap(sPicture);
    destroy_bitmap(rollBar);
    destroy_bitmap(sbIco);
    destroy_font(timesNewRoman12);
}

int LPIITV::select_type()
{
    int choosen = 0;
    Pos telinha;
    telinha.w = 250;
    telinha.h = 120;
    telinha.x = SCREEN_W/2 - telinha.w/2;
    telinha.y = SCREEN_H/2 - telinha.h/2;
    BITMAP* tela = create_bitmap(telinha.w, telinha.h);
    Button* movie = new Button(10, 10, 110, 40, "Filme", tela);
    Button* serie = new Button(130, 10, 110, 40, "Serie", tela);
    Button* docum = new Button(10, 60, 110, 40, "Documentario", tela);
    Button* show = new Button(130, 60, 110, 40, "Show", tela);
    movie->textOrientantion = serie->textOrientantion = docum->textOrientantion = show->textOrientantion = ORIENTATION_CENTRE;
    while(mouse_b == 1);
    bool quit = false;
    while(!quit)
    {
        clear_to_color(tela, GRAY(4));
        rectfill(tela, 2, 2, tela->w-3, tela->h-3, GRAY(8));

        movie->draw();
        serie->draw();
        docum->draw();
        show->draw();

        if (mouse_b == 1)
        {
            if (testMousePos(&(movie->pos), telinha.x, telinha.y))
            {
                movie->click();
                choosen = MOVIE;
            }
            if (testMousePos(&(serie->pos), telinha.x, telinha.y))
            {
                serie->click();
                choosen = SERIE;
            }
            if (testMousePos(&(docum->pos), telinha.x, telinha.y))
            {
                docum->click();
                choosen = DOCUMENTARY;
            }
            if (testMousePos(&(show->pos), telinha.x, telinha.y))
            {
                show->click();
                choosen = SHOW;
            }
            if (!testMousePos(&(telinha))) quit = true;
        }
        if (key[KEY_F]) choosen = MOVIE;
        if (key[KEY_S]) choosen = SERIE;
        if (key[KEY_D]) choosen = DOCUMENTARY;
        if (key[KEY_W]) choosen = SHOW;
        if (key[KEY_ESC]) quit = true;
        if (choosen) quit = true;

        draw_sprite(screen, tela, telinha.x, telinha.y);
        rest(waitTime);
    }
    while(key[KEY_ESC]);
    while(mouse_b == 1);

    delete movie;
    delete serie;
    delete docum;
    delete show;

    destroy_bitmap(tela);
    return choosen;
}

int LPIITV::watch_video(int videoId, int canWatch)
{
    int videoPos = videoHandle->search_video(videoId);
    if (videoPos == ERROR_FEEDBACK)
        return ERROR_FEEDBACK;
    Movie* tempMovie = (Movie*) videoHandle->get_list()[videoPos];
    Serie* tempSerie = (Serie*) videoHandle->get_list()[videoPos];
    Documentary* tempDoc = (Documentary*) videoHandle->get_list()[videoPos];
    Show* tempShow = (Show*) videoHandle->get_list()[videoPos];
    int temp;
    int heightType;
    {   // Calculando A largura e a altura da Janela

        temp = strlen(videoHandle->get_list()[videoPos]->get_name());
        switch(videoHandle->get_list()[videoPos]->get_videoType())
        {
            case MOVIE:
                    heightType = 215+25;
                    if (temp < strlen(tempMovie->get_director()))
                        temp = strlen(tempMovie->get_director());
                    if (temp < strlen(tempMovie->get_cast()))
                        temp = strlen(tempMovie->get_cast());
                    if (temp < strlen(tempMovie->get_description()))
                        temp = strlen(tempMovie->get_description());
                    break;
            case SERIE:
                    heightType = 260+25;
                    if (temp < strlen(tempSerie->get_director()))
                        temp = strlen(tempSerie->get_director());
                    if (temp < strlen(tempSerie->get_cast()))
                        temp = strlen(tempSerie->get_cast());
                    if (temp < strlen(tempSerie->get_description()))
                        temp = strlen(tempSerie->get_description());
                break;
            case DOCUMENTARY:
                    heightType = 200+25;
                    if (temp < strlen(tempDoc->get_director()))
                        temp = strlen(tempDoc->get_director());
                    if (temp < strlen(tempDoc->get_description()))
                        temp = strlen(tempDoc->get_description());
                break;
            case SHOW:
                    heightType = 200+25;
                    if (temp < strlen(tempShow->get_banda()))
                        temp = strlen(tempShow->get_banda());
                    if (temp < strlen(tempShow->get_place()))
                        temp = strlen(tempShow->get_place());
                break;
        }
    }

    BITMAP* sPicture = load_bitmap(videoHandle->get_list()[videoPos]->get_imgPath(), NULL);

    Pos window;
    window.w = 85 + temp * 8;
    if (window.w < 230) window.w = 230;
    if (sPicture) window.w += 120;
    window.h = heightType;
    window.x = SCREEN_W/2-window.w/2;
    window.y = SCREEN_H/2-window.h/2;
    BITMAP*tela = create_bitmap(window.w, window.h);
    clear_to_color(tela, GRAY(8));
    rectfill(tela, 3, 3, tela->w-4, tela->h-4, GRAY(9));

    int x = 10 + (sPicture?120:0);
    int y = 10+font->height+2;
    int color = BLACK;

    BITMAP*infoTela = create_bitmap(window.w, window.h);
    clear_to_color(infoTela, GRAY(8));

    textprintf_ex(infoTela, font, x, y, BLACK, -1, "Nome: %s", videoHandle->get_list()[videoPos]->get_name());
    y+=font->height+2;
    textprintf_ex(infoTela, font, x, y, BLACK, -1, "Ano: %d", videoHandle->get_list()[videoPos]->get_year());
    y+=font->height+2;
    textprintf_ex(infoTela, font, x, y, BLACK, -1, "Duracao: %d", videoHandle->get_list()[videoPos]->get_lenght());
    y+=font->height+2;

    switch(videoHandle->get_list()[videoPos]->get_videoType()) //Escrevendo Inforamções especificas
    {
        case MOVIE:
                textprintf_centre_ex(infoTela, font, window.w/2, 10, BLACK, -1, "Filme");
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Diretor: %s", tempMovie->get_director());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Elenco: %s", tempMovie->get_cast());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Descricao: %s", tempMovie->get_description());
                y+=font->height+2;
            break;
        case SERIE:
                textprintf_centre_ex(infoTela, font, window.w/2, 10, BLACK, -1, "Serie");
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Diretor: %s", tempSerie->get_director());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Elenco: %s", tempSerie->get_cast());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Descricao: %s", tempSerie->get_description());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Temporadas: %d", tempSerie->get_amountSeasons());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Episodios: %d", tempSerie->get_amountEpis());
                y+=font->height+2;
            break;
        case DOCUMENTARY:
                textprintf_centre_ex(infoTela, font, window.w/2, 10, BLACK, -1, "Documentario");
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Diretor: %s", tempDoc->get_director());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Descricao: %s", tempDoc->get_description());
                y+=font->height+2;
            break;
        case SHOW:
                textprintf_centre_ex(infoTela, font, window.w/2, 10, BLACK, -1, "Show");
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Banda: %s", tempShow->get_banda());
                y+=font->height+2;
                textprintf_ex(infoTela, font, x, y, BLACK, -1, "Local: %s", tempShow->get_place());
                y+=font->height+2;
            break;
    }

    Button* watchButton = new Button(10, tela->h - 50, 100, 30, "Assistir", tela);
    watchButton->textOrientantion = ORIENTATION_CENTRE;
    if (!canWatch)
    {
        watchButton->textColor = GRAY(12);
        //watchButton->color = GRAY(12);
    }
    Button* exitButton = new Button(120, tela->h - 50, 100, 30, "Voltar", tela);
    exitButton->textOrientantion = ORIENTATION_CENTRE;

    Pos ratePos;
    ratePos.w = 100;
    ratePos.h = 20;
    if (sPicture)
    {
        ratePos.x = 10;
        ratePos.y = 20+sPicture->h;
    }
    else
    {
        ratePos.x = x;
        ratePos.y = y;
    }

    BITMAP*starRate = create_bitmap(100, 20);
    clear_to_color(starRate, BLACK);
    BITMAP*star = load_bitmap("imgs/star.bmp", NULL);
    float rate = videoHandle->get_list()[videoPos]->get_rate();
    floodfill(star, 0,0, GRAY(8));
    {
        int temp = (int)(rate*100/5);
        rectfill(starRate, 0, 0, temp, star->h-1, RED+GREEN);
    }
    for (int i = 0; i<5; i++)
        draw_sprite(starRate, star, i*star->w, 0);
    destroy_bitmap(star);

    int mouseOverTime = 0;

    bool assistiu = false;
    bool sair = false;
    while(!sair)
    {
        draw_sprite(tela, infoTela, 0, 0);
        watchButton->draw();
        exitButton->draw();
        if (sPicture)
        {
            draw_sprite(tela, sPicture, 10, 20);
        }
        draw_sprite(tela, starRate, ratePos.x, ratePos.y);
        if (testMousePos(&(ratePos), window.x, window.y))
        {
            mouseOverTime++;
        }
        else mouseOverTime = 0;

        if (mouseOverTime > 200)
        {
            textprintf_ex(tela, font, 10+mouse_x-window.x, mouse_y-window.y, BLACK, GRAY(10), "Rate: %.2f", rate);
        }

        if (mouse_b == 1)
        {
            if (!testMousePos(&window))
            {
                sair = true;
            }
            if (testMousePos(&(watchButton->pos), window.x, window.y) && canWatch)
            {
                watchButton->click();
                draw_sprite(screen, tela, window.x, window.y);
                while (mouse_b == 1);
                if (testMousePos(&(watchButton->pos), window.x, window.y))
                {
                    sair = true;
                    assistiu = true;
                }

            }
            if (testMousePos(&(exitButton->pos), window.x, window.y))
            {
                exitButton->click();
                draw_sprite(screen, tela, window.x, window.y);
                while (mouse_b == 1);
                if (testMousePos(&(exitButton->pos), window.x, window.y))
                    sair = true;
            }
        }

        draw_sprite(screen, tela, window.x, window.y);
        clear_to_color(tela, GRAY(8));
        if (key[KEY_ESC]) sair = true;
        rest(waitTime);
    }
    while (mouse_b == 1);
    while (key[KEY_ESC]);

    if (assistiu) // Dar nota se assstiu
    {
        for (int i = 0; i<window.w; i+=2)
            for (int j = 0; j<window.h; j+=2)
            {
                putpixel(tela, i, j, BLACK);
            }
        draw_sprite(screen, tela, window.x, window.y);
        videoHandle->get_list()[videoPos]->do_rate(rate_video());
    }

    delete watchButton;
    delete exitButton;

    destroy_bitmap(tela);
    destroy_bitmap(infoTela);
    if (sPicture) destroy_bitmap(sPicture);

    if (assistiu) return SUCCESS_FEEDBACK;
    return ERROR_FEEDBACK;
}

int LPIITV::rate_video()
{
    Pos window;
    window.w = 100;
    window.h = 40;
    window.x = SCREEN_W/2 - window.w/2;
    window.y = SCREEN_H/2 - window.h/2;

    BITMAP* tela = create_bitmap(window.w, window.h);

    BITMAP*starRate = create_bitmap(100, 20);
    clear_to_color(starRate, BLACK);
    BITMAP*star = load_bitmap("imgs/star.bmp", NULL);
    int rate = 1;
    floodfill(star, 0,0, GRAY(8));
    rectfill(starRate, 0, 0, rate*100/5, star->h-1, RED+GREEN);
    for (int i = 0; i<5; i++)
        draw_sprite(starRate, star, i*star->w, 0);

    bool sair = false;
    while (!sair)
    {
        clear_to_color(tela, GRAY(5));
        textprintf_centre_ex(tela, font, window.w/2, 0, BLACK, -1, "De uma nota:");
        draw_sprite(tela, starRate, 0, window.h-starRate->h);

        if (testMousePos(window.x, window.y+window.h-starRate->h, window.x+window.w, window.y+window.h))
        {
            rate = (mouse_x - window.x)/star->w +1;
            clear_to_color(starRate, BLACK);
            floodfill(star, 0,0, GRAY(8));
            rectfill(starRate, 0, 0, rate*100/5, star->h-1, RED+GREEN);
            for (int i = 0; i<5; i++)
                draw_sprite(starRate, star, i*star->w, 0);
            if (mouse_b == 1)
            {
                return rate;
            }
        }

        if (mouse_b == 1 && !testMousePos(&window)) sair = true;
        if (key[KEY_ESC]) sair = true;
        draw_sprite(screen, tela, window.x, window.y);
        rest(waitTime);
    }
    while(key[KEY_ESC]);
    while(mouse_b);


    destroy_bitmap(tela);
    destroy_bitmap(starRate);
    destroy_bitmap(star);
}

int LPIITV::menu_user_window(int userCode, BITMAP*old, int division)
{
    BITMAP* tela = create_bitmap(SCREEN_W, SCREEN_H);
    clear_to_color(tela, GRAY(4));

    FONT* timesNewRoman12 = load_font(TNR12, NULL, NULL);
    FONT* padrao = font;
    font = timesNewRoman12;

    int shownMenu = 30;
    bool mouseOverMenu = false;
    BITMAP* user_menu = create_bitmap(180, SCREEN_H);
    Pos user_menu_pos;
    {
        clear_to_color(user_menu, GRAY(12));
        rectfill(user_menu, 0, 0, shownMenu, user_menu->h, GRAY(11));
        user_menu_pos.x = SCREEN_W - shownMenu;
        user_menu_pos.y = 0;
        user_menu_pos.w = user_menu->w;
        user_menu_pos.h = user_menu->h;
        textprintf_ex(user_menu, timesNewRoman12, shownMenu, 0, BLACK, -1, "Bem-Vindo(a)");
        textprintf_ex(user_menu, timesNewRoman12, shownMenu, timesNewRoman12->height, BLACK, -1, " %s", usersHandle->get_list()[userCode].get_name());

        vline(user_menu, shownMenu-5, 10, 40, GRAY(4));
        line(user_menu, shownMenu-5, 10, 4, 25, GRAY(4));
        line(user_menu, shownMenu-5, 40, 4, 25, GRAY(4));
        vline(user_menu, shownMenu-5, 10, 40, GRAY(10));
        line(user_menu, shownMenu-5, 11, 4, 26, GRAY(10));
        line(user_menu, shownMenu-5, 40, 4, 26, GRAY(10));

    }
    Selector *changeInfo = new Selector(shownMenu+1, 100, user_menu->w, 20, "Modificar Info.", user_menu);
    {
        changeInfo->selectColor = GRAY(13);
        changeInfo->color = GRAY(12);
        changeInfo->borderColor = GRAY(12);
        changeInfo->f = padrao;
        changeInfo->textOrientantion = ORIENTATION_LEFT;
    }
    Selector *logoff = new Selector(shownMenu+1, 120, user_menu->w, 20, "Logoff", user_menu);
    {
        logoff->selectColor = GRAY(13);
        logoff->color = GRAY(12);
        logoff->borderColor = GRAY(12);
        logoff->f = padrao;
        logoff->textOrientantion = ORIENTATION_LEFT;
    }

    PictureList *watchedList = new PictureList;
    {
        watchedList->tela = tela;
        watchedList->pos.y = 20;
        watchedList->pos.w = SCREEN_W-shownMenu;
        watchedList->set_amount(usersHandle->get_list()[userCode].get_videosWatched());
        watchedList->selectStyle = 4;
        watchedList->inicializar_paths(0, watchedList->amount);
        int videoPos = 0;
        for (int i=0; i<videoHandle->get_amount(); i++)
        {
            bool passa = false;
            for (int j = 0; j<usersHandle->get_list()[userCode].get_videosWatched(); j++) // verificando se o video ja foi assistido
            {
                if (videoHandle->get_list()[i]->get_id() == usersHandle->get_list()[userCode].get_watchedListId()[j])
                {
                    passa = true;
                    j = usersHandle->get_list()[userCode].get_videosWatched();
                }
            }
            if (passa)
            {
                if (strlen(videoHandle->get_list()[i]->get_imgPath()) > 0)
                {
                    watchedList->colorList[videoPos].set_img(videoHandle->get_list()[i]->get_imgPath());
                    watchedList->colorList[videoPos].useBitmap = true;
                }
                else
                {
                    watchedList->colorList[videoPos].path = new char[10];
                    watchedList->colorList[videoPos].img = create_bitmap(100, 100);
                    clear_to_color(watchedList->colorList[videoPos].img, makecol(rand()%255, rand()%255, rand()%255));
                    textprintf_ex(watchedList->colorList[videoPos].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
                    //textprintf_ex(watchedList->colorList[videoPos].img, padrao, 0, 40, BLACK, WHITE, "%d", videoHandle->get_list()[i]->get_id());
                    watchedList->colorList[videoPos].useBitmap = true;
                }
                videoPos++;
            }
        }
    }

    int *sugestionListId = videoHandle->get_sugestionList(usersHandle->get_list()[userCode].get_watchedListId() ,usersHandle->get_list()[userCode].get_videosWatched());
    int sugestionAmount = 0;
    while (sugestionListId[sugestionAmount]!=-1) sugestionAmount++;
    int sugestionListSort = SORT_RATE;
    PictureList *sugestionList = new PictureList;
    {
        sugestionList->tela = tela;
        sugestionList->pos.y = 160;
        sugestionList->inertia = sugestionList->roll = 0;
        sugestionList->pos.w = SCREEN_W-shownMenu;

        sugestionList->set_amount(sugestionAmount);
        sugestionList->inicializar_paths(0, sugestionList->amount);
        for (int i=0; i<sugestionAmount; i++)
        {
            int videoPos = videoHandle->search_video(sugestionListId[i]);
            if (strlen(videoHandle->get_list()[videoPos]->get_imgPath()) > 0)
            {
                sugestionList->colorList[i].set_img(videoHandle->get_list()[videoPos]->get_imgPath());
                sugestionList->colorList[i].useBitmap = true;
            }
            else
            {
                sugestionList->colorList[i].path = new char[10];
                sugestionList->colorList[i].img = create_bitmap(100, 100);
                clear_to_color(sugestionList->colorList[i].img, makecol(rand()%255, rand()%255, rand()%255));
                textprintf_ex(sugestionList->colorList[i].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[videoPos]->get_name());
                sugestionList->colorList[i].useBitmap = true;
            }
        }
        /** Sorting List */
        {
            int sortedList[sugestionList->amount];
            for (int i = 0; i<sugestionList->amount; i++)
                sortedList[i] = i;
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i<sugestionList->amount-1; i++)
                {
                    int videoPos = videoHandle->search_video(sugestionListId[i]);
                    int videoPos2 = videoHandle->search_video(sugestionListId[i+1]);
                    if (videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate())
                    {
                        int temp = sugestionListId[i];
                        sugestionListId[i] = sugestionListId[i+1];
                        sugestionListId[i+1] = temp;
                        temp = sortedList[i];
                        sortedList[i] = sortedList[i+1];
                        sortedList[i+1] = temp;
                        sorted = false;
                    }
                }
            }
            sugestionList->sort(sortedList);
        }
    }

    int watchableListId[videoHandle->get_amount() - usersHandle->get_list()[userCode].get_videosWatched()];
    int watchableListSort = SORT_RATE;
    PictureList *watchableList = new PictureList;
    {
        watchableList->tela = tela;
        watchableList->pos.y = 300;
        watchableList->inertia = watchableList->roll = 0;
        watchableList->pos.w = SCREEN_W-shownMenu;
        watchableList->set_amount(videoHandle->get_amount() - usersHandle->get_list()[userCode].get_videosWatched());
        watchableList->selectStyle = 4;
        watchableList->inicializar_paths(0, watchableList->amount);
        int videoPos = 0;
        for (int i=0; i<watchableList->amount; i++)
        {
            watchableList->colorList[i].color = makecol(rand()%255, rand()%255, rand()%255);
            for (int j = 0; j<usersHandle->get_list()[userCode].get_videosWatched(); j++) // verificando se o video ja foi assistido
            {
                if (videoHandle->get_list()[videoPos]->get_id() == usersHandle->get_list()[userCode].get_watchedListId()[j])
                {
                    videoPos++;
                    j = usersHandle->get_list()[userCode].get_videosWatched();
                }
                // videoPos ainda não foi assistido
            }
            watchableListId[i] = videoHandle->get_list()[videoPos]->get_id();
            if (strlen(videoHandle->get_list()[videoPos]->get_imgPath()) > 0)
            {
                watchableList->colorList[i].set_img(videoHandle->get_list()[videoPos]->get_imgPath());
                watchableList->colorList[i].useBitmap = true;
            }
            else
            {
                watchableList->colorList[i].path = NULL;
                watchableList->colorList[i].img = create_bitmap(100, 100);
                clear_to_color(watchableList->colorList[i].img, makecol(rand()%255, rand()%255, rand()%255));
                textprintf_ex(watchableList->colorList[i].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[videoPos]->get_name());
                //textprintf_ex(watchableList->colorList[i].img, padrao, 0, 40, BLACK, WHITE, "%d", videoHandle->get_list()[videoPos]->get_id());
                watchableList->colorList[i].useBitmap = true;
            }
            videoPos++;
        }
        /** Sorting List */
        {
            int sortedList[watchableList->amount];
            for (int i = 0; i<watchableList->amount; i++)
                sortedList[i] = i;
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i<watchableList->amount-1; i++)
                {
                    int videoPos = videoHandle->search_video(watchableListId[i]);
                    int videoPos2 = videoHandle->search_video(watchableListId[i+1]);
                    if (videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate())
                    {
                        int temp = watchableListId[i];
                        watchableListId[i] = watchableListId[i+1];
                        watchableListId[i+1] = temp;
                        temp = sortedList[i];
                        sortedList[i] = sortedList[i+1];
                        sortedList[i+1] = temp;
                        sorted = false;
                    }
                }
            }
            watchableList->sort(sortedList);
        }
    }

    int *movieListId = NULL;
    int movieListSort = SORT_RATE;
    PictureList *movieList = new PictureList;
    {
        movieList->tela = tela;
        movieList->pos.y = 300;
        movieList->inertia = movieList->roll = 0;
        movieList->pos.w = SCREEN_W-shownMenu;
        movieList->set_amount(0);
        movieList->selectStyle = 4;
        for (int i=0; i<videoHandle->get_amount(); i++) // Procurando por Filmes que ainda não foram assistidos
        {
            if (videoHandle->get_list()[i]->get_videoType() == MOVIE)
            {
                bool passa = false;
                for (int j = 0; j<usersHandle->get_list()[userCode].get_videosWatched(); j++) // verificando se o video ja foi assistido
                {
                    if (videoHandle->get_list()[i]->get_id() == usersHandle->get_list()[userCode].get_watchedListId()[j])
                    {
                        passa = true;
                        j = usersHandle->get_list()[userCode].get_videosWatched();
                    }
                }
                if (!passa) // i representa a posicao de um Filme que nao foi assistindo
                {
                    movieList->set_amount(movieList->amount+1);
                    int k = movieList->amount-1;
                    movieListId = (int*) realloc(movieListId, sizeof(int) * movieList->amount);
                    movieListId[k] = videoHandle->get_list()[i]->get_id();

                    if (strlen(videoHandle->get_list()[i]->get_imgPath()) > 0)
                    {
                        movieList->colorList[k].set_img(videoHandle->get_list()[i]->get_imgPath());
                        movieList->colorList[k].useBitmap = true;
                    }
                    else
                    {
                        movieList->colorList[k].path = NULL;
                        movieList->colorList[k].img = create_bitmap(100, 100);
                        clear_to_color(movieList->colorList[k].img, makecol(rand()%255, rand()%255, rand()%255));
                        textprintf_ex(movieList->colorList[k].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
                        //textprintf_ex(movieList->colorList[k].img, padrao, 0, 40, BLACK, WHITE, "%d", videoHandle->get_list()[i]->get_id());
                        movieList->colorList[k].useBitmap = true;
                    }
                }
            }
        }
         /** Sorting List */
        {
            //alert("MovieList Amount", itoa(movieList->amount, new char[10], 10), NULL, "OK", NULL, 'o', 0);
            int sortedList[movieList->amount];
            for (int i = 0; i<movieList->amount; i++)
                sortedList[i] = i;
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i<movieList->amount-1; i++)
                {
                    int videoPos = videoHandle->search_video(movieListId[i]);
                    int videoPos2 = videoHandle->search_video(movieListId[i+1]);
                    if (videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate())
                    {
                        int temp = movieListId[i];
                        movieListId[i] = movieListId[i+1];
                        movieListId[i+1] = temp;
                        temp = sortedList[i];
                        sortedList[i] = sortedList[i+1];
                        sortedList[i+1] = temp;
                        sorted = false;
                    }
                }
            }
            //alert("Trying to Sort", NULL, NULL, "OK", NULL, 'o', 0);
            if (movieList->sort(sortedList) == ERROR_FEEDBACK)
            {
                alert("ERROR", NULL, NULL, "OK", NULL, 'o', 0);
            }
        }

    }

    int *serieListId = NULL;
    int serieListSort = SORT_RATE;
    PictureList *serieList = new PictureList;
    {
        serieList->tela = tela;
        serieList->pos.y = 300;
        serieList->inertia = serieList->roll = 0;
        serieList->pos.w = SCREEN_W-shownMenu;
        serieList->set_amount(0);
        serieList->selectStyle = 4;
        for (int i=0; i<videoHandle->get_amount(); i++) // Procurando por Filmes que ainda não foram assistidos
        {
            if (videoHandle->get_list()[i]->get_videoType() == SERIE)
            {
                bool passa = false;
                for (int j = 0; j<usersHandle->get_list()[userCode].get_videosWatched(); j++) // verificando se o video ja foi assistido
                {
                    if (videoHandle->get_list()[i]->get_id() == usersHandle->get_list()[userCode].get_watchedListId()[j])
                    {
                        passa = true;
                        j = usersHandle->get_list()[userCode].get_videosWatched();
                    }
                }
                if (!passa) // i representa um Filme que nao foi assistindo
                {
                    serieList->set_amount(serieList->amount+1);
                    serieListId = (int*) realloc(serieListId, sizeof(int) * serieList->amount);
                    serieListId[serieList->amount-1] = videoHandle->get_list()[i]->get_id();
                    int k = serieList->amount-1;
                    if (strlen(videoHandle->get_list()[i]->get_imgPath()) > 0)
                    {
                        serieList->colorList[k].set_img(videoHandle->get_list()[i]->get_imgPath());
                        serieList->colorList[k].useBitmap = true;
                    }
                    else
                    {
                        serieList->colorList[k].path = NULL;
                        serieList->colorList[k].img = create_bitmap(100, 100);
                        clear_to_color(serieList->colorList[k].img, makecol(rand()%255, rand()%255, rand()%255));
                        textprintf_ex(serieList->colorList[k].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
                        //textprintf_ex(serieList->colorList[k].img, padrao, 0, 40, BLACK, WHITE, "%d", videoHandle->get_list()[i]->get_id());
                        serieList->colorList[k].useBitmap = true;
                    }
                }
            }
        }
        /** Sorting List */
        {
            int sortedList[serieList->amount];
            for (int i = 0; i<serieList->amount; i++)
                sortedList[i] = i;
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i<serieList->amount-1; i++)
                {
                    int videoPos = videoHandle->search_video(serieListId[i]);
                    int videoPos2 = videoHandle->search_video(serieListId[i+1]);
                    if (videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate())
                    {
                        int temp = serieListId[i];
                        serieListId[i] = serieListId[i+1];
                        serieListId[i+1] = temp;
                        temp = sortedList[i];
                        sortedList[i] = sortedList[i+1];
                        sortedList[i+1] = temp;
                        sorted = false;
                    }
                }
            }
            serieList->sort(sortedList);
        }
    }

    int *docListId = NULL;
    int docListSort = SORT_RATE;
    PictureList *docList = new PictureList;
    {
        docList->tela = tela;
        docList->pos.y = 300;
        docList->inertia = docList->roll = 0;
        docList->pos.w = SCREEN_W-shownMenu;
        docList->set_amount(0);
        docList->selectStyle = 4;
        for (int i=0; i<videoHandle->get_amount(); i++) // Procurando por Filmes que ainda não foram assistidos
        {
            if (videoHandle->get_list()[i]->get_videoType() == DOCUMENTARY)
            {
                bool passa = false;
                for (int j = 0; j<usersHandle->get_list()[userCode].get_videosWatched(); j++) // verificando se o video ja foi assistido
                {
                    if (videoHandle->get_list()[i]->get_id() == usersHandle->get_list()[userCode].get_watchedListId()[j])
                    {
                        passa = true;
                        j = usersHandle->get_list()[userCode].get_videosWatched();
                    }
                }
                if (!passa) // i representa um Filme que nao foi assistindo
                {
                    docList->set_amount(docList->amount+1);
                    docListId = (int*) realloc(docListId, sizeof(int) * docList->amount);
                    docListId[docList->amount-1] = videoHandle->get_list()[i]->get_id();
                    int k = docList->amount-1;
                    if (strlen(videoHandle->get_list()[i]->get_imgPath()) > 0)
                    {
                        docList->colorList[k].set_img(videoHandle->get_list()[i]->get_imgPath());
                        docList->colorList[k].useBitmap = true;
                    }
                    else
                    {
                        docList->colorList[k].path = NULL;
                        docList->colorList[k].img = create_bitmap(100, 100);
                        clear_to_color(docList->colorList[k].img, makecol(rand()%255, rand()%255, rand()%255));
                        textprintf_ex(docList->colorList[k].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
                        docList->colorList[k].useBitmap = true;
                    }
                }
            }
        }
        /** Sorting List */
        {
            int sortedList[docList->amount];
            for (int i = 0; i<docList->amount; i++)
                sortedList[i] = i;
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i<docList->amount-1; i++)
                {
                    int videoPos = videoHandle->search_video(docListId[i]);
                    int videoPos2 = videoHandle->search_video(docListId[i+1]);
                    if (videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate())
                    {
                        int temp = docListId[i];
                        docListId[i] = docListId[i+1];
                        docListId[i+1] = temp;
                        temp = sortedList[i];
                        sortedList[i] = sortedList[i+1];
                        sortedList[i+1] = temp;
                        sorted = false;
                    }
                }
            }
            docList->sort(sortedList);
        }
    }

    int *showListId = NULL;
    int showListSort = SORT_RATE;
    PictureList *showList = new PictureList;
    {
        showList->tela = tela;
        showList->pos.y = 300;
        showList->inertia = showList->roll = 0;
        showList->pos.w = SCREEN_W-shownMenu;
        showList->set_amount(0);
        showList->selectStyle = 4;
        for (int i=0; i<videoHandle->get_amount(); i++) // Procurando por Filmes que ainda não foram assistidos
        {
            if (videoHandle->get_list()[i]->get_videoType() == SHOW)
            {
                bool passa = false;
                for (int j = 0; j<usersHandle->get_list()[userCode].get_videosWatched(); j++) // verificando se o video ja foi assistido
                {
                    if (videoHandle->get_list()[i]->get_id() == usersHandle->get_list()[userCode].get_watchedListId()[j])
                    {
                        passa = true;
                        j = usersHandle->get_list()[userCode].get_videosWatched();
                    }
                }
                if (!passa) // i representa um Show que nao foi assistindo
                {
                    showList->set_amount(showList->amount+1);
                    showListId = (int*) realloc(showListId, sizeof(int) * showList->amount);
                    showListId[showList->amount-1] = videoHandle->get_list()[i]->get_id();
                    int k = showList->amount-1;
                    if (strlen(videoHandle->get_list()[i]->get_imgPath()) > 0)
                    {
                        showList->colorList[k].set_img(videoHandle->get_list()[i]->get_imgPath());
                        showList->colorList[k].useBitmap = true;
                    }
                    else
                    {
                        showList->colorList[k].path = NULL;
                        showList->colorList[k].img = create_bitmap(100, 100);
                        clear_to_color(showList->colorList[k].img, makecol(rand()%255, rand()%255, rand()%255));
                        textprintf_ex(showList->colorList[k].img, padrao, 0, 0, BLACK, WHITE, "%s", videoHandle->get_list()[i]->get_name());
                        showList->colorList[k].useBitmap = true;
                    }
                }
            }
        }
        /** Sorting List */
        {
            int sortedList[showList->amount];
            for (int i = 0; i<showList->amount; i++)
                sortedList[i] = i;
            bool sorted = false;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i<showList->amount-1; i++)
                {
                    int videoPos = videoHandle->search_video(showListId[i]);
                    int videoPos2 = videoHandle->search_video(showListId[i+1]);
                    if (videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate())
                    {
                        int temp = showListId[i];
                        showListId[i] = showListId[i+1];
                        showListId[i+1] = temp;
                        temp = sortedList[i];
                        sortedList[i] = sortedList[i+1];
                        sortedList[i+1] = temp;
                        sorted = false;
                    }
                }
            }
            showList->sort(sortedList);
        }
    }

    ThingList *listOfLists = new ThingList;
    {
        listOfLists->tela = tela;
        listOfLists->pos.y = 0;
        listOfLists->inertia = listOfLists->roll = 0;
        listOfLists->pos.w = SCREEN_W-shownMenu;
        listOfLists->pos.h = SCREEN_H;
        listOfLists->inicializar_paths(0, listOfLists->amount);
        listOfLists->horizontal = false;
        listOfLists->spaceBetweenPics = 20;
        listOfLists->picHeight = watchedList->pos.h;
        listOfLists->useMouseWheel = true;
        listOfLists->roll = 100;

        listOfLists->insert(watchedList);
        listOfLists->insert(sugestionList);
        listOfLists->insert(watchableList);
        listOfLists->insert(movieList);
        listOfLists->insert(serieList);
        listOfLists->insert(docList);
        listOfLists->insert(showList);
    }

    BITMAP*oldOne = create_bitmap(SCREEN_W, SCREEN_H);
    draw_sprite(oldOne, old, 0, 0);

    /** Mostrar animacao de entrada */
    {
        listOfLists->draw();
        watchedList->draw();
        sugestionList->draw();
        watchableList->draw();
        movieList->draw();
        serieList->draw();
        docList->draw();
        showList->draw();
        textprintf_ex(tela, timesNewRoman12, 0, watchedList->pos.y-timesNewRoman12->height, WHITE, -1, "Assistidos");
        textprintf_ex(tela, timesNewRoman12, SCREEN_W-50-shownMenu, 0, WHITE, -1, "MENU");
        textprintf_ex(tela, timesNewRoman12, 0, sugestionList->pos.y-timesNewRoman12->height, WHITE, -1, "Sugestoes");
        textprintf_ex(tela, timesNewRoman12, 0, watchableList->pos.y-timesNewRoman12->height, WHITE, -1, "Disponiveis");
        textprintf_ex(tela, timesNewRoman12, 0, movieList->pos.y-timesNewRoman12->height, WHITE, -1, "Filmes");
        textprintf_ex(tela, timesNewRoman12, 0, serieList->pos.y-timesNewRoman12->height, WHITE, -1, "Series");
        textprintf_ex(tela, timesNewRoman12, 0, docList->pos.y-timesNewRoman12->height, WHITE, -1, "Documentarios");
        draw_sprite(tela, user_menu, user_menu_pos.x, user_menu_pos.y);
        change_screen(old, SCREEN_W - 200, tela, OPENING);
    }

    int fps = 0;
    int atualSec = time(NULL);
    int frames = 0;

    bool sair = false;
    while(!sair)
    {
        // desenhando a tela
        clear_to_color(tela, GRAY(4));
        {
            watchedList->draw();
            watchedList->mouse_movement();
            sugestionList->draw();
            sugestionList->mouse_movement();
            watchableList->draw();
            watchableList->mouse_movement();
            movieList->draw();
            movieList->mouse_movement();
            serieList->draw();
            serieList->mouse_movement();
            docList->draw();
            docList->mouse_movement();
            showList->draw();
            showList->mouse_movement();
            listOfLists->draw();
            listOfLists->mouse_movement();
        }

        if (watchedList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                int temp = usersHandle->get_list()[userCode].get_watchedListId()[watchedList->selectedId];
                // temp é equivalente a watchedListId[watchedList->selectedId]
                watch_video(temp, 0);
            }
            watchedList->selectedId = -1;
        }
        if (sugestionList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                if (watch_video(sugestionListId[sugestionList->selectedId], 1) == SUCCESS_FEEDBACK)
                {
                    int videoPos = videoHandle->search_video(sugestionListId[sugestionList->selectedId]);
                    alert("Assistindo filme", videoHandle->get_list()[videoPos]->get_name(), NULL, "&OK", NULL, 'o', 0);
                    usersHandle->get_list()[userCode].insertVideo(sugestionListId[sugestionList->selectedId]);
                    sair = true;
                }
            }
            sugestionList->selectedId = -1;
        }
        if (watchableList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                if (watch_video(watchableListId[watchableList->selectedId], 1) == SUCCESS_FEEDBACK)
                {
                    int videoPos = videoHandle->search_video(watchableListId[watchableList->selectedId]);
                    alert("Assistindo", videoHandle->get_list()[videoPos]->get_name(), NULL, "&OK", NULL, 'o', 0);
                    usersHandle->get_list()[userCode].insertVideo(watchableListId[watchableList->selectedId]);
                    sair = true;
                }
            }
            watchableList->selectedId = -1;
        }
        if (movieList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                if (watch_video(movieListId[movieList->selectedId], 1) == SUCCESS_FEEDBACK)
                {
                    int videoPos = videoHandle->search_video(movieListId[movieList->selectedId]);
                    alert("Assistindo", videoHandle->get_list()[videoPos]->get_name(), NULL, "&OK", NULL, 'o', 0);
                    usersHandle->get_list()[userCode].insertVideo(movieListId[movieList->selectedId]);
                    sair = true;
                }
            }
            movieList->selectedId = -1;
        }
        if (serieList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                if (watch_video(serieListId[serieList->selectedId], 1) == SUCCESS_FEEDBACK)
                {
                    int videoPos = videoHandle->search_video(serieListId[serieList->selectedId]);
                    alert("Assistindo", videoHandle->get_list()[videoPos]->get_name(), NULL, "&OK", NULL, 'o', 0);
                    usersHandle->get_list()[userCode].insertVideo(serieListId[serieList->selectedId]);
                    sair = true;
                }
            }
            serieList->selectedId = -1;
        }
        if (docList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                if (watch_video(docListId[docList->selectedId], 1) == SUCCESS_FEEDBACK)
                {
                    int videoPos = videoHandle->search_video(docListId[docList->selectedId]);
                    alert("Assistindo", videoHandle->get_list()[videoPos]->get_name(), NULL, "&OK", NULL, 'o', 0);
                    usersHandle->get_list()[userCode].insertVideo(docListId[docList->selectedId]);
                    sair = true;
                }
            }
            docList->selectedId = -1;

        }
        if (showList->selectedId != -1)
        {
            if (!mouseOverMenu)
            {
                if (watch_video(showListId[showList->selectedId], 1) == SUCCESS_FEEDBACK)
                {
                    int videoPos = videoHandle->search_video(showListId[showList->selectedId]);
                    alert("Assistindo", videoHandle->get_list()[videoPos]->get_name(), NULL, "&OK", NULL, 'o', 0);
                    usersHandle->get_list()[userCode].insertVideo(showListId[showList->selectedId]);
                    sair = true;
                }
            }
            showList->selectedId = -1;

        }

        if (key[KEY_ESC]) sair = true;

        /** Movimentar menu de usuario */
        logoff->draw();
        changeInfo->draw();
        if (testMousePos(&user_menu_pos))
        {
            mouseOverMenu = true;
            user_menu_pos.x-=8;
            if (user_menu_pos.x < SCREEN_W - user_menu->w) user_menu_pos.x = SCREEN_W - user_menu->w;
        }
        else
        {
            mouseOverMenu = false;
            user_menu_pos.x+=5;
            if (user_menu_pos.x > SCREEN_W - shownMenu) user_menu_pos.x = SCREEN_W - shownMenu;
        }
        if (testMousePos(&(changeInfo->pos), user_menu_pos.x, user_menu_pos.y))
            if (mouse_b == 1)
            {
                changeInfo->click();
                menu_editUser_window(userCode);
            }
            else changeInfo->set_highlight(true);
        else changeInfo->set_highlight(false);
        if (testMousePos(&(logoff->pos), user_menu_pos.x, user_menu_pos.y))
            if (mouse_b == 1)
            {
                logoff->click();
                sair=true;
            }
            else logoff->set_highlight(true);
        else logoff->set_highlight(false);

        /** Sorting Sytem */
        textprintf_ex(tela, timesNewRoman12, 100, sugestionList->pos.y-timesNewRoman12->height, WHITE, -1, "%s", (sugestionListSort==SORT_NAME?"Por Nome":"Por Nota"));
        if (testMousePos(100, sugestionList->pos.y-timesNewRoman12->height, 160, sugestionList->pos.y-timesNewRoman12->height+timesNewRoman12->height))
        {
            hline(tela, 100, sugestionList->pos.y-timesNewRoman12->height+timesNewRoman12->height-2, (sugestionListSort==SORT_NAME?170:165), WHITE);
            if (mouse_b == 1)
            {
                sugestionListSort = (sugestionListSort==SORT_NAME?SORT_RATE:SORT_NAME);
                int sortedList[sugestionList->amount];
                for (int i = 0; i<sugestionList->amount; i++)
                    sortedList[i] = i;
                bool sorted = false;
                while (!sorted)
                {
                    sorted = true;
                    for (int i = 0; i<sugestionList->amount-1; i++)
                    {
                        int videoPos = videoHandle->search_video(sugestionListId[i]);
                        int videoPos2 = videoHandle->search_video(sugestionListId[i+1]);
                        int value;
                        if (sugestionListSort == SORT_RATE)
                        {
                            value = videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate();
                        }
                        else
                        {
                            if (strcmpi(videoHandle->get_list()[videoPos]->get_name(), videoHandle->get_list()[videoPos2]->get_name()) > 0 )
                            {
                                value = 1;
                            }else value = 0;
                        }
                        if ( value )
                        {
                            int temp = sugestionListId[i];
                            sugestionListId[i] = sugestionListId[i+1];
                            sugestionListId[i+1] = temp;
                            temp = sortedList[i];
                            sortedList[i] = sortedList[i+1];
                            sortedList[i+1] = temp;
                            sorted = false;
                        }
                    }
                }
                sugestionList->sort(sortedList);
            } while(mouse_b);
        }
        textprintf_ex(tela, timesNewRoman12, 100, watchableList->pos.y-timesNewRoman12->height, WHITE, -1, "%s", (watchableListSort==SORT_NAME?"Por Nome":"Por Nota"));
        if (testMousePos(100, watchableList->pos.y-timesNewRoman12->height, 160, watchableList->pos.y-timesNewRoman12->height+timesNewRoman12->height))
        {
            hline(tela, 100, watchableList->pos.y-timesNewRoman12->height+timesNewRoman12->height-2, (watchableListSort==SORT_NAME?170:165), WHITE);
            if (mouse_b == 1)
            {
                watchableListSort = (watchableListSort==SORT_NAME?SORT_RATE:SORT_NAME);
                int sortedList[watchableList->amount];
                for (int i = 0; i<watchableList->amount; i++)
                    sortedList[i] = i;
                bool sorted = false;
                while (!sorted)
                {
                    sorted = true;
                    for (int i = 0; i<watchableList->amount-1; i++)
                    {
                        int videoPos = videoHandle->search_video(watchableListId[i]);
                        int videoPos2 = videoHandle->search_video(watchableListId[i+1]);
                        int value;
                        if (watchableListSort == SORT_RATE)
                        {
                            value = videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate();
                        }
                        else
                        {
                            if (strcmpi(videoHandle->get_list()[videoPos]->get_name(), videoHandle->get_list()[videoPos2]->get_name()) > 0 )
                            {
                                value = 1;
                            }else value = 0;
                        }
                        if ( value )
                        {
                            int temp = watchableListId[i];
                            watchableListId[i] = watchableListId[i+1];
                            watchableListId[i+1] = temp;
                            temp = sortedList[i];
                            sortedList[i] = sortedList[i+1];
                            sortedList[i+1] = temp;
                            sorted = false;
                        }
                    }
                }
                watchableList->sort(sortedList);
            } while(mouse_b);
        }
        textprintf_ex(tela, timesNewRoman12, 80, movieList->pos.y-timesNewRoman12->height, WHITE, -1, "%s", (movieListSort==SORT_NAME?"Por Nome":"Por Nota"));
        if (testMousePos(80, movieList->pos.y-timesNewRoman12->height, 150, movieList->pos.y-timesNewRoman12->height+timesNewRoman12->height))
        {
            hline(tela, 80, movieList->pos.y-timesNewRoman12->height+timesNewRoman12->height-2, (movieListSort==SORT_NAME?150:145), WHITE);
            if (mouse_b == 1)
            {
                movieListSort = (movieListSort==SORT_NAME?SORT_RATE:SORT_NAME);
                int sortedList[movieList->amount];
                for (int i = 0; i<movieList->amount; i++)
                    sortedList[i] = i;
                bool sorted = false;
                while (!sorted)
                {
                    sorted = true;
                    for (int i = 0; i<movieList->amount-1; i++)
                    {
                        int videoPos = videoHandle->search_video(movieListId[i]);
                        int videoPos2 = videoHandle->search_video(movieListId[i+1]);
                        int value;
                        if (movieListSort == SORT_RATE)
                        {
                            value = videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate();
                        }
                        else
                        {
                            if (strcmpi(videoHandle->get_list()[videoPos]->get_name(), videoHandle->get_list()[videoPos2]->get_name()) > 0 )
                            {
                                value = 1;
                            }else value = 0;
                        }
                        if ( value )
                        {
                            int temp = movieListId[i];
                            movieListId[i] = movieListId[i+1];
                            movieListId[i+1] = temp;
                            temp = sortedList[i];
                            sortedList[i] = sortedList[i+1];
                            sortedList[i+1] = temp;
                            sorted = false;
                        }
                    }
                }
                movieList->sort(sortedList);
            } while(mouse_b);
        } /** Sorting Movies End */
        textprintf_ex(tela, timesNewRoman12, 80, serieList->pos.y-timesNewRoman12->height, WHITE, -1, "%s", (serieListSort==SORT_NAME?"Por Nome":"Por Nota"));
        if (testMousePos(80, serieList->pos.y-timesNewRoman12->height, 150, serieList->pos.y-timesNewRoman12->height+timesNewRoman12->height))
        {
            hline(tela, 80, serieList->pos.y-timesNewRoman12->height+timesNewRoman12->height-2, (serieListSort==SORT_NAME?150:145), WHITE);
            if (mouse_b == 1)
            {
                serieListSort = (serieListSort==SORT_NAME?SORT_RATE:SORT_NAME);
                int sortedList[serieList->amount];
                for (int i = 0; i<serieList->amount; i++)
                    sortedList[i] = i;
                bool sorted = false;
                while (!sorted)
                {
                    sorted = true;
                    for (int i = 0; i<serieList->amount-1; i++)
                    {
                        int videoPos = videoHandle->search_video(serieListId[i]);
                        int videoPos2 = videoHandle->search_video(serieListId[i+1]);
                        int value;
                        if (serieListSort == SORT_RATE)
                        {
                            value = videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate();
                        }
                        else
                        {
                            if (strcmpi(videoHandle->get_list()[videoPos]->get_name(), videoHandle->get_list()[videoPos2]->get_name()) > 0 )
                            {
                                value = 1;
                            }else value = 0;
                        }
                        if ( value )
                        {
                            int temp = serieListId[i];
                            serieListId[i] = serieListId[i+1];
                            serieListId[i+1] = temp;
                            temp = sortedList[i];
                            sortedList[i] = sortedList[i+1];
                            sortedList[i+1] = temp;
                            sorted = false;
                        }
                    }
                }
                serieList->sort(sortedList);
            } while(mouse_b);
        } /** Sorting Serie End */
        textprintf_ex(tela, timesNewRoman12, 120, docList->pos.y-timesNewRoman12->height, WHITE, -1, "%s", (docListSort==SORT_NAME?"Por Nome":"Por Nota"));
        if (testMousePos(120, docList->pos.y-timesNewRoman12->height, 190, docList->pos.y-timesNewRoman12->height+timesNewRoman12->height))
        {
            hline(tela, 120, docList->pos.y-timesNewRoman12->height+timesNewRoman12->height-2, (docListSort==SORT_NAME?190:185), WHITE);
            if (mouse_b == 1)
            {
                docListSort = (docListSort==SORT_NAME?SORT_RATE:SORT_NAME);
                int sortedList[docList->amount];
                for (int i = 0; i<docList->amount; i++)
                    sortedList[i] = i;
                bool sorted = false;
                while (!sorted)
                {
                    sorted = true;
                    for (int i = 0; i<docList->amount-1; i++)
                    {
                        int videoPos = videoHandle->search_video(docListId[i]);
                        int videoPos2 = videoHandle->search_video(docListId[i+1]);
                        int value;
                        if (docListSort == SORT_RATE)
                        {
                            value = videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate();
                        }
                        else
                        {
                            if (strcmpi(videoHandle->get_list()[videoPos]->get_name(), videoHandle->get_list()[videoPos2]->get_name()) > 0 )
                            {
                                value = 1;
                            }else value = 0;
                        }
                        if ( value )
                        {
                            int temp = docListId[i];
                            docListId[i] = docListId[i+1];
                            docListId[i+1] = temp;
                            temp = sortedList[i];
                            sortedList[i] = sortedList[i+1];
                            sortedList[i+1] = temp;
                            sorted = false;
                        }
                    }
                }
                docList->sort(sortedList);
            } while(mouse_b);
        } /** Sorting Doc End */
        textprintf_ex(tela, timesNewRoman12, 70, showList->pos.y-timesNewRoman12->height, WHITE, -1, "%s", (showListSort==SORT_NAME?"Por Nome":"Por Nota"));
        if (testMousePos(70, showList->pos.y-timesNewRoman12->height, 150, showList->pos.y-timesNewRoman12->height+timesNewRoman12->height))
        {
            hline(tela, 70, showList->pos.y-timesNewRoman12->height+timesNewRoman12->height-2, (showListSort==SORT_NAME?140:135), WHITE);
            if (mouse_b == 1)
            {
                showListSort = (showListSort==SORT_NAME?SORT_RATE:SORT_NAME);
                int sortedList[showList->amount];
                for (int i = 0; i<showList->amount; i++)
                    sortedList[i] = i;
                bool sorted = false;
                while (!sorted)
                {
                    sorted = true;
                    for (int i = 0; i<showList->amount-1; i++)
                    {
                        int videoPos = videoHandle->search_video(showListId[i]);
                        int videoPos2 = videoHandle->search_video(showListId[i+1]);
                        int value;
                        if (showListSort == SORT_RATE)
                        {
                            value = videoHandle->get_list()[videoPos]->get_rate() < videoHandle->get_list()[videoPos2]->get_rate();
                        }
                        else
                        {
                            if (strcmpi(videoHandle->get_list()[videoPos]->get_name(), videoHandle->get_list()[videoPos2]->get_name()) > 0 )
                            {
                                value = 1;
                            }else value = 0;
                        }
                        if ( value )
                        {
                            int temp = showListId[i];
                            showListId[i] = showListId[i+1];
                            showListId[i+1] = temp;
                            temp = sortedList[i];
                            sortedList[i] = sortedList[i+1];
                            sortedList[i+1] = temp;
                            sorted = false;
                        }
                    }
                }
                showList->sort(sortedList);
            } while(mouse_b);
        } /** Sorting Shows End */

        textprintf_ex(tela, timesNewRoman12, 0, watchedList->pos.y-timesNewRoman12->height, WHITE, -1, "Assistidos");
        textprintf_ex(tela, timesNewRoman12, SCREEN_W-50-shownMenu, 0, WHITE, -1, "MENU");
        textprintf_ex(tela, timesNewRoman12, 0, sugestionList->pos.y-timesNewRoman12->height, WHITE, -1, "Sugestoes");
        textprintf_ex(tela, timesNewRoman12, 0, watchableList->pos.y-timesNewRoman12->height, WHITE, -1, "Disponiveis");
        textprintf_ex(tela, timesNewRoman12, 0, movieList->pos.y-timesNewRoman12->height, WHITE, -1, "Filmes");
        textprintf_ex(tela, timesNewRoman12, 0, serieList->pos.y-timesNewRoman12->height, WHITE, -1, "Series");
        textprintf_ex(tela, timesNewRoman12, 0, docList->pos.y-timesNewRoman12->height, WHITE, -1, "Documentarios");
        textprintf_ex(tela, timesNewRoman12, 0, showList->pos.y-timesNewRoman12->height, WHITE, -1, "Shows");
        draw_sprite(tela, user_menu, user_menu_pos.x, user_menu_pos.y);

        //textprintf_ex(tela, font, 0, 0, BLACK, -1, "%d x %d", fps, waitTime);
        draw_sprite(screen, tela, 0, 0);

        rest(waitTime);
        frames++;
        if (atualSec != time(NULL))
        {
            fps = frames;
            frames = 0;
            atualSec = time(NULL);
            if (fps > MAXIMUM_FPS) waitTime++;
            if (fps < MINIMUM_FPS) waitTime--;
        }
    }
    while(key[KEY_ESC]);
    while(mouse_b == 1);

    draw_sprite(old, oldOne, 0, 0);
    change_screen(tela, SCREEN_W - 200, oldOne, CLOSING);

    font = padrao;

    delete logoff;
    delete changeInfo;

    delete watchedList;
    delete sugestionList;
    delete watchableList;
    delete movieList;
    delete serieList;
    delete docList;
    delete showList;
    delete listOfLists;

    destroy_bitmap(tela);
    destroy_bitmap(user_menu);
    destroy_bitmap(oldOne);
    destroy_font(timesNewRoman12);

    return 0;
}

void LPIITV::menu_editUser_window(int userCode)
{
    BITMAP* tela = create_bitmap(250, 150);

    FONT* timesNewRoman12 = load_font(TNR12, NULL, NULL);

    CaixaDeTexto* nameBox = new CaixaDeTexto(15, 100, 10);
    nameBox->tela = tela;
    nameBox->set_legenda("Nome: ", LEFT);
    CaixaDeTexto* pwBox = new CaixaDeTexto(15, 100, 50);
    pwBox->set_legenda("Senha: ", LEFT);
    pwBox->is_password(true);
    pwBox->tela = tela;
    nameBox->pos.h = pwBox->pos.h = timesNewRoman12->height + 2;
    nameBox->timeChange = pwBox->timeChange = 0;

    strcpy(nameBox->text, usersHandle->get_list()[userCode].get_name());
    strcpy(pwBox->text, usersHandle->get_list()[userCode].get_password());

    Button* saveButton = new Button(10, 100, 100, 30, "Salvar", tela);

    Button* exitButton = new Button(130, 100, 100, 30, "Cancelar", tela);
    saveButton->textOrientantion = exitButton->textOrientantion = ORIENTATION_CENTRE;
    saveButton->f = exitButton->f = timesNewRoman12;

    Pos window;
    window.x = SCREEN_W/2 - tela->w/2;
    window.y = SCREEN_H/2 - tela->h/2;

    bool sair = false;
    while (!sair)
    {
        clear_to_color(tela, GRAY(8));

        nameBox->drawTextBox();
        pwBox->drawTextBox();
        saveButton->draw();
        exitButton->draw();

        if (mouse_b == 1)
        {
            if (testMousePos(&(nameBox->pos), window.x, window.y))
            {
                nameBox->active = true;
            } else nameBox->active = false;
            if (testMousePos(&(pwBox->pos), window.x, window.y))
            {
                pwBox->active = true;
            } else pwBox->active = false;
            if (testMousePos(&(saveButton->pos), window.x, window.y))
            {
                saveButton->click();
                usersHandle->get_list()[userCode].set_name(nameBox->text);
                usersHandle->get_list()[userCode].set_password(pwBox->text);
                sair = true;
            }
            if (testMousePos(&(exitButton->pos), window.x, window.y))
            {
                exitButton->click();
                sair = true;
            }
        }

        if (key[KEY_TAB])
        {
            if (key_shifts == KB_SHIFT_FLAG)
            {
                if (pwBox->active)
                {
                    nameBox->active = true;
                    pwBox->active = false;
                }
            }
            else if (nameBox->active)
            {
                nameBox->active = false;
                pwBox->active = true;
            }

        }

        if (nameBox->active)
        {
            inputTextBox(nameBox->text, nameBox->maxLenght);
        }
        else if (pwBox->active)
        {
            inputTextBox(pwBox->text, pwBox->maxLenght);
        }

        if (key[KEY_ESC]) sair = true;

        draw_sprite(screen, tela, window.x, window.y);
        rest(waitTime);
    }
    while(key[KEY_ESC]);
    while(mouse_b == 1);

    delete nameBox;
    delete pwBox;
    delete saveButton;
    delete exitButton;

    destroy_bitmap(tela);
    destroy_font(timesNewRoman12);
}

