//Pins
const int buttonPin = 3;
const int redPin =  9;
const int greenPin =  10;
const int bluePin =  11;

//Color codes
const int WHITE[] = {255,255,255};
const int RED[] = {255,0,0};
const int GREEN[] = {0,255,0};
const int BLUE[] = {0,0,255};
const int YELLOW[] = {255,150,0};
const int CYAN[] = {0,255,255};
const int ORANGE[] = {255,50,0};
const int MAGENTA[] = {255,0,255};

//State variables
double newFadeBrightness = 0;
unsigned long nextFadeTime = 0;
int fadeRefresh = 25;
int buttonState = 0;
int pulseState = 1;
int currentColor[] = {255,255,255};
double currentBrightness = 0;


//Runs at startup
void setup() {
//  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  updateLED();
}

//Runs continuously
void loop() {
  
  continuePulse(2,4,3.0,WHITE);

  //If button is pressed...
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {

    //Cast cyan spell
    setColor(CYAN);
    fadeToBrightness(100, 0.3);
    pulseForDuration(50,100,3.0,10);
    fadeToBrightness(0, 0.5);

    //Wait 1 seconds
    delay(1000);

    //Blink when recovered
    setColor(WHITE);
    setBrightness(100);
    delay(100);
    setBrightness(0);
    delay(100);
    setBrightness(100);
    delay(100);
    setBrightness(0);
  }
}





//HELPER METHODS


//Immedietly set the LED's color (each channel between 0 and 255)
void setColor(const int color[]) {
  for (int i=0; i<3; i++) {
    if (color[i] > 255) currentColor[i] = 255;
    else if (color[i] < 0) currentColor[i] = 0;
    else currentColor[i] = color[i];
  }
  updateLED();
}

//Immedietly set the LED's brightness (between 0 and 100)
void setBrightness(double percentage) {
  if (percentage > 100) currentBrightness = 100.0;
  else if (percentage < 0) currentBrightness = 0.0;
  else currentBrightness = percentage;
  updateLED();
}

//Fade the LED to the 'color' over the course of 'seconds'
void fadeToColor(const int color[], double seconds) {
  double ms = seconds*1000;
  for (int i = 0; i <= ms; i += fadeRefresh) {
    int tempColor[] = {
      int(currentColor[0]+(color[0]-currentColor[0])*(i/ms)),
      int(currentColor[1]+(color[1]-currentColor[1])*(i/ms)),
      int(currentColor[2]+(color[2]-currentColor[2])*(i/ms))};
    setColor(tempColor);
    delay(fadeRefresh);
  }
  setColor(color);
}

//Fade the LED to 'brightness' over the course of 'seconds'
void fadeToBrightness(double brightness, double seconds) {
  double ms = seconds*1000;
  for (int i = 0; i <= ms; i += fadeRefresh) {
    setBrightness(int(currentBrightness+(brightness-currentBrightness)*(i/ms)));
    delay(fadeRefresh);
  }
  setBrightness(brightness);
}

//Continue pulsing between (minVal, maxVal) with a cyclic 'period' (in seconds) and in 'color'
void continuePulse(int minVal, int maxVal, double period, const int color[]) {
  setColor(color);
  continuePulse(minVal, maxVal, period);
}

//Continue pulsing between (minVal, maxVal) with a cyclic 'period' (in seconds)
void continuePulse(int minVal, int maxVal, double period) {
  if (millis() >= nextFadeTime) {
    
    if (millis()-nextFadeTime > 4*fadeRefresh) newFadeBrightness = currentBrightness;
    setBrightness(newFadeBrightness);

    double brightnessChange = (double(maxVal-minVal)*2.0)/(double(period*1000)/fadeRefresh);
    nextFadeTime = millis() + fadeRefresh;
    double newBrightness = newFadeBrightness+(double(pulseState)*brightnessChange);
    
    if (newBrightness < minVal) {
      pulseState = 1;
      newFadeBrightness = minVal;
    } else if (newBrightness > maxVal) {
      pulseState = -1;
      newFadeBrightness = maxVal;
    } else newFadeBrightness = newBrightness;
  }
}

//Pulse the LED for a duration:
    // minVal/maxVal: The bounds of the fade brightness
    // period: The period (in seconds) of one full cycle of brightness
    // color: The color to fade in
    // duration: The amount of time to pulse for
void pulseForDuration(int minVal, int maxVal, double period, const int color[], double duration) {
  setColor(color);
  pulseForDuration(minVal, maxVal, period, duration);
}

//Pulse the LED for a duration:
    // minVal/maxVal: The bounds of the fade brightness
    // period: The period (in seconds) of one full cycle of brightness
    // duration: The amount of time to pulse for
void pulseForDuration(int minVal, int maxVal, double period, double duration) {
  unsigned long endTime = millis() + (duration*1000);
  while(millis() < endTime) continuePulse(minVal, maxVal, period);
}

//Set the color/brightness of the LED to 'currentBrightness' and 'currentColor'
void updateLED() {
  analogWrite(redPin, int(255-int(currentBrightness*double(currentColor[0]))/100.0));
  analogWrite(greenPin, int(255-int(currentBrightness*double(currentColor[1]))/100.0));
  analogWrite(bluePin, int(255-int(currentBrightness*double(currentColor[2]))/100.0));
}

