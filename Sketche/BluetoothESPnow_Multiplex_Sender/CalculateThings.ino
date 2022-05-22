void StartMinMax()
{
  for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
    for (int j = 0; j < 3; ++j) {
      Fingers[i].joints[j].Min = 5000;
      Fingers[i].joints[j].Max = 1;
    }
  }
}

void SetMinMax()
{
  if (FilterDone == true) {
    for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
      for (int j = 0; j < 3; ++j) {
        if (Fingers[i].joints[j].Val < Fingers[i].joints[j].Min) Fingers[i].joints[j].Min = Fingers[i].joints[j].Val ;
        if (Fingers[i].joints[j].Val > Fingers[i].joints[j].Max) Fingers[i].joints[j].Max = Fingers[i].joints[j].Val;
      }
    }
  }
}

void SetMin()
{
  if (FilterDone == true) {
    for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
      for (int j = 0; j < 3; ++j) {
        if (Fingers[i].joints[j].Val < Fingers[i].joints[j].Min) Fingers[i].joints[j].Min = Fingers[i].joints[j].Val ;
      }
    }
  }
}

void SetMax()
{
  if (FilterDone == true) {
    for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
      for (int j = 0; j < 3; ++j) {
        if (Fingers[i].joints[j].Val > Fingers[i].joints[j].Max) Fingers[i].joints[j].Max = Fingers[i].joints[j].Val;
      }
    }
  }
}

void Map() {
  int minMap = 0;
  int maxMap = 90;
  for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
    for (int j = 0; j < 3; ++j) {
      Fingers[i].joints[j].Val = map(Fingers[i].joints[j].Val, Fingers[i].joints[j].Min, Fingers[i].joints[j].Max, minMap, maxMap);
    }
  }
}

void Map2() {
  int minMap = 0;
  int maxMap = 205;
  for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
    for (int j = 0; j < 3; ++j) {
      Fingers[i].joints[j].Val = map(Fingers[i].joints[j].Val, Fingers[i].joints[j].Min, Fingers[i].joints[j].Max, minMap, maxMap);
    }
  }
}

void Filter() {
  for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
    for (int j = 0; j < 3; ++j) {
      calcNumber = 0;
      for (int k = 0; k < 10; ++k) {
        calcNumber = calcNumber + Fingers[i].joints[j].Data[k];
      }
      calcNumber = calcNumber / 10;
      Fingers[i].joints[j].Val = calcNumber;
      //      Serial.print("Filter for ");
      //      Serial.print(Fingers[i].label);
      //      Serial.print(j);
      //      Serial.print(" is: ");
      //      Serial.println(calcNumber);
    }
  }
}


void SpikeBandPassFilter() { //take avg, weight middle 8 data and calculate their usage by closeness to avg to get filtered value.
  for (int i = 0; i < 5; ++i) { // ++i may take less cpu cycles.
    for (int j = 0; j < 3; ++j) {
      //isort(Fingers[i].joints[j].Data, 10);
      int averageMiddle = 0;
      for (int k = 0; k < 10; ++k) {
        calcNumber = calcNumber + Fingers[i].joints[j].Data[k];
      }
      averageMiddle = calcNumber / 10;
      int val = 0;
      for (int h = 1; h < 9; ++h) {
        val = val + percentOfPass(averageMiddle, calcNumber = calcNumber + Fingers[i].joints[j].Data[h]);
      }
      Fingers[i].joints[j].Val = val / 8;
    }
  }
}

int percentOfPass(int avg, int num) {
  int returnNr = num * (100 / avg * num);
}


void isort(int *a, int n)
{
  for (int i = 1; i < n; ++i)
  {
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--)
    {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}

/* LegacyBandPass by https://www.norwegiancreations.com/2016/03/arduino-tutorial-simple-high-pass-band-pass-and-band-stop-filtering/
 *  SpikeBandPass gave better effects
float EMA_a_low = 0.3;    //initialization of EMA alpha
float EMA_a_high = 0.5;
int EMA_S_low = 0;        //initialization of EMA S
int EMA_S_high = 0;
int highpass = 0;
int bandpass = 0;

void FilterBandPass() {
  
  EMA_S_low = (EMA_a_low*sensorValue) + ((1-EMA_a_low)*EMA_S_low);  //run the EMA
  EMA_S_high = (EMA_a_high*sensorValue) + ((1-EMA_a_high)*EMA_S_high);
   
  highpass = sensorValue - EMA_S_low;     //find the high-pass as before (for comparison)
  bandpass = EMA_S_high - EMA_S_low;      //find the band-pass
}
*/
