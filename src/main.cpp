#include "mbed.h"

Serial pc (SERIAL_TX, SERIAL_RX);

int i1 = 9870;
float f1 = (float) i1 / 1000;

Timer t1;

int main() {

  t1.start();

  while(1) {
    t1.reset();
    wait_us(5);
    pc.printf("a: %d\n", t1.read_us());
    pc.printf("b: %d\n", t1.read_us());
  }
}

// PID
// error = target - current
// cumulative_error = cumulative_error + error
// last_error
// slope = error - last_error
// p = p_gain * error
// +
// i = i_gain * cumulative_error ? variable roll over?
// d = d_gain * slope
