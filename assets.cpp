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
    metal_texture = LoadTexture("data/images/metal_block.png");
    cracked_metal_texture = LoadTexture("data/images/cracked_metal.png");
    paddle_texture = LoadTexture("data/images/paddle.gif");
    background_texture = LoadTexture("data/images/background.png");
    ball_sprite = load_sprite("data/images/ball/ball", ".png", 2, true, 10);

    if (FileExists("data/images/metal_block.png")) {
        metal_texture = LoadTexture("data/images/metal_block.png");
    } else {
        Image img = GenImageColor(64, 64, Color{100, 100, 120, 255});
        metal_texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }
    if (FileExists("data/images/cracked_metal.png")) {
        cracked_metal_texture = LoadTexture("data/images/cracked_metal.png");
    } else {
        Image img = GenImageColor(64, 64, Color{80, 80, 100, 255});
        cracked_metal_texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    if (FileExists("data/images/heart.png")) {
        heart_texture = LoadTexture("data/images/heart.png");
    } else {
        Image img = GenImageColor(32, 32, Color{255, 50, 50, 255});
        heart_texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    if (FileExists("data/images/backgrounds/menu_background.png")) {
        menu_background = LoadTexture("data/images/backgrounds/menu_background.png");
    } else {
        Image img = GenImageColor(1280, 720, Color{10, 10, 30, 255});
        menu_background = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    if (FileExists("data/images/backgrounds/win_screen.png")) {
        win_screen_bg = LoadTexture("data/images/backgrounds/win_screen.png");
    } else {
        Image img = GenImageColor(1280, 720, Color{0, 50, 0, 200});
        win_screen_bg = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    if (FileExists("data/images/backgrounds/lose_screen.png")) {
        lose_screen_bg = LoadTexture("data/images/backgrounds/lose_screen.png");
    } else {
        Image img = GenImageColor(1280, 720, Color{50, 0, 0, 200});
        lose_screen_bg = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    if (FileExists("data/images/backgrounds/pause_screen.gif")) {
        pause_screen_bg = LoadTexture("data/images/backgrounds/pause_screen.gif");
    } else {
        Image img = GenImageColor(1280, 720, Color{20, 20, 20, 200});
        pause_screen_bg = LoadTextureFromImage(img);
        UnloadImage(img);
    }
}

void unload_textures()
{
    UnloadTexture(wall_texture);
    UnloadTexture(void_texture);
    UnloadTexture(block_texture);
    UnloadTexture(paddle_texture);
    UnloadTexture(background_texture);
    UnloadTexture(metal_texture);
    UnloadTexture(cracked_metal_texture);
    UnloadTexture(heart_texture);

    UnloadTexture(menu_background);
    UnloadTexture(win_screen_bg);
    UnloadTexture(lose_screen_bg);
    UnloadTexture(pause_screen_bg);

    unload_sprite(ball_sprite);
}
void load_sounds() {
        win_sound  = LoadSound("data/sounds/win.wav");
        lose_sound = LoadSound("data/sounds/lose.wav");

    if (FileExists("data/sounds/paddle.wav")) {
        paddle_sound = LoadSound("data/sounds/paddle.wav");
        SetSoundVolume(paddle_sound, 0.2f);
    }
    if (FileExists("data/sounds/block.wav")) {
        block_sound = LoadSound("data/sounds/block.wav");
        SetSoundVolume(block_sound, 0.2f);
    }
    if (FileExists("data/sounds/wall.wav")) {
        wall_sound = LoadSound("data/sounds/wall.wav");
        SetSoundVolume(wall_sound, 0.2f);
    }
    if (FileExists("data/sounds/metal.wav")) {
        metal_sound = LoadSound("data/sounds/metal.wav");
        SetSoundVolume(metal_sound, 0.2f);
    }

        game_music    = LoadMusicStream("data/sounds/candy.wav");

        SetMusicVolume(game_music, 0.6f);
    }

    void unload_sounds()
    {
        UnloadSound(win_sound);
        UnloadSound(lose_sound);
        UnloadSound(paddle_sound);
        UnloadSound(block_sound);
        UnloadSound(wall_sound);
        UnloadSound(metal_sound);

        if (game_music.stream.buffer != nullptr)
            UnloadMusicStream(game_music);

        CloseAudioDevice();
    }

