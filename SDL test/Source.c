
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if (SDL_CreateWindowAndRenderer(w, h, NULL, window, renderer) != 0) {
        fprintf(stderr, "Erreur CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
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

int main(int argc, char* argv)
{
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* image = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color blanc = { 255, 255, 255, 255 };
    if (0 != init(&window, &renderer, 640, 480)) /* ecrire cette fonction */
        goto Quit;

    image = loadImage("bmp.bmp", renderer); /* ecrire cette fonction*/
    if (NULL == image)
        goto Quit;

    statut = EXIT_SUCCESS;
    setWindowColor(renderer, blanc); /* ecrire cette fonction */
    SDL_RenderCopy(renderer, image, NULL, NULL); //copier l'image sur le render
    SDL_RenderPresent(renderer);

    while (!quit)
    {
        //SDL_WaitEvent(&event); //le programme est bloqué tant qu'il n'y a pas d'événement
        while(SDL_PollEvent(&event)) //ne bloque pas le programme
            if (event.type == SDL_QUIT) //si l'utilisateur essaie de quiter la fenêtre
                //quit = SDL_TRUE; //quiter la fenêtre
                goto Quit;
        SDL_Delay(20); //attendre 20ms entre chaque vérification d'event, pour avoir de meilleures performances
    }
Quit:
    if (NULL != image)
        SDL_DestroyTexture(image);
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}