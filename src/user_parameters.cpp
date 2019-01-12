namespace user_parameters {

  // Loop timing user parameters
  const int current_loop_frequency_hz = 1600;
  const int velocity_loop_frequency_hz = 400;
  const int position_loop_frequency_hz = 100;
  const int base_loop_frequency_hz = 20;
  const int spin_period = 10;

  // Current loop user parameters
  const int current_sense_multiplier = 34; // ADC multiplier for "real world" units (Amps)
  const int current_offset_amps = 0; // Added to commanded current
  const float maximum_current_limit_amps = 0; // Maximum allowed commanded current
  const float continuous_current_limit_amps = 0; // Maximum allowed continuous commanded current_sense
  const int I2T_time_limit_milliseconds = 0; // I2T (Ampere squared seconds) time limit expressed in milliseconds
  const int current_proportional_unitless = 0;
  const int current_integral_unitless = 0;
  const int current_derivitive_unitless = 0;

  // Velocity loop user parameters
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

}
