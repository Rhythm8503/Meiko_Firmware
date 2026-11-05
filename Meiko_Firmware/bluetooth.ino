/***********************************************************************************************
  FFCosplays & Su Yin Sama Meiko Bracelet and Collar    
  Arduino Core: V3.2.1
  ESP32-C3 Board
***********************************************************************************************/

void BLE_Management(void* pvParameters) {
    Serial.println("BLE Management Task Started");

    while(1) {
        Bluetooth_Handle();
        vTaskDelay(pdMS_TO_TICKS(50));
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

  if (currentContext == MAIN) {
    if (strcmp(cmd, "LED") == 0) {
      currentContext = LED_MENU;
      sendResponse("Type LED Mode or LED Brightness");
    } else if (strcmp(cmd, "MOTION") == 0) {
      currentContext = MOTION_MENU;
      sendResponse("Type Motion Command");
    } else {
      sendResponse("Invalid command. Use LED, MOTION");
    }
  }

  else if (currentContext == LED_MENU) {
    if () { //Fill this in later 

    }
    else if (strcmp(cmd, "EXIT") == 0) {
      currentContext = MAIN;
      sendResponse("Main menu, Select LED or MOTION");
    }
  }

  else if (currentContext == MOTION_MENU) {
    if () { //Fill this in later 

    }
    else if (strcmp(cmd, "EXIT") == 0) {
      currentContext = MAIN;
      sendResponse("Main menu, Select LED or MOTION");
    }
  }
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