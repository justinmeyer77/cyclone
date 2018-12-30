#include "mbed.h"

Serial pc (SERIAL_TX, SERIAL_RX);

int period = 50; // Period @ 20khz

int main() {

    int i = 1; // Keep loop running unless shit goes south

    while(i) {

        wait_us(period); // Period

    }
}
