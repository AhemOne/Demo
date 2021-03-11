#ifndef HELPER_H
#define HELPER_H
/**********************************************/
/****************** helper.h  *****************/
/**********************************************/

#define SERIAL_LINK Serial1

static char STRING_FOR_SERIAL_PRINT_MACRO[64];
#define  Serial_print(a, ...)   sprintf_P(STRING_FOR_SERIAL_PRINT_MACRO, PSTR(a), __VA_ARGS__); SERIAL_LINK.print(STRING_FOR_SERIAL_PRINT_MACRO);
#define bluetooth SERIAL_LINK

struct angle {
  double wrap(float b) {
    while (b > PI) b -= 2*PI;
    while (b < -PI) b += 2*PI;
    return b;
  }
};

class wheel {
public:
  static constexpr double diameter = 48.5;        // mm
  static constexpr double radius = diameter/2.0; // mm
  static constexpr double separation = 102;      // distance between wheels, mm
  static constexpr double separation_radius = separation/2;      // distance between wheels, mm
};

class timekeeper {
private:
  unsigned long update_period_ms;
  unsigned long next_update_ms;

protected:
  virtual void _update() = 0;
  
public:
  timekeeper(unsigned long int period_ms = 1) : update_period_ms(period_ms) { }

  void update() {
    unsigned long now = millis();
    // if next update has overflowed then now will falsely be > next, check now+period. this will overflow now and fail
    if (now > next_update_ms && (now + update_period_ms) > next_update_ms) {
      // update timer
      next_update_ms += update_period_ms;

      _update();
    }
  }

  void set_update_period(unsigned long new_period_ms) {
    update_period_ms = new_period_ms;
  }

  double dt() {
    return update_period_ms/1000.0;
  }
};

#endif
