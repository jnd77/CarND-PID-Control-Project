# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Objective

The objective of this project is to implement a PID controller on the steering, to ensure the car has a smooth trajectory around the lake.

### PID implementation

The PID controller is quite simple to implement.
There are 3 parameters (Kp, Ki and Kd) which have all the following role:

--> Kp: based on how far the car is from the ideal trajectory, it will correct the steering in the proper direction
--> Ki: based on how far the car has been from the center since the simulator started, it will correct accordingly.
        The main role is to correct for any bias in the controller and ensure we correct for it.
--> Kd: based on whether the car is getting closer or not to the center; this will smooth the trajectory.

The main challenge however is to calibrate these 3 parameters.

### Twiddle implementation

This algorithm is here to calibrate the 3 parameters.
Basically, it tweks each parameter up and down independently and measures the total error to know if the tweak is an improvement or not.
If tweaking up or down does not improve the PID controller, then the tweak's size is reduced by a factor of 10%.
Similarly, if one of the tweak provides an improvement, then we keep the improved value and also increases the tweak's size by 10%.

The Twiddle algorithm is run by switching a boolean from true to false in the code, and some logs are provided to follow the number of iterations.

The implementation is quite standard. The simulator is run for 4,000 steps, to make sure the car is reaching the first curb.
We also noticed it was a good idea to cap the CTE at 5 (i.e. the width of the road), to make sure the best solution is not to be stuck immediately on the kerb.


### Parameters influence

While Twiddle is running, one can easily view the influence of the parameters on the trajectory.

When Kp is negative, the car is steering away from the trajectory.
When it's too high, then the steering back towards the center is very strong, so strong that the car can drive off the road on the opposite side.
It keeps overshooting the center, and steers back in the other direction, making it a very jerky ride.

When Ki is too high (in absolute value), then the car quickly steers off the road (as the error has accumulates).

Finally, when Kd is too high, the car does not go back to the center.
If it's negative, it actually drives in a circle ...


### Twiddle results

We run the Twiddle algorithm a first time.

The first run (with all parameters set at 0) generates a total error of 79,000 (i.e. the car is on average 4.5 meters away from the center).
After 80 iterations, the best parameters are respectively 1.4, 0.0004 and 6.8, with an error of 185 (error of only 20 centimeters).

However this shows that the delta is way too big for Ki, and the parameter is not optimized.


So we run the algorithm a second time with starting values of (1., 0.0005, 10.) and deltas of (0.2, 0.0001, 1.).
After a dozen of iterations, the parameters settle to around (1.36, 0.0005, 11.1), being an error of 85 (or less than 15 centimeters).


### Final implementation

Unfortunetely when using the parameters fine-tuned above, the car goes off track in the sharp turns at the end of the track (it is too far to be taken into account during Twiddle).
The likely cause of going off track is the car going too fast, and therefore going off track before it has time to compensate.

So we add a few lines of code to control the throttle (only when speed is already large enough). 
When the steering is quite small, we give full throttle.
We lower the throttle then as the sterring increases, even applying brake where we are near full steering.

Once we run this code, the car runs quite smoothly around the lake.


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

