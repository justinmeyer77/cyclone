#include "mbed.h"
#include "calcs.cpp"

Serial pc (SERIAL_TX, SERIAL_RX);

const int real_world_value = 100;
const int calculated_value = test(real_world_value);

// Control loop timing
// Cascaded PID loops
// Position -> Velocity -> Current
const int current_period = 625; // Current loop period in microseconds (1600hz)
const int velocity_period_mulitiplier = 4; // Velocity loop multiplier relative to current loop (400hz)
const int position_period_mulitplier = 16; // Position loop multiplier relative to current loop (100hz)
const int spin_period = 10; // Time to wait while polling for end of current loop period in microseconds

int velocity_period_counter = 0;
int position_period_counter = 0;

// Current loop
const int current_sense_multiplier = 34; // ADC multiplier for "real world" units (Amps)
const int current_offset = 0; // Added to commanded current
const float maximum_current_limit = 0; // Maximum allowed commanded current
const float continuous_current_limit = 0; // Maximum allowed continuous commanded current_sense
const int I2T_time_limit = 0; // I2T (Ampere squared seconds) time limit expressed in milliseconds
const int current_proportional = 0;
const int current_integral = 0;

// Velocity loop
const int velocity_limit = 0; // Top speed limit
const int acceleration_limit = 0; // Do we need this in position mode?
const int deceleration_limit = 0; // Do we need this in position mode?
const int fast_stop_ramp = 0; // Hardware stop deceleration Limit
const int velocity_proportional = 0;
const int velocity_integral = 0;
const int velocity_integral_drain = 0;
const int veloity_feed_forward = 0;
int velocity = 0;
int last_position = 0;
const int velocity_multiplier = 0;

// Position loop
const int acceleration_feed_forward = 0;
const int velocity_feed_forward = 0;
const int position_proportional = 0;
const int position_integral = 0;
const int position_integral_drain = 0;
const int position_deritive = 0;
const int gains_multiplier = 0;
const int ticks_per_inch = 28800;


InterruptIn a_input(PC_8, PullUp); // Arbitrary input @ "A"
InterruptIn b_input(PC_6, PullUp); // Arbitrary input @ "B"
InterruptIn c_input(PC_5, PullUp); // Arbitrary input @ "C"
AnalogIn speed_dial (PA_0); // 10k rotary pot @ speed control inputs

DigitalOut current_scope_output(PC_4, 0); // Current loop timing output @ o'scope
// DigitalOut velocity_loop_output(); // Velocity loop timing output @ o'scope
// DigitalOut position_loop_output(); // Position loop timing output @ o'scope

DigitalIn M1EN(PC_10); // High = Half-Bridge A & B Enabled, Low = Half-Bridge A & B Disabled (Combination of VHN5019 ENA/DIAGA & ENB/DIAGA Pins)
DigitalOut M1INA(PC_12, 0); // Clockwise Input
DigitalOut M1INB(PC_11, 0); // Counter-Clockwise Input
PwmOut M1PWM(D9); // PWM Input, 20KHZ Max, Low = Off, High = On, Dependent On xINA/B Pins (PC_7?)
AnalogIn M1CS(PC_0); // Output Of Current Sense Proportional To Motor Current

const int motor_pwm_period = 50;

InterruptIn encoder_a_phase(PA_10, PullUp); // A phase @ encoder
DigitalIn encoder_b_phase(PB_3, PullUp); // B phase @ encoder

int encoder_ticks = 0; // 1" = 25352, 29910, 27635 calc 28800

void encoder_tick() {
  if (encoder_b_phase.read()) { encoder_ticks--; }
  else { encoder_ticks++; }
}

void a_input_rise() { M1INA = 0; }
void a_input_fall() { M1INA = 1; }

void b_input_rise() { M1INB = 0; }
void b_input_fall() { M1INB = 1; }

void c_input_rise() {}
void c_input_fall() { encoder_ticks = 0;}

Timer loop_timer;
const int loop_error = 150; // "Do Not Exceed" loop cycle time (us)

int main() {
    pc.printf("Entering main\n");

    int loop_enable = 1;

    encoder_a_phase.rise(&encoder_tick);

    a_input.rise(&a_input_rise);
    a_input.fall(&a_input_fall);
    b_input.rise(&b_input_rise);
    b_input.fall(&b_input_fall);
    c_input.fall(&c_input_fall);

    // Setup PWM
    M1PWM.period_us(motor_pwm_period); // Set to 20KHZ
    M1PWM.write(0.25f); // Set Duty Cycle

    loop_timer.start(); // Start timing loop

    while(loop_enable) {
        loop_timer.reset();

        velocity_period_counter++;
        position_period_counter++;

        // Current
        pc.printf("current: %f\n", M1CS.read());

        // Velocity
        if (velocity_period_counter >= velocity_period_mulitiplier) {

          velocity_period_counter = 0;
        }

        //Position
        if (position_period_counter >= position_period_mulitplier) {
          pc.printf("position: %d\n", encoder_ticks);
          position_period_counter = 0;
        }

        while (loop_timer.read_us() < current_period) {
          wait_us(spin_period); // Period
        }

        current_scope_output = !current_scope_output;
    }
}
