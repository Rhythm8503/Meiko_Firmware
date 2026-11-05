/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void ADXL345_Task(void* pvParameters) {

    Serial.println("ADXL345 Task Started");

    while(1) {
        /* Get a new sensor event */ 
        sensors_event_t event; 
        accel.getEvent(&event);

        // Process the accelerometer data
        float x = event.acceleration.x;
        float y = event.acceleration.y;
        float z = event.acceleration.z;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}