#include "mbed.h"

Serial pc (SERIAL_TX, SERIAL_RX);

int period = 50; // Period @ 20KHZ

// TIM1
// PA8
// PA9
/**
DigitalIn top_limit(PB_3, PullUp); // Top Limit Switch
DigitalIn bottom_limit(PB_4, PullUp); // Bottom Limit Switch (Prime)
DigitalIn start_stop(PB_5, PullUp); // Emergency Stop (Should be paired to relay/trans out)

// LPD3806 600BM G5 24C
// Red...5-24VDC...
// Black...Ground...
// Green...A Phase...
// White...B Phase...
// Must be pulled up

// Pololu Dual VNH5019 Motor Driver Shield for Arduino
// Driver 1
DigitalOut M1INA(PC_8); // Clockwise Input
DigitalOut M1INB(PC_6); // Counter-Clockwise Input
DigitalIn M1EN_DIAG(PC_5); // High = Half-Bridge A & B Enabled, Low = Half-Bridge A & B Disabled (Combination of VHN5019 ENA/DIAGA & ENB/DIAGA Pins)
PwmOut M1PWM(PC_9); // PWM Input, 20KHZ Max, Low = Off, High = On, Dependent On xINA/B Pins
AnalogIn M1CS(PB_8); // Output Of Current Sense Proportional To Motor Current If CS_DIS Is Low Or Open (CD_DIS Not Connected)

https://www.st.com/content/ccc/resource/technical/document/user_manual/98/2e/fa/4b/e0/82/43/b7/DM00105823.pdf/files/DM00105823.pdf/jcr:content/translations/en.DM00105823.pdf
**/

int main() {

    int i = 1; // Keep loop running unless shit goes south
    /**
    // Turn off direction inputs
    M1INA.write(0);
    M1INB.write(0);

    // Setup PWM
    M1PWM.period_us(50); // Set to 20KHZ
    M1PWM.write(0.2f); // Set Duty Cycle to 20%
    **/
    while(i) {
        /**
        M1INA = top_limit;
        M1INB = bottom_limit;

        pc.printf("top: %d, bottom: %d, other: %d, current: %f\n", top_limit, bottom_limit, start_stop, M1CS);
        **/
        pc.printf("tick...\n");
        wait_us(period); // Period
    }
}
