#include "mbed.h"
#include "machine_parameters.cpp"
#include "io.cpp"

Serial pc (SERIAL_TX, SERIAL_RX);

int current_period_counter = 0;
int velocity_period_counter = 0;
int position_period_counter = 0;

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
int loop_enable = 1;

// PID
// error = target - current
// cumulative_error = cumulative_error + error
// last_error
// slope = error - last_error
// p = p_gain * error
// +
// i = i_gain * cumulative_error ? variable roll over?
// d = d_gain * slope

int main() {
    pc.printf("Entering main\n");

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


        // Velocity
        if (velocity_period_counter >= velocity_period_mulitiplier) {
          velocity_period_counter = 0;
        }

        //Position
        if (position_period_counter >= position_period_mulitplier) {
          position_period_counter = 0;
        }

        while (loop_timer.read_us() < current_period) {
          wait_us(spin_period); // Period
        }

        current_scope_output = !current_scope_output;
    }
}
