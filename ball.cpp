#include "ball.h"
#include "assets.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

#include <cmath>
#include <numbers>

void spawn_ball()
{
    for (int column = 0; column < current_level.columns; column++) {
        for (int row = 0; row < current_level.rows; row++) {
            if (get_level_cell(row, column) == BALL) {
                set_level_cell(row, column, VOID);
                ball_pos = { static_cast<float>(column), static_cast<float>(row) };
                constexpr float ball_launch_angle_radians = ball_launch_angle_degrees * (std::numbers::pi_v<float> / 180.0f);
                ball_vel.y = -ball_launch_vel_mag * std::sin(ball_launch_angle_radians);
                ball_vel.x = (rand() % 2 == 0) ? ball_launch_vel_mag * std::cos(ball_launch_angle_radians) : -ball_launch_vel_mag * std::cos(ball_launch_angle_radians);
                goto outer_loop_end;
            }
        }
    }
outer_loop_end:;
}

void move_ball()
{
    Vector2 next_ball_pos = {
        ball_pos.x + ball_vel.x,
        ball_pos.y + ball_vel.y
    };

    if (is_colliding_with_level_cell(next_ball_pos, ball_size, WALL)) {
        if (is_colliding_with_level_cell({ next_ball_pos.x, ball_pos.y }, ball_size, WALL)) {
            ball_vel.x = -ball_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball_pos.x, next_ball_pos.y }, ball_size, WALL)) {
            ball_vel.y = -ball_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }
        PlaySound(wall_sound);
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, METAL)) {
        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, METAL);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball_pos.y }, ball_size, METAL)) {
            ball_vel.x = -ball_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball_pos.x, next_ball_pos.y }, ball_size, METAL)) {
            ball_vel.y = -ball_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = CRACKED_METAL;
        PlaySound(metal_sound);
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, CRACKED_METAL)) {
        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, CRACKED_METAL);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball_pos.y }, ball_size, CRACKED_METAL)) {
            ball_vel.x = -ball_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball_pos.x, next_ball_pos.y }, ball_size, CRACKED_METAL)) {
            ball_vel.y = -ball_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = VOID;
        --current_level_blocks;
        PlaySound(metal_sound);
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, BLOCKS)) {
        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, BLOCKS);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball_pos.y }, ball_size, BLOCKS)) {
            ball_vel.x = -ball_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball_pos.x, next_ball_pos.y }, ball_size, BLOCKS)) {
            ball_vel.y = -ball_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = VOID;
        --current_level_blocks;
        PlaySound(block_sound);
    } else if (is_colliding_with_paddle(next_ball_pos, ball_size)) {
        ball_vel.y = -std::abs(ball_vel.y);
        PlaySound(paddle_sound);
    }

    ball_pos = next_ball_pos;
}

void spawn_second_ball()
{
    ball2_pos = ball_pos;
    constexpr float ball_launch_angle_radians = ball_launch_angle_degrees * (std::numbers::pi_v<float> / 180.0f);
    ball2_vel.y = -ball_launch_vel_mag * std::sin(ball_launch_angle_radians);
    ball2_vel.x = -ball_launch_vel_mag * std::cos(ball_launch_angle_radians);
    ball2_active = true;
}

void move_second_ball()
{
    if (!ball2_active)
        return;

    Vector2 next_ball_pos = {
        ball2_pos.x + ball2_vel.x,
        ball2_pos.y + ball2_vel.y
    };

    if (is_colliding_with_level_cell(next_ball_pos, ball_size, WALL)) {
        if (is_colliding_with_level_cell({ next_ball_pos.x, ball2_pos.y }, ball_size, WALL)) {
            ball2_vel.x = -ball2_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball2_pos.x, next_ball_pos.y }, ball_size, WALL)) {
            ball2_vel.y = -ball2_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }
        PlaySound(wall_sound);
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, METAL)) {
        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, METAL);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball2_pos.y }, ball_size, METAL)) {
            ball2_vel.x = -ball2_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball2_pos.x, next_ball_pos.y }, ball_size, METAL)) {
            ball2_vel.y = -ball2_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = CRACKED_METAL;
        PlaySound(metal_sound);
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, CRACKED_METAL)) {
        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, CRACKED_METAL);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball2_pos.y }, ball_size, CRACKED_METAL)) {
            ball2_vel.x = -ball2_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball2_pos.x, next_ball_pos.y }, ball_size, CRACKED_METAL)) {
            ball2_vel.y = -ball2_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = VOID;
        --current_level_blocks;
        PlaySound(metal_sound);
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, BLOCKS)) {
        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, BLOCKS);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball2_pos.y }, ball_size, BLOCKS)) {
            ball2_vel.x = -ball2_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball2_pos.x, next_ball_pos.y }, ball_size, BLOCKS)) {
            ball2_vel.y = -ball2_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = VOID;
        --current_level_blocks;
        PlaySound(block_sound);
    } else if (is_colliding_with_paddle(next_ball_pos, ball_size)) {
        ball2_vel.y = -std::abs(ball2_vel.y);
        PlaySound(paddle_sound);
    }

    ball2_pos = next_ball_pos;
}

bool is_ball2_inside_level()
{
    return is_inside_level(static_cast<int>(ball2_pos.y), static_cast<int>(ball2_pos.x));
}

bool is_ball_inside_level()
{
    return is_inside_level(static_cast<int>(ball_pos.y), static_cast<int>(ball_pos.x));
}