#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <strings.h>
#include <pico/bootrom.h>
#include <hardware/watchdog.h>

#include "Side_LEDs.h"

const uint8_t SIDE_LED_PIN = 15;

//const uint LED_PIN = PICO_DEFAULT_LED_PIN;
char* readBuf;
uint8_t readIndex;

bool power = true;
bool on;

Side_LEDs *side_leds;

void handleCommand(char *buf, uint8_t length) {
    if (strncasecmp(buf, "flash", length) == 0) {
        printf("Entering Bootloader\n");
        reset_usb_boot(0, 0);
        return;
    }

    if (strncasecmp(buf, "clear", length) == 0) {
        printf("Clearing Neopixels\n");
        side_leds->Clear();
        return;
    }

    if (strncasecmp(buf, "red", length) == 0) {
        printf("Setting Neopixels\n");
        side_leds->StartCycle(128, 0, 0, 0, 0, 2, 24);
        return;
    }

    if (strncasecmp(buf, "green", length) == 0) {
        printf("Setting Neopixels\n");
        side_leds->StartCycle(0, 128, 0, 0, 0, 2, 24);
        return;
    }

    if (strncasecmp(buf, "blue", length) == 0) {
        printf("Setting Neopixels\n");
        side_leds->StartCycle(0, 0, 128, 0, 0, 2, 24);
        return;
    }
}

int main() {
    stdio_usb_init();

    while(!stdio_usb_connected());

    sleep_ms(500);

    if (watchdog_enable_caused_reboot()) {
        printf("Entering Bootloader\n");
        stdio_flush();
        sleep_ms(1000);
        reset_usb_boot(0, 0);
        return 0;
    }

    watchdog_enable(5000, true);

    printf("Starting on pin %i...\n", SIDE_LED_PIN);
    stdio_flush();

    side_leds = new Side_LEDs(SIDE_LED_PIN);

    readBuf = (char*)malloc(100 * sizeof(char));
    readIndex = 0;

    while (true) {
        watchdog_update();
        int character = getchar_timeout_us(0);
        if (character != PICO_ERROR_TIMEOUT) {
            if (character == '\n' || character == '\r') {
                readBuf[readIndex] = '\0';
                printf("Command! '%s'\n", readBuf);
                handleCommand(readBuf, readIndex + 1);
                readIndex = 0;
            } else {
                readBuf[readIndex] = (uint8_t)character;
                if (readIndex < 100) {
                    readIndex++;
                } else {
                    readIndex = 0;
                }
            }
        }
    }
}
