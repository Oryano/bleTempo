
String beanName = "Tempo";
const uint8_t ledScratch = 1; //what is LED scrach?

void setup() {
  // Setup bean
  Bean.setBeanName(beanName);
  Bean.enableWakeOnConnect(true);

  // Reset the scratch data area 1. 
  uint8_t resetLedBuffer[] = {0, 0, 0}; //other type???? is this the data received format?
  Bean.setScratchData(ledScratch, resetLedBuffer, 3);
}

// the loop routine runs over and over again forever:
void loop() {

  bool connected = Bean.getConnectionState();

  if(connected) {

    //Update the clock
    ScratchData receivedData = Bean.readScratchData(ledScratch); 
    
    uint8_t hours = receivedData.data[0];
    uint8_t minutes = receivedData.data[1];

    //adjust clock by the new set hour
    //adjust fequency by the new Freq
    //adjust LED to on or off
    //adjust to incoming "story pattern"
    //manipulate time to the new time
    
   }
  else {
    //sleep for 5 minutes Bean.sleep(0xFFFFFFFF); 
    //wake up for pattern (2 seconds) and battery status
    //back to sleep for 4:58
  }
    
    
//    // Update LED
//    ScratchData receivedData = Bean.readScratchData(ledScratch); 
//
//    uint8_t redLed = receivedData.data[0];
//    uint8_t greenLed = receivedData.data[1];
//    uint8_t blueLed = receivedData.data[2];
//
//    Bean.setLed(redLed, greenLed, blueLed);
//  }
//  else {
//    
//    // Turn LED off and put to sleep. 
//    Bean.setLed(0, 0, 0);
//    Bean.sleep(0xFFFFFFFF); 
//  }
}

