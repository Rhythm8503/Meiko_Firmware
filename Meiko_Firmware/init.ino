/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void FFT_Init() {
    sampling_period_us = round(1000000*(1.0/samplingFrequency));
}