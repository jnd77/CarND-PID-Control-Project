#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_, double min_, double max_) {
  // Initialize the 3 PID parameters
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;

  // As well as min and max
  min = min_;
  max = max_;

  // Set the 3 errors to zero
  p_error = 0.;
  i_error = 0.;
  d_error = 0.;
  total_error = 0.;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;
  
  // Total error is used during Twiddle, but if not capping then best error is reached 
  // when car is driving straight off the road and stays stuck
  double capped_cte = cte;
  if (cte > 5)
    capped_cte = 5;
  total_error += capped_cte * capped_cte;
}

double PID::ControlValue() {
  double value = - Kp * p_error - Ki * i_error - Kd * d_error;

  if (value > max) {
    return max;
  } else if (value < min) {
    return min;
  } else {
    return value;
  }

}


double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return total_error;
}