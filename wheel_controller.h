#ifndef WHEEL_CONTROLLER_H
#define WHEEL_CONTROLLER_H
/**********************************************/
/************* wheel_controller.h  ************/
/**********************************************/
#include "helper.h"

template <class motor, class wheel = wheel>
class wheel_controller : public motor, public timekeeper {
  double _cv = 0;     // current velocity, mm/s
  double _dv = 0;     // desired velocity, mm/s
  double _s = 0;
  
  static constexpr double K_proportional = 5;

  static constexpr unsigned long min_delay = 15;
  static constexpr double min_delay_s = min_delay/1000.0;

  void _update() {
    double e = _dv - _cv;                // error in velocity mm/s
    double a = K_proportional*e;         // a = k*e           mm/s^2
    
    _cv += a*dt();                       // predict new velocity

    if ( _cv > max_speed())  _cv = max_speed();
    if ( _cv < -max_speed()) _cv = -max_speed();
     
    _s += _cv*dt();                      // predict new position
    
    if ( _s > step_size_mm()  ) {
      motor::step_forward();
      _s -= step_size_mm();
    } else if ( _s < -step_size_mm() ) {
      motor::step_backward();
      _s += step_size_mm();
    }
  }
  
public:
  wheel_controller() { }

  void begin(bool verbose = false) {
    motor::begin(verbose);
    if (verbose) report();
  }

  void print_hardware() {    
    Serial_print("Wheel controller attached\n", 0);
  }

  void print_constant() {
    Serial_print(" Controller constants:\n", 0); 
    Serial_print( "  K_proportional = %s\n", String(K_proportional, 3).c_str());
    Serial_print("  Wheel radius: %s mm\n", String(wheel_rad_mm(), 3).c_str());
    Serial_print("  Step distance: %s mm\n", String(step_size_mm(), 3).c_str());
    Serial_print("  max speed: %s mm/s\n", String(max_speed(), 3).c_str());
    Serial_print("  minimum period: %d ms\n", min_delay);
  }

  void print_status() {    
    Serial_print(" Controller status:\n", 0); 
    Serial_print("  Current speed: %s mm/s\n", String(get_speed(), 3).c_str());
    Serial_print("  Set speed: %s mm/s\n", String(get_setting(), 3).c_str());
    Serial_print("  Encoder absolute position: %s mm\n", String(abs_dist_mm(), 3).c_str());
    Serial_print("  Encoder relative position: %s mm\n", String(enc_dist_mm(), 3).c_str());
  }

  void report() {
    print_hardware();
    print_constant();
    print_status();
  }
  
  void set_speed(double v) { _dv = v; }
  double get_speed() { return _cv; }
  double get_setting() { return _dv; }
  inline double max_speed() { return step_size_mm()/min_delay_s; }  
  
  inline double wheel_rad_mm()  { return wheel::radius; }
  inline double step_size_mm()  { return motor::step_size_rad() * wheel_rad_mm(); }
  inline double abs_dist_mm()   { return motor::abs_rad() * wheel_rad_mm(); }
  inline double enc_dist_mm()   { return motor::enc_rad() * wheel_rad_mm(); }
};

#endif
