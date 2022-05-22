void GetData() { //only used with Bluetooth
  if (!SerialBT.available()) return; //only read if serial is used
  str = SerialBT.readString(); // read String and save
  if (str.toInt() != 1) {
    int e1 = 1; //1 because skip Hand info
    for (int i = 0; i < 5; ++i) { // Mapping
      //parse through string to give corresponding data to motor. space is used to differentiate
      int a1 = e1;  //start parse
      int e1 = str.indexOf(' ', e1 + i); //end parse for space
      int position = map(str.substring(a1, e1).toInt(), Fingers[i].joints[1].Min, Fingers[i].joints[1].Max, 0, 180);  //take position of the parse, string as int, recalculate into value between min and max value for the 180° rotation of motor
      Motor[i].val = position; //just save it
      Motor[i].Motor.write(Motor[i].val); //save it
    }
  }
  SerialBT.setTimeout(1);
  delay(1);
}

void TestData() {
  //Spreiz
  Fingers[0].joints[0].Val = 11; //pinky1
  Fingers[1].joints[0].Val =  12;
  Fingers[2].joints[0].Val =  13;
  Fingers[3].joints[0].Val =  14;
  Fingers[4].joints[0].Val =  15;
  //Erstes Gelenk
  Fingers[0].joints[1].Val = 21; //pinky2
  Fingers[1].joints[1].Val =  22;
  Fingers[2].joints[1].Val = 23;
  Fingers[3].joints[1].Val =  24;
  Fingers[4].joints[1].Val =  25;
  //Zweites Gelenk
  Fingers[0].joints[2].Val = 31; //pinky3
  Fingers[1].joints[2].Val =  32;
  Fingers[2].joints[2].Val = 33;
  Fingers[3].joints[2].Val =  34;
  Fingers[4].joints[2].Val = 35;
}

void RealData2() {
  digitalWrite(Reset, LOW); //Daten einfliesen lassen in Multiplexer
  //Spreiz
  Fingers[0].joints[0].Val = readMux(5, 1); //Pinky1
  Fingers[1].joints[0].Val = readMux(6, 1); //Ring1
  Fingers[2].joints[0].Val = readMux(7, 1); //Middle1
  Fingers[3].joints[0].Val = readMux(0, 2); //Index1
  Fingers[4].joints[0].Val = readMux(1, 2); //Thumb1
  //erstes gelenk
  Fingers[0].joints[1].Val = readMux(0, 1); //Pinky2
  Fingers[1].joints[1].Val = readMux(1, 1); //Ring2
  Fingers[2].joints[1].Val = readMux(2, 1); //Middle2
  Fingers[3].joints[1].Val = readMux(3, 1); //Index2
  Fingers[4].joints[1].Val = readMux(4, 1); //Thumb2
  //zweites gelenk
  Fingers[0].joints[2].Val = readMux(2, 2); //Pinky3
  Fingers[1].joints[2].Val = readMux(3, 2); //Ring3
  Fingers[2].joints[2].Val = readMux(4, 2); //Middle3
  Fingers[3].joints[2].Val = readMux(5, 2); //Index3
  Fingers[4].joints[2].Val = readMux(6, 2); //Thumb3
  FilterNum = FilterNum + 1;
  if (FilterNum == 10) {
    FilterNum = 0;
    FilterDone = true;
  }
  digitalWrite(Reset, HIGH); //Data reset
  //vTaskDelay(TickRate / portTICK_PERIOD_MS);
}

void RealData(void * parameter) {
  for (;;) {
    digitalWrite(Reset, LOW); //Daten einfliesen lassen in Multiplexer
    //Spreiz
    Fingers[0].joints[0].Data[FilterNum] = readMux(5, 1); //Pinky1
    Fingers[1].joints[0].Data[FilterNum] = readMux(6, 1); //Ring1
    Fingers[2].joints[0].Data[FilterNum] = readMux(7, 1); //Middle1
    Fingers[3].joints[0].Data[FilterNum] = readMux(0, 2); //Index1
    Fingers[4].joints[0].Data[FilterNum] = readMux(1, 2); //Thumb1
    //erstes gelenk
    Fingers[0].joints[1].Data[FilterNum] = readMux(0, 1); //Pinky2
    Fingers[1].joints[1].Data[FilterNum] = readMux(1, 1); //Ring2
    Fingers[2].joints[1].Data[FilterNum] = readMux(2, 1); //Middle2
    Fingers[3].joints[1].Data[FilterNum] = readMux(3, 1); //Index2
    Fingers[4].joints[1].Data[FilterNum] = readMux(4, 1); //Thumb2
    //zweites gelenk
    Fingers[0].joints[2].Data[FilterNum] = readMux(2, 2); //Pinky3
    Fingers[1].joints[2].Data[FilterNum] = readMux(3, 2); //Ring3
    Fingers[2].joints[2].Data[FilterNum] = readMux(4, 2); //Middle3
    Fingers[3].joints[2].Data[FilterNum] = readMux(5, 2); //Index3
    Fingers[4].joints[2].Data[FilterNum] = readMux(6, 2); //Thumb3
    FilterNum = FilterNum + 1;
    if (FilterNum == 10) {
      FilterNum = 0;
      FilterDone = true;
    }
  }
  digitalWrite(Reset, HIGH); //Data reset
  delay(1);
  //vTaskDelay(TickRate / portTICK_PERIOD_MS);
}

void ArraysAndSend() {
  for (int i = 0; i < 5; ++i) {
    if (BluetoothOverWifi == true) {
      byte package[17] = {99, HandschuhNr, Fingers[0].joints[0].Val, Fingers[0].joints[1].Val, Fingers[0].joints[2].Val, Fingers[1].joints[0].Val, Fingers[1].joints[1].Val, Fingers[1].joints[2].Val, Fingers[2].joints[0].Val, Fingers[2].joints[1].Val, Fingers[2].joints[2].Val, Fingers[3].joints[0].Val, Fingers[3].joints[1].Val, Fingers[3].joints[2].Val, Fingers[4].joints[0].Val, Fingers[4].joints[1].Val, Fingers[4].joints[2].Val};
      SerialBT.write(package, 17);
    } else {
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Fingers[i], sizeof(Fingers[i]));
    }
    // Serial.println(Fingers[i].HandPlayerLabel);
    //    if (result == ESP_OK) {
    //      Serial.println("Sent with success");
    //    }
    //    else {
    //      Serial.println("Error sending the data");
    //    }
  }
}

int readMux(int Kanal, int Multiplexer) {  //choose channel of Multiplexer, read out of it
  int selectPin[] = { A, B, C };
  int muxKanal[8][3] = {
    {0, 0, 0},  //Kanal 0
    {1, 0, 0},  //Kanal 1
    {0, 1, 0},  //Kanal 2
    {1, 1, 0},  //Kanal 3
    {0, 0, 1},  //Kanal 4
    {1, 0, 1},  //Kanal 5
    {0, 1, 1},  //Kanal 6
    {1, 1, 1},  //Kanal 7
  };

  //Setzt den ausgewählten Kanal
  for (int i = 0; i < 3; i++) {
    digitalWrite(selectPin[i], muxKanal[Kanal][i]);
  }
  int value;
  if (Multiplexer == 1) {
    value = analogRead(Z);
  }
  if (Multiplexer == 2) {
    value = analogRead(y);
  }
  return value;
}
