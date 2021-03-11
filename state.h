#ifndef STATE_H
#define STATE_H
/**********************************************/
/******************* state.h ******************/
/**********************************************/
#include "helper.h"

struct state_vector {
  double _x = 0;
  double _y = 0; 
  double _theta = 0;

  enum element {
    x, y, theta
  };

  state_vector(double x = 0, double y = 0, double theta = 0) : _x(x), _y(y), _theta(theta) { }
  state_vector(const state_vector& in) : _x(in._x), _y(in._y), _theta(in._theta) { }
};

class state : public angle, protected state_vector, private wheel {
public:
  state(double x = 0, double y = 0, double theta = 0) : state_vector(x, y, wrap(theta)) { }
  state(const state& in) : state_vector(in) { }
        
  inline double x()      { return _x;     }
  inline double y()      { return _y;     }
  inline double theta()  { return _theta; }
  
  inline state copy() { return *this; }
  void print() { Serial_print("%6s, %6s, %6s", String(x(), 3).c_str(), String(y(), 3).c_str(), String(theta(), 3).c_str()); }

  state& operator=(state rhs) noexcept {
    this->_x = rhs.x();
    this->_y = rhs.y();
    this->_theta = rhs.theta();
    return *this;
  }

  double& operator()(state_vector::element e) {
    switch(e) {
      case element::x:
        return state_vector::_x;
      case element::y:
        return state_vector::_y;
      case element::theta:
        _theta = wrap(_theta);
        return state_vector::_theta;
    }
  }

  state& operator+=(const state& rhs) {
    this->_x += rhs._x;
    this->_y += rhs._y;
    this->_theta = wrap(this->_theta + rhs._theta);
    return *this;
  }
  
  friend state operator+(state lhs, const state& rhs) { return (lhs += rhs); }

  state& operator-=(const state& rhs) {
    this->_x -= rhs._x;
    this->_y -= rhs._y;
    this->_theta = wrap(this->_theta - rhs._theta);
    return *this;
  }
  
  friend state operator-(state lhs, const state& rhs) { return (lhs -= rhs); }

  float distance() {
    double x = this->_x;
    double y = this->_y;
    return sqrt(x*x+y*y);
  }

  float angle() {
    double x = this->_x;
    double y = this->_y;
    // atan2 -PI -> PI, no wrap needed
    return atan2(y, x);
  }

  float d_x(state& lhs)     { return lhs.x() - this->_x;         }
  float d_y(state& lhs)     { return lhs.y() - this->_y;         }
  float d_theta(state& lhs) { return wrap(lhs.theta() - this->_theta); }
  state dState(state lhs)   { return state(d_x(lhs), d_y(lhs), d_theta(lhs)); }

};

#endif
