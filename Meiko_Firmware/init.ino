/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void FFT_Init() {
    sampling_period_us = round(1000000*(1.0/samplingFrequency));
}

void ADXL345_Init() {
    Serial.println("Initializing ADXL345...");

    accel.begin();
    accel.setRange(ADXL345_RANGE_16_G);

    Serial.println("ADXL345 Initialized");
}

void ESP32_Init() {
    Serial.begin(115200);
    Serial.println("ESP32-C3 Initialized");
    
    // Create FreeRTOS Tasks
    xTaskCreatePinnedToCore(LED_Matrix, "LED_Matrix Task Handle", 2048, NULL, 0, &LEDTaskHandle, 1);
    xTaskCreatePinnedToCore(ADXL345_Task, "ADXL345 Task Handle", 2048, NULL, 0, &ADXLTaskHandle, 1);
    xTaskCreatePinnedToCore(BLE_Management, "BLE Task Handle", 2048, NULL, 0, &BLE_TaskHandle, 0);

    Serial.println("FreeRTOS Tasks Created");
}

void BLE_init() {
    Serial.println("Initializing BLE...");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();
    pServer->getAdvertising()->start();
    startTime = millis();

    Serial.println("BLE Initialized");
}