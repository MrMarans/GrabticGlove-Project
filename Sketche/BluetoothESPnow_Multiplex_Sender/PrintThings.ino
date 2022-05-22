void printSensors()
{
  for (int i = 0; i < 5; ++i) { // ++i benoetigt 1 cpu zyklus weniger als i++
    for (int j = 0; j < 3; ++j) {
      Serial.print(Fingers[i].label + "finger");
      Serial.print(j);
      Serial.print(": ");
      Serial.print(Fingers[i].joints[j].Val);
      Serial.print(" ");
    }
    Serial.println("");
  }
  Serial.println("");
} 
void printMotors() {
  //  //Print den Motorenwert
    for (int j = 0; j < 5; ++j) {
      Serial.print(Motor[j].label+" val is: ");
      Serial.println(Motor[j].val);
    }
}

void printMinMax() {
  Serial.println("MIN MAX Values in Order: ");

  for (int i = 0; i < 5; ++i) { // ++i benoetigt 1 cpu zyklus weniger als i++
    for (int j = 0; j < 3; ++j) {
      Serial.print(Fingers[i].label);
      Serial.print(j);
      Serial.print(" Min:");
      Serial.print(Fingers[i].joints[j].Min);
      Serial.print(" Max:");
      Serial.print(Fingers[i].joints[j].Max);
      Serial.print("     ");
    }
    Serial.println("");
  }
}
