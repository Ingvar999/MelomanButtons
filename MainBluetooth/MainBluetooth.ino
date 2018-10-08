#include <RCSwitch.h>
#include <MsTimer2.h>

const byte LOWVOLTAGE = bit(2);
const byte NOTHING = bit(3);
const byte GREENLIGHT = bit(4);
const byte REDLIGHT = bit(5);
const byte GREENBLINK = bit(6);
const byte REDBLINK = bit(7);
const int redLed = 13;
const int greenLed = 12;
const uint16_t threshold1 = 4900;
const uint16_t threshold2 = 4700;

uint32_t Vcc;
RCSwitch mySwitch = RCSwitch();
bool activeButtons[4] = {true, true, true, true};
const byte id = 0;
bool isPushed = false;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);

  mySwitch.enableReceive(0);
  mySwitch.enableTransmit(4);
  mySwitch.setRepeatTransmit(7);

  attachInterrupt(1, OnPushButton, RISING);
  MsTimer2::set(1000, Timer);
  MsTimer2::start();

  Blink(greenLed);
}

void loop() {
  if (isPushed){
    delay(50);
  }
  
  ButtonsDataHandle();

  if (isPushed) {
    Serial.print(String(id) + "p");
    isPushed = false;
  }

  HostDataHandle();
}

void ButtonsDataHandle(){
   if ( mySwitch.available() ) {
    byte value = mySwitch.getReceivedValue();
    byte senderId = value & 3;
    switch (value & 252) {
      case LOWVOLTAGE:
        Serial.print(String(senderId) + "l");
        break;
      case 0:
        if (activeButtons[senderId]) {
          Serial.print(String(senderId) + "p");
          activeButtons[senderId] = false;
        }
        break;
    }
    mySwitch.resetAvailable();
  }
}

void HostDataHandle(){
  if ( Serial.available() ) {
    String value = Serial.readString();
    if (value.length() == 2) {

      if (value[0] == '0' + id)
        switch (value[1]) {
          case 'n':
            digitalWrite(redLed, LOW);
            digitalWrite(greenLed, LOW);
            break;
          case 'r':
            digitalWrite(greenLed, LOW);
            digitalWrite(redLed, HIGH);
            break;
          case 'g':
            digitalWrite(redLed, LOW);
            digitalWrite(greenLed, HIGH);
            break;
          case 'y':
            Blink(redLed);
            break;
          case 'u':
            Blink(greenLed);
            break;
        }
      else if (value[0] >= '1' && value[0] <= '3')
        switch (value[1]) {
          case 'n':
            mySwitch.send((value[0] - '0') | NOTHING, 8);
            break;
          case 'r':
            mySwitch.send((value[0] - '0') | REDLIGHT, 8);
            break;
          case 'g':
            mySwitch.send((value[0] - '0') | GREENLIGHT, 8);
            break;
          case 'y':
            mySwitch.send((value[0] - '0') | REDBLINK, 8);
            break;
          case 'u':
            mySwitch.send((value[0] - '0') | GREENBLINK, 8);
            break;
        }
    }
  }
}

void OnPushButton() {
  if (activeButtons[0]) {
    isPushed = true;
    activeButtons[0] = false;
  }
}

void Timer() {
  for (int i = 0; i < 4; ++i)
    activeButtons[i] = true;
  if (Vcc = ReadVcc() < threshold1) {
    Serial.print(String(id) + "l");
    if (Vcc < threshold2)
      Blink(redLed);
  }
  if (Vcc = (((uint32_t)analogRead(A5) * 10000) >> 10) < threshold1) {
    Serial.print("bl");
    if (Vcc < threshold2)
      Blink(greenLed);
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
