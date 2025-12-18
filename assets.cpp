#include "assets.h"

#include "raylib.h"


void load_fonts()
{
    menu_font = LoadFontEx("data/fonts/Daydream.otf", 256, nullptr, 0);
}

void unload_fonts()
{
    UnloadFont(menu_font);
}

void load_textures()
{
    wall_texture = LoadTexture("data/images/block.png");
    void_texture = LoadTexture("data/images/void.png");
    block_texture = LoadTexture("data/images/img.png");
    paddle_texture = LoadTexture("data/images/paddle.png");
    background_texture = LoadTexture("data/images/background.png");
    ball_sprite = load_sprite("data/images/ball/ball", ".gif", 2, true, 10);
}

void unload_textures()
{
    UnloadTexture(wall_texture);
    UnloadTexture(void_texture);
    UnloadTexture(block_texture);
    UnloadTexture(paddle_texture);
    UnloadTexture(background_texture);
    unload_sprite(ball_sprite);
}

void load_sounds()
{
    InitAudioDevice();
    win_sound = LoadSound("data/sounds/win.wav");
    lose_sound = LoadSound("data/sounds/lose.wav");

    if (FileExists("data/sounds/candy.wav")) {
        game_music = LoadMusicStream("data/sounds/candy.wav");
        SetMusicVolume(game_music, 0.6f);
    }
}

void unload_sounds()
{
    UnloadSound(win_sound);
    UnloadSound(lose_sound);
    if (game_music.stream.buffer != nullptr)
        UnloadMusicStream(game_music);
    CloseAudioDevice();
}
