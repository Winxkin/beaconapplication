/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

//----firebase--------
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

//-------BLE------------
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>




//-------define firebase ----------



#define API_KEY "WFlGeZDmqtQqeEkFlcSXPtIVt4KPt8BtNH2KyBn7"
#define DATABASE_URL "https://esp32-7691f-default-rtdb.firebaseio.com/"


FirebaseData fbdo;

FirebaseAuth auth;

//-------define BLE scan-----------
#define Interval 200
#define Window  150

int scanTime = 2; //In seconds
BLEScan* pBLEScan;

 String Address ;
 String S_RSSI  ;
 String Name ;
 String Data ;

 bool Havedevice = 0;
std::string BLEaddress = "f5:6f:bd:af:ba:e4" ;
 
  

//-------------------------------
class WinAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
     public: 
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
      
      //---------------
    if (advertisedDevice.getAddress().toString().c_str() == BLEaddress ) {
       Havedevice =1;
      
       Serial.printf("Advertised address: %s \n",advertisedDevice.getAddress().toString().c_str()  );
       Serial.printf("Advertised name: %s \n", advertisedDevice.getName().c_str() );
       Serial.printf("Advertised data: %s \n", advertisedDevice.getManufacturerData().c_str() );
        Serial.printf("Advertised RSSI: %i \n", advertisedDevice.getRSSI() );

          Address = advertisedDevice.getAddress().toString().c_str();
          S_RSSI  = String (advertisedDevice.getRSSI()) ;
          Name = advertisedDevice.getName().c_str();
          Data = advertisedDevice.getManufacturerData().c_str();

 
        
    }
    
    
  }

};





//-----------Smartconfig Wifi setup--------------------------
void smartconfigwifi () {
  WiFi.mode(WIFI_AP_STA);
  /* start SmartConfig */
  WiFi.beginSmartConfig();
 
  /* Wait for SmartConfig packet from mobile */
  Serial.println("Waiting for SmartConfig...");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig done!");
 
  /* Wait for WiFi to connect to AP */
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  
  }





//---------------firebase setup------------------------------
void setupfirebase () {

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); 
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  
  }
  

//--------------BLEsetup-------------------------


void BLEsetup () {
  Serial.println("Scanning....");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new WinAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(Interval);  //ms
  pBLEScan->setWindow(Window);  // less or equal setInterval value
  Serial.println("Scan done ^-^");
}



//---------------Setup---------------------
void setup() {
  Serial.begin(115200);
 
  smartconfigwifi();
  setupfirebase ();
  BLEsetup();
  
}

 // ---- Loop function----
 void BLEscan () {
   Serial.println("Scanning....");
  pBLEScan->start(scanTime, false);
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  }

void Sent_data () {
   Firebase.setString(fbdo,  "/NRF51822/name" , Name );
   Firebase.setString(fbdo,  "/NRF51822/address" , Address );
   Firebase.setString(fbdo,  "/NRF51822/data" , Data );
   Firebase.setString(fbdo,  "/NRF51822/RSSI" , S_RSSI );
  }

 void Sent_nulldata () {
   Firebase.setString(fbdo,  "/NRF51822/name" , "null" );
   Firebase.setString(fbdo,  "/NRF51822/address" , "null" );
   Firebase.setString(fbdo,  "/NRF51822/data" , "null" );
   Firebase.setString(fbdo,  "/NRF51822/RSSI" , "null" );
  }

 


  
//------------Loop----------------------
void loop() {
  BLEscan ();


   if (Havedevice ==1 ) {
     Sent_data ();
    }
   else {
     Sent_nulldata ();
    }

  
  Havedevice = 0;

  
}


//last version