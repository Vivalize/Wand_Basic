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
int buttonState = 0;
int pulseState = 1;
int currentColor[] = {255,255,255};
int currentBrightness = 0;


//Runs at startup
void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  updateLED();
}

//Runs continuously
void loop() {
  
  pulse(0,30,WHITE);

  //If button is pressed...
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {

    //Cast red spell
    setColor(RED);
    fadeToBrightness(100, 0.3);
    delay(1000);
    fadeToBrightness(0, 0.5);

    //Wait 10 seconds
    delay(10000);

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
void setBrightness(int percentage) {
  if (percentage > 100) currentBrightness = 100;
  else if (percentage < 0) currentBrightness = 0;
  else currentBrightness = percentage;
  updateLED();
}

//Fade the LED to the 'color' over the course of 'seconds'
void fadeToColor(const int color[], double seconds) {
  double ms = seconds*1000;
  for (int i = 0; i <= ms; i += 25) {
    int tempColor[] = {
      int(currentColor[0]+(color[0]-currentColor[0])*(i/ms)),
      int(currentColor[1]+(color[1]-currentColor[1])*(i/ms)),
      int(currentColor[2]+(color[2]-currentColor[2])*(i/ms))};
    setColor(tempColor);
    delay(25);
  }
  setColor(color);
}

//Fade the LED to 'brightness' over the course of 'seconds'
void fadeToBrightness(int brightness, double seconds) {
  double ms = seconds*1000;
  for (int i = 0; i <= ms; i += 25) {
    setBrightness(int(currentBrightness+(brightness-currentBrightness)*(i/ms)));
    delay(25);
  }
  setBrightness(brightness);
}

//Fade the brightness of the LED between 'minVal' and 'maxVal' using 'color'
void pulse(int minVal, int maxVal, const int color[]) {
  setColor(color);
  if (currentBrightness < minVal) pulseState = 1;
  else if (currentBrightness > maxVal) pulseState = -1;
  setBrightness(currentBrightness+pulseState*3);
}

//Set the color/brightness of the LED to 'currentBrightness' and 'currentColor'
void updateLED() {
  analogWrite(redPin, 255-(currentBrightness*currentColor[0])/100);
  analogWrite(greenPin, 255-(currentBrightness*currentColor[1])/100);
  analogWrite(bluePin, 255-(currentBrightness*currentColor[2])/100);
}

