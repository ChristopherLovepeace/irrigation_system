#include <ArduinoBLE.h>

BLEService gardeningService("1101");
BLEUnsignedCharCharacteristic moistureLevelChar("2101", BLERead | BLENotify);

struct sensorData
{
  int moisture;
};

void setup() {
 
  Serial.begin(9600);
  while (!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLE.begin()) 
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("GardeningMonitor");
  BLE.setAdvertisedService(gardeningService);
  gardeningService.addCharacteristic(moistureLevelChar);
  BLE.addService(gardeningService);
  
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
  }

void loop() {
  struct sensorData sd;
  BLEDevice central = BLE.central();

  if (central){
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

  while (central.connected()) {

    int moisture_raw = random(0,1023);
    sd.moisture = map(moisture_raw, 0, 1023, 0, 100);
    Serial.print("Moisture level is: ");
    Serial.println(sd.moisture);
    moistureLevelChar.writeValue(sd.moisture);
    delay(8000);
    }
}
digitalWrite(LED_BUILTIN, LOW);
Serial.print("Disconnected from central: ");
Serial.println(central.address());
}
