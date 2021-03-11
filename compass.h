#ifndef COMPASS_H
#define COMPASS_H
/**********************************************/
/****************** compass.h *****************/
/**********************************************/
#include <Arduino.h>
#include "helper.h"
#include <Wire.h>
#include "state_controller.h"

class compass_hm : public timekeeper, protected angle {
private:
  double _global_north;
  double _local_north;
  double _global_tilt;
  double _local_tilt;

  int16_t max_y = -1024, min_y = 1024;

  static uint8_t constexpr base_address = (0x3C >> 1);
  static constexpr float R = PI/180*3;
  
  enum reg : byte {
    configuration_A = 0,
    configuration_B = 1,
    mode = 2,
    data = 3,
    status = 9,
  };
  
  struct mask {
    enum configuration_A : byte {
      samples = B01100000,
      rate    = B00011100,
      bias    = B00000011
    };
    
    enum status : byte {
      ready = B00000001,
      lock  = B00000010,
    };
  };

  void _update() {
    // request status byte
    Wire.beginTransmission(base_address);
    Wire.write(reg::status);
    Wire.endTransmission();

    // read status byte
    Wire.requestFrom(base_address, (uint8_t) 1);
    while (Wire.available() < 1);
    byte status = Wire.read();

    // when status shows a new reading
    if (status & mask::status::ready) {
      // request data
      Wire.beginTransmission(base_address);
      Wire.write(reg::data);
      Wire.endTransmission();

      // read data
      int16_t X, Y, Z;
      Wire.requestFrom(base_address, (uint8_t) 6);
      while (Wire.available() < 6);   // wait for 6 bytes
      
      X = Wire.read() << 8;
      X |= Wire.read();           // read in X data
      Z = Wire.read() << 8;
      Z |= Wire.read();           // read in Z data
      Y = Wire.read() << 8;
      Y |= Wire.read();           // read in Y data

      if (Y > max_y) max_y = Y;
      if (Y < min_y) min_y = Y;
      int16_t Y_o = Y - (max_y + min_y)/2;

      // reading is  local to robot
      _local_north = atan2(Y_o, X);
      
      //Serial_print("bearing: %s deg ", String(wrap(_global_north - _local_north)*180/PI).c_str());
      //Serial_print("x: %d ",  X);
      //Serial_print("y: %d ",  Y_o);
      //Serial_print("bearing: %s\n",  String(bearing()*180/PI).c_str());
      //Serial_print("z: %d (%d)\n", Z, max_z - min_z);
      //Serial_print("tilt: %s deg\n",   String(wrap(_global_tilt  - _local_tilt )*180/PI).c_str());

      innovate(pose, bearing());
    }
  }
  
  void innovate(Pose & pose, float bearing) {
    pose.innovate_theta(bearing + PI/2, R);
  }
  
public:
  compass_hm() : timekeeper(1000/75) { }

  void begin(bool verbose = false) {
    Wire.begin();
    Wire.beginTransmission(base_address);
    Wire.write(reg::configuration_A);      // write from 
    Wire.write(B00111000);      // set config A - 2 samples averaged, 75 Hz, no bias
    Wire.write(B00000000);      // set config B - maximum gain
    Wire.write(B00000000);      // set mode - continuous
    Wire.endTransmission();
    zero();
    if (verbose) report();
  }
  
  void print_hardware() {    
    Serial.println(F("Compass attached"));
    Serial.println(F(" Connections:"));
    Serial.println(F("  Using SDA, SCL (i2c)"));
  }

  void print_constant() {   
    Serial.println(F(" Compass constants:"));
    Serial_print("  north = %s rad\n", String(_global_north, 3).c_str());
  }

  void print_status() {    
    update();
    Serial.println(F(" Compass status:"));
    Serial_print("  bearing: %s radians\n", String(bearing(), 3).c_str());
    Serial_print("  tilt:    %s radians\n", String(tilt(), 3).c_str());
  }

  void report() {
    print_hardware();
    print_constant();
    print_status();
  }

  void calibrate() { 
    zero();
    update();
    reset();
  }
  
  void reset()   {
    _global_north = _local_north;
    _global_tilt = _local_tilt;
  } 
  
  void zero()    { 
    _global_north = 0;  // calibrated north
    _local_north = 0;
    _global_tilt = 0;
    _local_tilt = 0;
  } 
  
  double bearing() { return wrap(_global_north - _local_north); }
  double tilt()    { return wrap(_local_tilt);                  }
};

#endif
