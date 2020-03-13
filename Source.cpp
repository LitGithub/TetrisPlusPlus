#include<iostream>
using namespace std;
#include <allegro5\allegro.h> 
#include<allegro5\allegro_primitives.h> 
#include<allegro5\allegro_image.h> 
#include<allegro5\allegro_font.h> 
#include<allegro5\allegro_ttf.h> 
#include <allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include<vector> 
#include<ctime>
#include "line.h" 
#include "piece.h" 
#include "square.h" 
#include "lblock.h" 
//#include" squiggle.h" 
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 800;
const int FPS = 60;
bool cullGrid(); //the only non-class function!

int grid[10][16] = {
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0
};

//enumeration to help you remember what numbers represent which directions
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

int main() {
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();

	srand(time(NULL));
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_load_font("Minecraft.ttf", 40, 0);
	ALLEGRO_SAMPLE* music = al_load_sample("Tetris.mp3");
	ALLEGRO_SAMPLE_INSTANCE* sampleInstance = al_create_sample_instance(music);
	al_reserve_samples(1);
	al_set_sample_instance_playmode(sampleInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(sampleInstance, al_get_default_mixer());
	
	bool doexit = false;
	int ticker = 0;
	int moveTimer = 0;
	bool key[] = { false, false, false, false, false };
	bool redraw = true;
	bool ActivePiece = false;
	int pieceChoice = 0;
	vector<piece*> pieces;
	vector<piece*>::iterator iter;
	//allegro start up stuff
	al_set_target_bitmap(al_get_backbuffer(display));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(200, 200, 200), 200, 200, NULL, "Tetris!");
	al_flip_display();
	al_rest(3);
	al_play_sample_instance(sampleInstance);
	while (!doexit) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		//timer (physics) section////////////////////////////////////////
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			ticker++;
			moveTimer++;
			//cull the grid
			if (ticker > 200)
				if (cullGrid() == true)
					ticker = 0;

			if (moveTimer > 5) {
				if (key[KEY_LEFT]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->MoveSideways('l', grid);
					}
				}
				if (key[KEY_RIGHT]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->MoveSideways('r', grid);
					}
				}
				if (key[KEY_DOWN]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->MoveSideways('d', grid);
					}
				}
				if (key[KEY_SPACE]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->turn('r');
					}
				}
				moveTimer = 0;
			}
			//create new piece
			
			if (ActivePiece == false && ticker > 150) {
				pieceChoice = rand() % 1 + 1; //randomly pick next piece type
				if (pieceChoice == 1) {
					line* newline = new line((rand() % 10) * 50, 0); pieces.push_back(newline);
					cout << "pushed line" << endl;
				}
				if (pieceChoice == 2) {
					square* newline = new square((rand() % 9) * 50, 0); pieces.push_back(newline);
					cout << "pushed square" << endl;
				}
				ActivePiece = true;
				if (pieceChoice != 2 && pieceChoice != 1) {
					ActivePiece = false;
					cout << ActivePiece << endl;
				}
			}
			// check if player has lost
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				if ((*iter)->checkCollision(grid)) {
					cout << "y value is " << (*iter)->getY() << endl;
					if ((*iter)->getY() <= 0) {
						cout << "game over!" << endl;
						doexit = true;
					}
				}
			}
			// check if a piece has hit the bottom or other piece (changed into a function from part 1)
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				if ((*iter)->checkCollision(grid)) {
					(*iter)->land(grid);
					ActivePiece = false;
					iter = pieces.erase(iter); //erase the piece from the vector when it lands (this breaks code)
					break; //this fixed it :)
				}
			}
			//move pieces if they haven't hit bottom
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				if (!(*iter)->isLanded())
					(*iter)->move();
			}
			//wipe map
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 16; j++) {
					if (grid[i][j] < 10)
						grid[i][j] = 0;
				}
			}
			redraw = true;
		}//end timer section


			//keyboard and screen sections//////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		//render section/IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//draw pieces
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				(*iter)->draw(grid);
			}
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 16; j++) {
					if (grid[i][j] == 0)//draws grid lines
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(255, 255, 255), 2);

					if (grid[i][j] == 1) {//draws active line pieces 
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(255, 50, 55));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(25, 50, 55), 4);
					}
					if (grid[i][j] == 10) {//draws inactive line pieces
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(255, 50, 55));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(25, 50, 55), 4);
					}
					if (grid[i][j] == 2) {//draws active line pieces 
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(50, 255, 55));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(50, 25, 55), 4);
					}
					if (grid[i][j] == 20) {//draws inactive line pieces
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(50, 255, 55));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(50, 25, 55), 4);
					}

					if (grid[i][j] == 4) {//draws active line pieces 
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(50, 255, 255));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(0, 50, 55), 4);
					}


				}
			}
			al_flip_display();
		}//end render


	}//end game loop

	//clean up memory
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

bool cullGrid() {
	return rand() % 2;
}