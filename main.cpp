#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <strings.h>
#include <pico/bootrom.h>
#include <hardware/watchdog.h>

#include "pico/stdlib.h"

//const uint LED_PIN = PICO_DEFAULT_LED_PIN;
char* readBuf;
uint8_t readIndex;

static repeating_timer_t updateTimer;

bool power = true;
bool on;

void handleCommand(char *buf, uint8_t length) {
    if (strncasecmp(buf, "flash", length) == 0) {
        printf("Entering Bootloader\n");
        reset_usb_boot(0, 0);
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

    readBuf = (char*)malloc(100 * sizeof(char));
    readIndex = 0;

/*    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);*/

    uint8_t column, row;
    column = 0;
    row = 0;
    bool clear = false;

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
