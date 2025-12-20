#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

#include "sprite.h"

inline Font menu_font;

inline Texture2D wall_texture;
inline Texture2D void_texture;
inline Texture2D paddle_texture;
inline Texture2D block_texture;
inline Texture2D background_texture;
inline Texture2D metal_texture;
inline Texture2D cracked_metal_texture;
inline Texture2D heart_texture;

inline sprite ball_sprite;

inline Texture2D menu_background;
inline Texture2D win_screen_bg;
inline Texture2D lose_screen_bg;
inline Texture2D pause_screen_bg;

inline Sound win_sound;
inline Sound lose_sound;
inline Sound game_sound;
inline Sound paddle_sound;
inline Sound block_sound;
inline Sound wall_sound;
inline Sound metal_sound;

inline Music game_music;

void load_fonts();
void unload_fonts();

void load_textures();
void unload_textures();

void load_sounds();
void unload_sounds();

#endif // ASSETS_H
