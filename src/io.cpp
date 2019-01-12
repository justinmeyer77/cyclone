#include "mbed.h"

namespace io {

  InterruptIn a_input(PC_8, PullUp); // Arbitrary input @ "A"
  InterruptIn b_input(PC_6, PullUp); // Arbitrary input @ "B"
  InterruptIn c_input(PC_5, PullUp); // Arbitrary input @ "C"
  AnalogIn a_pot (PA_0); // Arbitrary 10k pot input @ "A"
  AnalogIn b_pot (PA_1); // Arbitrary 10k pot input @ "B"

}
