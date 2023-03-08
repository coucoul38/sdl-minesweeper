
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

int* pointeurSize = NULL;
int toPlace;
int difficulty = 0;
SDL_Color blanc = { 255, 255, 255, 255 };
SDL_Color gris = { 133,133,133,255 };
int location = 0;

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h) {
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

SDL_Texture *loadImage(const char path[], SDL_Renderer* renderer) {
    SDL_Surface* tmp = NULL;
    SDL_Texture* texture = NULL;
    //printf("Path: %s\n", path);
    
    tmp = SDL_LoadBMP(path);
    if (tmp == NULL) {
        //fprintf(stderr, "Erreur LoadBMP : %s", SDL_GetError());
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

int askDifficulty(SDL_Renderer* renderer, SDL_Window*window) {
    setWindowColor(renderer, gris);

    int w = NULL, h = NULL;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect recteasy = { w/2-72, 0, 144, 48 };
    SDL_Rect rectmed = { w / 2 - 72, 48, 144, 48 };
    SDL_Rect recthard = { w / 2 - 72, 96, 144, 48 };

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
    SDL_RenderCopy(renderer, easy, NULL, &recteasy); //copier l'image sur le render
    SDL_RenderCopy(renderer, medium, NULL, &rectmed); //copier l'image sur le render
    SDL_RenderCopy(renderer, hard, NULL, &recthard); //copier l'image sur le render 
    SDL_RenderPresent(renderer);
}

int askSize(SDL_Renderer* renderer, SDL_Window* window) {
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
}

int main(int argc, char* argv)
{
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int statut = EXIT_FAILURE;
    if (0 != init(&window, &renderer, 640, 480)) /* ecrire cette fonction */
        goto Quit;

    statut = EXIT_SUCCESS;

    while (!quit)
    {
        switch (location)
        {
        case 0:
            askSize(renderer, window);
            break;
        case 1:
            askDifficulty(renderer, window);
            break;
        default:
            break;
        }
        //SDL_RaiseWindow(window);
        //SDL_WaitEvent(&event); //le programme est bloqué tant qu'il n'y a pas d'événement
        while(SDL_PollEvent(&event)) //ne bloque pas le programme
            if (event.type == SDL_QUIT) //si l'utilisateur essaie de quiter la fenêtre
                //quit = SDL_TRUE; //quiter la fenêtre
                goto Quit;
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_A)
                    printf("scancode A\n");
                if (event.key.keysym.sym == SDLK_a)
                    printf("keysym A\n");
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    if (location == 1) {
                        int w = NULL, h = NULL;
                        SDL_GetWindowSize(window, &w, &h);
                        SDL_Rect recteasy = { w / 2 - 72, 0, 144, 48 };
                        SDL_Rect rectmed = { w / 2 - 72, 48, 144, 48 };
                        SDL_Rect recthard = { w / 2 - 72, 96, 144, 48 };
                        SDL_Point cursor = { event.button.x, event.button.y };
                        if (SDL_PointInRect(&cursor, &recteasy) == SDL_TRUE) {
                            printf("Easy");
                            
                        }
                        else if (SDL_PointInRect(&cursor, &rectmed) == SDL_TRUE) {
                            printf("Med");
                        }
                        else if (SDL_PointInRect(&cursor, &recthard) == SDL_TRUE) {
                            printf("Hard");
                        }
                    } else if (location == 0) {
                        int w = NULL, h = NULL;
                        SDL_GetWindowSize(window, &w, &h);
                        SDL_Rect rect5 = { w / 2 - 72, 0, 144, 48 };
                        SDL_Rect rect10 = { w / 2 - 72, 48, 144, 48 };
                        SDL_Rect rect15 = { w / 2 - 72, 96, 144, 48 };
                        SDL_Rect rect20 = { w / 2 - 72, 144, 144, 48 };
                        SDL_Rect rectCustom = { w / 2 - 72, 194, 144, 48 };
                        SDL_Point cursor = { event.button.x, event.button.y };
                        if (SDL_PointInRect(&cursor, &rect5) == SDL_TRUE) {
                            printf("5");
                            location++;
                        }
                        else if (SDL_PointInRect(&cursor, &rect10) == SDL_TRUE) {
                            printf("10");
                            location++;
                        }
                        else if (SDL_PointInRect(&cursor, &rect15) == SDL_TRUE) {
                            printf("15");
                            location++;
                        }
                        else if (SDL_PointInRect(&cursor, &rect20) == SDL_TRUE) {
                            printf("20");
                            location++;
                        }
                        else if (SDL_PointInRect(&cursor, &rectCustom) == SDL_TRUE) {
                            printf("Choose your size");
                        }
                    }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    printf("Au moins un clic droit\n");
            }

        SDL_Delay(20); //attendre 20ms entre chaque vérification d'event, pour avoir de meilleures performances
    }
Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

//-----------------------------------------------
// compter le nombre de mines adjacentes à une case
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

    //si il n'y a aucune mine autour, découvrir les cases adjacentes
    if (count == 0) {
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

void showdisplay(char** display, int size, int timer) {
    // display
    printf(GRAYBG "Mines: " RED "%d" RESET, toPlace);
    printf(GRAYBG " -- Difficulte: " RED "%i" RESET, difficulty);
    printf(GRAYBG " -- Time: " RED "%d\n" RESET, timer);
    for (int row = -1; row < size; row++) {
        for (int col = -1; col < size; col++) {
            if (row == -1) {
                //ALIGNEMENT DES ROWS
                if (size > 9) {
                    if (col > 9) {
                        printf(BMAGENTA "%d " RESET, col + 1);
                    }
                    else {
                        printf(BMAGENTA "%d  " RESET, col + 1);
                    }
                }
                else {
                    printf(BMAGENTA "%d " RESET, col + 1);
                }
            }
            else if (col == -1) {
                //ALIGNEMENT DES ROWS
                if (size > 9) {
                    if (row > 8) {
                        printf(BMAGENTA "%d " RESET, row + 1);
                    }
                    else {
                        printf(BMAGENTA "%d  " RESET, row + 1);
                    }
                }
                else {
                    printf(BMAGENTA "%d " RESET, row + 1);
                }
            }
            else {
                if (display[row][col] == 'F') {
                    printf(REDBG "F" RESET " ");
                }
                else if (display[row][col] == '?' /* || display[row][col] == '0'*/) {
                    printf("%c ", display[row][col]);
                }
                else {
                    switch (display[row][col])
                    {
                    case '0':
                        printf(COLOR0 "0 " RESET);
                        break;
                    case '1':
                        printf(BLUE "1 " RESET);
                        break;
                    case '2':
                        printf(GREEN "2 " RESET);
                        break;
                    case '3':
                        printf(RED "3 " RESET);
                        break;
                    case '4':
                        printf(MAGENTA "4 " RESET);
                        break;
                    case '5':
                        printf(YELLOW "5 " RESET);
                        break;
                    case '6':
                        printf(CYAN "6 " RESET);
                        break;
                    case 'X':
                        printf(RED "X " RESET);
                        break;
                    case 'x':
                        printf(RED WHITEBG "X" RESET " ");
                        break;
                    default:
                        printf("%c ", display[row][col]);
                        break;
                    }
                }
                //ALIGNEMENT DES ROWS
                if (size > 9) {
                    printf(" ");
                }
            }
            if (col == size - 1) {
                printf("\n");
            }
        }
    }
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


int oldmain() {
    bool lost = false;
    int size = 0;

    while (!(size >= 3 && size <= 78)) {
        printf("Choisir une taille pour la grille de jeu (entre 3 et 78): ");
        scanf_s("%d", &size);
    }


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
    // grid à afficher
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            display[row][col] = '?';
        }
    }

    //populate with mines


    //change number of mines depending on difficulty

    //vérifier que la difficulté entrée est valide
    bool valide = false;
    while (!valide) {
        printf("\nChoisir la difficulte (entre 1 et 3): ");
        scanf_s("%d", &difficulty);
        if (difficulty > 0 && difficulty < 4 || difficulty == 999) {
            valide = true;
        }
    }
    // change le nombre de mines suivant la difficulté
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

    while (!lost)
    {
        currentTime = time(NULL);
        timer = difftime(currentTime, startTime);
        // display
        system("cls"); //clear console
        //showgrid(grid, size);
        showdisplay(display, size, timer);

        int inputR, inputC;
        char c1, c2;
        bool valide = false;
        while (!valide) {
            // récupérer les coordonnées
            printf("\nEntrez les coordonees de la case ainsi: Ligne Colone (and flag)\nex: 1 3\n");
            scanf_s("%d %d", &inputR, &inputC);

            c1 = getchar(); //récupère le 1er char du buffer (probablement un espace)

            while (c1 != '\n' && c1 == ' ')
            {
                c1 = getchar(); //on parcours le buffer jusqu'à trouver un char ou arriver à la fin
            }

            c2 = c1;
            while (c2 != '\n')
            {
                c2 = getchar(); //vider le buffer
            }

            // Check si la case est valide
            if ((inputC <= size && inputC > 0) && (inputR <= size && inputR > 0)) {
                valide = true;
                if (c1 == 'f') {
                    printf("test");
                    if (display[inputR - 1][inputC - 1] == 'F') {
                        printf("F");
                        display[inputR - 1][inputC - 1] = '?';
                    }
                    else {
                        display[inputR - 1][inputC - 1] = 'F';
                    }
                }
            }
            else { // redemander si elle ne l'est pas
                printf("\nCoordonees invalides, reessayez\n");
            }
        }
        // check si la case à été découverte
        if (display[inputR - 1][inputC - 1] == '?') {
            if (grid[inputR - 1][inputC - 1] == 1) {
                if (c1 != 'f') {
                    for (row = 0; row < size; row++) {
                        for (col = 0; col < size; col++) {
                            if (grid[row][col] == 1) {
                                display[row][col] = 'X';
                            }
                        }
                    }
                    display[inputR - 1][inputC - 1] = 'x';
                    lost = true;
                    printf("\nPERDU!\n");
                }
            }
            else if (c1 != 'f') {
                countNearby(inputR, inputC, size, grid, display);
            }
        }
        if (checkWin(size, display)) {
            system("cls"); //clear console
            printf("\nBravo, vous avez gagne! \n");
            currentTime = time(NULL);
            timer = difftime(currentTime, startTime);
            showdisplay(display, size, timer);
            return(0);
        }


        //CHECK FOR MINES
        /*if (grid[inputR - 1][inputC - 1] == 1) {
            lost = true;
            printf("Perdu !");
        }*/
    }
    system("cls"); //clear console
    currentTime = time(NULL);
    timer = difftime(currentTime, startTime);
    showdisplay(display, size, timer);
    free(display);
    free(grid);
    return 0;
}