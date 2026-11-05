/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void LED_Matrix(void* pvParameters) {

    Serial.println("LED Matrix Task Started");
    
    while(1) {
        Visualizer();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}



void Visualizer(){
  //Collect Samples
  getSamples();
  
  //Update Display
  //displayUpdate();
  
  FastLED.show();
}

// void displayUpdate(){
//   int color = 0;
//   for(int i = 0; i < LED_MATRIX_WIDTH; i++){
//     for(int j = 0; j < LED_MATRIX_HEIGHT; j++){
//       if(j <= Intensity[i]){                                // Light everything within the intensity range
//         if(j%2 == 0){
//           leds[(xres*(j+1))-i-1] = CHSV(color, 255, BRIGHTNESS);
//         }
//         else{
//           leds[(xres*j)+i] = CHSV(color, 255, BRIGHTNESS);
//         }
//       }
//       else{                                                  // Everything outside the range goes dark
//         if(j%2 == 0){
//           leds[(xres*(j+1))-i-1] = CHSV(color, 255, 0);
//         }
//         else{
//           leds[(xres*j)+i] = CHSV(color, 255, 0);
//         }
//       }
//     }
//     color += 255/xres;                                      // Increment the Hue to get the Rainbow
//   }
// }