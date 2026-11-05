/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void BLE_Management(void* pvParameters) {
    Serial.println("BLE Management Task Started");

    while(1) {
        
    }
}


void Bluetooth_Handle() {
  if (newCommandReceived) {
    handleCommand(commandBuffer);
    newCommandReceived = false;
  }

  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    Serial.println("Start advertising");
    oldDeviceConnected = deviceConnected;
  }
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}

void handleCommand(char *cmd) {
  char response[32];

}

void sendResponse(const char *response) {
  char fullResponse[128];
  sprintf(fullResponse, "Received: %s\n%s", originalCommand, response);
  pTxCharacteristic->setValue((uint8_t *)fullResponse, strlen(fullResponse));
  pTxCharacteristic->notify();
}

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue = pCharacteristic->getValue();
    rxValue.trim(); /* Trim off White Space */

    if (rxValue.length() > 0 && rxValue.length() < 32) {
      
      strncpy(originalCommand, rxValue.c_str(), 31);
      originalCommand[31] = '\0';
      strncpy(commandBuffer, rxValue.c_str(), 31);
      commandBuffer[31] = '\0';

      #if DEBUGSYS
      Serial.println(commandBuffer);
      #endif

      newCommandReceived = true;
    }
  }
};