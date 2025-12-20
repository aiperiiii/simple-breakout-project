#include "graphics.h"

#include "assets.h"
#include "ball.h"
#include "game.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

#include <cmath>
#include <iostream>
#include <string>

struct Text {
    std::string str;
    Vector2 position = { 0.50f, 0.50f };
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = nullptr;
};

constexpr float cell_scale = 0.6f;
constexpr float screen_scale_divisor = 700.0f;

constexpr size_t victory_ball_launch_degree_offset = 3;
constexpr size_t victory_balls_count = 360 / victory_ball_launch_degree_offset;
constexpr float victory_balls_speed = 10.0f;
constexpr float victory_balls_size = 5.0f;

Vector2 screen_size;
float screen_scale;
float cell_size;
Vector2 shift_to_center;

Vector2 victory_balls_pos[victory_balls_count];
Vector2 victory_balls_vel[victory_balls_count];

size_t game_frame = 0;

void draw_image(const Texture2D& image, const float x, const float y, const float width, const float height)
{
    const Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    const Rectangle destination = { x, y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void draw_image(const Texture2D& image, const float x, const float y, const float size)
{
    draw_image(image, x, y, size, size);
}

void draw_sprite(sprite& sprite, const float x, const float y, const float width, const float height)
{
    draw_image(sprite.frames[sprite.frame_index], x, y, width, height);

    if (sprite.prev_game_frame == game_frame) {
        return;
    }
    if (sprite.frames_skipped < sprite.frames_to_skip) {
        ++sprite.frames_skipped;
    } else {
        sprite.frames_skipped = 0;

        ++sprite.frame_index;
        if (sprite.frame_index >= sprite.frame_count) {
            sprite.frame_index = sprite.loop ? 0 : sprite.frame_count - 1;
        }
    }
    sprite.prev_game_frame = game_frame;
}

void draw_sprite(sprite& sprite, const float x, const float y, const float size)
{
    draw_sprite(sprite, x, y, size, size);
}

void draw_text(const Text& text)
{
    const auto [x, y] = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    const Vector2 pos = {
        screen_size.x * text.position.x - 0.5f * x,
        screen_size.y * text.position.y - 0.5f * y
    };
    DrawTextEx(*text.font, text.str.c_str(), pos, y, text.spacing, text.color);
}

void derive_graphics_metrics()
{
    screen_size.x = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = std::min(screen_size.x / static_cast<float>(current_level.columns), screen_size.y / static_cast<float>(current_level.rows));
    screen_scale = std::min(screen_size.x, screen_size.y) / screen_scale_divisor;

    const float level_width = static_cast<float>(current_level.columns) * cell_size;
    const float level_height = static_cast<float>(current_level.rows) * cell_size;
    shift_to_center = {
        (screen_size.x - level_width) * 0.5f,
        (screen_size.y - level_height)
    };
}

void draw_menu()
{
    if (menu_background.id != 0) {
        DrawTexturePro(
            menu_background,
            Rectangle { 0, 0, (float)menu_background.width, (float)menu_background.height },
            Rectangle { 0, 0, screen_size.x, screen_size.y },
            Vector2 { 0, 0 },
            0.0f,
            WHITE);
    } else {
        ClearBackground(BLACK);
    }

    const Text game_title = {
        "Breakout",
        { 0.50f, 0.40f },
        200.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(game_title);

    const Text game_subtitle = {
        "Press Enter to Start",
        { 0.50f, 0.60f },
        38.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(game_subtitle);
}

void draw_ui()
{
    const Text level_counter = {
        "LEVEL " + std::to_string(current_level_index + 1) + " / " + std::to_string(level_count),
        { 0.5f, 0.0375f },
        48.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(level_counter);

    const Text boxes_remaining = {
        "BLOCKS " + std::to_string(current_level_blocks),
        { 0.5f, 0.9625f },
        48.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(boxes_remaining);
}

void draw_level()
{
    const float tile_width = background_texture.width;
    const float tile_height = background_texture.height;

    for (float y = 0; y < screen_size.y; y += tile_height) {
        for (float x = 0; x < screen_size.x; x += tile_width) {
            DrawTexture(background_texture, x, y, WHITE);
        }
    }

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            const char data = current_level.data[row * current_level.columns + column];
            const float texture_x_pos = shift_to_center.x + static_cast<float>(column) * cell_size;
            const float texture_y_pos = shift_to_center.y + static_cast<float>(row) * cell_size;

            switch (data) {
            case WALL:
                draw_image(wall_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case BLOCKS:
                draw_image(block_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case METAL:
                draw_image(metal_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case CRACKED_METAL:
                draw_image(cracked_metal_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            default:;
            }
        }
    }
}

void draw_paddle()
{
    Vector2 size = paddle_expanded ? paddle_size_expanded : paddle_size;
    const float texture_x_pos = shift_to_center.x + paddle_pos.x * cell_size;
    const float texture_y_pos = shift_to_center.y + paddle_pos.y * cell_size;
    draw_image(paddle_texture, texture_x_pos, texture_y_pos, size.x * cell_size, size.y * cell_size);
}

void draw_ball()
{
    const float texture_x_pos = shift_to_center.x + ball_pos.x * cell_size;
    const float texture_y_pos = shift_to_center.y + ball_pos.y * cell_size;
    draw_sprite(ball_sprite, texture_x_pos, texture_y_pos, cell_size);
    if (ball2_active) {
        const float texture_x_pos2 = shift_to_center.x + ball2_pos.x * cell_size;
        const float texture_y_pos2 = shift_to_center.y + ball2_pos.y * cell_size;
        draw_sprite(ball_sprite, texture_x_pos2, texture_y_pos2, cell_size);
    }
}

void draw_pause_menu()
{

    draw_image(pause_screen_bg, 0, 0, screen_size.x, screen_size.y);

    const Text paused_title = {
        "PAUSED",
        { 0.50f, 0.45f },
        150.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(paused_title);

    const Text paused_subtitle = {
        "Press Escape to Resume",
        { 0.50f, 0.60f },
        40.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(paused_subtitle);
}

void init_victory_menu()
{
    for (size_t i = 0; i < victory_balls_count; ++i) {
        victory_balls_pos[i] = { screen_size.x / 2, screen_size.y / 2 };
        victory_balls_vel[i] = {
            std::cos(static_cast<float>(i * victory_ball_launch_degree_offset)) * victory_balls_speed,
            std::sin(static_cast<float>(i * victory_ball_launch_degree_offset)) * victory_balls_speed
        };
    }
}

void animate_victory_menu()
{
    for (size_t i = 0; i < victory_balls_count; ++i) {
        if (victory_balls_pos[i].x + victory_balls_vel[i].x > screen_size.x || victory_balls_pos[i].x + victory_balls_vel[i].x < 0) {
            victory_balls_vel[i].x *= -1.0f;
        }
        if (victory_balls_pos[i].y + victory_balls_vel[i].y > screen_size.y || victory_balls_pos[i].y + victory_balls_vel[i].y < 0) {
            victory_balls_vel[i].y *= -1.0f;
        }
        victory_balls_pos[i] = {
            victory_balls_pos[i].x + victory_balls_vel[i].x,
            victory_balls_pos[i].y + victory_balls_vel[i].y
        };
    }
}

void draw_victory_menu()

{
    animate_victory_menu();

    draw_image(win_screen_bg, 0, 0, screen_size.x, screen_size.y);

    for (const auto& [x, y] : victory_balls_pos) {
        DrawCircleV({ x, y }, victory_balls_size, PINK);
    }

    const Text victory_title = {
        "You Won!",
        { 0.50f, 0.50f },
        100.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(victory_title);

    const Text victory_subtitle = {
        "Press Enter to Restart",
        { 0.50f, 0.65f },
        32.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(victory_subtitle);
}

void draw_lose_menu()
{
    draw_image(lose_screen_bg, 0, 0, screen_size.x, screen_size.y);

    const Text lose_title = {
        "You Lost!",
        { 0.50f, 0.45f },
        100.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(lose_title);

    const Text lose_subtitle = {
        "Press Enter to Try Again",
        { 0.50f, 0.60f },
        32.0f,
        PINK,
        4.0f,
        &menu_font
    };
    draw_text(lose_subtitle);
}
void draw_lives()
{
    const float heart_size = 25.0f;
    const float spacing = 40.0f;
    const float start_x = 30.0f;
    const float y_pos = 40.0f;

    for (int i = 0; i < lives; i++) {
        DrawTexture(heart_texture, start_x + i * spacing, y_pos, WHITE);
    }
}
