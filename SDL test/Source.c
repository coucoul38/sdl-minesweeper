
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA   "\x1B[35m"
#define CYAN   "\x1B[36m"
#define BMAGENTA   "\x1B[95m"
#define COLOR0 "\x1B[93m"
#define REDBG "\x1B[41m"
#define WHITEBG "\x1B[47m"
#define GRAYBG "\x1B[100m"
#define RESET "\x1B[0m"

typedef int bool;
#define true 1
#define false 0


int toPlace;
int difficulty = 0;
bool stop = false;
SDL_Color blanc = { 255, 255, 255, 255 };
SDL_Color gris = { 133,133,133,255 };

int initSDL(SDL_Window** window, SDL_Renderer** renderer, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, window, renderer) != 0) {
        fprintf(stderr, "Erreur CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    /*if (SDL_SetWindowFullscreen(*window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
        fprintf(stderr, "Erreur SetWindowFullscreen : %s", SDL_GetError());
        return -1;
    }*/
    SDL_RaiseWindow(*window);
    SDL_SetWindowTitle(*window, "Minesweeper");
    return 0;
}

SDL_Texture* loadImage(const char path[], SDL_Renderer* renderer) {
    SDL_Surface* tmp = NULL;
    SDL_Texture* texture = NULL;
    //printf("Path: %s\n", path);

    tmp = SDL_LoadBMP(path);
    if (tmp == NULL) {
        fprintf(stderr, "Erreur LoadBMP : %s", SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (texture == NULL) {
        fprintf(stderr, "Erreur CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int setWindowColor(SDL_Renderer* renderer, SDL_Color color) {
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0) {
        fprintf(stderr, "Erreur SetRenderDrawColor : %s", SDL_GetError());
        return -1;
    }
    if (SDL_RenderClear(renderer) != 0) {
        fprintf(stderr, "Erreur RenderClear : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

int askDifficulty(SDL_Renderer* renderer, SDL_Window* window) {
    setWindowColor(renderer, gris);

    int w = NULL, h = NULL;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect rectEasy = { w / 2 - 72, 0, 144, 48 };
    SDL_Rect rectMed = { w / 2 - 72, 48, 144, 48 };
    SDL_Rect rectHard = { w / 2 - 72, 96, 144, 48 };

    SDL_Texture* easy = NULL;
    easy = loadImage("sprites/ui/easy.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == easy)
        return -1;
    SDL_Texture* medium = NULL;
    medium = loadImage("sprites/ui/medium.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == medium)
        return -1;
    SDL_Texture* hard = NULL;
    hard = loadImage("sprites/ui/hard.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == hard)
        return -1;
    SDL_RenderCopy(renderer, easy, NULL, &rectEasy); //copier l'image sur le render
    SDL_RenderCopy(renderer, medium, NULL, &rectMed); //copier l'image sur le render
    SDL_RenderCopy(renderer, hard, NULL, &rectHard); //copier l'image sur le render
    SDL_RenderPresent(renderer);

    SDL_Event event;

    while (difficulty == 0) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            if (NULL != renderer)
                SDL_DestroyRenderer(renderer);
            if (NULL != window)
                SDL_DestroyWindow(window);
            SDL_Quit();
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_Point cursor = { event.button.x, event.button.y };
                if (SDL_PointInRect(&cursor, &rectEasy) == SDL_TRUE) {
                    printf("Easy");
                    difficulty = 1;
                }
                else if (SDL_PointInRect(&cursor, &rectMed) == SDL_TRUE) {
                    printf("Medium");
                    difficulty = 2;
                }
                else if (SDL_PointInRect(&cursor, &rectHard) == SDL_TRUE) {
                    printf("Hard");
                    difficulty = 3;
                }
            }
        }
    }
    return 0;
}

int askSize(SDL_Renderer* renderer, SDL_Window* window) {
    int size = 0;
    setWindowColor(renderer, gris);

    int w = NULL, h = NULL;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect rect5 = { w / 2 - 72, 0, 144, 48 };
    SDL_Rect rect10 = { w / 2 - 72, 48, 144, 48 };
    SDL_Rect rect15 = { w / 2 - 72, 96, 144, 48 };
    SDL_Rect rect20 = { w / 2 - 72, 146, 144, 48 };
    SDL_Rect rectCustom = { w / 2 - 72, 194, 144, 48 };

    SDL_Texture* size5 = NULL;
    size5 = loadImage("sprites/ui/size5.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == size5)
        return -1;
    SDL_Texture* size10 = NULL;
    size10 = loadImage("sprites/ui/size10.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == size10)
        return -1;
    SDL_Texture* size15 = NULL;
    size15 = loadImage("sprites/ui/size15.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == size15)
        return -1;
    SDL_Texture* size20 = NULL;
    size20 = loadImage("sprites/ui/size20.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == size20)
        return -1;
    SDL_Texture* sizeCustom = NULL;
    sizeCustom = loadImage("sprites/ui/sizeCustom.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == sizeCustom)
        return -1;
    SDL_RenderCopy(renderer, size5, NULL, &rect5); //copier l'image sur le render
    SDL_RenderCopy(renderer, size10, NULL, &rect10); //copier l'image sur le render
    SDL_RenderCopy(renderer, size15, NULL, &rect15); //copier l'image sur le render
    SDL_RenderCopy(renderer, size20, NULL, &rect20); //copier l'image sur le render 
    SDL_RenderCopy(renderer, sizeCustom, NULL, &rectCustom); //copier l'image sur le render 
    SDL_RenderPresent(renderer);

    SDL_Event event;

    while (size == 0) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            if (NULL != renderer)
                SDL_DestroyRenderer(renderer);
            if (NULL != window)
                SDL_DestroyWindow(window);
            SDL_Quit();
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_Point cursor = { event.button.x, event.button.y };
                if (SDL_PointInRect(&cursor, &rect5) == SDL_TRUE) {
                    printf("5");
                    size = 5;
                }
                else if (SDL_PointInRect(&cursor, &rect10) == SDL_TRUE) {
                    printf("10");
                    size = 10;
                }
                else if (SDL_PointInRect(&cursor, &rect15) == SDL_TRUE) {
                    printf("15");
                    size = 15;
                }
                else if (SDL_PointInRect(&cursor, &rect20) == SDL_TRUE) {
                    printf("20");
                    size = 20;
                }
                else if (SDL_PointInRect(&cursor, &rectCustom) == SDL_TRUE) {
                    while (!(size >= 3 && size <= 78)) {
                        printf("Choisir une taille pour la grille de jeu (entre 3 et 78): ");
                        scanf_s("%d", &size);
                    }
                }
            }
        }
    }
    return size;
}

//-----------------------------------------------
// compter le nombre de mines adjacentes � une case
int countNearby(int row, int col, int size, int** grid, char** display) {
    int R, C, count;
    count = 0;
    for (R = -2; R < 1; R++) {
        for (C = -2; C < 1; C++)
        {
            if (!(row + R < 0 || col + C < 0)) {
                if (row + R < size && col + C < size) {
                    if (grid[row + R][col + C] == 1 && !(R == -1 && C == -1)) {
                        count++;
                        //printf("[%d][%d] TRUE %d %d\n", row + R, col + C, R, C);
                    }
                }
            }
        }
    }
    //convertir int en string
    char nearby = count + '0';
    display[row - 1][col - 1] = nearby;

    //si il n'y a aucune mine autour, d�couvrir les cases adjacentes
    if (count == 0 && grid[row-1][col-1]!=1) {
        int rRelativeToInput, cRelativeToInput;
        for (rRelativeToInput = -2; rRelativeToInput < 1; rRelativeToInput++) {
            for (cRelativeToInput = -2; cRelativeToInput < 1; cRelativeToInput++) {
                if (!(row + rRelativeToInput < 0) && !(col + cRelativeToInput < 0) && (row + rRelativeToInput < size) && (col + cRelativeToInput < size)) {
                    if (display[row + rRelativeToInput][col + cRelativeToInput] == '?' || display[row + rRelativeToInput][col + cRelativeToInput] == 'F') {
                        //printf("\nCalling countNearby(%d,%d)\n", row + rRelativeToInput + 1, col + cRelativeToInput + 1);
                        countNearby(row + rRelativeToInput + 1, col + cRelativeToInput + 1, size, grid, display);
                    }
                }

            }
        }
    }
    return(count);
}

bool checkWin(int size, char** display) {
    int count, row, col;
    count = 0;
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (display[row][col] == '?' || display[row][col] == 'F') {
                count++;
            }
        }

    }
    if (count == toPlace) {
        return true;
    }
    else {
        return false;
    }

}

void showgrid(int** adress, int size)
{
    int row, col;
    for (row = -1; row < size; row++) {
        //printf("ROW: %d\n", row);
        for (col = -1; col < size; col++) {
            if (row == -1) {
                printf("%d ", col + 1);
            }
            else if (col == -1) {
                printf("%d ", row + 1);
            }
            else if (adress[row][col] == 1) {
                printf("\033[22;31m%i\033[0m ", adress[row][col]);
            }
            else if (adress[row][col] == 0) {
                printf("%i ", adress[row][col]);
            }

            if (col == size - 1) {
                printf("\n");
            }
        }


    }
}

bool showdisplay(char** display, int size, int timer, SDL_Renderer* renderer, SDL_Window* window, int** grid) {
    // display
    SDL_Event event;
    int*** coordinates = NULL;
    int w = NULL, h = NULL;
    SDL_GetWindowSize(window, &w, &h);
    int tailleCase = min(w, h) / size;

    setWindowColor(renderer, gris);
    SDL_RenderClear(renderer);

    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT)
            SDL_Quit();

    /*printf(GRAYBG "Mines: " RED "%d" RESET, toPlace);
    printf(GRAYBG " -- Difficulte: " RED "%i" RESET, difficulty);
    printf(GRAYBG " -- Time: " RED "%d\n" RESET, timer);*/
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            SDL_Rect dst = { col * tailleCase,row * tailleCase,tailleCase,tailleCase };
            SDL_Texture* image = NULL;

            /*coordinates[row][col][0] = dst.x;
            coordinates[row][col][1] = dst.y;
            coordinates[row][col][2] = dst.w;
            coordinates[row][col][3] = dst.h;*/

            if (display[row][col] == 'F') {
                image = loadImage("sprites/mines/flag.bmp", renderer);
                SDL_RenderCopy(renderer, image, NULL, &dst);
                //printf(REDBG "F" RESET " ");
            }
            else if (display[row][col] == '?' /* || display[row][col] == '0'*/) {
                image = loadImage("sprites/mines/blank.bmp", renderer);
                SDL_RenderCopy(renderer, image, NULL, &dst);
                //printf("%c ", display[row][col]);
            }
            else {
                switch (display[row][col])
                    {
                    case '0':
                        image = loadImage("sprites/mines/0.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(COLOR0 "0 " RESET);
                        break;
                    case '1':
                        image = loadImage("sprites/mines/1.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(BLUE "1 " RESET);
                        break;
                    case '2':
                        image = loadImage("sprites/mines/2.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(GREEN "2 " RESET);
                        break;
                    case '3':
                        image = loadImage("sprites/mines/3.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(RED "3 " RESET);
                        break;
                    case '4':
                        image = loadImage("sprites/mines/4.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(MAGENTA "4 " RESET);
                        break;
                    case '5':
                        image = loadImage("sprites/mines/5.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(YELLOW "5 " RESET);
                        break;
                    case '6':
                        image = loadImage("sprites/mines/6.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(CYAN "6 " RESET);
                        break;
                    case 'X':
                        image = loadImage("sprites/mines/bigX.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(RED "X " RESET);
                        break;
                    case 'x':
                        image = loadImage("sprites/mines/x.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf(RED WHITEBG "X" RESET " ");
                        break;
                    default:
                        image = loadImage("sprites/mines/blank.bmp", renderer);
                        SDL_RenderCopy(renderer, image, NULL, &dst);
                        //printf("%c ", display[row][col]);
                        break;
                    }
            }
        }
    }
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN && stop == false) {
            int rowClick = event.button.y / tailleCase;
            int colClick = event.button.x / tailleCase;
            SDL_Point cursor = { event.button.x, event.button.y };
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (!(rowClick > size - 1 || colClick > size - 1)) {
                    //inside the grid
                    if (display[rowClick][colClick] != 'F') {
                        countNearby(rowClick + 1, colClick + 1, size, grid, display);
                        if (grid[rowClick][colClick] == 1) {
                            for (int row = 0; row < size; row++) {
                                for (int col = 0; col < size; col++) {
                                    if (grid[row][col] == 1) {
                                        display[row][col] = 'X';
                                    }
                                }
                            }
                            display[rowClick][colClick] = 'x';
                            stop = true;
                            return(false);
                        }
                    }
                }
            }
            else {
                //flag
                if (!(rowClick > size - 1 || colClick > size - 1)) {
                    if (display[rowClick][colClick] == 'F') {
                        printf("F");
                        display[rowClick][colClick] = '?';
                    }
                    else {
                        display[rowClick][colClick] = 'F';
                    }
                }
            }
        }
        else if (event.type == SDL_QUIT) {
            SDL_Quit();
        }
    }
    SDL_RenderPresent(renderer);
}

void reveal(char** display, int** grid, int size) { //cherche un endroit avec 0 mines autour et le reveal (marche pas)
    int minRow = 0;
    int minCol = 0;
    int count;
    int minCount = 999;

    for (int row = 1; row < size + 1; row++) {
        for (int col = 1; col < size + 1; col++) {
            //printf("Counting on [%d][%d]\n", row, col);
            count = countNearby(row, col, size, grid, display); //this will reveal the entire grid, we need to reset it after
            if (count < minCount && grid[row - 1][col - 1] != 1) {
                minCount = count;
                minRow = row;
                minCol = col;
            }
        }
    }
    for (int row = 0; row < size; row++) { //reset grid
        for (int col = 0; col < size; col++) {
            display[row][col] = '?';
        }
    }
    //printf("Revealing [%d][%d]\n", minRow, minCol);
    countNearby(minRow, minCol, size, grid, display);
}

int main() {
    SDL_bool QUIT = SDL_FALSE;
    int size = 0;
    SDL_Event event;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int statut = EXIT_FAILURE;
    if (0 != initSDL(&window, &renderer, 720, 720)) /* ecrire cette fonction */
        goto Quit;
    bool init = false;
    statut = EXIT_SUCCESS;

    while (!(size >= 3 && size <= 78)) {
        size = askSize(renderer, window);
    }

    while (difficulty == 0) {
        askDifficulty(renderer, window);
    }

    /*------------------INIT-----------------------*/
    /* initialisation de la grid */
    int** grid = (int**)malloc(size * sizeof(int*)); // initialise les rows qui vont contennir les colonnes

    for (int num = 0; num < size; num++) {
        int* col = (int*)malloc(size * sizeof(int)); //initialise les array des colonnes
        grid[num] = col; //mettre la colone dans la row
    }
    // valeurs de base
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            grid[row][col] = 0;
        }
    }
    // initialisation de la grille display
    char** display = (char**)malloc(size * sizeof(char*)); // initialise les rows qui vont contennir les colonnes

    for (int num = 0; num < size; num++) {
        char* col = (char*)malloc(size * sizeof(char)); //initialise les array des colonnes
        display[num] = col; //mettre la colone dans la row
    }
    // valeurs de base
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (display[row][col]) {
                display[row][col] = '?';
            }
        }
    }

    printf("Size: %dx%d", size, size);
    int row, col;
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            grid[row][col] = 0;
        }
    }
    // grid � afficher
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            display[row][col] = '?';
        }
    }

    //populate with mines
    // change le nombre de mines suivant la difficult�
    switch (difficulty)
    {
    case 1:
        printf("\nDifficulty: Easy\n");
        printf("\nSize: %d", size);
        toPlace = (size * size) / 6;
        break;
    case 2:
        printf("\nDifficulty: Medium\n");
        toPlace = (size * size) / 5;
        break;
    case 3:
        printf("\nDifficulty: Hard\n");
        toPlace = (size * size) / 4;
        break;
    case 999:
        printf("Difficulty: Git Gud\n");
        toPlace = (size * size) - 1;
        break;
    default:
        break;
    }

    // RNG
    srand(time(NULL));
    int placed;
    // Randomly place mines
    for (placed = 0; placed < toPlace; placed++) {
        int rr = rand() % size;
        int rc = rand() % size;
        //no duplicates
        if (grid[rr][rc] == 0)
        {
            grid[rr][rc] = 1;
            //printf("%d, %d\n", rr, rc);
        }
        else {
            placed = placed - 1;
        }
    }

    // reveal a starting area to make it easier
    if (toPlace > 3 && difficulty != 999) {
        reveal(display, grid, size);
    }

    //timer setup
    time_t startTime, currentTime;
    startTime = time(NULL);
    int timer;
    init = true;

    while (!stop)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                goto Quit;

        currentTime = time(NULL);
        timer = difftime(currentTime, startTime);
        // display
        //system("cls"); //clear console
        //showgrid(grid, size);
        showdisplay(display, size, timer, renderer, window, grid);

        if (checkWin(size, display)) {
            system("cls"); //clear console
            printf("\nBravo, vous avez gagne! \n");
            currentTime = time(NULL);
            timer = difftime(currentTime, startTime);
            showdisplay(display, size, timer, renderer, window, grid);
            stop = true;
        }
    }

    //system("cls"); //clear console
    currentTime = time(NULL);
    timer = difftime(currentTime, startTime);
    printf("Time: %i", timer);
    while (!QUIT) {
        showdisplay(display, size, timer, renderer, window, grid);
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            QUIT = SDL_TRUE;
        }
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event != SDL_WINDOWEVENT_ENTER && event.window.event != SDL_WINDOWEVENT_LEAVE)
                showdisplay(display, size, timer, renderer, window, grid);
        }

    }
    free(display);
    free(grid);
Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}