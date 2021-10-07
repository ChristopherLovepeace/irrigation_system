#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"

#define MAX_RCV_BUF 12
#define PORT 8080
#define IPadr 192,168,43,150
//byte rcvBuffer[MAX_RCV_BUF];
char ssid[]=SSIDNAME;
char pass[]=PASSWORD;
//int keyIndex=0;
int status = WL_IDLE_STATUS;
IPAddress server(IPadr);
WiFiClient client;


struct sensorData
{
  int temp;
  int humidity;
  int timeNow;
};

void sendSensorData(int temp, int humidity, int timeNow)
{
  sensorData sd;
  sd.temp=temp;
  sd.humidity=humidity;
  sd.timeNow=timeNow;
  //Serial.println(sd);
  client.write((byte*)&sd,sizeof(sensorData));
}

void receiveSensorData(byte* rcv_buf, int max_len_buf)
{
  client.read(rcv_buf,max_len_buf);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Arduino IP Address: ");
  Serial.println(ip);
  Serial.print("Server (RPi) IP Address: ");
  Serial.println(server);
  Serial.print("Server (RPi) Port: ");
  Serial.println(PORT);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

int flag=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //is there a wifi shield?
  if(WiFi.status()==WL_NO_MODULE){
    Serial.println("\nWifi communication failed");
    while(true);//do not continue
  }
  //try to connect to Wifi
  while(status!=WL_CONNECTED){
    Serial.print("\nAttempting to connect to Network: ");
    Serial.print(ssid);
    //For connection to WPA/WPA2 network, change if connecting to WEP
    status = WiFi.begin(ssid,pass);
    delay(3000);
  }
  Serial.println("\nConnected to Wifi");
  printWiFiStatus();
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  //if (client.connect(server, PORT)) {
  if (client.connect(server, PORT)) {
    Serial.println("\nConnected to server");
    flag=1;
  }
}
int i=0;

byte received_data[MAX_RCV_BUF];

void loop() {
  //client.available() checks if there are any bytes being sent from the server
  if (!client.available() && i<=5 && flag==1) {
    Serial.println("Sending sensor data to server...\n");
    Serial.print("1,1,");
    Serial.println(i);
    sendSensorData(1,1,i++);
    
    delay(3000);
  }

  if(client.available() && flag==1)
  {
    sensorData* sd = (sensorData*)&received_data;
    receiveSensorData((byte*)&received_data, MAX_RCV_BUF);
    Serial.println("Received from RPi:\t");
    Serial.print("Temp:\t");
    Serial.println(sd->temp);
    Serial.print("Humidity:\t");
    Serial.println(sd->humidity);
    Serial.print("Time:\t");
    Serial.println(sd->timeNow);
    //Serial.println(received_data[1]);
    //Serial.println(received_data[2]);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected() && flag==1) {
    flag=0;
    
    Serial.println();
    Serial.println("\nDisconnecting from server.");
    client.stop();

    // do nothing forevermore:
    //while (true);
  }
  if(flag==0)
  {
    Serial.println("No server connected");
    for(;;);
   }

}
