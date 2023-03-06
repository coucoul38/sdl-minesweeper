#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

int main() {
	SDL_Window *window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;
	SDL_Color orange = { 255, 127, 40,255 };
	int status = EXIT_FAILURE;
	SDL_Rect rect = { 100, 100, 100, 100 }, dst = { 0,0,0,0 };
	SDL_Color rouge = { 255,0,0 ,255 }, bleu = {0,0,255,255 };

	if (0 != SDL_Init(SDL_INIT_VIDEO)) {
		//renvoyer l'erreur
		fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
		goto Quit;
	}
	//initialisation de la fenêtre
	if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer) != 0) {
		fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
		goto Quit;
	}
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 200, 200);
	if (texture == NULL) {
		fprintf(stderr, "Erreur CreateTexture : %s", SDL_GetError());
		goto Quit;
	}

	//si il n'y a eu aucun problème
	SDL_RaiseWindow(window);

	SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);
	SDL_RenderClear(renderer); // peint tout le render avec la couleur séléctionné
	
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rouge.r, rouge.g, rouge.b, rouge.a);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, bleu.r, bleu.g, bleu.b, bleu.a);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderTarget(renderer, NULL);
	
	SDL_RenderPresent(renderer);//mettre à jour l'écran
	
	//SDL_RenderCopy(renderer, texture, NULL, NULL); //copier la texture sur le render (remplir complétement)

	// copier la texture et l'afficher dans ses dimensions réelles
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h); //insére la largeur et hauteur de la texture dans dst (la zone dans laquelle la texture sera affichée)
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_Delay(500);

	SDL_RenderPresent(renderer);//mettre à jour l'écran
	SDL_Delay(500);


	status = EXIT_SUCCESS;

Quit:
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (window != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
	return status;
}