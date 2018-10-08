#include <Keyboard.h>
#include <RCSwitch.h>
#include <MsTimer2.h>

const byte LOWVOLTAGE = bit(2);

RCSwitch mySwitch = RCSwitch();
bool activeButtons[4] = {true, true, true, true};
const byte id = 0;
bool isPushed = false;

void setup() {
 
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(3, INPUT_PULLUP);
  
  mySwitch.enableReceive(0);

  attachInterrupt(1, OnPushButton, FALLING);
  MsTimer2::set(1000, Timer);
  MsTimer2::start();
}

void loop() {
  if (isPushed){
    delay(50);
  }
  
  ButtonsDataHandle();

  if (isPushed) {
    Keyboard.print('0');
    isPushed = false;
  }
}

void ButtonsDataHandle(){
   if ( mySwitch.available() ) {
    byte value = mySwitch.getReceivedValue();
    byte senderId = value & 3;
    switch (value & 252) {
      case LOWVOLTAGE:
        Keyboard.print(String(senderId) + "l");
        break;
      case 0:
        if (activeButtons[senderId]) {
          Keyboard.print(senderId + '0');
          activeButtons[senderId] = false;
        }
        break;
    }
    mySwitch.resetAvailable();
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
}
