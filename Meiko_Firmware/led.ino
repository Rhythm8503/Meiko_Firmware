/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void Visualizer(){
  //Collect Samples
  getSamples();
  
  //Update Display
  //displayUpdate();
  
  FastLED.show();
}

void getSamples(){
  for(int i = 0; i < SAMPLES; i++){
    vReal[i] = analogRead(MIC_IN);
    Serial.println(vReal[i]);
    vImag[i] = 0;
  }

  //FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  //Update Intensity Array
  for(int i = 2; i < (LED_MATRIX_WIDTH * Displacement)+2; i+=Displacement){
    vReal[i] = constrain(vReal[i],0 ,2047);            // set max value for input data
    vReal[i] = map(vReal[i], 0, 2047, 0, LED_MATRIX_HEIGHT);        // map data to fit our display

    Intensity[(i/Displacement)-2] --;                      // Decrease displayed value
    if (vReal[i] > Intensity[(i/Displacement)-2])          // Match displayed value to measured value
      Intensity[(i/Displacement)-2] = vReal[i];
  }
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