/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

/***************************************************************************************** 
                              Libraries and Library Pointers
******************************************************************************************/

/* Core Libraries */

#include <Wire.h>                   // i2C Initalization for ESP32-S3
#include "freertos/FreeRTOS.h"      // FreeRTOS OTA
#include "freertos/task.h"          // FreeRTOS Task
#include "freertos/event_groups.h"  // FreeRTOS Event Groups
#include <BLEDevice.h>              // BLE Driver
#include <BLEServer.h>              // BLE Driver
#include <BLEUtils.h>               // BLE Driver
#include <BLE2902.h>                // BLE Driver
#include <arduinoFFT.h>             // Arduino FFT Library
#include <FastLED.h>                // FastLED Library for LED Matrix
#include <Adafruit_Sensor.h>        // Adafruit Sensor Library
#include <Adafruit_ADXL345_U.h>     // Adafruit ADXL345 Accelerometer Library

/* Pointers for Libraries */
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/***************************************************************************************** 
                                  FreeRTOS Thread Handles
******************************************************************************************/
TaskHandle_t LEDTaskHandle;     // LED Process for LED Matrix
TaskHandle_t ADXLTaskHandle;    // ADXL345 Process for Motion Detection
TaskHandle_t BLE_TaskHandle;    // BLE Process for Bluetooth Communication

/***************************************************************************************** 
                                      Definitions
******************************************************************************************/

#define PI 3.14159265358979323846
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / PI)

/* Bluetooth Server UUIDs */
#define SERVICE_UUID "d1b789dc-2570-4e6d-bc6c-afa54acc21d3"               
#define CHARACTERISTIC_UUID_RX "dd52c5e5-0caa-48d4-86b9-16170b56fd53"
#define CHARACTERISTIC_UUID_TX "079081f4-3f8f-4806-bff6-4d5d1ac05148"

/* FFT Definitions */
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

/* Display Definitions */
#define LED_MATRIX_HEIGHT 8
#define LED_MATRIX_WIDTH 32

/***************************************************************************************** 
                                    Global Variables
******************************************************************************************/

/* FFT Variables */
const uint16_t samples = 32; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 4000; //Hz, must be less than 10000 due to ADC
unsigned int sampling_period_us;
unsigned long microseconds;
int num[samples] = {0, 3, 0, 3, 0, 3, 0, 3};

double vReal[samples];
double vImag[samples];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);
int num[samples] = {0, 3, 0, 3, 0, 3, 0, 3};

/* Display Variables */
int Intensity[xres] = { }; // initialize Frequency Intensity to zero
int Displacement = 1;

/***************************************************************************************** 
                              Boolean/Semaphore Definitions
******************************************************************************************/


/***************************************************************************************** 
                              Enumeration  Definitions
******************************************************************************************/