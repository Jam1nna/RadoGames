#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 3
#define TILE_SIZE 200

typedef enum {
    O_player,
    X_player,
    None
} Player_Turn;

typedef enum {
    Menu,
    Playing
} Game_state;

typedef enum {
    Normal,
    Weird,
    Glitch
} Difficulty;

Game_state current_state = Menu;
Player_Turn current_player = X_player;
Difficulty chosen_diff;

float glitch_flash_timer = 0.0f;

const char* diff[3] = {"Normal", "Weird", "Glitch"};

char board[BOARD_SIZE][BOARD_SIZE];

Texture2D menu_background;
Texture2D play_background;

// ...
// Rotate 90˚
void rotateBoard(void) {
    char tmp[BOARD_SIZE][BOARD_SIZE];
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            tmp[x][BOARD_SIZE - 1 - y] = board[y][x];
    memcpy(board, tmp, sizeof(board));
    // strcpy(glitch_msg, "GLITCH: board rotated!");
}

// Flip one random filled cell to the opponent's symbol
void flipCell(void) {
    // collect filled cells
    int fy[9], fx[9], fc = 0;
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            if (board[y][x] != ' ') { fy[fc] = y; fx[fc] = x; fc++; }
    if (fc == 0) return;
    int pick = GetRandomValue(0, fc - 1);
    board[fy[pick]][fx[pick]] = (board[fy[pick]][fx[pick]] == 'X') ? 'O' : 'X';
    // strcpy(glitch_msg, "GLITCH: a cell flipped!");
}

// Swap two random cells
void swap2Cells(void) {
    int ay = GetRandomValue(0, BOARD_SIZE-1), ax = GetRandomValue(0, BOARD_SIZE-1);
    int by = GetRandomValue(0, BOARD_SIZE-1), bx = GetRandomValue(0, BOARD_SIZE-1);
    char tmp = board[ay][ax];
    board[ay][ax] = board[by][bx];
    board[by][bx] = tmp;
    // strcpy(glitch_msg, "GLITCH: cells swapped!");
}

void glitchChooser(Difficulty diff) {
    int chance = 0;
    if (diff == Weird) {
        chance = GetRandomValue(1, 200);
    } else if (diff == Glitch) {
        chance = GetRandomValue(1, 100);
    }
    if (chance == 10) {
        rotateBoard();
        glitch_flash_timer = 0.3f;
    } else if (chance <= 20) {
        flipCell();
        glitch_flash_timer = 0.3f;
    } else if (chance <= 30) {
        swap2Cells();
        glitch_flash_timer = 0.3f;
    }
}

void placeChar(int y, int x) {
    if (board[y][x] != ' ') return;
    if (current_player == X_player) {
        board[y][x] = 'X';
        current_player = O_player;
    } else if (current_player == O_player) {
        board[y][x] = 'O';
        current_player = X_player;
    }
    glitchChooser(chosen_diff);
}

void resetBoard() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = ' ';
        }
    }
}

int checkWin(char p) {
    // check Rows
    for (int y = 0; y < BOARD_SIZE; y++) {
        if (board[y][0] == p && board[y][1] == p && board[y][2] == p) {
            return 1;
        }
    }
    // check Cols
    for (int x = 0; x < BOARD_SIZE; x++) {
        if (board[0][x] == p && board[1][x] == p && board[2][x] == p) {
            return 1;
        }
    }
    // check Diagonal
    if ((board[0][0] == p && board[1][1] == p && board[2][2] == p) || (board[0][2] == p && board[1][1] == p && board[2][0] == p)) {
        return 1;
    }
    return 0;
}

int boardFull(void) {
    bool full = true;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == ' ') {
                full = false;
            }
        }
    }
    return full;
}

int main(void) {

    const int width = 1200;
    const int height = 1200;

    InitWindow(width, height, "TikTakToe");
    SetTargetFPS(60);
    srand(time(NULL));

    menu_background = LoadTexture("assets/TTT_menu_bg.jpg");
    play_background = LoadTexture("assets/TTT_bg.jpg");

    resetBoard();

    Vector2 mouse = {0, 0};
    while (!WindowShouldClose()) {

    	BeginDrawing();


    	switch (current_state) {

        case Menu:
        	// Draw BG
        	DrawTexturePro(menu_background,
                (Rectangle){0, 0, menu_background.width, menu_background.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);

        	// Text
        	// Header
        	const char* h_t = "TikTakToe";
        	const int h_fs = 125; // font size
        	DrawText(h_t, (width - MeasureText(h_t, h_fs)) / 2-8, 60-8, h_fs, VIOLET);
            DrawText(h_t, (width - MeasureText(h_t, h_fs)) / 2, 60, h_fs, PURPLE);

        	// Instructions
        	// const char* i_t = "Press ENTER to start";
        	// const int i_fs = 50;
        	// DrawText(i_t, (width - MeasureText(i_t, i_fs)) / 2, 350, i_fs, WHITE);
        	const char* i_t2 = "Choose difficulty:";
        	const int i_fs2 = 80;
        	DrawText(i_t2, (width - MeasureText(i_t2, i_fs2)) / 2, 380, i_fs2, MAROON);
            DrawText(i_t2, (width - MeasureText(i_t2, i_fs2)) / 2-5, 380-5, i_fs2, RED);
            const char* i_t3 = "1-Normal";
            const int i_fs3 = 50;
            DrawText(i_t3, (width - MeasureText(i_t3, i_fs3)) / 2-3, 470-3, i_fs3, RED);
            DrawText(i_t3, (width - MeasureText(i_t3, i_fs3)) / 2, 470, i_fs3, GOLD);
            const char* i_t4 = "2-Weird";
            const int i_fs4 = 50;
            DrawText(i_t4, (width - MeasureText(i_t4, i_fs4)) / 2-3, 540-3, i_fs4, DARKGREEN);
            DrawText(i_t4, (width - MeasureText(i_t4, i_fs4)) / 2, 540, i_fs4, GREEN);
            const char* i_t5 = "3-Glitch";
            const int i_fs5 = 50;
            DrawText(i_t5, (width - MeasureText(i_t5, i_fs5)) / 2-3, 610-3, i_fs5, RED);
            DrawText(i_t5, (width - MeasureText(i_t5, i_fs5)) / 2+3, 610+3, i_fs5, BLUE);
            DrawText(i_t5, (width - MeasureText(i_t5, i_fs5)) / 2, 610, i_fs5, WHITE);

            if (IsKeyPressed(KEY_ONE)) {
                current_state = Playing;
                chosen_diff = Normal;
                resetBoard();
            } else if (IsKeyPressed(KEY_TWO)) {
                current_state = Playing;
                chosen_diff = Weird;
                resetBoard();
            } else if (IsKeyPressed(KEY_THREE)) {
                current_state = Playing;
                chosen_diff = Glitch;
                resetBoard();
            }

        	break;
        case Playing:

            if (IsKeyPressed(KEY_R)) {
                current_state = Menu;
            }

        	// Draw BG
        	DrawTexturePro(play_background,
                (Rectangle){0, 0, menu_background.width, menu_background.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0}, 0.0f, WHITE);

        	mouse = GetMousePosition();

            // draw outlines
            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int x = 0; x < BOARD_SIZE; x++) {
                    Rectangle rect = {
                        GetScreenWidth() / 4 + x * TILE_SIZE,
                        GetScreenHeight() / 4 + y * TILE_SIZE,
                        TILE_SIZE,
                        TILE_SIZE
                        };

                    // a lil visual separation
                    DrawRectangleLinesEx(rect, 5.0f, WHITE);

                    // draw symbols
                    if (board[y][x] == 'X') {
                        int cx = rect.x + TILE_SIZE/2;
                        int cy = rect.y + TILE_SIZE/2;
                        DrawLineEx((Vector2){cx-50, cy-50}, (Vector2){cx+50, cy+50}, 10.0f, BLACK);
                        DrawLineEx((Vector2){cx+50, cy-50}, (Vector2){cx-50, cy+50}, 10.0f, BLACK);
                    } else if (board[y][x] == 'O') {
                        DrawRing((Vector2){rect.x + TILE_SIZE/2, rect.y + TILE_SIZE/2}, 42, 52, 0, 360, 36, BLACK);
                    }

                    // check click
                    if (!(checkWin('X') || checkWin('O') || boardFull()))
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, rect)) {
                        // draw X or O
                        placeChar(y, x);
                    }
                }
            }

            // reversed cuz y not
            if (checkWin('O')) {
                DrawText("X won", 400, 150, 150, YELLOW); //temp
                // current_state = X_Win;
            } else if (checkWin('X')) {
                DrawText("O won", 400, 150, 150, YELLOW); // temp
                // current_state = O_Win;
            } else if (boardFull()){
                DrawText("Draw => You are bad at this game!", 150, 200, 50, YELLOW); // temp
                // current_state = Draw;
            }

            switch (current_player) {

                case O_player:
                    DrawText("Turn => O", 50, 50, 50, PINK);
                    break;
                case X_player:
                    DrawText("Turn => X", 50, 50, 50, PINK);
                    break;
                case None:
                    DrawText("Turn => None", 50, 50, 50, GRAY);
                  break;
                }

            const int diff_fs = 50;
            const char* diff_t;
            // Only change glitch function. Game code outside!
            switch (chosen_diff) {

            case Normal:
                diff_t = diff[0];
                DrawText(diff_t, 50, 100, diff_fs, WHITE);
                break;
            case Weird:
                diff_t = diff[1];
                DrawText(diff_t, 50, 100, diff_fs, WHITE);
                break;
            case Glitch:
                diff_t = diff[2];
                DrawText(diff_t, 50, 100, diff_fs, WHITE);
              break;
            }

            // red flash overlay
            if (glitch_flash_timer > 0.0f) {
                float alpha = glitch_flash_timer / 0.25f * 0.45f;
                glitch_flash_timer -= GetFrameTime();
                DrawRectangle(0, 0, width, height, Fade(RED, alpha));
            }

          break;
        }

    	EndDrawing();
    }
    UnloadTexture(menu_background);
    CloseWindow();
}