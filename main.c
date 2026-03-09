#include "raylib.h"
#include <stdbool.h>

#define CELL_SIZE 80
#define COLS 10
#define ROWS 10
#define MARGIN 20
#define MAX_SNAKE_LENGTH ROWS * COLS


typedef enum {
    Menu_scene,
    Play_scene,
    Loose_scene,
    Win_scene
} Scene;

Texture2D menu_background;

Vector2 snake[MAX_SNAKE_LENGTH];
Vector2 direction = {1, 0};
Vector2 food = {5, 5};

int snakeLength = 3;
float moveTimer = 0;
float moveInterval = 0.20f;

bool game_over = false;
bool game_won = false;

Scene current_scene = Menu_scene;


void resetGame(void) {
    snakeLength = 3;
    direction = (Vector2){1, 0};
    food = (Vector2){5, 5};
    moveTimer = 0;
    game_over = false;
    game_won = false;
    snake[0] = (Vector2){COLS / 2, ROWS / 2};
    snake[1] = (Vector2){COLS / 2 - 1, ROWS / 2};
    snake[2] = (Vector2){COLS / 2 - 2, ROWS / 2};
}


int main(void) {

    const int width = MARGIN * 2 + COLS * CELL_SIZE;
    const int height = MARGIN * 2 + ROWS * CELL_SIZE;

    InitWindow(width, height, "SNAKE");
    SetTargetFPS(60);

    menu_background = LoadTexture("assets/menu_bg1.png");

    snake[0] = (Vector2){COLS / 2, ROWS / 2};
    snake[1] = (Vector2){COLS / 2 - 1, ROWS / 2};
    snake[2] = (Vector2){COLS / 2 - 2, ROWS / 2};


    while (!WindowShouldClose()) {

        // switch menu/game
        switch (current_scene) {

            case Menu_scene:{
                if (IsKeyPressed(KEY_ENTER)) {
                    resetGame();
                    current_scene = Play_scene;
                }
                break;
            }
            case Play_scene:
                if (IsKeyPressed(KEY_Q)) {
                    current_scene = Menu_scene;
                }
                break;
            case Loose_scene: {
                if (IsKeyPressed(KEY_Q)) {
                    current_scene = Menu_scene;
                }
                game_over = false;
                break;
            }
            case Win_scene:{
                if (IsKeyPressed(KEY_Q)) {
                    current_scene = Menu_scene;
                }
                game_won = false;
                break;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (current_scene) {
            case Menu_scene: {
                //Img and text
                DrawTexturePro(
                    menu_background,
                    (Rectangle) {
                    0, 0, menu_background.width, menu_background.height
                },
                    (Rectangle) {
                    0, 0, GetScreenWidth(), GetScreenHeight()
                },
                    (Vector2) {
                    0, 0
                },
                    0.0f,
                    WHITE
                );

                const char* menu_t = "SNAKE";
                const int menu_t_font = 100;
                const int menu_t_width = MeasureText(menu_t, menu_t_font);
                DrawText(menu_t, (width - menu_t_width) / 2, 100, menu_t_font, ORANGE);

                const char* submenu_t = "Press ENTER to start!";
                const int submenu_t_font = 30;
                const int submenu_t_width = MeasureText(submenu_t, submenu_t_font);
                DrawText(submenu_t, (width - submenu_t_width)/2 , 300, submenu_t_font, WHITE);
                break;
            }
            case Play_scene:{
                // draw grid
                for (int x = 0; x <= COLS; x++) {
                    DrawLine(
                        MARGIN + x * CELL_SIZE, MARGIN,
                        MARGIN + x * CELL_SIZE, MARGIN + ROWS * CELL_SIZE,
                        Fade(WHITE, 0.1f)
                    );
                }
                for (int y = 0; y <= ROWS; y++) {
                    DrawLine(
                        MARGIN, MARGIN + y * CELL_SIZE,
                        MARGIN + COLS * CELL_SIZE, MARGIN + y * CELL_SIZE,
                        Fade(WHITE, 0.1f)
                    );
                }

                // movement
                if (IsKeyPressed(KEY_D) && direction.x != -1)
                        direction = (Vector2){1, 0};
                    else if (IsKeyPressed(KEY_A) && direction.x != 1)
                        direction = (Vector2){-1, 0};
                    else if (IsKeyPressed(KEY_S) && direction.y != -1)
                        direction = (Vector2){0, 1};
                    else if (IsKeyPressed(KEY_W) && direction.y != 1)
                        direction = (Vector2){0, -1};

                // main logic
                moveTimer += GetFrameTime();

                if (moveTimer >= moveInterval) {
                    moveTimer = 0;

                    // did snake eat food?
                    if (snake[0].x == food.x && snake[0].y == food.y) {
                        snakeLength++;
                        food.x = GetRandomValue(0, COLS - 1);
                        food.y = GetRandomValue(0, ROWS - 1);
                    }

                    // move body after head
                    for (int i = snakeLength - 1; i > 0; i--) {
                        snake[i] = snake[i - 1];
                    }

                    snake[0].x += direction.x;
                    snake[0].y += direction.y;


                    if (snakeLength == MAX_SNAKE_LENGTH) {
                        game_won = true;
                    }

                    // wall collision
                    if (snake[0].x < 0 || snake[0].x >= COLS ||
                        snake[0].y < 0 || snake[0].y >= ROWS) {
                        game_over = true;
                    }

                    // self collision
                    for (int i = 1; i < snakeLength; i++) {
                        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                            game_over = true;
                        }
                    }

                }

                // draw food
                DrawRectangle(
                MARGIN + food.x * CELL_SIZE,
                MARGIN + food.y * CELL_SIZE,
                CELL_SIZE - 1,
                CELL_SIZE - 1,
                RED
                );

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
              break;
            }
            case Loose_scene: {
                const char* loose_t = "You Lost =(";
                const int loose_t_font = 60;
                const int loose_t_width = MeasureText(loose_t, loose_t_font);
                DrawText(loose_t, (width - loose_t_width) / 2, 100, loose_t_font, RED);

                const char* loose_submenu_t = "Press Q to go to menu!";
                const int loose_submenu_t_font = 30;
                const int loose_submenu_t_width = MeasureText(loose_submenu_t, loose_submenu_t_font);
                DrawText(loose_submenu_t, (width - loose_submenu_t_width)/2 , 250, loose_submenu_t_font, WHITE);
                break;
            }
            case Win_scene: {
                const char* loose_t = "You Won =)";
                const int loose_t_font = 60;
                const int loose_t_width = MeasureText(loose_t, loose_t_font);
                DrawText(loose_t, (width - loose_t_width) / 2, 100, loose_t_font, YELLOW);

                const char* win_submenu_t = "Press Q to go to menu!";
                const int win_submenu_t_font = 30;
                const int win_submenu_t_width = MeasureText(win_submenu_t, win_submenu_t_font);
                DrawText(win_submenu_t, (width - win_submenu_t_width)/2 , 250, win_submenu_t_font, WHITE);
                break;
            }
        }



        if (game_won) {
            current_scene = Win_scene;
        } else if (game_over) {
            current_scene = Loose_scene;
        }


        EndDrawing();
    }
    UnloadTexture(menu_background);
    CloseWindow();
    return 0;
}