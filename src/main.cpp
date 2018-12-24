#include "mbed.h"

// Debug Stuff
Serial pc (SERIAL_TX, SERIAL_RX);

// Limit Switches
DigitalIn top_limit(D3, PullUp); // Top Limit Switch
DigitalIn bottom_limit(D5, PullUp); // Bottom Limit Switch (Prime)

// Emergency Stop
DigitalIn start_stop(D11, PullUp); // Emergency Stop (Should be paired to relay/trans out)

// Parameter Control (Keypad, Encoder, Buttons, etc)

// Pololu Dual VNH5019 Motor Driver Shield for Arduino
// Driver 1
DigitalOut M1INA(D2); // Clockwise Input
DigitalOut M1INB(D4); // Counter-Clockwise Input
DigitalIn M1EN_DIAG(D6); // High = Half-Bridge A & B Enabled, Low = Half-Bridge A & B Disabled (Combination of VHN5019 ENA/DIAGA & ENB/DIAGA Pins)
PwmOut M1PWM(D9); // PWM Input, 20KHZ Max, Low = Off, High = On, Dependent On xINA/B Pins
AnalogIn M1CS(A0); // Output Of Current Sense Proportional To Motor Current If CS_DIS Is Low Or Open (CD_DIS Not Connected)

// The Big Show
int main() {
    // Setting up shop
    int i = 1; // Keep the shitshow running

    // Sampling Rate
    int period = 50; // Period
    int velocity_period = 3; // Period mulitplier for velocity sampling
    int velocity_period_counter = 0; // Velocity loop counter
    int current_period = 0; // Period mulitplier for current sampling
    int current_period_counter = 0; // Current loop counter
    int input_period = 5; // Period mulitplier for input sampling
    int input_period_counter = 0; // Input loop counter
    int terminal_period = 5000; // Period multiplier for terminal outputs
    int terminal_period_counter = 0; // Terminal loop outputs

    // Turn off direction inputs
    M1INA.write(0);
    M1INB.write(0);

    // Setup PWM
    M1PWM.period_us(50); // Set to 20KHZ
    M1PWM.write(0.0f); // Set Duty Cycle to 0%

    // Motion Control
    int is_running = 0;
    int is_starting = 0;

    // Current
    int current_sense_mulitplier = 34; // Not sure we need to do this - arbitrary values should work fine, I think...
    float current_sense = 0;

    // Velocity
    float velocity_limit = 0.75f; // Maximum velocity command (as PWM duty cycle)
    float acceleration_limit = 0.01f; // Maximum acceleration rate (as PWM duty cycle p/ period)
    float deceleration_limit = 0.02f; // Maximum deceleration rate (as PWM duty cycle p/ period)
    float fast_stop_ramp = 0.1f; // Maximum deceleration rate for emergency stop (as PWM duty cycle p/ period)

    // Inputs
    int at_top_limit = !top_limit;
    int at_bottom_limit = !bottom_limit;
    int is_start_stop = start_stop;

    int top_limit_debounce = 10;
    int bottom_limit_debounce = 10;
    int start_stop_debounce = 10;

    // Loop
    while(i) {
        // Increment counts
        velocity_period_counter++;
        current_period_counter++;
        input_period_counter++;
        terminal_period_counter++;

        // Velocity loop
        if (velocity_period_counter >= velocity_period) {
            M1PWM.write(0.1f);
            M1INA = start_stop;
            velocity_period_counter = 0;
        }

        // Current loop
        if (current_period_counter >= current_period) {
            current_sense = M1CS.read(); // Poll current sense
            current_period_counter = 0;
        }

        // Input loop
        if (input_period_counter >= input_period) {
            at_top_limit = !top_limit; // Poll top limit switch
            at_bottom_limit = !bottom_limit; // Poll bottom limit switch
            // Poll start/stop switch
            if (start_stop && !is_starting) {
                is_starting = 1;
                if (is_running) {
                    is_running = 0;
                }
                else {
                    is_running = 1;
                }
            }
            else {
                if (!start_stop) {
                    is_starting = 0;
                }
            }

            input_period_counter = 0;
        }

        // Terminal loop
        if (terminal_period_counter >= terminal_period) {
            // Wait for lull in motion
            if (!M1INA && !M1INB) {
                pc.printf("Top: %d, Bottom: %d, Start: %d, Current: %f\n", at_top_limit, at_bottom_limit, is_start_stop, current_sense);
                terminal_period_counter = 0;
            }
        }

        wait_us(period); // Period
    }
}
