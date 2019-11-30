 #include<ESP8266WiFi.h>
// #include<dht.h>
#include<ESP8266HTTPClient.h>

const char* ssid = "Redmi";
const char* password = "12345678";

byte sensorInterrupt = D3;  // 0 = digital pin 2
byte sensorPin       = 2;

float calibrationFactor = 4.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void setup()
{
  
  WiFi.begin(ssid,password);
  Serial.begin(9600);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\n");
  Serial.println("Wifi Connected");
  Serial.println("Access Your Content At:");
  Serial.print(WiFi.localIP());
  
  //pinMode(statusLed, OUTPUT);
  //digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, RISING);
}

/**
 * Main program loop
 */
void loop()
{
   if (WiFi.status() == WL_CONNECTED)
{
     if((millis() - oldTime) > 10000)   
    { 
      
      detachInterrupt(sensorInterrupt);
          
   
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
      
     
      oldTime = millis();
      
      
      flowMilliLitres = (flowRate / 60) * 1000;
      
     
      totalMilliLitres += flowMilliLitres;
        
      
      
      int sensorValue = analogRead(A0);
      float voltage = sensorValue * (5.0 / 1024.0); 
      Serial.print("Turbidity: "); 
      Serial.print(voltage);
      Serial.println("");
      Serial.print("  Current Liquid Flowing: ");             // Output separator
      Serial.print(flowMilliLitres);
      Serial.print("mL/Sec");
      
  
      // Reset the pulse counter so we can start incrementing again
      pulseCount = 0;
      
      // Enable the interrupt again now that we've finished sending output
      attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

      String url =("http://192.168.43.23:8080/BHack/node2.jsp?");
      HTTPClient http;
      url.concat("flow=");
      url.concat(String(flowMilliLitres));
      http.begin(url);
      int httpCode=http.GET();
      if(httpCode>0)
      http.end();
    }
  }
}
