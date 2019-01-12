#include "mbed.h"
#include "conversion.cpp"
#include "user_parameters.cpp"

namespace machine_parameters {

  const int base_period = conversion::from_herz_to_microseconds(user_parameters::base_loop_frequency_hz);
  const int current_period_multiplier = conversion::from_herz_to_microseconds(user_parameters::current_loop_frequency_hz);
  const int velocity_period_mulitiplier = conversion::from_herz_to_microseconds(user_parameters::velocity_loop_frequency_hz);
  const int position_period_mulitplier = conversion::from_herz_to_microseconds(user_parameters::position_loop_frequency_hz);

}
