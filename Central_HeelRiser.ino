/*
Joseph Del Toro
4/22/2024 

Release: Final 

SEED Team 03, Central_heel Riser code for arduino Nano BLE sense rev2. 

All functions used: 

“void setup() #Code executed only once 
BLE.begin() # Load BLE Library, does not return anything, Not called 

BLE.scanforName: #Scan for BLE device with local name, takes in string input 

Void Loop {} #this code will be constantly looped through until power is removed.   
  BLEDevice peripheral #this function creates a BLE peripheral, once a peripheral is scanned. 

  BLE.available() #returns a list of all available BLE devices, this code uses a BLE.scanforName function which ignores all other BLE devices. 

  Peripheral.localName #returns Boolean true or false depending if the correct name is found 

Void controlled() #input a selected BLE peripheral, determines if peripheral has valid characteristics. 
  Peripheral.connect() #returns Boolean True or false, if not connected the code exits the loop and returns to the void loop()

  Peripheral.discoverAttributes() # Returns Boolean True: code continues, Boolean False: Central disconnects from Peripheral and exit to void loop()

  BLECharacteristic buttonCharacteristic = peripheral.characteristic(“128bit char) # Creates a custom BLE characteristic called ButtonCharacteristic, IF the central is able to find that particular characteristic. 
  buttonCharacteristic.canRead() # Returns Boolean True or false, 

  buttonCharacteristic.canSubscribe() #Returns Boolean true or false, code proceeds if found 

  peripheral.connected() #Returns Boolean true or false. If true code proceeds to check for button presses.
  IF false code will loop through and try to find peripheral 

  buttonCharacteristic.valueUpdated() #checks the button Characteristic to see if a new value has been uploaded to the server. 
  If the characteristic is not updated the code loops through again. 

  Value #unsigned character, stores 1 byte array for the button presses 
  buttonCharacteristic.readValue(value,1) #reads the value updated from the buttonCharacteristic, input a “value” to store the data, and 1 refers to the number of byte arrays used. 

*/





#include <ArduinoBLE.h>
const int forwardPin = 5; 
const int backwardPin = 4;


void setup() {
  Serial.begin(9600);
  //while (!Serial);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForName("DelToro");
  pinMode(forwardPin,OUTPUT);
  pinMode(backwardPin,OUTPUT);
  Serial.println("scanning for Del Toro");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "DelToro") {
      Serial.println("device discovered checking characteristics...");
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForName("DelToro");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the button characteristic
  BLECharacteristic buttonCharacteristic = peripheral.characteristic("19b10011-e8f2-537e-4f6c-d104768a1214");

  if (!buttonCharacteristic) {
    Serial.println("Peripheral does not have button characteristic!");
    peripheral.disconnect();
    return;
  } else if (!buttonCharacteristic.canRead()) {
    Serial.println("Peripheral does not have a readable button characteristic!");
    peripheral.disconnect();
    return;
  }


  Serial.println("Subscribing to pressure characteristic ...");
  if (!buttonCharacteristic) {
    Serial.println("No pressure characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!buttonCharacteristic.canSubscribe()) {
    Serial.println("Pressure characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!buttonCharacteristic.subscribe()) {
    Serial.println("Subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
    Serial.println("Please wait for data:");
  }

  

 
  while (peripheral.connected()) {
    
    delay(50);
    // while the peripheral is connected
    if (buttonCharacteristic.valueUpdated()) {
      unsigned char value[1]; // byte array to store data
      buttonCharacteristic.readValue(value, 1);
      int buttonValue = value[0]; // extract button value from the received data
      Serial.print("Button state: ");
      Serial.println(buttonValue);
      Serial.println("waiting for next value: ");
        if (buttonValue ==1) {
          Serial.print("moving motor: ");
          digitalWrite(forwardPin,LOW);
          digitalWrite(backwardPin,HIGH);
          delay(4700);

          digitalWrite(forwardPin,LOW);
          digitalWrite(backwardPin,LOW);
        }

        if (buttonValue ==2) { 
          Serial.println("Moving motor backwards"); 
          digitalWrite(forwardPin, HIGH); 
          digitalWrite(backwardPin,LOW); 

          delay(4700); // Will need to perfect this 

          digitalWrite(forwardPin,LOW); 
          digitalWrite(backwardPin,LOW); 
        }
      
       

     
      
    }
  }

  Serial.println("Peripheral disconnected");
}
