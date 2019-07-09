#ifndef TWIDDLE_H
#define TWIDDLE_H

#include <limits>
#include <iostream>
#include <uWS/uWS.h>
#include "PID.h"

class Twiddle {
 public:
  /**
   * Constructor
   */
  Twiddle();

  /**
   * Destructor.
   */
  virtual ~Twiddle();

  double NewStep(uWS::WebSocket<uWS::SERVER> ws, double cte);
  

 private:
  PID pid;
  /**
   * Number of steps in current iteration
   */
  double num_steps;

  /**
   * PID Coefficients and the deltas
   */ 
  std::vector<double> params;
  std::vector<double> delta_params;

  // Aux variables
  int twiddle_var;		// 0 if Kp, 1 if Ki, 2 if Kd
  int twiddle_phase;    // 0 if we tried upper value, 1 lower value
  double best_error;
  int iter;
};

#endif  // TWIDDLE_H