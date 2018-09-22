#include <RCSwitch.h>
#include <MsTimer2.h>

const byte LOWVOLTAGE = bit(2);
const byte NOTHING = bit(3);
const byte GREENLIGHT = bit(4);
const byte REDLIGHT = bit(5);
const byte GREENBLINK = bit(6);
const byte REDBLINK = bit(7);
const byte redLed = 13;
const byte greenLed = 12;

RCSwitch mySwitch = RCSwitch();
bool activeButton = true;
byte id;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);

  mySwitch.enableReceive(0);
  mySwitch.enableTransmit(4);

  attachInterrupt(1, OnPushButton, RISING);
  MsTimer2::set(1000, Timer);
  MsTimer2::start();

  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  id = 1 + digitalRead(10) + digitalRead(11);
  digitalWrite(8, LOW);

  Blink(greenLed);
  Serial.println(id);
}

void loop() {
  if ( mySwitch.available() ) {
    byte value = mySwitch.getReceivedValue();

    if ((value & 3) == id)
      switch (value & 252) {
        case NOTHING:
          digitalWrite(redLed, LOW);
          digitalWrite(greenLed, LOW);
          break;
        case REDLIGHT:
          digitalWrite(greenLed, LOW);
          digitalWrite(redLed, HIGH);
          break;
        case GREENLIGHT:
          digitalWrite(redLed, LOW);
          digitalWrite(greenLed, HIGH);
          break;
        case REDBLINK:
          Blink(redLed);
          break;
        case GREENBLINK:
          Blink(greenLed);
          break;
      }

    mySwitch.resetAvailable();
  }
}

void OnPushButton() {
  if (activeButton) {
    mySwitch.send(id, 3);
    Serial.print(id);
    activeButton = false;
  }
}

void Timer() {
  activeButton = true;
  if (ReadVcc() < 4800) {
    mySwitch.send(id & LOWVOLTAGE, 3);
    Blink(redLed);
  }
}

void Blink(byte pin) {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(pin, HIGH);
  delay(200);
  digitalWrite(pin, LOW);
}

uint32_t ReadVcc()
{
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

  delay(30);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  uint8_t low  = ADCL;
  uint8_t high = ADCH;
  uint32_t result = (high << 8) | low;
  result = 1125300UL / result;
  return result;
}
