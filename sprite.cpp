// sprite.cpp
#include "sprite.h"
#include "raylib.h"
#include <string>
#include <iostream>

sprite load_sprite(const char* path_prefix, const char* extension, size_t frame_count, bool loop, size_t frames_to_skip)
{
    sprite s;
    s.frame_count = frame_count;
    s.frames = new Texture2D[frame_count];
    s.loop = loop;
    s.frames_to_skip = frames_to_skip;
    s.frame_index = 0;
    s.frames_skipped = 0;
    s.prev_game_frame = 0;

    for (size_t i = 0; i < frame_count; i++) {
        std::string path = std::string(path_prefix) + "_" + std::to_string(i) + extension;

        if (FileExists(path.c_str())) {
            s.frames[i] = LoadTexture(path.c_str());
        } else {
            std::cout << "Warning: " << path << " not found, creating placeholder\n";
            // Create a simple colored placeholder
            Image img = GenImageColor(32, 32, Color{200, 100, 100, 255});
            s.frames[i] = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    return s;
}

void unload_sprite(sprite& s)
{
    if (s.frames) {
        for (size_t i = 0; i < s.frame_count; i++) {
            UnloadTexture(s.frames[i]);
        }
        delete[] s.frames;
        s.frames = nullptr;
    }
}
