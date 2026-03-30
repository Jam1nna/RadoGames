#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#define CELL_SIZE 80
#define SNAKE_COLS 10
#define SNAKE_ROWS 10
#define MARGIN 20
#define MAX_SNAKE_LENGTH SNAKE_ROWS * SNAKE_COLS
#define START_LENGTH 10
#define MIN_SNAKE_LENGTH 1
#define GROW_INTERVAL 5.0f


typedef enum {
    Menu_scene,
    Play_scene,
    Loose_scene,
    Win_scene
} Scene;


Texture2D sn_menu_background;

Vector2 snake[MAX_SNAKE_LENGTH];
Vector2 direction = {1, 0};
Vector2 food[20];
Vector2 golden_apple = {0, 0};
int g_food_chance = 0;

int snakeLength = START_LENGTH;
// ...


float totalGameTime = 0.0f;
float growTimer = 0.0f;

int food_amount = 2;

bool game_over = false;
bool game_won = false;

Scene current_scene = Menu_scene;

float moveTimer = 0;
float moveInterval = 0.20f;

float flash_time = 0.0f;
int new_food_amount = 2;


void respawnFood(int idx) {
    while (true) {
        int rx = GetRandomValue(0, SNAKE_COLS - 1);
        int ry = GetRandomValue(0, SNAKE_ROWS - 1);
        bool onSnake = false;
        for (int i = 0; i < snakeLength; i++)
            if (snake[i].x == rx && snake[i].y == ry) { onSnake = true; break; }
        if (!onSnake) { food[idx] = (Vector2){rx, ry}; break; }
    }
}

void resetGame(void) {
    snakeLength = START_LENGTH;
    direction = (Vector2){1, 0};
    food_amount = 2;
    new_food_amount = 2;
    moveTimer = 0;
    totalGameTime = 0;
    game_over = false;
    game_won = false;
    golden_apple = (Vector2){-10, -10};

    for (int i = 0; i < START_LENGTH; i++) {
        snake[i] = (Vector2){SNAKE_COLS / 2 - i, SNAKE_ROWS / 2};
    }
    for (int i = 0; i < food_amount; i++) {
        respawnFood(i);
    }
}


void run_snake(void) {
	const int width  = MARGIN * 2 + SNAKE_COLS * CELL_SIZE;
    const int height = MARGIN * 2 + SNAKE_ROWS * CELL_SIZE;

    // resize window for snake
    SetWindowSize(width, height);
    SetWindowTitle("SNAKE");

    respawnFood(0);
	respawnFood(1);

    sn_menu_background = LoadTexture("assets/menu_bg1.png");

    while (!WindowShouldClose()) {

    	float dt = GetFrameTime();


    	BeginDrawing();

    	ClearBackground(BLACK);


    	switch (current_scene) {

        case Menu_scene:
        	resetGame();

        	// Draw Background
        	DrawTexturePro(sn_menu_background,
                    (Rectangle){0, 0, sn_menu_background.width, sn_menu_background.height},
                    (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                    (Vector2){0, 0}, 0.0f, WHITE);

        	// ... Text


        	// Header
        	const char* h_t = "SNAKE";
        	const int h_fs = 125; // font size
        	DrawText(h_t, (width - MeasureText(h_t, h_fs)) / 2, 30, h_fs, ORANGE);

        	// Instructions
        	const char* i_t = "Press ENTER to start";
        	const int i_fs = 50;
        	DrawText(i_t, (width - MeasureText(i_t, i_fs)) / 2, 350, i_fs, WHITE);
        	const char* i_t2 = "Avoid food at all cost!";
        	const int i_fs2 = 30;
        	DrawText(i_t2, (width - MeasureText(i_t2, i_fs2)) / 2, height - 100, i_fs2, RED);
            const char* i_t3 = "Find the gold one to win!";
            const int i_fs3 = 30;
            DrawText(i_t3, (width - MeasureText(i_t3, i_fs3)) / 2, height - 50, i_fs3, GOLD);
            const char* i_t4 = "Score >= 20 and 1/5000";
            const int i_fs4 = 15;
            DrawText(i_t4, (width - MeasureText(i_t4, i_fs4)) / 2, height - 20, i_fs4, LIGHTGRAY);


        	if (IsKeyPressed(KEY_ENTER)) {
        		current_scene = Play_scene;
        	}
            // Q on snake menu = back to RadoGames
            if (IsKeyPressed(KEY_Q)) {
                UnloadTexture(sn_menu_background);
                current_scene = Menu_scene;
                EndDrawing();
                return;
            }
        	break;
        case Play_scene:

        	float fps = GetFPS();
        	totalGameTime += dt;
        	growTimer += dt;


        	// Top hud
        	DrawText(TextFormat("Time: %0.1fs", totalGameTime), 150, 0, 20, GREEN);
        	DrawText(TextFormat("Score: %d", snakeLength), 20, 0, 20, GREEN);
        	DrawText(TextFormat("FPS: %0.1f", fps),width - MeasureText(TextFormat("FPS: %0.1f", fps), 20) - 300, 0, 20, GREEN);
        	DrawText(TextFormat("Food: %d", food_amount), width - MeasureText(TextFormat("Food: %d", food_amount), 20) - 20, 0, 20, GREEN);

        	// Grow timer bar on the right
            float ratio = growTimer / GROW_INTERVAL;
            int bar_full_height = height - MARGIN * 2;
            int bar_h = (int)(bar_full_height * (1.0f - ratio)); // drains down
            int bar_x = width - 15;
            int bar_y = MARGIN + (bar_full_height - bar_h);

            Color bar_color = GREEN;
            if (ratio > 0.66f) bar_color = RED;
            else if (ratio > 0.33f) bar_color = ORANGE;

            DrawRectangle(bar_x, MARGIN, 10, bar_full_height, Fade(WHITE, 0.1f)); // bg track
            DrawRectangle(bar_x, bar_y, 10, bar_h, bar_color);


        	if (flash_time > 0) flash_time -= dt;

        	if (flash_time > 0) {
                float alpha = flash_time / 0.25f * 0.45f;
                DrawRectangle(0, 0, width, height, Fade(GREEN, alpha));
            }

        	// Growing by time interval
        	if (growTimer >= GROW_INTERVAL) {
        		snakeLength += 1;
        		growTimer = 0.0f;
        		for (int i = 0; i < food_amount; i++) {
        			respawnFood(i);
        		}
        	}

        	// --- Food logic ---
        	// Increasing food amount
        	if (totalGameTime >= 10.0f && new_food_amount == 2) {
        		new_food_amount = 3;
        		flash_time = 0.25f;
        	} else if (totalGameTime >= 25.0f && new_food_amount == 3) {
        		new_food_amount = 4;
        		flash_time = 0.25f;
        	} else if (totalGameTime >= 50.0f && new_food_amount == 5) {
        		new_food_amount = 6;
        		flash_time = 0.25f;
        	} else if (totalGameTime >= 80.0f && new_food_amount == 6) {
        		new_food_amount = 8;
        		flash_time = 0.25f;
        	}
        	if (new_food_amount > food_amount) {
        		for (int i = 0; i < food_amount; i++) {
        			respawnFood(i);
        		}
        	} food_amount = new_food_amount;

            // Golden apple --> Win

            DrawText(TextFormat("G: %0.d", g_food_chance), 20, height - 20, 20, GOLD);
            if (g_food_chance == 531 && snakeLength >= 20) {
                if (golden_apple.x == -10 && golden_apple.y == -10) {
                    golden_apple.x = GetRandomValue(0, SNAKE_COLS - 1);
                    golden_apple.y = GetRandomValue(0, SNAKE_ROWS - 1);
                }
                // Is in body or food?
                for (int i = 1; i < snakeLength; i++) {
                    if ((snake[i].x == golden_apple.x && snake[i].y == golden_apple.y) || (food[i].x == golden_apple.x && food[i].y == golden_apple.y)) {
                        golden_apple.x = GetRandomValue(0, SNAKE_COLS - 1);
                        golden_apple.y = GetRandomValue(0, SNAKE_ROWS - 1);
                    }
                }

                // Draw
                DrawRectangle(MARGIN + golden_apple.x * CELL_SIZE, MARGIN + golden_apple.y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, GOLD);

                // Win
                if (snake[0].x == golden_apple.x && snake[0].y == golden_apple.y) {
                    game_won = true;
                }
            } else { g_food_chance = GetRandomValue(1, 5000); }



        	// Draw food
        	for (int i = 0; i < food_amount; i++) {
                    DrawRectangle(MARGIN + food[i].x*CELL_SIZE, MARGIN + food[i].y*CELL_SIZE, CELL_SIZE-1, CELL_SIZE-1, RED);
                    int fx = MARGIN + food[i].x*CELL_SIZE + CELL_SIZE/2;
                    int fy = MARGIN + food[i].y*CELL_SIZE + CELL_SIZE/2;
                    DrawLineEx((Vector2){fx-20, fy-20}, (Vector2){fx+20, fy+20}, 4.0f, BLACK);
                    DrawLineEx((Vector2){fx+20, fy-20}, (Vector2){fx-20, fy+20}, 4.0f, BLACK);
                }


            // did snake eat food?
            for (int i_f = 0; i_f < food_amount; i_f++) {
                if (snake[0].x == food[i_f].x && snake[0].y == food[i_f].y) {
                    snakeLength--;
                    respawnFood(i_f);
                }
            }

            // did food spawn in snake
            for (int i = 1; i < snakeLength; i++) {
                for (int y = 0; y < food_amount; y++) {
                    if (food[y].x == snake[i].x && food[y].y == snake[i].y) {
                        respawnFood(y);
                    }
                }
            }



            // --- Snake logic ---

            // Movement
                if      ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && direction.x != -1) direction = (Vector2){ 1,  0};
                else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  && direction.x !=  1) direction = (Vector2){-1,  0};
                else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  && direction.y != -1) direction = (Vector2){ 0,  1};
                else if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    && direction.y !=  1) direction = (Vector2){ 0, -1};

            	moveTimer += GetFrameTime();

                if (moveTimer >= moveInterval) {
                    moveTimer = 0;

	            // move body after head
	                    for (int i = snakeLength - 1; i > 0; i--) {
	                        snake[i] = snake[i - 1];
	                    }

	                    snake[0].x += direction.x;
	                    snake[0].y += direction.y;


	                    // if (snakeLength == MAX_SNAKE_LENGTH - food_amount) {
	                    //     game_won = true;
	                    // }

	                    // wall collision
	                    if (snake[0].x < 0 || snake[0].x >= SNAKE_COLS ||
	                        snake[0].y < 0 || snake[0].y >= SNAKE_ROWS) {
	                        game_over = true;
	                    }

	                    // self collision
	                    for (int i = 1; i < snakeLength; i++) {
	                        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
	                            game_over = true;
	                        }
	                    }
	            }

	            // draw snake
	                for (int i = 0; i < snakeLength; i++) {
	                    DrawRectangle(
	                        MARGIN + snake[i].x * CELL_SIZE,
	                        MARGIN + snake[i].y * CELL_SIZE,
	                        CELL_SIZE - 1,
	                        CELL_SIZE - 1,
	                        Fade(GREEN, 1.0f - i*(1.0f/(snakeLength*2)))
	                    );
	                }

        	// Draw grid
                for (int x = 0; x <= SNAKE_COLS; x++)
                    DrawLine(MARGIN + x*CELL_SIZE, MARGIN, MARGIN + x*CELL_SIZE, MARGIN + SNAKE_ROWS*CELL_SIZE, Fade(WHITE, 0.1f));
                for (int y = 0; y <= SNAKE_ROWS; y++)
                    DrawLine(MARGIN, MARGIN + y*CELL_SIZE, MARGIN + SNAKE_COLS*CELL_SIZE, MARGIN + y*CELL_SIZE, Fade(WHITE, 0.1f));
        	break;

        case Loose_scene:
        	// Loose Header
        	const char* lh_t = "YOU LOST";
        	const int lh_fs = 125; // font size
        	DrawText(lh_t, (width - MeasureText(lh_t, h_fs)) / 2, 30, lh_fs, RED);

        	// Loose Instructions
        	const char* li_t = "Press Q or SPACE to go to menu";
        	const int li_fs = 40;
        	DrawText(li_t, (width - MeasureText(li_t, li_fs)) / 2, 350, li_fs, WHITE);
        	break;

        case Win_scene:
        	// Win Header
        	const char* wh_t = "YOU WON";
        	const int wh_fs = 125; // font size
        	DrawText(wh_t, (width - MeasureText(wh_t, wh_fs)) / 2, 30, wh_fs, YELLOW);

        	// Win Instructions
        	const char* wi_t = "Press Q or SPACE to go to menu";
        	const int wi_fs = 40;
        	DrawText(wi_t, (width - MeasureText(wi_t, wi_fs)) / 2, 350, wi_fs, WHITE);
          break;
        }
        // Scene control
        if (IsKeyPressed(KEY_Q ) || IsKeyPressed(KEY_SPACE)) {
        	current_scene = Menu_scene;
        	game_over = game_won = false;
        }
        if (game_over) { current_scene = Loose_scene; }
        if (game_won) { current_scene = Win_scene; }



    	EndDrawing();
    }
    UnloadTexture(sn_menu_background);
}
