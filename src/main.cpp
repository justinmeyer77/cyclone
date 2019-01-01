#include "mbed.h"

Serial pc (SERIAL_TX, SERIAL_RX);

int period = 50; // Period @ 20KHZ

// TIM1
// PA8
// PA9

DigitalIn a_input(PC_8, PullUp);
DigitalIn b_input(PC_6, PullUp);
DigitalIn c_input(PC_5, PullUp);


DigitalIn M1EN(PC_10); // High = Half-Bridge A & B Enabled, Low = Half-Bridge A & B Disabled (Combination of VHN5019 ENA/DIAGA & ENB/DIAGA Pins)
DigitalOut M1INA(PC_12); // Clockwise Input
DigitalOut M1INB(PC_11); // Counter-Clockwise Input
PwmOut M1PWM(D9); // PWM Input, 20KHZ Max, Low = Off, High = On, Dependent On xINA/B Pins (PC_7?)
AnalogIn M1CS(PC_0); // Output Of Current Sense Proportional To Motor Current If CS_DIS Is Low Or Open (CD_DIS Not Connected)

// LPD3806 600BM G5 24C
// Red...5-24VDC...
// Black...Ground...
// Green...A Phase...
// White...B Phase...
// Must be pulled up

int main() {

    int i = 1; // Keep loop running unless shit goes south

    // Turn off direction inputs
    M1INA.write(0);
    M1INB.write(0);

    // Setup PWM
    M1PWM.period_us(50); // Set to 20KHZ
    M1PWM.write(0.05f); // Set Duty Cycle to 20%

    while(i) {
        M1INA = !a_input;
        M1INB = !b_input;
        pc.printf("a: %d, b: %d, c: %d, current: %f\n", a_input.read(), b_input.read(), c_input.read(), M1CS.read());
        wait_us(100000); // Period
    }
}
