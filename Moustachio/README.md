# Moustachio

## Introduction

Moustachio was a Unversity group project. The project aim was to design a robot within a set of parameters which was able to navigate autonomously through a maze. This involved many parts; of which I was assigned the hardware design. We chose to use stepper motors to drive the wheels using differential drive, a trio of ultrasonic sensors to detect walls ahead and to the sides of the robot and a compass module in order to orient the robot with respect to the earths magnetic field. It might be worth noting that we were ecouraged to use templating in order to increase the size of the program code and decrease the RAM footprint. 

## Parts

### compass.h

Driver file to interface with the HMC5883L 3-axis compass via I2C.

### drive_controller.h

The drive controller is an interface for the wheels to allow for the user to give a waypoint on the ground and the wheels to then move themselves to that position. Two versions are included, the uncommented version removed the wheel_controller dependency as outlined in ### wheel_controller.h, the commented out version uses the wheel_controller as I had initally intended.

### hardware.h

Driver file for various hardware attached directly to the microcontroller, importantly the unipolar_stepper which provides the ability for the motor to take a step forwards or backwards.

### helper.h

Contains a timekeeper object used to work out the next time an object should be updated. This was a quick and dirty solution because I should have used a built-in timer however in planning we didnt allocate the time to explore the hardware timers and besides, this worked well enough for the purpose.

### state_controller.h

This wasn't used but implements a kalmann filter in order to better estimate the robots real position in space. I did this to prove that a EKF filter could be reasonably implemented if the redundant and zero terms could be eliminated from the calculations however the stepper motors provided very accurate dead reckoning and so the overhead caused did not justify the increase in accuracy provided by the filter given the overheads associated.

### state.h

This provides a type which defines the robot's position in space.

### ultrasonic_controller.h

This file provide an interface for to the ultrasonic sensors. The user can poll this to see how far the front and sides are from a wall, if it can detect one.

### ultrasonic.h

The is the driver file for a standard ultrasonic time-of-flight module

### wheel_controller.h

This file contains the code to abstract a stepper motor into a constantly rotating wheel so the drive controllers could simply set the desired speed. This was done because we expected the possibility to change to regular DC motors if the steppers didn't work out. This was eventually made redundant in an effort to shave a little bit more off the RAM requirements (48 bytes) as the pathfinding algorithms were runnning out of memory.

## Interface

the drive controller, compass and ultrasonic objects must be instantiated before use, then begin() called on each.

The main functionality is then provided as follows;

Get global direction of travel

```cpp double bearing = compass.bearing(); ```

move to a global waypoint

```cpp drive.to(waypoint); ```

get the distance to a wall

```cpp
double left_distance = ultrasonic.left();
double right_distance = ultrasonic.right();
double forward_distance = ultrasonic.forward();
```

