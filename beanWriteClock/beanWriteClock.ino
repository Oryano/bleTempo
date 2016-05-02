
String beanName = "Tempo";
const uint8_t timeScratch = 1; //what is LED scrach? what is 1?
bool calcTime = false;
ScratchData receivedData; //a var type that holds the data received
unsigned long timeFromPhone;
unsigned long finalString;
unsigned long currentMillis;
unsigned long beanTime;
int hours;
int minutes;
long count = 0; //for millis update

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
  currentMillis = millis(); //+ (count*10000);
  
  //Serial.println(currentMillis);
  //delay(100);


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






    //adjust clock by the new set hour
    //adjust fequency by the new Freq
    //adjust LED to on or off
    //adjust to incoming "story pattern"
    //manipulate time to the new time

