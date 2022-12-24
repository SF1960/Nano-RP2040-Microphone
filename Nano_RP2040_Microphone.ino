#include <WiFiNINA.h>
#include <PDM.h>

#define LED_RED 12              // connect a Red LED to pin 12
#define LED_GREEN 11            // connect a Green LED to pin 11
#define LED_BLUE 10             // connect a Blue LED to pin 10
#define LED_YELLOW 9            // connect a Yellow LED to pin 9
#define LED_WHITE 8             // connect a White LED to pin 8

bool LED_SWITCH = false;

// default number of output channels
static const char channels = 1;

// default PCM output frequency
static const int frequency = 16000;
static const int Loudness = 2500;

// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[512];

// Number of audio samples read
volatile int samplesRead;

void setup() {
  Serial.begin(9600);
  //define onboard RGB LED as Output
  pinMode(LEDB, OUTPUT);
  
  //define external LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  
  
  while (!Serial);
  // Configure the data receive callback
  PDM.onReceive(onPDMdata);

  // Optionally set the gain
  // Defaults to 20 on the BLE Sense and -10 on the Portenta Vision Shields
  // PDM.setGain(30);

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
  // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shields
  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void loop() {
  // Wait for samples to be read
  if (samplesRead) {

    // Print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {
      if (channels == 2) {
        Serial.print("L:");
        Serial.print(sampleBuffer[i]);
        Serial.print(" R:");
        i++;
      }
      Serial.println(sampleBuffer[i]);

      if (sampleBuffer[i] > Loudness || sampleBuffer[i] <= -Loudness) {
        LED_SWITCH = !LED_SWITCH;
        if (LED_SWITCH) {
          Serial.println();
          digitalWrite(LEDB, HIGH);
          turnOnLEDS();
          
          Serial.println("ON!");
          Serial.println();
          delay(2000);
        }
        else {
          Serial.println();
          digitalWrite(LEDB, LOW);
          turnOffLEDS();
          
          Serial.println("OFF!");
          Serial.println();
          delay(2000);
        }
      }
    }

    // Clear the read count
    samplesRead = 0;
  }
}

/**
   Callback function to process the data from the PDM microphone.
   NOTE: This callback is executed as part of an ISR.
   Therefore using `Serial` to print messages inside this function isn't supported.
 * */
void onPDMdata() {
  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}

void turnOnLEDS(){
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_YELLOW, HIGH); 
  digitalWrite(LED_WHITE, HIGH);  
}

void turnOffLEDS(){
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW); 
  digitalWrite(LED_WHITE, LOW);  
  
}