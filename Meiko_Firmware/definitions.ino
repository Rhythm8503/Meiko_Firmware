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
#include <FastLED_NeoMatrix.h>      // FastLED NeoMatrix Library
#include <Adafruit_Sensor.h>        // Adafruit Sensor Library
#include <Adafruit_ADXL345_U.h>     // Adafruit ADXL345 Accelerometer Library


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
#define SAMPLES         1024          // Must be a power of 2
#define SAMPLING_FREQ   40000         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       1600          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define AUDIO_IN_PIN    4             // Signal in on this pin
#define LED_PIN         5             // LED strip data
#define COLOR_ORDER     GRB           // If colours look wrong, play with this
#define CHIPSET         WS2812B       // LED strip type
#define MAX_MILLIAMPS   3000          // Careful with the amount of power here if running off USB port
#define NUM_BANDS       32            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE           600           // Used as a crude noise filter, values below this are ignored

/* Display Definitions */
const uint8_t kMatrixWidth = 16;                          // Matrix width
const uint8_t kMatrixHeight = 16;                         // Matrix height
#define NUM_LEDS       (kMatrixWidth * kMatrixHeight)     // Total number of LEDs
#define BAR_WIDTH      (kMatrixWidth  / (NUM_BANDS - 1))  // If width >= 8 light 1 LED width per bar, >= 16 light 2 LEDs width bar etc
#define TOP            (kMatrixHeight - 0)                // Don't allow the bars to go offscreen
#define SERPENTINE     true                               // Set to false if you're LEDS are connected end to end, true if serpentine

/***************************************************************************************** 
                                    Global Variables
******************************************************************************************/

/* FFT Variables */
unsigned int sampling_period_us;
byte peak[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};              // The length of these arrays must be >= NUM_BANDS
int oldBarHeights[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ);

/* Display Variables */
int Intensity[xres] = { }; // initialize Frequency Intensity to zero
int Displacement = 1;

/* LED Variables */
volatile uint8_t LED_Mode = 0;
volatile uint8_t LED_Brightness = 30;

/*****************************************************************************************
            LED Modes | 0 = Default RED | 1 = Blue | 2 = Green | 3 = Rainbow
******************************************************************************************/

/* Motion Variables */
volatile uint8_t Motion_Mode = 0;

/*****************************************************************************************
                Motion Modes | 0 = OFF | 1 = LED Pulse | 2 = LED Flash
******************************************************************************************/

/* Bluetooth Variables */
char commandBuffer[32];         // Buffer for command parsing
char originalCommand[32];       // Buffer to store original command for echo

/***************************************************************************************** 
                              Boolean/Semaphore Definitions
******************************************************************************************/

/* Bluetooth Booleans */
bool deviceConnected = false;
bool oldDeviceConnected = false;
bool newCommandReceived = false;

/***************************************************************************************** 
                              Enumeration  Definitions
******************************************************************************************/

// Context enum for menu state [Bluetooth Communication]
enum Context {
  MAIN,
  LED_MENU,
  MOTION_MENU
};

// State variables
Context currentContext = MAIN;

/***************************************************************************************** 
                              Library  Definitions
******************************************************************************************/

/* Pointers for Libraries */
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, kMatrixWidth, kMatrixHeight,
  NEO_MATRIX_TOP        + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS       + NEO_MATRIX_ZIGZAG +
  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS);