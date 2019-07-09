#include "Twiddle.h"

Twiddle::Twiddle() {
  num_steps = 0;

  params = {1., 0.0005, 10.};
  pid.Init(params[0], params[1], params[2], -1., 1.);
  delta_params = {0.2, 0.0001, 1.};

  best_error = std::numeric_limits<double>::max();
  twiddle_var = 2.;
  twiddle_phase = 1;
  iter = 0;
}

Twiddle::~Twiddle() {}

double Twiddle::NewStep(uWS::WebSocket<uWS::SERVER> ws, double cte) {
  if (num_steps < 4000) {
    pid.UpdateError(cte);
	double steer_value = pid.ControlValue();
	num_steps ++;
	return steer_value;
  }

  double total_error = pid.TotalError();

  if (twiddle_phase == 0) {
    // Phase 0 (i.e. variable increase) is completed
	// If error was better, then can increase the delta, and move on to next param
	// Otherwise go to phase 1 (i.e. variable decrease)
    if (total_error < best_error) {
	  best_error = total_error;
	  delta_params[twiddle_var] *= 1.1;
	  twiddle_var = (twiddle_var + 1) % 3;
	  params[twiddle_var] += delta_params[twiddle_var];
	} else {
	  twiddle_phase = 1;
	  params[twiddle_var] -= 2 * delta_params[twiddle_var];
	}
  } else {
    // Phase 1 (i.e. variable decrease) is completed
	// If error was better, then can increase the delta
	// Otherwise revert param and reduce delta
	if (total_error < best_error) {
	  best_error = total_error;
  	  delta_params[twiddle_var] *= 1.1;
	} else {
	  params[twiddle_var] += delta_params[twiddle_var];
	  delta_params[twiddle_var] *= 0.9;
	}

	// Go on to next variable
	twiddle_var = (twiddle_var + 1) % 3;
	params[twiddle_var] += delta_params[twiddle_var];
	twiddle_phase = 0;
  }

  std::cout << "Delta Kp: " << delta_params[0] << "Delta Ki: " << delta_params[1] <<"Delta Kd: " << delta_params[2]
			<< " Kp = " << params[0] << " Ki = " << params[1] << " Kd = " << params[2] 
			<< " Phase = " << twiddle_phase << " Variable = " << twiddle_var  << " Prev Error = " << total_error 
            << std::endl;

  if (twiddle_var == 0 && twiddle_phase == 0) {
    iter++;
	double total_delta = delta_params[0] + delta_params[1] + delta_params[2];
	std::cout << "Iter: " << iter << " Total delta: " << total_delta  << " Best error: " << best_error 
					<< " Kp = " << params[0] - delta_params[0] << " Ki = " << params[1] << " Kd = " << params[2] 
                    << std::endl;
  }
  std::string reset_msg = "42[\"reset\",{}]";
  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
  pid.Init(params[0], params[1], params[2], -1., 1.);
  num_steps = 0;
  return 0.;
}

