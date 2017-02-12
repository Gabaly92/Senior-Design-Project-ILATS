                                                                                                                            /* Hallway 1*/
/*----------------------------------------------------------------------------------------------------------------------------Header Files---------------------------------------------------------------------------------------------------------------------------------*/
/* Ethernet */
#include <EtherCard.h>
/*----------------------------------------------------------------------------------------------------------------------------Defined Variables----------------------------------------------------------------------------------------------------------------------------*/
/* Ethernet Shield */
#define SEND_INTERVAL  500
#define TIMEOUT        400
#define STATUS_IDLE    0
#define STATUS_SENT    1
#define STATIC         0
/*---------------------------------------------------------------------------------------------------------------------------Global Variables------------------------------------------------------------------------------------------------------------------------------*/
/* Ethernet Shield */
static byte mymac[] = {0x94,0x91,0xA5,0xA6,0x0E,0x4F}; 
byte Ethernet::buffer[700];
char website[] PROGMEM = "www.gabaly.byethost10.com";
char password[] PROGMEM = "password";
unsigned long previousMillis = 0;
static byte session_id;
byte actual_status;
boolean LastServerResponse = true;
/* FFT Variables */
String R1Devices = "0";
int FFT_One = 9;
int FFT_Two = 2;
int Read_One = 0;
int Read_Two = 0;
boolean DeviceDetected = false;
/* Teensy Reset Variables */
unsigned char Reset_Signal = 3;
unsigned char Receive_Count = 0;
/*---------------------------------------------------------------------------------------------------------------------------Functions-------------------------------------------------------------------------------------------------------------------------------------*/
/* Ethernet Shield */
/* send_Devices */
void send_Devices(String Devices) 
{
  /* Send Detected Devices */
    Stash stash;
    byte sd = stash.create();
    stash.print(Devices);
    stash.save();
    Stash::prepare(PSTR("GET /SaveHallway1Devices.php?temp=$H&pwd=$F HTTP/1.0" "\r\n"
    "Host: $F" "\r\n" "\r\n"),
    sd, password, website);
    session_id = ether.tcpSend();
    Serial.print("sent to website...");
    actual_status = STATUS_SENT;
}
/* checkResponse */
void checkResponse() 
{
  const char* reply = ether.tcpReply(session_id);
  if(reply > 0) 
  {
    if(strstr(reply, "KO - ") != 0) Serial.println(strstr(reply, "KO - "));
    else
    {
      Serial.println(" OK");
      /* Last Server Response Succeeded */
      LastServerResponse = true;
      /* Last Server Response Succeeded */
    }
    actual_status = STATUS_IDLE;  
  }
}
/*---------------------------------------------------------------------------------------------------------------------------Setup-----------------------------------------------------------------------------------------------------------------------------------------*/
void setup () {  
  Serial.begin(9600);
  /* Initialize FFT pins */
  pinMode(FFT_One,INPUT);
  pinMode(FFT_Two,INPUT);
  
  /* Initialize Teensy Reset Pin */
  pinMode(Reset_Signal,OUTPUT);
  Serial.println("FFT pins Initialized");
  Serial.println("Dislpay Hallway 1 Devices");
  Serial.println();
  while (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
  {
    Serial.println( "Failed to access HallWay 1 Ethernet controller");
  }
  Serial.println( "Accessed HallWay 1 Ethernet controller");
  Serial.println( "Sent DHCP Request");
  while (!ether.dhcpSetup())
  {
    Serial.println("DHCP Request Failed, Resending DHCP Request");
  }
  Serial.println( "DHCP Request Succeeded");
  Serial.println("Please Wait, Resolving Server IP....");
  while (!ether.dnsLookup(website)) 
  {
    Serial.print("Unable to resolve Server IP");
    Serial.println("Resolving Server ip");    
  } 
  Serial.println("Server IP resolved");
  Serial.println();
  ether.printIp("Halway 1 Arduino IP: ", ether.myip);
  ether.printIp("Netmask:\t ", ether.mymask);
  ether.printIp("Gateway:\t ", ether.gwip);
  ether.printIp("DNS:    \t ", ether.dnsip);
  ether.printIp("Server IP:\t ", ether.hisip);
 
  Serial.println();
}
/*---------------------------------------------------------------------------------------------------------------------------Loop------------------------------------------------------------------------------------------------------------------------------------------*/
void loop() 
{
  /* Ethernet Shield */
  ether.packetLoop(ether.packetReceive());  
  unsigned long currentMillis = millis();
   
  switch(actual_status) 
  { 
    case STATUS_IDLE:
    /* Loop forever if no Device is detected */
    while( (LastServerResponse == true) )
    {
      Read_One = digitalRead(FFT_One);
      Read_Two = digitalRead(FFT_Two);
      /* if no devices are detected */
      if( (Read_One == LOW) && (Read_Two == LOW) )
      {
        Serial.println("No Device Detected");
      }
      /* if device 1 is detected */
      else if( (Read_One == HIGH) && (Read_Two == LOW) )
      {
        R1Devices = "1";
        Serial.println("500 Hz Device Detected ");
        
         /* Reset Decision */
        Receive_Count++;
        if(Receive_Count >= 3)
        {
        digitalWrite(Reset_Signal,LOW);
        Receive_Count = 0;
        }
        /* Reset Decision */
        
        break;
      }
      /* if device 2 is detected */
       else if( (Read_One == LOW) && (Read_Two == HIGH) )
       {
         R1Devices = "2";
         Serial.println("250 Hz Device Detected ");
         
        /* Reset Decision */
        Receive_Count++;
        if(Receive_Count >= 3)
        {
        digitalWrite(Reset_Signal,LOW);
        Receive_Count = 0;
        }
        /* Reset Decision */
         
         break;
       }
    }
    /* Send Detected Device to server */
    if(currentMillis - previousMillis > SEND_INTERVAL) 
    {
       previousMillis = currentMillis;
      /* Act based on last Response */
      send_Devices(R1Devices);
      /* Act based on last response */
    }
    break;
    case STATUS_SENT:
    if(currentMillis - previousMillis > TIMEOUT) 
    {
      Serial.println(" No response");
      /* Last Server Response = Failed */
      LastServerResponse = false;
      Serial.print(" Resending Device: ");
      /* Last Server Response = Failed */
      previousMillis = currentMillis;
      actual_status = STATUS_IDLE;
    }
    checkResponse();
  }
}
/*--------------------------------------------------------------------------------------------------------------------------End--------------------------------------------------------------------------------------------------------------------------------------------*/





