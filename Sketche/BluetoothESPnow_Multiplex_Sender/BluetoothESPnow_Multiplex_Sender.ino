#include "BluetoothSerial.h"
#include <ESP32Servo.h>
#include <WiFi.h>
#include "esp_now.h"
#include <math.h>
BluetoothSerial SerialBT;
TaskHandle_t Task1;

bool BluetoothOverWifi = false; //false == ESPNOW/Wifi is used, true == Bluetooth is used (Bluetooth only recommended without motors!!)

//Address of the dongle to send data
uint8_t broadcastAddress[] = {0x9C, 0x9C, 0x1F, 0xE2, 0x17, 0xF4}; //Mac address of ESP you want to connect with. Future version will do it automatically with bluetooth connection to ask for Mac address

//Dongle: {0x9C, 0x9C, 0x1F, 0xE2, 0x17, 0xF4};
//Right:  {0x08, 0x3A, 0xF2, 0xAC, 0xDF, 0xE8};

//Name for WifiConnection and Bluetooth.
//Number shows which Hand this is. 1= Left of Player 1, 2 is Right of Player 1, 3 is Left of Player 2...
int HandschuhNr = 1;
String BTName = "Hand" + String(HandschuhNr);

//pins for the Servo Motors in use, 1 is pinky, 5 is thumb
int servoPin1 = 21;
int servoPin2 = 23;
int servoPin3 = 4;
int servoPin4 = 2;
int servoPin5 = 15;

const int ONBOARD_LED = 2;
const int CALIBRATION_PIN = 12; //Pin for the callibration button
bool calMin;
int lastState;

//Multiplexer pins
const int A = 26;
const int B = 25;
const int C = 33;
const int Reset = 32;
const int Z = 34; //Multiplexer 1
const int y = 35; //Multiplexer 2



//Values that just need to be there
int FilterNum;
int calcNumber;
String str;
bool FilterDone = false; //gets true if Data[] Array of FingerJoint is full after first 10 reads of the values
bool MotorDatas;

//Finger- and jointStructs
struct FingerJoint {
  int Min = 9000;
  int Max = 1;
  int Val;
  int Data[10];
};

struct Finger {
  String label;
  int labelNum;
  int HandschuhNr; //cant send struct in struct in struct because esp just crashes, so handNr must be saved here instead of sending a hand, with 5 fingers, each having 3 joints...
  FingerJoint joints[3];
};
//creating the Struct as object with names and labels for wifi-communication
struct Finger Fingers[5] = { { "Pinky", 0, HandschuhNr }, { "Ring", 1, HandschuhNr }, { "Middle", 2, HandschuhNr }, { "Index", 3, HandschuhNr }, { "Thumb", 4, HandschuhNr } };

ESP32PWM pwm;
//struct for Motors
struct Motors {
  String label;
  int labelnum;
  int pin;
  int val;
  Servo Motor;
};
//creating the Struct as object with names and labels for wifi-communication
struct Motors Motor[5] = { { "Pinky", 0, servoPin1 }, { "Ring", 1 , servoPin2}, { "Middle", 2, servoPin3 }, { "Index", 3, servoPin4 }, { "Thumb", 4, servoPin5 } };
struct Motors receivedMotor; //for getting data

void setup() {
    ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  Serial.begin(115200);
  Serial.setTimeout(5);

  pinMode(servoPin5, INPUT_PULLUP);
  pinMode(servoPin4, INPUT_PULLUP);
  pinMode(servoPin3, INPUT_PULLUP);
  pinMode(servoPin2, INPUT_PULLUP);
  pinMode(servoPin1, INPUT_PULLUP);

  //MultiPlex for Fingers.
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(Z, INPUT);
  pinMode(Reset, OUTPUT);
  digitalWrite (A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  pinMode(CALIBRATION_PIN, INPUT_PULLUP);
  pinMode(ONBOARD_LED, OUTPUT);

  if (BluetoothOverWifi == true)  { //start bluetooth if used
    SerialBT.begin(BTName);
  }

  //Wifi things. CANT BE IN IF() BECAUSE IT CRASHES!
  WiFi.mode(WIFI_STA);
  Serial.print("Mac Address for this ESP is: ");
  Serial.println(WiFi.macAddress());

  //Wifi things
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  //Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  //MultiTask for reading Data
  xTaskCreatePinnedToCore(
    RealData, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0); /* Core where the task should run */


  StartMinMax(); //give all joints a sick max and min value, legacy

  Serial.println("Start");  //loop activates now
}


void loop() {
  if (BluetoothOverWifi == true) {
    GetData(); //only used if Bluetooth is used. Read Bluetooth Data for motor data
  }
  DataAndSend();
}

void DataAndSend() { //Do Data work and send them. Words in the stars show where those functions are located. like *ThisFile* or *Functions*
  //TestData(); //legacy, only for debugging *Functions*
  //RealData2(); //get data on this core, runs already with Task on another core, not really needed. *Functions*
  Filter(); //legacy, takes average number of latest 10 values *CalculateThings*
  //SpikeBandPassFilter(); //A sharp Band pass filter, worked best for me. Weights past data based on distance to avg number to give a new filtered value. *CalculateThings*
  //ButtonCalib(); //enables calibration with button press *ThisFile*
  SetMinMax(); //Set Min and Max Value for each Sensor, only if ButtonCalib is not used *CalculateThings*
  Map2(); //Maps data to 90° Rotation *CalculateThings*
  ArraysAndSend(); //Creates Array for Bluetooth or just sends Data via ESPnow *Functions*
  printSensors(); //for debugging *PrintThings*
  //printMinMax(); //for debugging *PrintThings*
  //printMotors(); //for debugging *PrintThings*
  if (BluetoothOverWifi == true)  { //prevent overflow
    SerialBT.setTimeout(1);
    delay(1);
  }
}

void ButtonCalib() { //Calibrate it with Button, Onboard LED blinks for Min, then for Max multiple times
  int ButtonState = digitalRead(CALIBRATION_PIN);
  if (ButtonState == HIGH && lastState == LOW) {
    if (calMin == false) {
      SetMin(); calMin = true;
      digitalWrite(ONBOARD_LED, HIGH);
      delay(500);
      digitalWrite(ONBOARD_LED, LOW);
    }
    else {
      SetMax();
      calMin = false;
      for (int i = 0; i < 5; ++i) {
        digitalWrite(ONBOARD_LED, HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED, LOW);
        delay(100);
      }
    }
    int lastState = ButtonState;
  }
}
