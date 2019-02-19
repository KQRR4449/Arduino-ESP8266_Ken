// Column locations.
//3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
//       1         2         3         4         5         6         7         8         9     9
//const unsigned long FAKE_LONG     =          10000UL; // Fake variable for column positions.

const byte            VER_MAJOR     =                1; // Major version.
const byte            VER_MINOR     =                1; // Minor version.
const byte            BLINK_PIN     =                2; // Pin number of blink LED.

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.printf_P(PSTR("\nStarting BlinkKen version %d.%d, Build date %s %s\n"),  VER_MAJOR,
                                                                                  VER_MINOR,
                                                                                  __DATE__,
                                                                                  __TIME__);
      
  pinMode(BLINK_PIN, OUTPUT);
}

void loop() {
  static unsigned long  count       =              0UL; // Loop counter.

  Serial.printf_P(PSTR("Loop %10lu\n"), ++count);
  
  digitalWrite(BLINK_PIN, HIGH);
  delay(500);
  digitalWrite(BLINK_PIN, LOW);
  delay(500);
}
