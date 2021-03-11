#include <avr/sleep.h>

union v16_t {
  struct {
    byte low;
    byte high;
  };
  uint16_t value;
};

class global_clock_t {
protected:
  unsigned int elapsed_ms = 0;
  static constexpr unsigned int ms_per_sec = 1000;
  static constexpr unsigned int sec_per_min = 60;
  unsigned int elapsed_min = 0;
  static constexpr unsigned int min_per_hour = 60;
  static constexpr unsigned int hour_per_day = 24;
  unsigned int elapsed_day = 0;
  
public:
  global_clock_t() {
    v16_t v16;
    TCCR4B = 0x00;
    TCCR4A = 0x00;
    TCCR4C = 0x00;
    TCCR4D = 0x00;
    TCCR4E = 0x00;
    v16.value = 1000;
    TC4H = v16.high;
    OCR4C = v16.low;
    v16.value = 0;
    TC4H = v16.high;
    TCNT4 = v16.low;
  }

  void begin() {
    TIMSK4 = 0x05;
    TCCR4B = 0x04;
  }
  
  unsigned int millis() {
    return elapsed_ms % ms_per_sec;
  }
  
  unsigned int micros() {
    v16_t v16;
    v16.low = TCNT4;
    v16.high = TC4H;
    return v16.value;
  }
  
  unsigned int secs() {
    return elapsed_ms / ms_per_sec;
  }
  
  unsigned int mins() {
    return elapsed_min % min_per_hour;
  }
  
  unsigned int hours() {
    return elapsed_min / min_per_hour;
  }
  
  unsigned int days() {
    return elapsed_day;
  }
} static global_clock;

ISR(TIMER4_OVF_vect) {
  unsigned char sreg = SREG;
  cli();
  struct c : global_clock_t { 
    void inc() { 
      if (++elapsed_ms == ms_per_sec * sec_per_min) {
        if (++elapsed_min == min_per_hour * hour_per_day) {
          ++elapsed_day;
          elapsed_min = 0;
        }
        elapsed_ms = 0;
      }
    } 
  } * clk = (c*) &global_clock;
  clk->inc();
  SREG = sreg;
}

#define setPort(port, mask) (port |=  (byte) mask)
#define clrPort(port, mask) (port &= ~(byte) mask)
#define tglPort(port, mask) (port ^=  (byte) mask)

#define P0 (0x01)
#define P1 (0x02)
#define P2 (0x04)
#define P3 (0x08)
#define P4 (0x10)
#define P5 (0x20)
#define P6 (0x40)
#define P7 (0x80)

class HD44780 {
  static constexpr unsigned char RS = 8;// -> PB4
  static constexpr unsigned char E = 9; // -> PB5
  // data 4 -> PD4
  // data 5 -> PC6
  // data 6 -> PD7
  // data 7 -> PE6
  
  // instructions
  static constexpr unsigned char clear_display =          0x01;
  static constexpr unsigned char return_home =            0x02;
  static constexpr unsigned char entry_mode_set =         0x04;
  static constexpr unsigned char  address_decrements =    0x01;
  static constexpr unsigned char  address_increments =    0x00;
  static constexpr unsigned char  display_shift =         0x02;
  static constexpr unsigned char display_io_control =     0x08;
  static constexpr unsigned char  display_on =            0x04;
  static constexpr unsigned char  cursor_on =             0x02;
  static constexpr unsigned char  cursor_blink =          0x01;
  static constexpr unsigned char cursor_display_shift =   0x10;
  static constexpr unsigned char  shift_cursor_left =     0x00;
  static constexpr unsigned char  shift_cursor_right =    0x04;
  static constexpr unsigned char  shift_display_left =    0x08;
  static constexpr unsigned char  shift_display_right =   0x0C;
  static constexpr unsigned char function_set =           0x20;
  static constexpr unsigned char  two_lines_small_font =  0x08;
  static constexpr unsigned char  one_line_small_font =   0x00;
  static constexpr unsigned char  one_line_large_font =   0x04;
  static constexpr unsigned char set_cgram_address =      0x40;
  static constexpr unsigned char  cgram_address_mask =    0x3F;
  static constexpr unsigned char set_ddrram_address =     0x80;
  static constexpr unsigned char  ddrram_address_mask =   0x7F;

  inline void w4(unsigned char data) {
    //digitalWrite(E, HIGH);
    setPort(PORTB, 0x20); // PB5 HIGH 0 1 2 3 4 5
    
    //digitalWrite(7, data & 0x08);
    //digitalWrite(6, data & 0x04);
    //digitalWrite(5, data & 0x02);
    //digitalWrite(4, data & 0x01);
    PORTE = data & 0x08 ? PORTE | P6 : PORTE & ~P6;
    PORTD = data & 0x04 ? PORTD | P7 : PORTD & ~P7;
    PORTC = data & 0x02 ? PORTC | P6 : PORTC & ~P6;
    PORTD = data & 0x01 ? PORTD | P4 : PORTD & ~P4;
    
    #define rs(port, mask, sr) sr ? setPort(port, mask) : clrPort(port, mask) 
    //rs(PORTE, 0x40, data & 0x08); // 7:PE6
    //rs(PORTD, 0x80, data & 0x04); // 6:PD7
    //rs(PORTC, 0x40, data & 0x02); // 5:PC6
    //rs(PORTD, 0x10, data & 0x01); // 4:PD4
    #undef rs
    delayMicroseconds(40);
    //digitalWrite(E, LOW);
    clrPort(PORTB, 0x20); // PB5 LOW
  }

  void write_4(unsigned char data) {
    if (0) {
      Serial.print("setting port to ");
      Serial.println(data & 0x0F, HEX);
    }
    w4(data);
    delayMicroseconds(5);
  }

  void write(unsigned char data) {
    write_4(data>>4);
    write_4(data);
  }
  
  void instruct(unsigned char instruction) {
    if (0) {
      Serial.print("Sending instruction ");
      if (instruction < 16) Serial.print('0');
      Serial.println(instruction, HEX);
    }
    digitalWrite(RS, LOW);
    write(instruction);
    delayMicroseconds(40);
    if ( (instruction | 0x01) == 0x03 ) delayMicroseconds(1500);
  }
  
public:
  HD44780() {
    // pins 4,5,6,7 are data
    // pin  8 -> RS, register select. 0: instruction register, 1: data
    // 9 -> E -> start write

    
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(RS, OUTPUT);
    //DDRD |= 0x90; // pin 4, 6
    //DDRC |= 0x40; // pin 5
    //DDRE |= 0x40; // pin 7
    //DDRB |= 0x30; // pin 8 (RS), 9(E)
    digitalWrite(RS, LOW);
    //PORTB &= ~0x10;
    digitalWrite(7, 0);
    digitalWrite(6, 0);
    digitalWrite(5, 1);
    digitalWrite(4, 0);
    //PORTD &= ~0x90; // pin 4, 6
    //PORTC |=  0x40; // pin 5
    //PORTE &= ~0x40; // pin 7
    //PORTB &= ~0x30; // pin 8 (RS), 9(E)
    
    pinMode(E, OUTPUT);
    digitalWrite(E, LOW);
    //PORTB &= ~0x20;
  }
  
  void begin() {
    delay(5);
    instruct(function_set | two_lines_small_font);
    delay(5);
    //instruct(cursor_display_shift | shift_cursor_left | shift_display_right);
    instruct(display_io_control | display_on | cursor_on);
    instruct(entry_mode_set | display_shift);
    clear();
  }

  void send(char c) {
    if (0) {
      Serial.print("Sending data ");
      if (c < 16) Serial.print('0');
      Serial.print(c, HEX);
      Serial.print(" [");
      Serial.print(c);
      Serial.println(']');
    }
    digitalWrite(RS, HIGH);
    write(c);
  }

  void clear() {
    instruct(clear_display);
    delay(5);
  }

  void line1() {
    instruct(set_ddrram_address | 0x00);
  }
  
  void line2() {
    instruct(set_ddrram_address | 0x40);
  }
  
} static LCD;

void regdump() {
  static constexpr size_t values_per_line = 16;
  static constexpr size_t from_addr = 0x20;
  static constexpr size_t to_addr = 0xff;
  size_t printed;
  Serial.println("Register file:");
  Serial.print("  | ");
  for (size_t i = 0; i < values_per_line; i++) {
    printed = Serial.print(i, HEX);
    while (printed < 3) printed += Serial.print(' ');
  }
  Serial.println();
  Serial.print("--+-");
  for (size_t i = 0; i < values_per_line; i++) Serial.print("---");
  Serial.println();
  for (size_t i = from_addr; i < to_addr;) {
    printed = Serial.print(i, HEX);
    while (printed < 2) printed += Serial.print(' ');
    printed += Serial.print('|');
    for (size_t j = i + values_per_line; i < j && i < to_addr; i++) {
      unsigned char * b = nullptr;
      Serial.print(' ');
      printed = Serial.print(b[i], HEX);
      while (printed < 2) printed += Serial.print(' ');
    }
    Serial.println();
  }
  
  v16_t v16;
    
  Serial.print("OCR4C: ");
  v16.low = OCR4C;
  v16.high =  TC4H;
  Serial.println(v16.value, HEX);

  Serial.print("TCNT4: ");
  v16.low = TCNT4;
  v16.high =  TC4H;
  Serial.println(v16.value, HEX);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  global_clock.begin();
  delay(3000);
  LCD.begin();
  pinMode(13, OUTPUT);
  for (int i = 0; i < 10; i++) {
    digitalWrite(13, i % 2);
    delay(500);
  }
}

unsigned int last_sec = 0;

void loop() {
  sleep_disable();
  
  if ( global_clock.secs() != last_sec ) {
    char str[16];
    last_sec = global_clock.secs();
    unsigned int h = global_clock.hours();
    int n = sprintf(str, "%2d:%02d:%02d %s", 
                              (h%12) + 1, 
                              global_clock.mins(),
                              global_clock.secs(),
                              h < 12 ? "AM" : "PM");
    LCD.line1();
    for (int i = 0; i < 16; i++) LCD.send(i < n ? str[i] : ' ');
    enum {
         Monday,
        Tuesday,
      Wednesday,
       Thursday,
         Friday,
       Saturday,
         Sunday,
    };

    #define s(x, n) (((char*)F(#x))[n])
    #define c(x) ((char*)F(#x))

    const char * mon = c(Monday);
    
    LCD.line2();
    n = global_clock.days();
    LCD.send(n == Wednesday ?  s(Wednesday, 0) : ' ');
    LCD.send(n == Wednesday ? s(Wednesday, 1) : n == Thursday ? s(Thursday, 0) : n == Saturday ? s(Saturday, 0) : ' ');
    LCD.send(n == Tuesday ? s(Tuesday, 0) : n == Wednesday ? s(Wednesday, 2) : n == Thursday ? s(Thursday, 1) : n == Saturday ? s(Saturday, 1) : ' ');
    LCD.send(n == Monday ? mon[0] : n == Tuesday ? s(Tuesday, 1) : n == Wednesday ? s(Wednesday, 3) : n == Thursday ? s(Thursday, 2) : n == Friday ? s(Friday, 0) : n == Saturday ? s(Saturday, 2) : s(Sunday, 0));
    LCD.send(n == Monday ? mon[1] : n == Tuesday ? s(Tuesday, 2) : n == Wednesday ? s(Wednesday, 4) : n == Thursday ? s(Thursday, 3) : n == Friday ? s(Friday, 1) : n == Saturday ? s(Saturday, 3) : s(Sunday, 1));
    LCD.send(n == Monday ? mon[2] : n == Tuesday ? s(Tuesday, 3) : n == Wednesday ? s(Wednesday, 5) : n == Thursday ? s(Thursday, 4) : n == Friday ? s(Friday, 2) : n == Saturday ? s(Saturday, 4) : s(Sunday, 2));
    LCD.send(mon[3]);
    LCD.send(mon[4]);
    LCD.send(mon[5]);

    #undef c
    #undef s
    
    //for (int i = 0; i < 16; i++) LCD.send(i < n ? str[i] : ' ');
    //digitalWrite(13, !digitalRead(13));
    //~PC7
    tglPort(PINC, P7);
  }
  
  sleep_enable();
  sei();
  sleep_cpu();
}
