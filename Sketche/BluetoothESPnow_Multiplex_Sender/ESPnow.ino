void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedMotor, incomingData, sizeof(receivedMotor));


  //  Motor[0].Motor.attach(servoPin1, 500, 2400);
  //  Motor[1].Motor.attach(servoPin2, 500, 2400);
  //  Motor[2].Motor.attach(servoPin3, 500, 2400);
  //  Motor[3].Motor.attach(servoPin4, 500, 2400);
  //  Motor[4].Motor.attach(servoPin5, 500, 2400);
  //  Motor[0].Motor.setPeriodHertz(50);
  //  Motor[1].Motor.setPeriodHertz(50);
  //  Motor[2].Motor.setPeriodHertz(50);
  //  Motor[3].Motor.setPeriodHertz(50);
  //  Motor[4].Motor.setPeriodHertz(50);
  //
  //
  //
  //
  //  Motor[0].Motor.detach();
  //  Motor[1].Motor.detach();
  //  Motor[2].Motor.detach();
  //  Motor[3].Motor.detach();
  //  Motor[4].Motor.detach();
  //  pwm.detachPin(27);

  Motor[receivedMotor.labelnum].val = receivedMotor.val;
  Serial.println(Motor[receivedMotor.labelnum].val);


}
