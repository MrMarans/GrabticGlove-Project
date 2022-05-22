void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedFinger, incomingData, sizeof(receivedFinger));
  //Serial.print("FingerReceived ");

  int HandNum = receivedFinger.HandschuhNr;
  if (HandNum != 0) {
    //transfer Data into Hands, Fingers and labels
    Hands[HandNum].Fingers[receivedFinger.labelNum].labelNum = receivedFinger.labelNum;
    Hands[HandNum].Fingers[receivedFinger.labelNum].label = receivedFinger.label;
    Hands[HandNum].Fingers[receivedFinger.labelNum].joints[0] = receivedFinger.joints[0];
    Hands[HandNum].Fingers[receivedFinger.labelNum].joints[1] = receivedFinger.joints[1];
    Hands[HandNum].Fingers[receivedFinger.labelNum].joints[2] = receivedFinger.joints[2];
    receivedFinger.TIME;
    Hands[HandNum].Fingers[receivedFinger.labelNum].TIME = receivedFinger.TIME;
    Hands[HandNum].inUse = true;
  }
  delay(1);
}
