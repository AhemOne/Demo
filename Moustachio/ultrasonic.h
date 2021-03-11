#pragma once
#include "helper.h"
#include "hardware_definition.h"
/**********************************************/
/**************** ultrasonic.h  ***************/
/**********************************************/

namespace hardware {
  template <pin_t vcc_pin, pin_t gnd_pin, pin_t tx_pin, pin_t rx_pin>
  class ultrasonic {
  private:
    using VCC = digital_pin<vcc_pin>;
    using GND = digital_pin<gnd_pin>;
    using TX = digital_pin<tx_pin>;
    using RX = digital_pin<rx_pin>;

    static constexpr long int timeout = 8000;
    static constexpr double m = 1/5.81;
    static constexpr double b = 24;
    
  
  public:
    ultrasonic() {
      VCC::config_io_mode(io_mode::output);
      GND::config_io_mode(io_mode::output);
      TX::config_io_mode(io_mode::output);
      RX::config_io_mode(io_mode::input);
    }
    
    auto enable () -> void {
      VCC::high();
      GND::low();
    }
    
    auto stop () -> void {
      VCC::low();
      GND::low();
    }
    
    auto reading () -> double {
      TX::high();
      delayMicroseconds(timeout/2);
      TX::low();
      unsigned long int t = pulseIn(RX::pin_number, HIGH, timeout/2);
      return t ? m*(t - b) : 0;
    }
  };
};

