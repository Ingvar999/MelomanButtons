#include <TimerThree.h>
#include <RCSwitch.h>
#include <Keyboard.h>

const byte LOWVOLTAGE = bit(2);

RCSwitch mySwitch = RCSwitch();
bool activeButtons[4] = {true, true, true, true};
const byte id = 0;

void setup() {
  Keyboard.begin();
  pinMode(3, INPUT_PULLUP);
  
  mySwitch.enableReceive(1);

  Timer3.initialize(1000000);
  Timer3.attachInterrupt(Timer);
  Timer3.start();
}
//22
void loop() {

 ButtonsDataHandle();

  if (activeButtons[0] && digitalRead(3) == LOW) {
    Keyboard.print('0');
    activeButtons[0] = false;
  }
}

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

void Timer() {
  for (int i = 0; i < 4; ++i)
    activeButtons[i] = true;
}
