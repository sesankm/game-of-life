#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define GRID_ROWS 25
#define GRID_COLS 25
#define CELL_WIDTH (SCREEN_WIDTH / GRID_ROWS)
#define CELL_HEIGHT (SCREEN_HEIGHT / GRID_COLS)

static int* cells[GRID_ROWS];

void init_cells(){
	for(int i = 0; i < GRID_ROWS; i++){
		cells[i] = (int*) malloc(sizeof(int) * GRID_COLS);
		for(int j = 0; j < GRID_COLS; j++){
			cells[i][j] = 0;
		}
	}
}

void draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int r, int g, int b){
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void draw_sq(SDL_Renderer* renderer, int x1, int y1, int w, int r, int g, int b){
	SDL_Rect rect;
	rect.x = x1;
	rect.y = y1;
	rect.w = w;
	rect.h = w;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void draw_grid(SDL_Renderer* renderer){
	for(int i = 1; i < GRID_ROWS; i++)
		draw_line(renderer, i * CELL_WIDTH, 0, i * CELL_WIDTH, SCREEN_HEIGHT, 200, 200, 200);
	for(int i = 1; i < GRID_COLS; i++)
		draw_line(renderer, 0, i * CELL_HEIGHT, SCREEN_WIDTH, i * CELL_HEIGHT, 200, 200, 200);
}

void render_cells(SDL_Renderer* renderer){
	for(int row = 0; row < GRID_ROWS; row++){
		for(int col = 0; col < GRID_ROWS; col++){
			if(cells[row][col] > 0){
				draw_sq(renderer, row * CELL_WIDTH, col * CELL_HEIGHT, CELL_WIDTH, 30, 30, 30);
			}
		}
	}
}

/* Rules
 * Live cells 2 or 3 live neighbors live
 * dead cell with three live neighbors becomes live
 * all other live cells die next gen, all other dead cells stay dead */
void update_cells(){
	int* new_cells[GRID_ROWS];
	for(int i = 0; i < GRID_ROWS; i++){
		new_cells[i] = (int*) malloc(sizeof(int) * GRID_COLS);
		for(int j = 0; j < GRID_COLS; j++){
			new_cells[i][j] = 0;
		}
	}
	for(int row = 0; row < GRID_ROWS; row++){
		for(int col = 0; col < GRID_COLS; col++){
			int counter = 0;
			if(row > 0){
				if(cells[row - 1][col] == 1) counter ++;
				if(cells[row - 1][col - 1] == 1) counter++;
				if(cells[row - 1][col + 1] == 1) counter++;
			}
			if(row < GRID_ROWS - 1){
				if(cells[row + 1][col] == 1) counter ++;
				if(cells[row + 1][col + 1] == 1) counter++;
				if(cells[row + 1][col - 1] == 1) counter++;
			}
			if(cells[row][col + 1] == 1) counter ++;
			if(cells[row][col - 1] == 1) counter ++;

			if(cells[row][col] > 0){
				if(counter == 2 || counter == 3){
					new_cells[row][col] = 1;
					continue;
				}
				else{
					new_cells[row][col] = 0;
					continue;
				}
			}
			else{
				if(counter == 3){
					new_cells[row][col] = 1;
					continue;
				}
				else{
					new_cells[row][col] = 0;
					continue;
				}
			}
		}
	}
	for(int row = 0; row < GRID_ROWS; row++){
		for(int col = 0; col < GRID_COLS; col++){
			cells[row][col] = new_cells[row][col];
		}
	}
}

void handle_mouse_event(SDL_Event* event){
	int x,y;
	x = event->button.x;
	y = event->button.y;

	int row, col;
	row = (x - (x % CELL_WIDTH)) / CELL_WIDTH;
	col = (y - (y % CELL_HEIGHT)) / CELL_HEIGHT;

	cells[row][col] = !(cells[row][col]);
}

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Conway's Game of Life",
										  0, 0,
										  SCREEN_WIDTH,
										  SCREEN_HEIGHT,
										  SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	init_cells();
	int quit = 0;
	int play = 0;

	while(!quit){
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_QUIT: 
				quit = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				handle_mouse_event(&event);
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_SPACE)
					play = !play;
			}
		}
		SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
		SDL_RenderClear(renderer);
		draw_grid(renderer);
		render_cells(renderer);
		if(play)
			update_cells();
		SDL_RenderPresent(renderer);
		usleep(60000);
	}
	return 0;
}
