#include <Base64.h>
#include <neosensory_bluefruit.h>
#include <Adafruit_CircuitPlayground.h>
#include <sensiglove_inference.h>

NeosensoryBluefruit NeoBluefruit;

// Function to get raw feature data
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

// Printf function for debug output
void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };
    va_list args;
    va_start(args, format);
    vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);
    Serial.write(print_buf);
}

// Defining which capactive touch pin corresponds to which finger
int indexf = 10;
int middle = 6;
int ring = 0;
int pinky = 2;
int thumb = 9;
int palm = 3;

// Threshold for determining capacitive touch readings corresponding to human skin (from trial and error data collection)
int touchThreshold = 1000;
float bodyTemp = 33.0;
float tempScale;
int tempWheel;

// Raw features array for SensiGlove Inference
static const float features[] = {
    // Copy raw features here
};

// Function prototypes
boolean touchTrigger(uint8_t touchPin);
void connectionFlash(int red, int green, int blue);
void tempSense();
void touchSense();
void lightSense();
void onConnected(bool success);
void onDisconnected(uint16_t conn_handle, uint8_t reason);
void onReadNotify(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len);
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr);
void ei_printf(const char *format, ...);


// Function to indicate Bluetooth connectivity
void connectionFlash(int red, int green, int blue) {
  for (int i = 0; i < 9; i++) {
    CircuitPlayground.setPixelColor(i, red, green, blue);
  }
  delay(200);
  for (int i = 0; i < 9; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
  delay(100);
  for (int i = 0; i < 9; i++) {
    CircuitPlayground.setPixelColor(i, red, green, blue);
  }
  delay(200);
  for (int i = 0; i < 9; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
}


//Temperature sensation toggling (experimental and optional)
void tempSense() {
  float senseTemp = CircuitPlayground.temperature();
  float diffTemp = senseTemp - bodyTemp;

  if (diffTemp >= 0) {
    tempScale = map(diffTemp, 0, 20, 0, 1000);
    tempWheel = map(diffTemp, 0, 15, 0, 9);

    for (int i = 0; i < tempWheel; i++) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0);
    }

    delay(200);

    for (int i = 0; i < tempWheel; i++) {
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
    }

    delay(10);
  }

  else if (diffTemp < 0) {
    tempScale = map(diffTemp, 0, -20, 0, 1000);
    tempWheel = map(diffTemp, 0, -15, 0, 9);

    for (int i = 0; i < tempWheel; i++) {
      CircuitPlayground.setPixelColor(i, 0, 0, 255);
    }

    delay(200);

    for (int i = 0; i < tempWheel; i++) {
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
    }

    delay(20);
  }

  float tempIntensity = (tempScale / 1000) / 2;


  Serial.println(tempWheel);
  float vibrationIntensities[] = {tempIntensity, tempIntensity, tempIntensity, tempIntensity};
  NeoBluefruit.vibrateMotors(vibrationIntensities);
}


//Haptic feedback mechanism based on capactive touch
void touchSense() {
  if (touchTrigger(indexf) || touchTrigger(middle)) {
    NeoBluefruit.vibrateMotor(1, 1);
    CircuitPlayground.setPixelColor(5, 255, 0, 0);
    CircuitPlayground.setPixelColor(6, 255, 0, 0);
  }
  else {
    NeoBluefruit.vibrateMotor(1, 0);
    CircuitPlayground.setPixelColor(5, 0, 0, 0);
    CircuitPlayground.setPixelColor(6, 0, 0, 0);
  }
  if (touchTrigger(ring) || touchTrigger(pinky)) {
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
    NeoBluefruit.vibrateMotor(0, 1);
    CircuitPlayground.setPixelColor(7, 125, 125, 125);
    CircuitPlayground.setPixelColor(8, 125, 125, 125);
  }
  else {
    NeoBluefruit.vibrateMotor(0, 0);
    CircuitPlayground.setPixelColor(7, 0, 0, 0);
    CircuitPlayground.setPixelColor(8, 0, 0, 0);
  }
  if (touchTrigger(palm)) {
    NeoBluefruit.vibrateMotor(3, 1);
    CircuitPlayground.setPixelColor(0, 0, 0, 255);
    CircuitPlayground.setPixelColor(1, 0, 0, 255);
    CircuitPlayground.setPixelColor(2, 0, 0, 255);
    CircuitPlayground.setPixelColor(9, 0, 0, 255);
  }
  else {
    NeoBluefruit.vibrateMotor(3, 0);
    CircuitPlayground.setPixelColor(0, 0, 0, 0);
    CircuitPlayground.setPixelColor(1, 0, 0, 0);
    CircuitPlayground.setPixelColor(2, 0, 0, 0);
    CircuitPlayground.setPixelColor(9, 0, 0, 0);
  }
}


//Function to feel light intensity through haptic feedback (experimental and optional)
void lightSense() {
  float lightIntensity = (CircuitPlayground.lightSensor())/1023.;
  Serial.println(lightIntensity);

  float lightVibrate[] = {lightIntensity, lightIntensity, lightIntensity, lightIntensity};
  NeoBluefruit.vibrateMotors(lightVibrate);
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

//Initialization of microcontroller and Neosensory Buzz BLE
void setup() {
    Serial.begin(115200);
    CircuitPlayground.begin();
    CircuitPlayground.setBrightness(20);
    NeoBluefruit.begin();
    NeoBluefruit.setConnectedCallback(onConnected);
    NeoBluefruit.setDisconnectedCallback(onDisconnected);
    NeoBluefruit.setReadNotifyCallback(onReadNotify);
    NeoBluefruit.startScan();
    while (!NeoBluefruit.isConnected() || !NeoBluefruit.isAuthorized()) {}
    NeoBluefruit.deviceInfo();
    NeoBluefruit.deviceBattery();
}


void loop() {
    if (NeoBluefruit.isConnected() && NeoBluefruit.isAuthorized()) {
        if (CircuitPlayground.slideSwitch()) {
            touchSense();
        } else {
            tempSense();
        }
        lightSense(); // Optional, remove if not needed

        ei_impulse_result_t result = { 0 };
        signal_t features_signal;
        features_signal.total_length = sizeof(features) / sizeof(features[0]);
        features_signal.get_data = &raw_feature_get_data;
        EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
        // Handle classification results
    }
    delay(1000);
}


