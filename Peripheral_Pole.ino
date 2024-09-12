/*
  
  Joseph Del Toro
  3/25/2024
  Release: Final 
  UVM SEED Team 3: 

  This is the BLE peripheral Code to the Auto Lift Heel riser, all code is modifed from the arduino BLE
  library example. 
  This code resides in the arduino that is inside a ski pole. 
  It Creates a BLE Characteristic that the BLE central device can connect to, Once connected Data is only sent once a button press 
  is detected on the ski pole. 





  FUNCTOINS: 
  Void setup() # initialize the Serial communication and pins
      PinMode(Pin,State) #Used to initialize the button and Leds pins. Input Pin number or name, and the state: (Ouput or input, or inputPullup which includes a resistor). 

  BLE.setLocalName #Set the local name of our device

  BLE.setAdvertsiedService() # input our actual service, and begin broadcasting on the BLE bandwidth. Returns nothing, no functions call it 

  ledService.addCharacteristic() #adds characteristic to the LED service. Input: characteristics with UUID. Returns nothing, no functions call it 

Void Loop() # constantly loops, preforms no actions unless a button press is detected. If a press is detected the appropriate value will be uploaded to the button characteristic. 
buttonCharacteristic.writeValue() # updates button Characteristic with a (0x01) or (0x02) depending on the pressed button. Not called by any other function. Will not return anything 








*/

#include <ArduinoBLE.h>

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED
const int buttonPin1 = 5; 
const int buttonPin2 = 6; 
const int redled = 3;

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // Not used but left in not to break code 

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  //while (!Serial);  IMPORTANT: IF you are connected via computer usb, uncomment this code. IF connected via battery power keep code commented out! 

  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin1, INPUT_PULLUP); 
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(redled,OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  
  BLE.setLocalName("DelToro");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);

  // add the service
  BLE.addService(ledService);

  ledCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);

  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
  digitalWrite(ledPin,HIGH);
  digitalWrite(redled,HIGH);
  delay(500);
  digitalWrite(ledPin,LOW);
  digitalWrite(redled,LOW);
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();
  int buttonValue1 = digitalRead(buttonPin1); 
  int buttonValue2 = digitalRead(buttonPin2);

  // check if the button state has changed from high to low
  static bool buttonWasHigh1 = true;
  static bool buttonWasHigh2 = true; 


  if (buttonValue1 == LOW && buttonWasHigh1) {
    // update characteristics only if button changes from high to low

    ledCharacteristic.writeValue(1); // Left because code breaks without these characteristics 
    buttonCharacteristic.writeValue(1); // Button state is pressed
    digitalWrite(ledPin, HIGH);

    delay(400); 

    digitalWrite(ledPin,LOW);
    buttonWasHigh1 = false;
  } else if (buttonValue1 == HIGH) {
    // reset the button state tracking when button goes back to high
    
    digitalWrite(ledPin,LOW);
    buttonWasHigh1 = true;
  }


   if (buttonValue2 == LOW && buttonWasHigh2) {
    // update characteristics only if button changes from high to low

    ledCharacteristic.writeValue(2); // LED Char does nothing, here so code doesn't break 
    buttonCharacteristic.writeValue(2); // Button state is pressed
    digitalWrite(ledPin, HIGH); 
    digitalWrite(redled,HIGH);

    delay(400); 
    digitalWrite(redled,LOW); 

    buttonWasHigh2 = false;
  } else if (buttonValue2 == HIGH) {
    // reset the button state tracking when button goes back to high
    //buttonCharacteristic.writeValue(2);
    digitalWrite(ledPin,LOW);
    buttonWasHigh2 = true;
  }



}






