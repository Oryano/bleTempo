
String beanName = "Tempo";
const uint8_t timeScratch = 1; //what is LED scrach? what is 1?
bool printTime = false;
ScratchData receivedData; //a var type that holds the data received
unsigned long timeFromPhone;

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



  bool connected = Bean.getConnectionState();
  if (connected) {
    Serial.print("Tempo device connected");
    printTime = true;
    

    //Update time from phone
    receivedData = Bean.readScratchData(timeScratch);

    //uint8_t hours = receivedData.data[0];

  }

  if (printTime) {
        
    String one = String(receivedData.data[0]);
    String two = String((receivedData.data[1] * 255) + receivedData.data[2]);
    String three = "000";
    String finalString = one + two + three;
    String test = "1234556789";
    timeFromPhone = long("1234556789");  //long converts to a long number
    Serial.println(timeFromPhone);
     long number = timeFromPhone + 200;
     Serial.println(number);
     
  }


  // use data received
  ScratchData receivedData = Bean.readScratchData(timeScratch);

  //    uint8_t redLed = receivedData.data[0];
  //    uint8_t greenLed = receivedData.data[1];

  if (receivedData.data[0] != 0) {
    Bean.setLed(255, 20, 20);
  }

  // Turn LED off and put to sleep.
  //Bean.setLed(0, 0, 0);
  Bean.sleep(0xFFFFFFFF);
}






    //adjust clock by the new set hour
    //adjust fequency by the new Freq
    //adjust LED to on or off
    //adjust to incoming "story pattern"
    //manipulate time to the new time

