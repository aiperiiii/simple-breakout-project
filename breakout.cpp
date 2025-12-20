#include "assets.h"
#include "ball.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

void update()
{
    if (game_music.stream.buffer != nullptr) {
        UpdateMusicStream(game_music);
    }

    switch (game_state) {
    case menu_state:
        if (IsKeyPressed(KEY_ENTER)) {
            load_level();
            PlayMusicStream(game_music);
            game_state = in_game_state;
            current_level_index = 0;
            lives = 5;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            game_state = instructions_state;
        }
        break;

    case instructions_state:
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = menu_state;
        }
        break;
    case in_game_state: {
        if (game_music.stream.buffer != nullptr && !IsMusicStreamPlaying(game_music)) {
            PlayMusicStream(game_music);
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = paused_state;
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            move_paddle(-paddle_speed);
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            move_paddle(paddle_speed);
        }

        move_ball();
        if (ball2_active) {
            move_second_ball();
        }

        bool ball1_out = !is_ball_inside_level();
        bool ball2_out = ball2_active && !is_ball2_inside_level();

        if (ball1_out && (!ball2_active || ball2_out)) {
            lives--;

            if (lives <= 0) {
                game_state = lose_state;
                current_level_index = 0;
                PlaySound(lose_sound);
            } else {
                load_level(0);
            }
        } else if (ball1_out && ball2_active && !ball2_out) {
            spawn_ball();
        } else if (ball2_out && ball2_active) {
            ball2_active = false;
        }

        if (current_level_blocks == 0) {
            if (current_level_index + 1 < level_count) {
                load_level(1);
                PlaySound(win_sound);
            } else {
                game_state = victory_state;
                init_victory_menu();
                PlaySound(win_sound);
            }
        }
        break;
    }

    case paused_state:
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = in_game_state;
            PlayMusicStream(game_music);
        }
        break;
    case victory_state:
        animate_victory_menu();

        if (IsKeyPressed(KEY_ENTER)) {
            game_state = menu_state;
            current_level_index = 0;
        }
        break;

    case lose_state:
        if (IsKeyPressed(KEY_ENTER)) {
            lives = 5;
            load_level();
            game_state = in_game_state;
        }
        if (game_music.stream.buffer != nullptr && !IsMusicStreamPlaying(game_music)) {
            PlayMusicStream(game_music);
        }
        break;

    default:;
    }
}

void draw()
{
    // TODO

    switch (game_state) {
    case menu_state:
        draw_menu();

        break;
    case in_game_state:
        draw_level();
        draw_paddle();
        draw_ball();
        draw_ui();
        draw_lives();
        break;
    case paused_state:
        draw_pause_menu();
        break;
    case victory_state:
        draw_victory_menu();
        break;

    case lose_state:
        draw_lose_menu();
        break;

    default:;
    }
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Breakout");
    SetTargetFPS(60);

    InitAudioDevice();

    load_fonts();
    load_textures();
    load_level();
    load_sounds();

    if (game_music.stream.buffer != nullptr) {
        PlayMusicStream(game_music);
    }

    while (!WindowShouldClose()) {
        if (game_music.stream.buffer != nullptr) {
            UpdateMusicStream(game_music);
        }
        BeginDrawing();
        SetExitKey(KEY_NULL);

        draw();
        update();

        EndDrawing();
    }
    CloseWindow();

    unload_sounds();
    unload_level();
    unload_textures();
    unload_fonts();

    return 0;
}