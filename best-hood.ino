#define RELAY1  5
#define RELAY2  4
#define RELAY3  3
#define RELAY4  2

#define RELAY_PAUSE 250

class Button {
  public:
    unsigned long _previous_LOW_ms = 0;
    unsigned long _previous_HIGH_ms = 0;
    unsigned long _gap = 0;
    unsigned long _ms = 0;
    int _maximum_gap = 1000;
    int _minimum_gap = 35;
};

Button Buttons[3];

int Pins[] = {13,12,11,10,9,8,7};
int PinsCount = sizeof(Pins) / sizeof(int);

int PinButton = 6;

int PinReset = Pins[6];
int PinMotor = Pins[4];
int PinTimer = Pins[2];
int PinLight = Pins[1];

bool StatusReset = false;
bool StatusMotor = false;
bool StatusTimer = false;
bool StatusLight = false;

bool LastStatus[] = {false, false, false, false};

bool PowerStatus = false;
int SpeedStatus = 0;
bool LightStatus = false;

int loopNumber = 0;


int Numbers[4][7] = { {0,1,1,1,1,1,1}, //0
                      {1,1,1,1,0,1,0}, //1
                      {0,0,0,0,0,1,1}, //2
                      {0,1,0,0,0,1,0}};//3

void setup() {
  //segment pins
  for (int i=0; i < PinsCount; i++) {
    pinMode(Pins[i], OUTPUT);
  }
  //button pin
  pinMode(PinButton, INPUT_PULLUP);
  //relays pins
  //set off all relays
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  Serial.begin(9600);
}

//write number to 7-segment display
void writeNumber(int number) {
  int a = 0;
  for (int i = 0; i < PinsCount; i++) {
    digitalWrite(Pins[i], Numbers[number][a]);
    a++;  
  }
}

void resetAllPins() {
  //reset all pins
  for (int i = 0; i < PinsCount; i++) {
    digitalWrite(Pins[i], HIGH);
  }  
}

void SetSpeed(int speed) {
  switch (speed) {
    case 0:
      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY2, HIGH);
      digitalWrite(RELAY3, HIGH);
    break;
    case 1:
      digitalWrite(RELAY3, HIGH);
      digitalWrite(RELAY2, HIGH);
      delay(RELAY_PAUSE);
      digitalWrite(RELAY1, LOW);
    break;
    case 2:
      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY3, HIGH);
      delay(RELAY_PAUSE);
      digitalWrite(RELAY2, LOW);
    break;
    case 3:
      digitalWrite(RELAY2, HIGH);
      digitalWrite(RELAY1, HIGH);
      delay(RELAY_PAUSE);
      digitalWrite(RELAY3, LOW);
    break;
  }
}

//Power Off device
void PowerOff() {
  Serial.println("power off");
  SpeedStatus = 0;
  SetSpeed(SpeedStatus);
}

//Power On device
void PowerOn() {
  SpeedStatus = 1;
  SetSpeed(SpeedStatus);
  Serial.println("power on");
}

//change power status
void ChangePower() {
  if(PowerStatus) PowerStatus = false; else PowerStatus = true;
  if(PowerStatus) {
    PowerOn();
  } else {
    PowerOff();
  }
}

//Change status of lights
void ChangeLight() {
  if(LightStatus) LightStatus = false; else LightStatus = true;
  //todo relay
  Serial.print("Light: ");
  if(LightStatus) {
    Serial.println("zapnuto");
    digitalWrite(RELAY4, LOW);
  } else {
    Serial.println("vypnuto");
    digitalWrite(RELAY4, HIGH);
  }
}

//change speed of power 1-3
void ChangeSpeed() {
  PowerStatus = true;
  SpeedStatus++;
  if (SpeedStatus > 3) SpeedStatus = 1;
  writeNumber(SpeedStatus);
  SetSpeed(SpeedStatus);
  Serial.print("Speed: ");
  Serial.println(SpeedStatus);
}

//get button pressed
void detectButton() {
  resetAllPins();
  digitalWrite(PinReset, LOW);
  StatusReset = digitalRead(PinButton) == LOW;
  resetAllPins();
  digitalWrite(PinMotor, LOW);
  StatusMotor = digitalRead(PinButton) == LOW;
  resetAllPins();
  digitalWrite(PinTimer, LOW);
  StatusTimer = digitalRead(PinButton) == LOW;
  resetAllPins();
  digitalWrite(PinLight, LOW);
  StatusLight = digitalRead(PinButton) == LOW;

 //Debounce and long press button STATUS
  if(StatusMotor == true) {
    if(LastStatus[1] != true) {
      Buttons[1]._ms = millis();
      Buttons[1]._gap = Buttons[1]._ms - Buttons[1]._previous_LOW_ms;
      Buttons[1]._previous_HIGH_ms = Buttons[1]._ms;
    }
    
  } else {
     if(LastStatus[1] != false) {
       Buttons[1]._ms = millis();
       Buttons[1]._gap = Buttons[1]._ms - Buttons[1]._previous_HIGH_ms;
       Buttons[1]._previous_LOW_ms = Buttons[1]._ms;
       if(Buttons[1]._gap > 1000) {
        PowerOff();
       } else if (Buttons[1]._gap > 50) {
        ChangeSpeed();
       }
     }
  }

 //Debounce and long press button LIGHT
  if(StatusLight == true) {
    if(LastStatus[3] != true) {
      Buttons[3]._ms = millis();
      Buttons[3]._gap = Buttons[3]._ms - Buttons[3]._previous_LOW_ms;
      Buttons[3]._previous_HIGH_ms = Buttons[3]._ms;
    }
    
  } else {
     if(LastStatus[3] != false) {
       Buttons[3]._ms = millis();
       Buttons[3]._gap = Buttons[3]._ms - Buttons[3]._previous_HIGH_ms;
       Buttons[3]._previous_LOW_ms = Buttons[3]._ms;
       if(Buttons[3]._gap > 50) {
        ChangeLight();
       }
     }
  }  
  
  LastStatus[0] = StatusReset;
  LastStatus[1] = StatusMotor;
  LastStatus[2] = StatusTimer;
  LastStatus[3] = StatusLight;
}

void loop() {
  // put your main code here, to run repeatedly:
  loopNumber++;
  if(loopNumber > 50) {
    detectButton();
    loopNumber = 0;
  }
  writeNumber(SpeedStatus);
  //Serial.println(num);
  delay(1);
}
