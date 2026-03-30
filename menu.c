#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MENU_WIDTH  1200
#define MENU_HEIGHT 1000

// forward declarations
void run_snake(void);
void run_TTT(void);
void run_minesweeper(void);

// game files included here
#include "snake.c"
#include "TTT.c"
#include "minesweeper.c"

Font main_menu_font;

int main(void) {

    InitWindow(MENU_WIDTH, MENU_HEIGHT, "RadoGames");
    SetTargetFPS(60);
    InitAudioDevice();
    srand(time(NULL));

    main_menu_font = LoadFontEx("04b03.ttf", 100, NULL, 0);

    while (!WindowShouldClose()) {

        // restore menu window size after returning from a game
        if (GetScreenWidth() != MENU_WIDTH || GetScreenHeight() != MENU_HEIGHT)
            SetWindowSize(MENU_WIDTH, MENU_HEIGHT);
        SetWindowTitle("RadoGames");

        BeginDrawing();

        ClearBackground(BLACK);

        // Display and choose games
        const char* main_menu_t = "RadoGames";
        const int main_menu_t_font_s = 100;
        const int main_menu_t_width = MeasureText(main_menu_t, main_menu_t_font_s);
        DrawText(main_menu_t, (MENU_WIDTH - main_menu_t_width) / 2, 50, main_menu_t_font_s, PINK);


        DrawText("Press 1-3 to choose a game.", 250, 200, 50, WHITE);
        DrawText("Snake - 1", 250, 260, 30, WHITE);
        DrawText("TiktakToe - 2", 250, 320, 30, WHITE);
        DrawText("Minesweeper - 3", 250, 380, 30, WHITE);

        DrawText("Press Q in any game menu to return here.", 180, 550, 28, GRAY);

        EndDrawing();

        // launch games, returns when player presses Q
        if (IsKeyPressed(KEY_ONE)) {
            run_snake();
        } else if (IsKeyPressed(KEY_TWO)) {
            run_TTT();
        } else if (IsKeyPressed(KEY_THREE)) {
            run_minesweeper();
        }
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
