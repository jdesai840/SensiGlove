#include <Base64.h>
#include <neosensory_bluefruit.h>
#include <Adafruit_CircuitPlayground.h>

NeosensoryBluefruit NeoBluefruit;

// Defining which capactive touch pin corresponds to which finger
int index_middle = 6;
int ring_pinky = 0;
int thumb = 9;
int palm = 2;

// Threshold for determining capacitive touch readings corresponding to human skin (from trial and error data collection)
int touchThreshold = 1000;


// Function to detect whether a certain finger is currently touch a human
boolean touchTrigger(uint8_t touchPin) {
    if (CircuitPlayground.readCap(touchPin) > touchThreshold) {
        return true;
    }
    else {
        return false;
    }
}


// Function to indicate Bluetooth connectivity
void connectionFlash(int red, int green, int blue) {
     for(int i = 0; i < 9; i++) {
         CircuitPlayground.setPixelColor(i, red, green, blue);
     }

     delay(200);

     for(int i = 0; i < 9; i++) {
         CircuitPlayground.setPixelColor(i, 0, 0, 0);
     }

     delay(100);

     for(int i = 0; i < 9; i++) {
         CircuitPlayground.setPixelColor(i, red, green, blue);
     }

     delay(200);

     for(int i = 0; i < 9; i++) {
         CircuitPlayground.setPixelColor(i, 0, 0, 0);
     }
}


//Initialization of microcontroller and Neosensory Buzz BLE
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  NeoBluefruit.begin();
  NeoBluefruit.setConnectedCallback(onConnected);
  NeoBluefruit.setDisconnectedCallback(onDisconnected);
  NeoBluefruit.setReadNotifyCallback(onReadNotify);
  NeoBluefruit.startScan();
  while (!NeoBluefruit.isConnected() || !NeoBluefruit.isAuthorized()) {}
  NeoBluefruit.deviceInfo();
  NeoBluefruit.deviceBattery();
}


//Haptic feedback mechanism based on capactive touch
void loop() {
  if (NeoBluefruit.isConnected() && NeoBluefruit.isAuthorized()) {
      if (touchTrigger(index_middle)) {
          NeoBluefruit.vibrateMotor(3, 1);
          CircuitPlayground.setPixelColor(5, 255, 0, 0);
          CircuitPlayground.setPixelColor(6, 255, 0, 0);
      }
      else {
          NeoBluefruit.vibrateMotor(3, 0);
          CircuitPlayground.setPixelColor(5, 0, 0, 0);
          CircuitPlayground.setPixelColor(6, 0, 0, 0);
      }
      if (touchTrigger(ring_pinky)) {
          NeoBluefruit.vibrateMotor(2, 1);
          CircuitPlayground.setPixelColor(3, 0, 255, 0);
          CircuitPlayground.setPixelColor(4, 0, 255, 0);
      }
      else {
          NeoBluefruit.vibrateMotor(2, 0);
          CircuitPlayground.setPixelColor(3, 0, 0, 0);
          CircuitPlayground.setPixelColor(4, 0, 0, 0);
      }
      if (touchTrigger(thumb)) {
          NeoBluefruit.vibrateMotor(1, 1);
          CircuitPlayground.setPixelColor(7, 125, 125, 125);
          CircuitPlayground.setPixelColor(8, 125, 125, 125);
      }
      else {
          NeoBluefruit.vibrateMotor(1, 0);
          CircuitPlayground.setPixelColor(7, 0, 0, 0);
          CircuitPlayground.setPixelColor(8, 0, 0, 0);
      }
      if (touchTrigger(palm)) {
          NeoBluefruit.vibrateMotor(0, 1);
          CircuitPlayground.setPixelColor(0, 0, 0, 255);
          CircuitPlayground.setPixelColor(1, 0, 0, 255);
          CircuitPlayground.setPixelColor(2, 0, 0, 255);
          CircuitPlayground.setPixelColor(9, 0, 0, 255);
      }
      else {
          NeoBluefruit.vibrateMotor(0, 0);
          CircuitPlayground.setPixelColor(0, 0, 0, 0);
          CircuitPlayground.setPixelColor(1, 0, 0, 0);
          CircuitPlayground.setPixelColor(2, 0, 0, 0);
          CircuitPlayground.setPixelColor(9, 0, 0, 0);
      }
  }
}


//Bluetooth connectivity callback for connection
void onConnected(bool success) {
  if (!success) {
    Serial.println("Attempted connection but failed.");
    return;
  }
  connectionFlash(0, 255, 0);
  Serial.println("Connected!");

  // Once we are successfully connected to the wristband,
  // send developer autherization command and commands
  // to stop sound-to-touch algorithm.
  NeoBluefruit.authorizeDeveloper();
  NeoBluefruit.acceptTermsAndConditions();
  NeoBluefruit.stopAlgorithm();
}


//Bluetooth connectivity callback for disconnection
void onDisconnected(uint16_t conn_handle, uint8_t reason) {
  Serial.println("\nDisconnected");
  connectionFlash(255, 0, 0);
}


void onReadNotify(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  for (int i = 0; i < len; i++) {
    Serial.write(data[i]);
  }
}
