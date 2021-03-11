#ifndef HARDWARE_H
#define HARDWARE_H
/**********************************************/
/***************** hardware.h *****************/
/**********************************************/
#include "Arduino.h"
#include "helper.h"

namespace hardware {
  enum class io_mode : uint8_t {
    input         = 0x00U,
    output        = 0x01U,
    input_pullup  = 0x02U,
    unset         = 0xFF
  };

  enum class logic_level : uint8_t {
    low   = 0,
    high  = 1
  };

  enum class interrupt_mode : int {
    low     = 0,
    change  = 1,
    rising  = 2,
    falling = 3
  };

  using pin_t         = uint8_t;
  using encoder_count = int;

  template <pin_t pin>
  class digital_pin {
  public:
    static constexpr auto pin_number = pin;
    static auto config_io_mode (io_mode mode) -> void { pinMode(pin, (int) mode); }
    static auto read () -> logic_level                { return (logic_level) digitalRead(pin); }
    static auto write (logic_level level) -> void     { digitalWrite(pin, (int) level); }
    static auto high () -> void                       { digitalWrite(pin, HIGH); }
    static auto low () -> void                        { digitalWrite(pin, LOW); }
    //static auto pwm_write (units::percentage duty_cycle) -> void;
    //static auto pulse_length (logic_level state = logic_level::high,
    //    units::microseconds timeout = 1000000_us) -> units::microseconds;
  };

  enum class analog_reference : uint8_t
  {
      vcc_default,
      internal,
      internal_1v1,
      internal_2v56,
      external
  };
  
  template <pin_t pin>
  class analog_pin : public digital_pin<pin>
  {
      static constexpr auto conversion_unit = 1024;
      static analog_reference ref_;
  
      public:
      static auto set_analog_reference (analog_reference ref) -> void { ref_ = ref; }
      static auto analog_read () -> double { return (double) analogRead(pin)/conversion_unit; }
  };

  template <typename pin>
  class interrupt : public digital_pin<pin::pin_number> {
  public:
    static auto attach_interrupt (void (*callback) (),
        interrupt_mode mode = interrupt_mode::rising) -> void;
    static auto detach_interrupt () -> void;
  };

  template <pin_t pin_a, pin_t pin_b, pin_t pin_c, pin_t pin_d, unsigned int steps_per_revolution = 48>
  class unipolar_stepper {
  private:
    int   _abs_angle;
    int   _enc_angle;
    bool  _enabled;
    
  private:
    using pin_A = digital_pin<pin_a>;
    using pin_B = digital_pin<pin_b>;
    using pin_C = digital_pin<pin_c>;
    using pin_D = digital_pin<pin_d>;
    
    void set_for_position(int position) {
      if (position < 0) {
        pin_A::low();
        pin_B::low();
        pin_C::low();
        pin_D::low();
      } else switch(position&0x3) {
            case 0:
                //Serial.print("AB");
                pin_A::high();
                pin_B::high();
                pin_C::low();
                pin_D::low();
                break;
            case 1:
                //Serial.print("BC");
                pin_A::low();
                pin_B::high();
                pin_C::high();
                pin_D::low();
                break;
            case 2:
                //Serial.print("CD");
                pin_A::low();
                pin_B::low();
                pin_C::high();
                pin_D::high();
                break;
            case 3:
                //Serial.print("DA");
                pin_A::high();
                pin_B::low();
                pin_C::low();
                pin_D::high();
                break;
            default:
                pin_A::low();
                pin_B::low();
                pin_C::low();
                pin_D::low();
                break;
        }
    }

  public:
    unipolar_stepper() {
      pin_A::config_io_mode(io_mode::output);
      pin_B::config_io_mode(io_mode::output);
      pin_C::config_io_mode(io_mode::output);
      pin_D::config_io_mode(io_mode::output);
      stop();
    }

    void begin(bool verbose = false) {
      enable();
      delay(10);
      enc_reset();
      stop();
      if (verbose) report();
    }
    
    void print_hardware() {      
      Serial.println(F("Unipolar stepper motor attached"));
      Serial.println(F(" Connections:"));
      Serial_print("  Pin A: digital pin %d\n", pin_A::pin_number);
      Serial_print("  Pin B: digital pin %d\n", pin_B::pin_number);
      Serial_print("  Pin C: digital pin %d\n", pin_C::pin_number);
      Serial_print("  Pin D: digital pin %d\n", pin_D::pin_number);
    }
  
    void print_constant() {     
      Serial.println(F(" Motor constants:")); 
      Serial_print("  Steps per revolution: %d\n", steps_per_rev());
      Serial_print("  Step size: %s radians\n", String(step_size_rad(),3).c_str());
    }
  
    void print_status() {      
      Serial.println(F(" Motor status:"));
      Serial_print("  Motor is currently: %sabled\n", _enabled?"En":"Dis");
      Serial_print("  Encoder absolute position: %d steps\n", abs_count());
      Serial_print("  Encoder relative position: %d steps\n", enc_count());
    }

    void report() {
      print_hardware();
      print_constant();
      print_status();
    }
    
    auto enable () -> void {
      _enabled = true;
      set_for_position(_abs_angle);
    }
    
    auto stop () -> void {
      _enabled = false;
      set_for_position(-1);
    }
    
    auto step_forward () -> void {
        if (_enabled) {
          ++_enc_angle;
          ++_abs_angle;
          set_for_position(_abs_angle&0x3);
        }
    }
    
    auto step_backward () -> void {
        if (_enabled) {
          --_enc_angle;
          --_abs_angle;
          set_for_position(_abs_angle&0x3);
        }
    }

    inline auto steps_per_rev () -> int    { return steps_per_revolution; }
    inline auto step_size_rad () -> double { return (2*PI)/steps_per_rev(); }

    inline auto abs_count () -> int        { return _abs_angle; }
    inline auto abs_rad () -> double       { return abs_count()*step_size_rad(); }
    
    inline auto enc_count () -> int        { return _enc_angle; }
    inline auto enc_rad () -> double       { return enc_count()*step_size_rad(); }
    inline auto enc_reset () -> void       { _enc_angle = 0; }
  };

  template <pin_t pin> 
  class LED {
  private:
    using Pin = digital_pin<pin>;

  public:
    LED() { 
      Pin::config_io_mode(io_mode::output);
      off();
    }
    void on()  { Pin::high();  }
    void off() { Pin::low(); }
  };
  
  template <pin_t pin1, pin_t pin2>
  class bicolour_LED {
    private:
      using Pin1 = digital_pin<pin1>;
      using Pin2 = digital_pin<pin2>;
      
    public:
      bicolour_LED() {
        Pin1::config_io_mode(io_mode::output);
        Pin2::config_io_mode(io_mode::output);
        off(); 
      }
      void red()    { Pin1::low(); Pin2::high(); }
      void green()  { Pin2::low(); Pin1::high(); }
      void off()    { Pin1::low(); Pin2::low(); }
  };
}    // namespace hardware

#endif
