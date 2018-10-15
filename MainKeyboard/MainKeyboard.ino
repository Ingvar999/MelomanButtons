#include <TimerThree.h>
#include <RCSwitch.h>
#include <Keyboard.h>

const byte LOWVOLTAGE = bit(2);
const uint16_t threshold1 = 5000;

RCSwitch mySwitch = RCSwitch();
bool activeButtons[4] = {true, true, true, true};
const byte id = 0;
bool isPushed = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT);
  
  mySwitch.enableReceive(1);

  attachInterrupt(0, OnPushButton, FALLING);
  
  Timer3.initialize(1000000);
  Timer3.attachInterrupt(Timer);
  Timer3.start();
}

void loop() {
  if (isPushed)
    delay(50);

 ButtonsDataHandle();
  
  if (isPushed) {
    Keyboard.print('0');
    isPushed = false;
  }
}
//
void ButtonsDataHandle(){
   if ( mySwitch.available() ) {
    byte value = mySwitch.getReceivedValue();
    char senderId = (value & 3) + '0';
    switch (value & 252) {
      case LOWVOLTAGE:
        Keyboard.print("l" + senderId);
        break;
      case 0:
        if (activeButtons[senderId-'0']) {
          Keyboard.print(senderId);
          activeButtons[senderId-'0'] = false;
        }
        break;
    }
    mySwitch.resetAvailable();
  }
}
//
void Timer() {
  for (int i = 0; i < 4; ++i)
    activeButtons[i] = true;
    if ((((uint32_t)analogRead(A1) * 9200) >> 10) < threshold1)
      Keyboard.print("l0"); 
}

void OnPushButton(){
  if (activeButtons[0]){
    isPushed = true;
    activeButtons[0] = false;
  }
}
