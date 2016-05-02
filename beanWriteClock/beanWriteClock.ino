
String beanName = "Tempo";
const uint8_t timeScratch = 1; //what is LED scrach? what is 1?
bool calcTime = false;
ScratchData receivedData; //a var type that holds the data received
unsigned long timeFromPhone;  
unsigned long finalString;
unsigned long currentMillis;
unsigned long beanTime;
long count = 0; //for millis update

int hours;      //is the RTC now boject
int minutes;    //is the RTC now boject
boolean intervalSession = true;
unsigned long wholeTime = 5000;      //5 = 300,000 minuts interval
const int workingTime = 2000;     //2 seconds pattern happening
unsigned long intervalTime = wholeTime - workingTime;

unsigned long lastCheck = 0;
//int vibePin[8];   //number of pins connected

unsigned long currentTime;          //from millis()
unsigned long lastPatternCheck;     //for 500 ms intervals

int patternState = 1; //there are 4 states (each 500 ms)




void setup() {
  Serial.begin(57600);

  // Setup bean
  Bean.setBeanName(beanName);
  Bean.enableWakeOnConnect(true);

  // Reset the scratch data area 1.
  uint8_t resetTimeBuffer[] = {0, 0, 0};
  Bean.setScratchData(timeScratch, resetTimeBuffer, 3);
}



void loop() {
  count++;
  currentMillis = millis(); //+ (count*10000); FOR WHEN BEAN SLEEPS!!!
  
  //Serial.println(currentMillis);
  //delay(100);
  
  if (currentMillis - lastCheck >= intervalTime) {
    if (intervalSession == true) {
      intervalSession = false;
      patternState = 1;
      lastPatternCheck = currentMillis;
      loadPattern(patternState);      //call function that takes the right pattern
    }
    if (currentMillis - lastCheck >= wholeTime) {
      intervalSession = true;
      lastCheck = currentMillis;
    }
  }

  if (intervalSession) {
    //interval time!! turn vibe and LED off - Do nothing ADD sleep here !!!
    //turn motors off
    //Bean.sleep(wholeTime);  
    
    //Serial.println("----I----");
  }
  else {  //the 2 seconds in which to do pattern

    if (currentMillis - lastPatternCheck > 500) { //4 frames of pattern 500 ms each
      patternState ++;  //go to next position of digit to read
      loadPattern(patternState);
      lastPatternCheck = currentMillis;  //timestamp of last update
    }

    //Serial.println("----I----");
  }
  

  bool connected = Bean.getConnectionState();
  if (connected) {
    Serial.println("Tempo device connected");
    calcTime = true;
    
    //Update time from phone
    receivedData = Bean.readScratchData(timeScratch);
    //uint8_t hours = receivedData.data[0];

  }

  if (calcTime) {
    String one = String(receivedData.data[0]);
    String two = String((receivedData.data[1] * 255) + receivedData.data[2]);
    String three = "000";
    String finalString = one + two + three;
    
    timeFromPhone = finalString.toInt();  //converts to a long number
    //Serial.println(timeFromPhone);
    beanTime = timeFromPhone + currentMillis % 86399000; //use REMINDER from 24 HOURS ms to reset    
    hours = beanTime / 60 / 60 / 1000;
    minutes = (((beanTime / 60 / 60) % 1000)*60)/1000 ;
    
    Serial.print(hours);
    Serial.print(":");
    Serial.println(minutes);
    Serial.println("----");
        
  }

  if (receivedData.data[3] == 1){ //if testButton was tapped - make a crazy pattern
    //pattern here
    
    Bean.setLed(30, 200, 255);
    delay(1000);
    Bean.setLed(0, 0, 0);
  }


  // use data received
  ScratchData receivedData = Bean.readScratchData(timeScratch);

  //    uint8_t redLed = receivedData.data[0];
  //    uint8_t greenLed = receivedData.data[1];

  // Turn LED off and put to sleep.
  //Bean.setLed(0, 0, 0);
  //Bean.sleep(10000);
}

void loadPattern( int clockDigit ) { //takes the digit from the c

  if (clockDigit == 1 ) {
    //show frame 1
    int smooth = floor(hours) / 10; //find the first digit of hour
    //is smooth the first digit ???
    //Serial.println(smooth);
    showPattern(smooth);
  }
  else if (clockDigit == 2 ) {
    //show frame 2
    int leftOver = hours % 10;  //find the second digit of hour
    showPattern(leftOver);
  }
  else if (clockDigit == 3 ) {
    //show frame 3
    int smooth = floor(minutes) / 10; //find the third digit of minutes
    showPattern(smooth);
  }
  else if (clockDigit == 4 ) {
    //show frame 4
    int leftOver = minutes % 10;  //find the fourth digit of minutes
    showPattern(leftOver);
    Serial.println( "---finish last pattern---" );
  }
}

void showPattern( int digitContent ) { //digitContent is the digit in the clock

  Serial.println( "-----------" );
  //Serial.println( content );

  if (digitContent == 0) {
    Serial.println( "sequence when digit is '0'" ); //all HIGH
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 1) {
    Serial.println( "sequence when digit is '1'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 2) {
    Serial.println( "sequence when digit is '2'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 3) {
    Serial.println( "sequence when digit is '3'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 4) {
    Serial.println( "sequence when digit is '4'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 5) {
    Serial.println( "sequence when digit is '5'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 6) {
    Serial.println( "sequence when digit is '6'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 7) {
    Serial.println( "sequence when digit is '7'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
   }
  else if (digitContent == 8) {
    Serial.println( "sequence when digit is '8'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
  else if (digitContent == 9) {
    Serial.println( "sequence when digit is '9'" );
    //turn all motors off first
    //go pattern when I have X anywhere in my clock
  }
}



    //adjust clock by the new set hour
    //adjust fequency by the new Freq
    //adjust LED to on or off
    //adjust to incoming "story pattern"
    //manipulate time to the new time

