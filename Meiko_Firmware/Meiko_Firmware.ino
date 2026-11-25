/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void setup() {
  // Arduino required setup function
  ESP32_Init();
  vTaskDelay(pdMS_TO_TICKS(50)); //RTOS Function Initalization

  FFT_Init();
  ADXL345_Init();
  BLE_init();

  vTaskDelay(pdMS_TO_TICKS(50)); //BLE Activation -> Minor current pulse
  
  LED_Matrix_Init() 
  vTaskDelay(pdMS_TO_TICKS(50)); //LED Activation -> Delay
}

void loop() {
// Core Arduino loop function

/* Notes for self

Logic to add: FFT -> LED Control
Pins to Audio, FFT, LED and etc. 
Functions such as LED Matrix, Accelerometer and Bluetooth 

*/

}