// sprite.h
#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"
#include <cstddef>

struct sprite {
    Texture2D* frames = nullptr;
    size_t frame_count = 0;
    size_t frame_index = 0;
    size_t frames_to_skip = 0;
    size_t frames_skipped = 0;
    size_t prev_game_frame = 0;
    bool loop = true;
};

sprite load_sprite(const char* path_prefix, const char* extension, size_t frame_count, bool loop, size_t frames_to_skip);
void unload_sprite(sprite& s);

#endif // SPRITE_H