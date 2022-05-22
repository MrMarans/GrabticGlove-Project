void GetData() {
  str = Serial.readString();
  //str = "1 110 2020 3040 4055 5060";
  int a1 = 0;
  int e1 = 0;
  a1 = e1;  //parse anfang
  e1 = str.indexOf(' ', e1 + 1); //parse ende
  int HandNr = str.substring(a1, e1).toInt();
  for (int i = 0; i < 5; ++i) { // Mapping
    //parse through data, space defines motor, scales with the for i
    a1 = e1;  //parse anfang
    e1 = str.indexOf(' ', e1 + 1); //parse ende
    //int position = map(str.substring(a1, e1).toInt(), Fingers[i].joints[1].Min, Fingers[i].joints[1].Max, 0, 180);  //position als parse zwischen anfang1 und ende 1, string als int, umwandeln in werte von min und max wert zu den 180° rotation des motors
    MotorL[i].val = str.substring(a1, e1).toInt();
    //Serial.println(MotorL[i].val);
    if (HandNr == 1) {
      esp_err_t result = esp_now_send(broadcastAddressLeft, (uint8_t *) &MotorL[i], sizeof(MotorL[i]));
    }
    if (HandNr == 2) {
      esp_err_t result = esp_now_send(broadcastAddressLeft, (uint8_t *) &MotorL[i], sizeof(MotorL[i]));
    }
    
    //    if (result == ESP_OK) {
    //      Serial.println("Sent with success");
    //    }
    //    else {
    //      Serial.println("Error sending the data");
    //    }
    delay(1);
  }  DoArray();
}



void DoArray() {
  //Dont know why anymore, anways, need to be -4

  for (int k = 1; k < 8; ++k) { //at last go through Hands
    if (Hands[k].inUse == true)     {    // byte package[6] = {99, k, i, Hands[k].Fingers[i].joints[0].Val, Hands[k].Fingers[i].joints[1].Val, Hands[k].Fingers[i].joints[2].Val};
      // byte packagePrep[6] = {99, k, i, Hands[k].Fingers[i].joints[0].Val, Hands[k].Fingers[i].joints[1].Val, Hands[k].Fingers[i].joints[2].Val};
      byte package[17] = {99, k, Hands[k].Fingers[0].joints[0].Val, Hands[k].Fingers[0].joints[1].Val, Hands[k].Fingers[0].joints[2].Val, Hands[k].Fingers[1].joints[0].Val, Hands[k].Fingers[1].joints[1].Val, Hands[k].Fingers[1].joints[2].Val, Hands[k].Fingers[2].joints[0].Val, Hands[k].Fingers[2].joints[1].Val, Hands[k].Fingers[2].joints[2].Val, Hands[k].Fingers[3].joints[0].Val, Hands[k].Fingers[3].joints[1].Val, Hands[k].Fingers[3].joints[2].Val, Hands[k].Fingers[4].joints[0].Val, Hands[k].Fingers[4].joints[1].Val, Hands[k].Fingers[4].joints[2].Val};
      Serial.write(package, 17); // who needs for() if you can just hardcode it fast
      //                for (int i = 0; i < sizeof(package); i++)
      //                  {
      //                    Serial.print(package[i]);
      //                  }
      //                  Serial.println();
      //                  Serial.println(Hands[k].Fingers[0].joints[0].Val);
      //                   Serial.println(Hands[k].Fingers[0].joints[1].Val);
    }
  }
}
