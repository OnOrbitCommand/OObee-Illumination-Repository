//
// Created by Brendan Ragan on 14/9/2024.
//

#ifndef ONORBITPICO_SIDELEDS_H
#define ONORBITPICO_SIDELEDS_H

#include <cstdlib>
#include <cstdint>

#include "Adafruit_NeoPixel.h"

class Side_LEDs {
public:
    explicit Side_LEDs(uint8_t data_pin, uint8_t Num_Pixels);
    void StartCycle(uint8_t r, uint8_t g, uint8_t b, uint8_t start_left, uint8_t start_right, uint8_t width, uint8_t ledPerSecond);
    bool Update(); //this should only be called by instances. one day this should be private.
    void Clear();

private:
    Adafruit_NeoPixel *raw_pixels;
    uint8_t num_Pixels;

    bool cycling;
    uint8_t cycle_pos_side_1;
    uint8_t cycle_pos_side_2;
    uint32_t color;
    uint8_t width;
};


#endif //ONORBITPICO_SIDELEDS_H
