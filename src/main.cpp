#include "mbed.h"

Serial pc (SERIAL_TX, SERIAL_RX);

int period = 50; // Period @ 20KHZ

// TIM1
// PA8
// PA9

InterruptIn a_input(PC_8, PullUp);
InterruptIn b_input(PC_6, PullUp);
InterruptIn c_input(PC_5, PullUp);


DigitalIn M1EN(PC_10); // High = Half-Bridge A & B Enabled, Low = Half-Bridge A & B Disabled (Combination of VHN5019 ENA/DIAGA & ENB/DIAGA Pins)
DigitalOut M1INA(PC_12); // Clockwise Input
DigitalOut M1INB(PC_11); // Counter-Clockwise Input
PwmOut M1PWM(D9); // PWM Input, 20KHZ Max, Low = Off, High = On, Dependent On xINA/B Pins (PC_7?)
AnalogIn M1CS(PC_0); // Output Of Current Sense Proportional To Motor Current If CS_DIS Is Low Or Open (CD_DIS Not Connected)

// LPD3806 600BM G5 24C
// Red...5-24VDC...
// Black...Ground...
// Green...A Phase...PA_10
// White...B Phase...PB_3
// Must be pulled up
// 1" = 25352, 29910, 27635 calc 28800
// Pos = Up

InterruptIn encoder_a_phase(PA_10, PullUp);
DigitalIn encoder_b_phase(PB_3, PullUp);

int encoder_ticks = 0;

void encoder_tick() {
  if (encoder_b_phase.read()) {
    encoder_ticks--;
  }
  else {
    encoder_ticks++;
  }
}

void reset_ticks() {
  encoder_ticks = 0;
}

void a_input_rise() {
  M1INA = 0;
}

void a_input_fall() {
  M1INA = 1;
}

void b_input_rise() {
  M1INB = 0;
}

void b_input_fall() {
  M1INB = 1;
}

/**

Target Position -> Trajectory Generator -> Position Loop -> Velocity Limiter -> Velocity Loop -> Current Limiter -> Current Loop -> Drive -> Motor|Encoder|Home

Target Position
- Up/Down?

Trajectory Generator
- Max velocity
- Max acceleration
- Max deceleration
- Abort develeration

**/

int main() {

    int i = 1; // Keep loop running unless shit goes south

    encoder_a_phase.rise(&encoder_tick);
    c_input.fall(&reset_ticks);
    a_input.rise(&a_input_rise);
    a_input.fall(&a_input_fall);
    b_input.rise(&b_input_rise);
    b_input.fall(&b_input_fall);

    // Turn off direction inputs
    M1INA.write(0);
    M1INB.write(0);

    // Setup PWM
    M1PWM.period_us(50); // Set to 20KHZ
    M1PWM.write(0.25f); // Set Duty Cycle to 20%

    while(i) {
        pc.printf("a: %d, b: %d, c: %d, a-phase: %d, b-phase: %d, ticks: %d, current: %f\n", a_input.read(), b_input.read(), c_input.read(), encoder_a_phase.read(), encoder_b_phase.read(), encoder_ticks, M1CS.read());
        //wait_us(period); // Period
        wait(0.250);
    }
}
