#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H
/**********************************************/
/************* drive_controller.h  ************/
/**********************************************/
#include "helper.h"
#include "state_controller.h"
#include "hardware_definition.h"

template <class left_motor_controller, class right_motor_controller, class enabled_led, class wheel = wheel> 
class drive_controller : public timekeeper {
  state waypoint;
    
  
public:
  enabled_led enabled_LED;
  left_motor_controller left_wheel;
  right_motor_controller right_wheel;
    
    front_ultrasonic front;
    left_ultrasonic left;
    right_ultrasonic right;

    
    Pose __update() {
        state dX = waypoint - pose; // desired - actual
        
        // find strengths of inputs
        double d_forwards       = dX.distance();
        double d_pose_theta     = angle().wrap(dX.angle() - pose.theta());
        double d_waypoint_theta = dX.theta();
        
        if (d_forwards > distance_threshold) {
            // when far from the target
            if (abs(d_pose_theta) > angle_threshold_A) {
                // and the waypoint angle is large
                if (d_pose_theta > 0) {
                    // step left
                    if (abs(d_pose_theta) > angle_threshold_B) left_wheel.step_backward();
                    right_wheel.step_forward();
                } else {
                    // step right
                    if (abs(d_pose_theta) > angle_threshold_B) right_wheel.step_backward();
                    left_wheel.step_forward();
                }
            } else {
                // step forwards
                right_wheel.step_forward();
                left_wheel.step_forward();
            }
        } else if (abs(d_waypoint_theta) > angle_threshold_A) {
            if (d_waypoint_theta > 0) {
                // step left
                left_wheel.step_backward();
                right_wheel.step_forward();
            } else {
                // step right
                right_wheel.step_backward();
                left_wheel.step_forward();
            }
        }
        predict(pose);
        return pose;
    }
  void _update() {
    state dX = waypoint - pose; // desired - actual

    // find strengths of inputs
    double d_forwards       = dX.distance();
    double d_pose_theta     = angle().wrap(dX.angle() - pose.theta());
    double d_waypoint_theta = dX.theta();

    if (d_forwards > distance_threshold) {
      // when far from the target
      if (abs(d_pose_theta) > angle_threshold_A) {
        // and the waypoint angle is large
        if (d_pose_theta > 0) {
          // step left
          if (abs(d_pose_theta) > angle_threshold_B) left_wheel.step_backward();
          right_wheel.step_forward();
        } else {
          // step right
          if (abs(d_pose_theta) > angle_threshold_B) right_wheel.step_backward();
          left_wheel.step_forward();
        }
      } else {
        // step forwards
          unsigned long int reading;
          bool f, l, r;
          
          reading = front.reading();
          f = reading && reading < 50;
          reading = left.reading();
          l = reading && reading < 50;
          reading = right.reading();
          r = reading && reading < 50;
          
        
        if (l) {
        left_wheel.step_forward();
        } else if (r) {
        right_wheel.step_forward();
        } else {
        right_wheel.step_forward();
        left_wheel.step_forward();
        }
      }
    } else if (abs(d_waypoint_theta) > angle_threshold_A) {
      if (d_waypoint_theta > 0) {
        // step left
        left_wheel.step_backward();
        right_wheel.step_forward();
      } else {
        // step right
        right_wheel.step_backward();
        left_wheel.step_forward();
      }
    }
    predict(pose);  
  }
private:
  static constexpr double angle_threshold_A = PI/30;
  static constexpr double angle_threshold_B = PI/60;
  static constexpr double distance_threshold = 5;
  
public:
  drive_controller(state starting_pose) : timekeeper(100) {
    waypoint = starting_pose;
  }

  void begin(int) {
    left_wheel.begin();
    right_wheel.begin();
  }

  void predict(Pose & pose) {
    pose.predict(left_wheel.enc_dist_mm(), right_wheel.enc_dist_mm());
    left_wheel.enc_reset();
    right_wheel.enc_reset();
  }

  void enable() {
    enabled_LED.green();
    left_wheel.enable();
    right_wheel.enable();
  }

  void stop() {
    enabled_LED.red();
    left_wheel.stop();
    right_wheel.stop();
  }

  void to(state new_waypoint) {
    waypoint = new_waypoint;
  }

  state current_waypoint() {
    return waypoint;
  }

  bool arrived() {
    return pose.dState(waypoint).distance() < distance_threshold;
  }
};

/*
template <class left_motor_controller, class right_motor_controller, class enabled_led, class wheel = wheel> 
class _drive_controller : public timekeeper {
  state waypoint;
  
public:
  enabled_led enabled_LED;
  left_motor_controller left_wheel;
  right_motor_controller right_wheel;

private:
  static constexpr double unit_length = 150; // mm

  float turn_speed(float theta) {
    return theta/PI;
  }

  float drive_speed(float distance) {
    float d_scaled = distance / unit_length;
    if (d_scaled > 1) d_scaled = 1;
    return d_scaled;
  }

  char mode = 'i';
  double _distance = 0;

  void predict(Pose & pose) {
    pose.predict(left_wheel.enc_dist_mm(), right_wheel.enc_dist_mm());
    left_wheel.enc_reset();
    right_wheel.enc_reset();
  }

  void __update() {
    left_wheel.update();
    right_wheel.update();
    
    if ( _distance < 1.5 && mode != 'i' ) {
      mode = 'i';
      //Serial_print("arrived\n", 0);
    }
    
    double speed = _distance/unit_length;
    if ( speed > 1.0 ) speed = 1.0;
    
    switch (mode) {
      case 'b':
        left_wheel.set_speed( -speed*left_wheel.max_speed() );
        right_wheel.set_speed(-speed*right_wheel.max_speed());
      case 'r':
        left_wheel.set_speed(  speed*left_wheel.max_speed() );
        right_wheel.set_speed(-speed*right_wheel.max_speed());
        break;
      case 'l':
        left_wheel.set_speed( -speed*left_wheel.max_speed() );
        right_wheel.set_speed( speed*right_wheel.max_speed());
        break;
      case 'f':
        left_wheel.set_speed(  speed*left_wheel.max_speed() );
        right_wheel.set_speed( speed*right_wheel.max_speed());
        break;
      default:
        left_wheel.set_speed(  0 );
        right_wheel.set_speed( 0 );
        break;
    }
    _distance -= (abs(left_wheel.enc_dist_mm()) + abs(right_wheel.enc_dist_mm()))/2;
  }

  void _update() {
    left_wheel.update();
    right_wheel.update();

    state dX = waypoint - pose; // desired - actual

    // find strengths of inputs
    double d_forwards       = dX.distance();
    double d_pose_theta     = dX.angle() - pose.theta();
    double d_waypoint_theta = dX.theta();

    //pose.print();
    //Serial_print("%8s %8s %8s ", String(abs(d_forwards),2).c_str(), String(abs(d_pose_theta),2).c_str(), String(abs(d_waypoint_theta),2).c_str());


    // normalize inputs
    double left = 0;
    double right = 0;
    // find importance
    if ( d_forwards > 5.0 ) {
      if ( abs(d_pose_theta) > 3*(PI/180) ) {
        //Serial.print('A');
        d_pose_theta /= PI;
        if ( d_pose_theta > 0.50  ) d_pose_theta = 1.0;
        if ( d_pose_theta < -0.50 ) d_pose_theta = -1.0;
        
        left = -d_pose_theta;
        right = d_pose_theta;
      } else {
        //Serial.print('F');
        d_forwards /= unit_length;
        if ( d_forwards > 0.10  ) d_forwards = 1.0;
        if ( d_forwards < -0.10 ) d_forwards = -1.0;
        
        left = d_forwards;
        right = d_forwards;
      }
    } else {
      //Serial.print('T');
      d_waypoint_theta /= PI;
      if ( d_waypoint_theta > 0.05  ) d_waypoint_theta = 1.0;
      if ( d_waypoint_theta < -0.05 ) d_waypoint_theta = -1.0;
      
      left = -d_waypoint_theta;
      right = d_waypoint_theta;
    }

    // move forwards
    //d -= (left_wheel.enc_dist_mm() + right_wheel.enc_dist_mm())/2;
    //left_wheel.set_speed((d/250.0)*left_wheel.max_speed());
    //right_wheel.set_speed((d/250.0)*right_wheel.max_speed());

    // rotate
    //dX.print();
    
    //d = angle().wrap(dX.theta())*wheel::separation;
    left_wheel.set_speed(left*left_wheel.max_speed());
    right_wheel.set_speed(right*right_wheel.max_speed());
    
    //Serial_print(" -> speeds: [ %7s, %7s]\n", String(left,3).c_str(), String(right,3).c_str());
    predict(pose);    
  }

public:
  _drive_controller(state starting_pose) : timekeeper(1) {
    waypoint = starting_pose;
  }

  void begin(bool verbose = false) {
    if (verbose) Serial_print("Left Wheel:\n", 0);
    left_wheel.begin(verbose);
    if (verbose) Serial_print("Right Wheel", 0);
    right_wheel.begin(verbose);
    if (verbose) report();
  }
  
  void print_hardware() {    
    Serial_print("Drive controller attached\n", 0);
  }

  void print_constant() {   
    Serial_print(" Controller constants:", 0); 
    Serial_print( "  Length of 1 box unit: %s mm\n", String(unit_length, 3).c_str());
    Serial_print( "  Wheel separation: %s mm\n", String(wheel::separation, 3).c_str());
    Serial_print( "  Wheel radial separation: %s mm\n", String(wheel::separation_radius, 3).c_str());
  }

  void print_status() {
  }

  void report() {
    print_hardware();
    print_constant();
    print_status();
  }

  void enable() {
    enabled_LED.green();
    left_wheel.enable();
    right_wheel.enable();
  }

  void stop() {
    enabled_LED.red();
    left_wheel.stop();
    right_wheel.stop();
  }

  void backward(float distance) {
    mode = 'b';
    _distance = distance;
  }

  void forward(float distance) {
    mode = 'f';
    _distance = distance;
  }
  
  void left(double angle_deg) {
    mode = 'l';
    float angle = angle_deg/180*PI;  // convert to radians
    _distance = angle*wheel::separation_radius;
  }
  
  void right(double angle_deg) {
    mode = 'r';
    float angle = angle_deg/180*PI;  // convert to radians
    _distance = angle*wheel::separation_radius;
    //Serial_print("right angle: %s distance: %s\n", String(angle).c_str(), String(_distance).c_str());
    while(_distance > 0) update();
  }

  void to(state new_waypoint) {
    waypoint = new_waypoint;
  }

  state current_waypoint() {
    return waypoint;
  }

  bool arrived() {
    return pose.dState(waypoint).distance() < 0.0025;
  }
};

*/
#endif
