#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

// #define CELL_SIZE 80
// #define COLS 10
// #define ROWS 10
#define MARGIN 20
#define MAX_SNAKE_LENGTH ROWS * COLS

typedef enum {
    Main_menu,
    Snake_game,
    TikTakToe,
    Minesweeper,
    Hangman
} What_menu;

What_menu current_menu = Main_menu;

Font main_menu_font;

int main(void) {

    const int width = 1200;
    const int height = 1000;


    InitWindow(width, height, "RadoGames");
    SetTargetFPS(60);
    srand(time(NULL));


    main_menu_font = LoadFontEx("04b03.ttf", 100, NULL, 0);

    BeginDrawing();

    while (!WindowShouldClose()) {

    ClearBackground(BLACK);

    switch (current_menu) {
        case Main_menu:
            // Display all games and be able to choose from them
            const char* main_menu_t = "RadoGames";
            const int main_menu_t_font_s = 100;
            const int main_menu_t_width = MeasureText(main_menu_t, main_menu_t_font_s);
            DrawText(main_menu_t, (width - main_menu_t_width) / 2, 50, main_menu_t_font_s, PINK);

            //temp
            DrawText("Press 1-4 to choose a game.", 250, 200, 50, WHITE);
            DrawText("Snake - 1", 250, 260, 30, WHITE);
            DrawText("TiktakToe - 2", 250, 320, 30, WHITE);
            DrawText("Minesweeper - 3", 250, 380, 30, WHITE);
            DrawText("Hangman - 4", 250, 440, 30, WHITE);

            // change games
            if (IsKeyPressed(KEY_ONE)) {
                current_menu = Snake_game;
            }
            if (IsKeyPressed(KEY_TWO)) {
                current_menu = TikTakToe;
            }
            if (IsKeyPressed(KEY_THREE)) {
                current_menu = Minesweeper;
            }
            if (IsKeyPressed(KEY_FOUR)) {
                current_menu = Hangman;
            }

            break;
        case Snake_game:

            const char* snake_menu_t = "Snake";
            const int snake_menu_t_font_s = 100;
            const int snake_menu_t_width = MeasureText(snake_menu_t, snake_menu_t_font_s);
            DrawText(snake_menu_t, (width - snake_menu_t_width) / 2, 50, snake_menu_t_font_s, YELLOW);

            break;
        case TikTakToe:

            const char* TTT_menu_t = "TikTakToe";
            const int TTT_menu_t_font_s = 100;
            const int TTT_menu_t_width = MeasureText(TTT_menu_t, TTT_menu_t_font_s);
            DrawText(TTT_menu_t, (width - TTT_menu_t_width) / 2, 50, TTT_menu_t_font_s, RED);

            break;
        case Minesweeper:

            const char* MS_menu_t = "MineSweeper";
            const int MS_menu_t_font_s = 100;
            const int MS_menu_t_width = MeasureText(MS_menu_t, MS_menu_t_font_s);
            DrawText(MS_menu_t, (width - MS_menu_t_width) / 2, 50, MS_menu_t_font_s, GREEN);

            break;
        case Hangman:

            const char* HM_menu_t = "Hangman";
            const int HM_menu_t_font_s = 100;
            const int HM_menu_t_width = MeasureText(HM_menu_t, HM_menu_t_font_s);
            DrawText(HM_menu_t, (width - HM_menu_t_width) / 2, 50, HM_menu_t_font_s, BLUE);

            break;
        }

    if (IsKeyPressed(KEY_Q) && current_menu != Main_menu) {
        current_menu = Main_menu;
    }

    EndDrawing();
    }


    CloseWindow();

    return 0;
}