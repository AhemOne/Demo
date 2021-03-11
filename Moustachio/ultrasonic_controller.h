#include "hardware_definition.h"
class ultrasonic_controller : public timekeeper {
  double d_left, d_forward, d_right;
  left_ultrasonic sonar_left;
  right_ultrasonic sonar_right;
  front_ultrasonic sonar_forward;
  int n = 3;

  static constexpr float offset_left = 0.012;
  static constexpr float offset_forward = 0.012;
  static constexpr float offset_right = 0.012;

  double R(double distance) {
    static constexpr double syx = 0.0365;
    return distance * syx * syx;
  }

  void innovate(Pose & pose, float reading, float theta) {
    // calculate expected distance
    //double th = pose.theta() + theta;
    //state d = pose + state(125, 125, 0) + state(reading*sin(th), reading*cos(th), 0);
    //int x = round(d.x()/250);
    //int y = round(d.y()/250);
    //pose.innovate_distance(reading, state(x*250, y*250, 0).distance(), theta, R(reading));
  }

  void _update() {
    n++;
    if ( n >= 3 ) n = 0;
    switch (n) {
      case 0:
        d_left = sonar_left.reading();
        if (d_left) innovate(pose, d_left + offset_left, PI/2);
        //Serial_print("L: %s ", String(d_left).c_str());
        break;
      case 1:
        d_forward = sonar_forward.reading();
        if (d_forward) innovate(pose, d_forward + offset_forward, 0);
        //Serial_print("F: %s ", String(d_forward).c_str());
        break;
      case 2:
        d_right = sonar_right.reading();
        if (d_left) innovate(pose, d_right + offset_right, -PI/2);
        //Serial_print("R: %s\n", String(d_right).c_str());
        break;
    }
  }
  
public:
  ultrasonic_controller(hardware::ultrasonic & Left, hardware::ultrasonic & Forward, hardware::ultrasonic & Right) : timekeeper(1000/45) {
    sonar_left = &Left;
    sonar_forward = &Forward;
    sonar_right = &Right;
    
    sonar_left.enable();
    sonar_forward.enable();
    sonar_right.enable();
  }

  double left() { return d_left + offset_left; }
  double forward() { return d_forward + offset_forward; }
  double right() { return d_right + offset_right; }
};
