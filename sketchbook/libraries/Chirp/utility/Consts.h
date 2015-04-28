#ifndef CONSTS_H

#define CONSTS_H

#define PI 3.14159265359
#define PI_2 6.283185307
#define ARD_RANDMAX 2147483640

#define CHIRP_MAXSPEED 450

//In mm.
const int WHEEL_DIAM = 42;
const float WHEEL_DIST = 78.4;
const float WHEEL_CIRC = WHEEL_DIAM * 3.14159265359;
const float ROBOT_CIRC = WHEEL_DIST * 3.14159265359;
const int STEPS_PER_REV = 512;
const float STEPS_PER_CM = STEPS_PER_REV/WHEEL_CIRC*10;

#endif