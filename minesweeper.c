#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 600

#define MAX_COLS 50
#define MAX_ROWS 50

int COLS       = 12;
int ROWS       = 12;
int TILE_WIDTH  = 50;
int TILE_HEIGHT = 50;

typedef struct {
    int x;
    int y;
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int nearbyMineCount;
} sTile;

sTile grid[MAX_COLS][MAX_ROWS];
int revealedTilesCount;
int minesPresentCount;

float timeGameStarted;
float timeGameEnded;
float timeAttackRemaining;

#define TIME_ATTACK_START      5.0f
#define TIME_ATTACK_TILE_BONUS  0.5f

Camera2D camera;

const char* labelGameWin  = "YOU WIN";
const char* labelGameLose = "GAME OVER";
const char* labelEnter    = "PRESS ENTER FOR MAIN MENU";

#define MAX_TEXTURES 1
typedef enum { TEXTURE_FLAG = 0 } texture_asset;
Texture2D textures[MAX_TEXTURES];

#define MAX_SOUNDS 3
typedef enum { SOUND_ONE = 0, SOUND_TWO, SOUND_THREE } sound_asset;
Sound sounds[MAX_SOUNDS];

#define MAX_MUSIC 1
typedef enum { MUSIC_ONE = 0 } music_asset;
Music music[MAX_MUSIC];

typedef enum {
    MODE_CLASSIC = 0,
    MODE_TIME_ATTACK,
    MODE_INFINITE
} game_mode;

game_mode currentMode;
bool modDoubleMines;
bool modBiggerBoard;

typedef enum {
    STATE_MAIN_MENU = 0,
    STATE_MODE_SELECT,
    STATE_PLAYING,
    STATE_LOSE,
    STATE_WIN
} game_states;

game_states gameState;

void GameStartup();
void GameUpdate();
void GameShutdown();
void GameRender();
void GameReset();
void GamePlaySound(int sound);

void ResetTiles();
void RenderTiles();
void RenderTile(sTile);
int  CountNearbyMines(int, int);
bool IsTileIndexValid(int, int);
void RevealTile(int, int);
void FlagTile(int, int);
void RevealTilesFrom(int, int);

void GamePlaySound(int sound) {
    PlaySound(sounds[sound]);
}

bool IsTileIndexValid(int col, int row) {
    return col >= 0 && col < COLS && row >= 0 && row < ROWS;
}

void RevealTilesFrom(int col, int row) {
    for (int colOffset = -1; colOffset <= 1; colOffset++) {
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
            if (colOffset == 0 && rowOffset == 0) continue;
            if (!IsTileIndexValid(col + colOffset, row + rowOffset)) continue;
            RevealTile(col + colOffset, row + rowOffset);
        }
    }
}

void FlagTile(int col, int row) {
    if (grid[col][row].isRevealed) return;
    grid[col][row].isFlagged = !grid[col][row].isFlagged;
    GamePlaySound(SOUND_THREE);
}

void RevealTile(int col, int row) {
    if (grid[col][row].isFlagged || grid[col][row].isRevealed) return;
    grid[col][row].isRevealed = true;

    if (grid[col][row].isMine) {
        gameState = STATE_LOSE;
        timeGameEnded = (float)GetTime();
        GamePlaySound(SOUND_TWO);
    } else {
        if (grid[col][row].nearbyMineCount == 0) {
            RevealTilesFrom(col, row);
        }
        revealedTilesCount++;

        if (currentMode == MODE_TIME_ATTACK) {
            timeAttackRemaining += TIME_ATTACK_TILE_BONUS;
        }

        if (revealedTilesCount >= (ROWS * COLS - minesPresentCount)) {
            gameState = STATE_WIN;
            timeGameEnded = (float)GetTime();
        }
    }
}

int CountNearbyMines(int col, int row) {
    int count = 0;
    for (int colOffset = -1; colOffset <= 1; colOffset++) {
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
            if (colOffset == 0 && rowOffset == 0) continue;
            if (!IsTileIndexValid(col + colOffset, row + rowOffset)) continue;
            if (grid[col + colOffset][row + rowOffset].isMine) count++;
        }
    }
    return count;
}

void ResetTiles() {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            grid[i][j] = (sTile){
                .x = i, .y = j,
                .isMine = false,
                .isRevealed = false,
                .isFlagged = false,
                .nearbyMineCount = -1
            };
        }
    }

    float mineRatio = (modDoubleMines && currentMode != MODE_INFINITE) ? 0.2f : 0.1f;
    minesPresentCount = (int)(ROWS * COLS * mineRatio);

    int minesToPlace = minesPresentCount;
    while (minesToPlace > 0) {
        int col = GetRandomValue(0, COLS - 1);
        int row = GetRandomValue(0, ROWS - 1);
        if (!grid[col][row].isMine) {
            grid[col][row].isMine = true;
            minesToPlace--;
        }
    }

    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (!grid[i][j].isMine) {
                grid[i][j].nearbyMineCount = CountNearbyMines(i, j);
            }
        }
    }
}

void RenderTile(sTile tile) {
    int screenX = tile.x * TILE_WIDTH;
    int screenY = tile.y * TILE_HEIGHT;

    if (tile.isRevealed) {
        if (tile.isMine) {
            DrawRectangle(screenX, screenY, TILE_WIDTH, TILE_HEIGHT, RED);
        } else {
            DrawRectangle(screenX, screenY, TILE_WIDTH, TILE_HEIGHT, LIGHTGRAY);
            if (tile.nearbyMineCount > 0) {
                DrawText(TextFormat("%d", tile.nearbyMineCount),
                         screenX + TILE_WIDTH / 4, screenY + 4,
                         TILE_HEIGHT - 8, DARKGRAY);
            }
        }
    } else if (tile.isFlagged) {
        Rectangle source = { 0, 0, (float)textures[TEXTURE_FLAG].width, (float)textures[TEXTURE_FLAG].height };
        Rectangle dest   = { (float)screenX, (float)screenY, (float)TILE_WIDTH, (float)TILE_HEIGHT };
        DrawTexturePro(textures[TEXTURE_FLAG], source, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    DrawRectangleLines(screenX, screenY, TILE_WIDTH, TILE_HEIGHT, WHITE);
}

void RenderTiles() {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            RenderTile(grid[i][j]);
        }
    }
}

void GameStartup() {
    ChangeDirectory(GetApplicationDirectory());

    // InitAudioDevice() removed - menu.c owns the audio device
    SetMasterVolume(1.0f);

    Image image1 = LoadImage("assets/flag.png");
    textures[TEXTURE_FLAG] = LoadTextureFromImage(image1);
    UnloadImage(image1);

    sounds[SOUND_ONE]   = LoadSound("assets/click.wav");
    sounds[SOUND_TWO]   = LoadSound("assets/explosion.wav");
    sounds[SOUND_THREE] = LoadSound("assets/pickupCoin.wav");

    
    music[MUSIC_ONE].looping = true;
    PlayMusicStream(music[MUSIC_ONE]);

    currentMode    = MODE_CLASSIC;
    modDoubleMines = false;
    modBiggerBoard = false;

    gameState = STATE_MAIN_MENU;
}

void GameUpdate() {
    UpdateMusicStream(music[MUSIC_ONE]);

    switch (gameState) {
    case STATE_MAIN_MENU:
        if (IsKeyPressed(KEY_N)) {
            GamePlaySound(SOUND_ONE);
            gameState = STATE_MODE_SELECT;
        }
        break;

    case STATE_MODE_SELECT:
        if (IsKeyPressed(KEY_ONE))   currentMode = MODE_CLASSIC;
        if (IsKeyPressed(KEY_TWO))   currentMode = MODE_TIME_ATTACK;
        if (IsKeyPressed(KEY_THREE)) currentMode = MODE_INFINITE;

        if (currentMode != MODE_INFINITE) {
            if (IsKeyPressed(KEY_D)) modDoubleMines = !modDoubleMines;
            if (IsKeyPressed(KEY_B)) modBiggerBoard = !modBiggerBoard;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            GamePlaySound(SOUND_ONE);
            GameReset();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            gameState = STATE_MAIN_MENU;
        }
        break;

    case STATE_PLAYING:
        if (currentMode == MODE_TIME_ATTACK) {
            timeAttackRemaining -= GetFrameTime();
            if (timeAttackRemaining <= 0.0f) {
                timeAttackRemaining = 0.0f;
                timeGameEnded = (float)GetTime();
                gameState = STATE_LOSE;
                GamePlaySound(SOUND_TWO);
            }
        }

        if (currentMode == MODE_INFINITE) {
            float camSpeed = 300.0f;
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    camera.target.y -= camSpeed * GetFrameTime();
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  camera.target.y += camSpeed * GetFrameTime();
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  camera.target.x -= camSpeed * GetFrameTime();
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += camSpeed * GetFrameTime();

            camera.target.x = Clamp(camera.target.x, SCREEN_WIDTH  / 2.0f, COLS * TILE_WIDTH  - SCREEN_WIDTH  / 2.0f);
            camera.target.y = Clamp(camera.target.y, SCREEN_HEIGHT / 2.0f, ROWS * TILE_HEIGHT - SCREEN_HEIGHT / 2.0f);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            int col, row;
            if (currentMode == MODE_INFINITE) {
                Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
                col = (int)(worldPos.x / TILE_WIDTH);
                row = (int)(worldPos.y / TILE_HEIGHT);
            } else {
                col = (int)(mousePos.x / TILE_WIDTH);
                row = (int)(mousePos.y / TILE_HEIGHT);
            }
            if (IsTileIndexValid(col, row)) {
                RevealTile(col, row);
                GamePlaySound(SOUND_ONE);
            }
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            Vector2 mousePos = GetMousePosition();
            int col, row;
            if (currentMode == MODE_INFINITE) {
                Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
                col = (int)(worldPos.x / TILE_WIDTH);
                row = (int)(worldPos.y / TILE_HEIGHT);
            } else {
                col = (int)(mousePos.x / TILE_WIDTH);
                row = (int)(mousePos.y / TILE_HEIGHT);
            }
            if (IsTileIndexValid(col, row)) {
                FlagTile(col, row);
            }
        }
        break;

    case STATE_LOSE:
    case STATE_WIN:
        if (IsKeyPressed(KEY_ENTER)) {
            GamePlaySound(SOUND_ONE);
            gameState = STATE_MAIN_MENU;
        }
        break;
    }
}

void GameRender() {
    switch (gameState) {
    case STATE_MAIN_MENU:
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DARKBLUE);
        DrawText("MAJNSVIPER", 20, 20, 40, WHITE);
        DrawText("[N]ova Hra", 120, 220, 20, WHITE);
        DrawText("ESC na VYPNUTIE", 120, 280, 20, WHITE);
        DrawText("Made by Metju a Miso", 120, 500, 20, WHITE);
        DrawText("[Q] Spat do RadoGames", 120, 540, 20, GRAY); // back to menu
        break;

    case STATE_MODE_SELECT:
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DARKBLUE);
        DrawText("MODIFIERS", SCREEN_WIDTH / 2 - MeasureText("MODIFIERS", 30) / 2, 40, 30, WHITE);

        DrawText("[1] Classic",       120, 130, 20, currentMode == MODE_CLASSIC     ? YELLOW : WHITE);
        DrawText("[2] Time Attack",    120, 160, 20, currentMode == MODE_TIME_ATTACK ? YELLOW : WHITE);
        DrawText("[3] Nekonecna mapa", 120, 190, 20, currentMode == MODE_INFINITE    ? YELLOW : WHITE);

        if (currentMode == MODE_TIME_ATTACK)
            DrawText("  Odhal dlazdice na zisk casu!", 140, 215, 14, WHITE);
        if (currentMode == MODE_INFINITE)
            DrawText("  Pohybuj sa pomocou WASD / sipiek", 140, 215, 14, WHITE);

        DrawText("MODIFIKATORY", 120, 270, 20, LIGHTGRAY);

        {
            Color modColor = (currentMode == MODE_INFINITE) ? DARKGRAY : WHITE;
            DrawText(TextFormat("[D] Dvojite miny:  %s", modDoubleMines ? "ZAP" : "VYP"), 120, 300, 20, modColor);
            DrawText(TextFormat("[B] Vacsia mapa:   %s", modBiggerBoard ? "ZAP" : "VYP"), 120, 330, 20, modColor);
            if (currentMode == MODE_INFINITE)
                DrawText("(modifikatory nie su dostupne pre tento mod)", 70, 360, 14, WHITE);
        }

        DrawText("[ENTER] Hrat", 120, 440, 20, GREEN);
        DrawText("[BACKSPACE] Spat",   120, 470, 20, WHITE);
        break;

    case STATE_PLAYING:
        if (currentMode == MODE_INFINITE) {
            BeginMode2D(camera);
            RenderTiles();
            EndMode2D();
        } else {
            RenderTiles();
        }

        if (currentMode == MODE_TIME_ATTACK) {
            Color timerColor = (timeAttackRemaining > 15.0f) ? WHITE : RED;
            DrawText(TextFormat("CAS: %.1f s", timeAttackRemaining), 10, 10, 24, timerColor);
        }
        break;

    case STATE_LOSE:
    case STATE_WIN: {
        if (currentMode == MODE_INFINITE) {
            BeginMode2D(camera);
            RenderTiles();
            EndMode2D();
        } else {
            RenderTiles();
        }

        const char* label = (gameState == STATE_WIN) ? labelGameWin : labelGameLose;
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(WHITE, 0.8f));
        DrawText(label, SCREEN_WIDTH / 2 - MeasureText(label, 60) / 2, SCREEN_HEIGHT / 2 - 10, 60, DARKGRAY);
        DrawText(labelEnter, SCREEN_WIDTH / 2 - MeasureText(labelEnter, 34) / 2, (int)(SCREEN_HEIGHT * 0.75f) - 10, 34, DARKGRAY);
        int seconds = (int)(timeGameEnded - timeGameStarted) % 60;
        DrawText(TextFormat("TIME PLAYED: %d s", seconds), 20, 40, 34, DARKGRAY);
        break;
    }
    }
}

void GameShutdown() {
    for (int i = 0; i < MAX_TEXTURES; i++) UnloadTexture(textures[i]);
    for (int i = 0; i < MAX_SOUNDS; i++)   UnloadSound(sounds[i]);
    StopMusicStream(music[MUSIC_ONE]);
    UnloadMusicStream(music[MUSIC_ONE]);
    // CloseAudioDevice() removed - menu.c owns the audio device
}

void GameReset() {
    if (currentMode == MODE_INFINITE) {
        COLS = 40;
        ROWS = 40;
        TILE_WIDTH  = 50;
        TILE_HEIGHT = 50;
        camera.target   = (Vector2){ COLS * TILE_WIDTH / 2.0f, ROWS * TILE_HEIGHT / 2.0f };
        camera.offset   = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom     = 1.0f;
    } else if (modBiggerBoard) {
        COLS = 20;
        ROWS = 20;
        TILE_WIDTH  = SCREEN_WIDTH  / COLS;
        TILE_HEIGHT = SCREEN_HEIGHT / ROWS;
    } else {
        COLS = 12;
        ROWS = 12;
        TILE_WIDTH  = SCREEN_WIDTH  / COLS;
        TILE_HEIGHT = SCREEN_HEIGHT / ROWS;
    }

    ResetTiles();

    revealedTilesCount = 0;
    timeGameStarted    = (float)GetTime();

    if (currentMode == MODE_TIME_ATTACK) {
        timeAttackRemaining = TIME_ATTACK_START;
    }

    gameState = STATE_PLAYING;
}

void run_minesweeper() {
    // resize window for minesweeper
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetWindowTitle("MINESWEEP");

    GameStartup();

    while (!WindowShouldClose()) {
        // Q on main menu = back to RadoGames
        if (gameState == STATE_MAIN_MENU && IsKeyPressed(KEY_Q)) {
            break;
        }
        GameUpdate();
        BeginDrawing();
        ClearBackground(DARKBLUE);
        GameRender();
        EndDrawing();
    }

    GameShutdown();
}
