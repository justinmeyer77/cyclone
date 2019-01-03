#include "mbed.h"

Serial pc (SERIAL_TX, SERIAL_RX);

int period = 100; // Period @ 10KHZ
int position_multiplier = 5; // 2KHZ
int spin_period = 10; // Wait for period to elapse (loop control & timing)

// TIM1
// PA8
// PA9

InterruptIn a_input(PC_8, PullUp); // Arbitrary input @ "A"
InterruptIn b_input(PC_6, PullUp); // Arbitrary input @ "B"
InterruptIn c_input(PC_5, PullUp); // Arbitrary input @ "C"

DigitalOut oscope(PC_4, 0); // Loop tuning output @ oscope

DigitalIn M1EN(PC_10); // High = Half-Bridge A & B Enabled, Low = Half-Bridge A & B Disabled (Combination of VHN5019 ENA/DIAGA & ENB/DIAGA Pins)
DigitalOut M1INA(PC_12, 0); // Clockwise Input
DigitalOut M1INB(PC_11, 0); // Counter-Clockwise Input
PwmOut M1PWM(D9); // PWM Input, 20KHZ Max, Low = Off, High = On, Dependent On xINA/B Pins (PC_7?)
AnalogIn M1CS(PC_0); // Output Of Current Sense Proportional To Motor Current If CS_DIS Is Low Or Open (CD_DIS Not Connected)

InterruptIn encoder_a_phase(PA_10, PullUp); // A phase @ encoder
DigitalIn encoder_b_phase(PB_3, PullUp); // B phase @ encoder

int encoder_ticks = 0; // 1" = 25352, 29910, 27635 calc 28800

// Encoder
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

Target Position -> Trajectory Generator -> Position Loop ->
Velocity Limiter -> Velocity Loop ->
Current Limiter -> Current Loop ->
Drive -> Motor|Encoder|Home

Target Position
- Up/Down?

Trajectory Generator
- Max velocity
- Max acceleration
- Max deceleration
- Abort develeration

**/

// Position


// Current
float current_now;

// Timer
Timer loop_timer;
int loop_min_time; // Min elapsed loop cycle time (us)
int loop_max_time; // Max elapsed loop cycle time (us)
int loop_error = 150; // "Do Not Exceed" loop cycle time (us)
int loop_current_time; //

int main() {
    pc.printf("STARTING SHIT\n");

    int i = 1; // Keep loop running unless shit goes south
    int position_loop_counter = 0;

    // Attach interrupt methods
    encoder_a_phase.rise(&encoder_tick);
    c_input.fall(&reset_ticks);
    a_input.rise(&a_input_rise);
    a_input.fall(&a_input_fall);
    b_input.rise(&b_input_rise);
    b_input.fall(&b_input_fall);

    // Setup PWM
    M1PWM.period_us(50); // Set to 20KHZ
    M1PWM.write(0.25f); // Set Duty Cycle to 20%

    loop_timer.start(); // Start timing loop

    while(i) {
        loop_timer.reset(); // Reset loop timer

        // Poll current sense
        current_now = M1CS.read();

        // "Current Loop" timing

        oscope = !oscope; // Current Loop Tuning Output (Square wave @ 1/2 freq)

        loop_current_time = loop_timer.read_us(); // Get elapsed time for loop

        if (loop_current_time < loop_min_time) { loop_min_time = loop_current_time; }
        else if (loop_current_time > loop_max_time) { loop_max_time = loop_current_time; }
        else if (loop_current_time >= loop_error) { i = 0; }

        while (loop_timer.read_us() < period) {
          wait_us(spin_period); // Period
        }
    }

    // Shit has gone south here
    pc.printf("SHIT HAS GONE SOUTH\n");
    pc.printf("min: %d, max: %d, current: %d\n", loop_min_time, loop_max_time, loop_current_time);
}
