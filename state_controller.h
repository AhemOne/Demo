#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H
/**********************************************/
/************* state_controller.h  ************/
/**********************************************/
#include "helper.h"
#include "state.h"

template <class wheel>
class state_controller : public state {
private:
  double P[3][3] = {{0}};
  static constexpr float Q = 0.0001;
  
public:
  state_controller() { zero(); }
  double x()     { return state::x(); }
  double y()     { return state::y(); }
  double theta() { return state::theta(); }

  void begin(bool verbose = 0) {
    if (verbose) report();
  }

  void print_hardware() {      
    Serial_print("State controller attached\n", 0);
  }

  void print_constant() {     
    Serial_print(" controller constants:\n", 0); 
    Serial_print("  Wheel spacing: %s mm\n", String(wheel::separation, 3).c_str());
    Serial_print("  Q: %s mm\n", String(Q, 5).c_str());
  }

  void print_status() {      
    Serial_print(" controller status:\n", 0);
    Serial_print("  Current coordinates: (%s mm, %s mm, %s rad\n", 
                 String(x(),3).c_str(),
                 String(y(),3).c_str(),
                 String(theta(),3).c_str());
  }

  void report() {
    print_hardware();
    print_constant();
    print_status();
  }

  void predict(double left_distance, double right_distance) {
    // dead reckon position
    double d_s =      (right_distance + left_distance)/2;
    double d_theta =  (right_distance - left_distance)/wheel::separation;
    double theta =    wrap(state::_theta + d_theta/2);

    double c_theta = d_s * cos(theta);
    double s_theta = d_s * sin(theta);

    // add to state
    state::_x += c_theta;
    state::_y += s_theta;
    state::_theta += d_theta;

   // Serial_print("x: %s y: %s theta: %s\n", String(state::x, 2).c_str(), String(state::y, 2).c_str(), String(state::theta, 2).c_str());

    // update P matrix
    double a = -s_theta,
          b = c_theta;
    double c = P[0][0],
          d = P[0][1],
          e = P[0][2],
          f = P[1][0],
          g = P[1][1],
          h = P[1][2],
          i = P[2][0],
          j = P[2][1],
          k = P[2][2];
    double ak = a*k,
          bk = b*k;
    double eak = e + ak,
          hbk = h + bk;
          
    P[0][0] = c + a*eak + a*i + Q;
    P[0][1] = d + b*eak + a*j;
    P[0][2] = eak;
    P[1][0] = f + a*hbk + b*i;
    P[1][1] = g + b*hbk + b*j + Q;
    P[1][2] = hbk;
    P[2][0] = i + ak;
    P[2][1] = j + bk;
    P[2][2] = k + Q;
  }
  
  void innovate_theta(float theta_c, float R) {
    double c = P[0][2],
           f = P[1][2],
           g = P[2][0],
           h = P[2][1],
           i = P[2][2];
  
    double d_theta = theta_c - state::_theta;
    double oorpi = 1/(R + i); // one over r plus i
    double dtoorpi = d_theta*oorpi;

    state::_x += c*dtoorpi;
    state::_y += f*dtoorpi;
    state::_theta += i*dtoorpi;

    double ioorpi = i*oorpi;
    P[0][0] -= c*g*oorpi;
    P[0][1] -= c*h*oorpi;
    P[0][2] -= c*i*oorpi;
    P[1][0] -= f*g*oorpi;
    P[1][1] -= f*h*oorpi;
    P[1][2] -= f*i*oorpi;
    P[2][0] -= ioorpi;
    P[2][1] -= ioorpi;
    P[2][2] -= ioorpi;
  }

  
  void innovate_distance(float measured_distance, float expected_distance, float angle, float R) {
    double a = P[0][0],
           b = P[0][1],
           c = P[0][2],
           d = P[1][0],
           e = P[1][1],
           f = P[1][2],
           g = P[2][0],
           h = P[2][1];
    double Y = measured_distance - expected_distance;  
    
    double Sin = sin(angle), Cos = cos(angle);
    double ooed = 1/expected_distance;
    
    double ced = Cos*ooed;
    double sed = Sin*ooed;
    
    double ased = a*sed;
    double bced = b*ced;
    double bsed = b*sed;
    double csed = c*sed;
    double dsed = d*sed;
    double dced = d*ced;
    double eced = e*ced;
    double fced = f*ced;
    double gsed = g*sed;
    double hced = h*ced;
    
    double bcas = bced + ased;
    double ecbs = eced + bsed;
    double dcas = dced + ased;
    double ecds = eced + dsed;
    double hcgs = hced + gsed;
    
    double Rness = 1/(R + ced*ecbs + sed*dcas);
    double YRness = Y*Rness;;
    
    double bcasR = bcas*Rness;
    double ecdsR = ecds*Rness;
    
    double sbcased = Sin*bcasR*ooed;
    double cecdsed = Cos*ecdsR*ooed;
    
    state::_x -= bcas*YRness;
    state::_y -= ecds*YRness;
    state::_theta -= hcgs*YRness;
    
    P[0][0] -= (sbcased + dced*bcasR);
    P[0][1] -= (sbcased + eced*bcasR);
    P[0][2] -= (sbcased + fced*bcasR);
    
    P[1][0] -= (cecdsed + ased*ecdsR);
    P[1][1] -= (cecdsed + bsed*ecdsR);
    P[1][2] -= (cecdsed + csed*ecdsR);
    
    P[2][0] -= (dcas*hcgs);
    P[2][1] -= (ecbs*hcgs);
    P[2][2] -= ((fced + csed)*hcgs);
  }

  // local to global
  state ltog(state local) {
    state global;
    double theta = angle::wrap(this->theta() + local.theta());
    return state(state::_x + local.x()*cos(theta), state::_y + local.x()*sin(theta), theta);
  }

  void zero() {
    state::_x = 0;
    state::_y = 0;
    state::_theta = PI/2;
  }
};
#endif
