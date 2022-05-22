//IN UE den Handfilter machen
//dies
#include <WiFi.h>
#include "esp_now.h"

uint8_t broadcastAddressLeft[] = {0x08, 0x3A, 0xF2, 0xAC, 0xDF, 0xE8};
uint8_t broadcastAddressRight[] =  {0xC8, 0xC9, 0xA3, 0xCA, 0xDA, 0xB8};
//Dongle: {0x9C, 0x9C, 0x1F, 0xE2, 0x17, 0xF4};
//Right:  {0xC8, 0xC9, 0xA3, 0xCA, 0xDA, 0xB8};
//Left: {0x08, 0x3A, 0xF2, 0xAC, 0xDF, 0xE8};

//Dongle is not allowed to have Serial.print, because this is interpretated by UE4


String str;



struct Motors {
  String label;
  int labelnum;
  int val;
  int pin;
};
struct Motors MotorL[5] = { { "Pinky", 0 }, { "Ring", 1 }, { "Middle", 2 }, { "Index", 3 }, { "Thumb", 4 } };
struct Motors MotorR[5] = { { "Pinky", 0 }, { "Ring", 1 }, { "Middle", 2 }, { "Index", 3 }, { "Thumb", 4 } };


//FingerStruct
struct FingerJoint {
  int Min = 9000;
  int Max = 1;
  int Val;
  int Data[10];
};

struct Finger {
  String label;
  int labelNum;
  int HandschuhNr;
  FingerJoint joints[3];
  int64_t TIME;
};
struct Hand {
  bool inUse = false; //see if we got Data at least once from that hand
  Finger Fingers[5] = { { "Pinky", 0 }, { "Ring", 1}, { "Middle", 2 }, { "Index", 3 }, { "Thumb", 4} };
};

struct Hand Hands[8];
struct Finger receivedFinger;
int ArraySize;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);


  //Wifi things. CANT BE IN IF() BECAUSE IT CRASHES!
  WiFi.mode(WIFI_STA);
  Serial.print("Mac Address for this ESP is: ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddressLeft, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);


  Serial.println("Start");
}


void loop() {
//    if (!Serial.available()) return;
//        while(Serial.available() == 0) {
//      }
  GetData(); //all in Functions from now on. //room to breath
  delay(1);
}
