/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void setup() {
  // Arduino required setup function
  ESP32_Init();
  FFT_Init();
  ADXL345_Init();
  BLE_init();
}

void loop() {
// Core Arduino loop function

/* Notes for self

Logic to add: FFT -> LED Control
Pins to Audio, FFT, LED and etc. 
Functions such as LED Matrix, Accelerometer and Bluetooth 

*/

}
