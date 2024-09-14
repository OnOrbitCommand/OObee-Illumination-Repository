//
// Created by Brendan Ragan on 14/9/2024.
//

#include "Side_LEDs.h"

#include <cstdio>

const uint8_t Num_Pixels = 48;
const neoPixelType Pixel_Type = NEO_GRB + NEO_KHZ800;

Adafruit_NeoPixel predefined_pixels(Num_Pixels, 15, Pixel_Type);

static repeating_timer_t updateTimer;

bool update(repeating_timer_t* timer) {
    if (!timer->user_data) return false;
    Side_LEDs *instance = (Side_LEDs*)timer->user_data;
    return instance->Update();
}

Side_LEDs::Side_LEDs(uint8_t data_pin) {
    raw_pixels = &predefined_pixels;
    raw_pixels->begin();
}

void Side_LEDs::StartCycle(
        uint8_t r, uint8_t g, uint8_t b,
        uint8_t start_left, uint8_t start_right, uint8_t newWidth, uint8_t ledPerSecond) {
    cycling = true;
    if (start_left == 0 || start_left > (Num_Pixels >> 1) - 1) {
        cycle_pos_side_1 = (Num_Pixels >> 1) - 1;
    } else {
        cycle_pos_side_1 = start_left - 1;
    }

    if (start_right == 0 || start_right > (Num_Pixels >> 1) - 1) {
        cycle_pos_side_2 = (Num_Pixels >> 1) - 1;
    } else {
        cycle_pos_side_2 = start_right - 1;
    }

    color = r << 16 | g << 8 | b;
    width = newWidth;

    int64_t update_in_us = 1000000 / ledPerSecond;

    cancel_repeating_timer(&updateTimer);
    add_repeating_timer_us(update_in_us, &update, this, &updateTimer);
}

bool Side_LEDs::Update() {
    if (!cycling) return false;
    cycle_pos_side_1++; cycle_pos_side_2++;
    uint8_t side_width = (Num_Pixels >> 1);
    if (cycle_pos_side_1 >= side_width) cycle_pos_side_1 = 0;
    if (cycle_pos_side_2 >= side_width) cycle_pos_side_2 = 0;
    raw_pixels->clear();
    raw_pixels->setPixelColor(cycle_pos_side_1, color);
    raw_pixels->setPixelColor(cycle_pos_side_2 + side_width, color);

    float frac = 1.0f/(width + 1); // NOLINT(*-narrowing-conversions)
    for(int i = 0; i < width; i++) {
        float thisFrac = frac * (width - i); // NOLINT(*-narrowing-conversions)
        uint8_t r = Adafruit_NeoPixel::gamma8(((uint8_t)(color >> 16)) * thisFrac);  // NOLINT(*-narrowing-conversions)
        uint8_t g = Adafruit_NeoPixel::gamma8(((uint8_t)(color >> 8)) * thisFrac); // NOLINT(*-narrowing-conversions)
        uint8_t b = Adafruit_NeoPixel::gamma8(((uint8_t)(color)) * thisFrac); // NOLINT(*-narrowing-conversions)
        uint8_t position = cycle_pos_side_1 - (i + 1);
        if (position >= side_width) position = side_width - (256 - position);
        raw_pixels->setPixelColor(position, r, g, b);
        position = cycle_pos_side_1 + (i + 1);
        if (position >= side_width) position -= side_width;
        raw_pixels->setPixelColor(position, r, g, b);

        position = cycle_pos_side_2 - (i + 1);
        if (position >= side_width) position = side_width - (256 - position);
        raw_pixels->setPixelColor(position + side_width, r, g, b);
        position = cycle_pos_side_2 + (i + 1);
        if (position >= side_width) position -= side_width;
        raw_pixels->setPixelColor(position + side_width, r, g, b);
    }
    raw_pixels->show();
    return true;
}

void Side_LEDs::Clear() {
    raw_pixels->clear();
    raw_pixels->show();
}