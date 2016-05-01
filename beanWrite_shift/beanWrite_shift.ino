///////////////////////////////////////////////////////////
//changing the names for the correct ones
//-------------block 0
#define motor0 5
#define motor1 0 //slow one
#define motor2 1
#define motor3 2
#define motor4 3
#define motor5 4
//-------------block 1
#define motor6 0
#define motor7 2
#define motor8 1
#define motor9 3
#define motor10 4
#define motor11 5
/////////////////////////////////////////////////////////



//Pin connected to latch pin (ST_CP) of 74HC595
int latchPin = 2;
//Pin connected to SHCP of 74HC595
int clockPin = 5;
//Pin connected to DS of 74HC595
int dataPin = 4;

byte block[] = {0, 0}; //there are 2 shift registers.
int motor[] = {motor0, motor1, motor2, motor3, motor4, motor5, motor6, motor7, motor8, motor9, motor10, motor11};

//NEOPIXels
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// NeoPixels pin
#define PIN 1

// How many NeoPixels
#define NUMPIXELS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool deviceOff;
bool ledOff;
bool testButton;
int longInterval;   //from phone- how long between patters
unsigned long currentTime;
String beanName = "Tempo1";
const uint8_t ledScratch = 1; //what is LED scrach? what is 1?

void setup() {
  Serial.begin(57600);
  Bean.sleep(3000); //wait for serial monitor to start

  //set shift register pins:
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  // Setup bean
  Bean.setBeanName(beanName);
  Bean.enableWakeOnConnect(true);

  // Reset the scratch data area 1.
  uint8_t resetLedBuffer[] = {0, 0, 0}; //other type???? is this the data received format?
  Bean.setScratchData(ledScratch, resetLedBuffer, 3);


  pixels.begin(); //  initializes the NeoPixel library
  pixels.show(); // Initialize all pixels to 'off'

  ledOff = false;
  deviceOff = false;
  testButton = false;
  currentTime = 0;
  longInterval = 10000; //by default 5 minuts = 300,000 millisconds
}

// the loop routine runs over and over again forever:
void loop() {
  currentTime = millis();
  bool connected = Bean.getConnectionState();

  if (connected) {

    //Update the led
    ScratchData receivedData = Bean.readScratchData(ledScratch);

    uint8_t hours = receivedData.data[0];
    uint8_t minutes = receivedData.data[1];
    uint8_t onOff = receivedData.data[2];

    if (deviceOff == false) { //wrappper for all the function

    //test run to all motors and LEDs up and down - (interupts a the sleep)
//    if (testButton == true){
//      test();
//    }

//    test();  //instead of test we will have pattern here
    Serial.println("pattern ended - sleep long interval");
    Bean.sleep(longInterval);


    } //end of wrapper to turn on/off
  } //end connected
}//end void loop



// use data received
ScratchData receivedData = Bean.readScratchData(ledScratch);

//    uint8_t redLed = receivedData.data[0];
//    uint8_t greenLed = receivedData.data[1];
uint8_t onOff = receivedData.data[2];
Bean.setLed(redLed, greenLed, blueLed);


// Turn LED off and put to sleep.
Bean.setLed(0, 0, 0);
Bean.sleep(0xFFFFFFFF);
}


void test() {         //run all LED and motors up and down.
  for (int i = 0; i < 12; i++) {
    if (i < 6) { //first block
      bitSet(block[0], motor[i]);
    } else { //second block
      bitSet(block[1], motor[i]);
    }
    updateShiftRegister();
    if (ledOff == false) {
      pixels.setPixelColor(i, pixels.Color(0, 120, 220));
      pixels.show();
    }
    Bean.sleep(100); //between each motor
    allOff();
  }
  allOff();
  testButton = false; //reset the button when done
}


void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, LSBFIRST, block[0]);
  shiftOut(dataPin, clockPin, LSBFIRST, block[1]);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
}


void allOff() {
  //write LOW all ShiftReg pins
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < 8; i++) {
    bitWrite(block[0], i, 0);
    bitWrite(block[1], i, 0);
  }
  updateShiftRegister();

  //turn off all pixels
  for (int i = 0; i < sizeof(pixels); i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
  }
  pixels.show();
}

