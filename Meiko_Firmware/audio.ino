/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/
void getSamples(){
  // Reset bandValues[]
  for (int i = 0; i < NUM_BANDS; i++){
    bandValues[i] = 0;
  }

  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }

  // Compute FFT
  FFT.dcRemoval();
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
    
    //19 bands, ~12-14kHz top band
      if (i<=2 )           bandValues[0]  += (int)vReal[i];
      if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i]; // Diff 1
      if (i>3   && i<=5  ) bandValues[2]  += (int)vReal[i]; // Diff 2
      if (i>5   && i<=7  ) bandValues[3]  += (int)vReal[i]; // Diff 2
      if (i>7   && i<=9  ) bandValues[4]  += (int)vReal[i]; // Diff 2
      if (i>9   && i<=13 ) bandValues[5]  += (int)vReal[i]; // Diff 4
      if (i>13  && i<=18 ) bandValues[6]  += (int)vReal[i]; // Diff 5
      if (i>18  && i<=25 ) bandValues[7]  += (int)vReal[i]; // Diff 7
      if (i>25  && i<=36 ) bandValues[8]  += (int)vReal[i]; // Diff 9
      if (i>36  && i<=47 ) bandValues[9]  += (int)vReal[i]; // Diff 11
      if (i>47  && i<=62 ) bandValues[10] += (int)vReal[i]; // Diff 15
      if (i>62  && i<=79 ) bandValues[11] += (int)vReal[i]; // Diff 17
      if (i>79  && i<=100) bandValues[12] += (int)vReal[i]; // Diff 21
      if (i>100 && i<=125) bandValues[13] += (int)vReal[i]; // Diff 25
      if (i>125 && i<=158) bandValues[14] += (int)vReal[i]; // Diff 33
      if (i>158 && i<=199) bandValues[15] += (int)vReal[i]; // Diff 41
      if (i>199 && i<=248) bandValues[16] += (int)vReal[i]; // Diff 49
      if (i>248 && i<=305) bandValues[17] += (int)vReal[i]; // Diff 57
      if (i>305          ) bandValues[18] += (int)vReal[i]; // Diff 65
    }
  }
}