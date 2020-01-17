//#include <WiFi.h>
//#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include <EEPROM.h>
#include <Ticker.h>

Ticker secondTick;

#define debug 1

volatile int watchdogCount = 0;

void ISRwatchdog() {
  watchdogCount++;
  if ( watchdogCount == 5 ) {
     // Only print to serial when debugging
     //(debug) && Serial.println("The dog bites!");
     ESP.reset();
  }
}

// GPIO the LED is attached to
static const int LED1pin = 5;//LED1pin white to MT pwm dimming pin
static const int LED2pin = 13; //LED2pin warm to cct change pwm
//flash memory storage variable 
unsigned long  LED1slide; 
unsigned long  LED2slide; 
unsigned long  LED1Button; 
unsigned long  LED1Butslide;


// Replace with your network credentials
const char* ssid     = "smrttest_1huzza";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(5);
String valueStringg = String(5);
//slider 3 button
//String valueStringb = String(5);
int lvl1 = 0;
int lvl2 = 0;
int lvlWa1 = 0;
int lvlWa2 = 0;

unsigned long val1;
unsigned long val2;
unsigned long val3;
unsigned long val4;
unsigned long val23;
unsigned long val33;
//slider 3 as button config
int butst1 = 0;
int butst2 = 0;



String output5State = "slidervalue";
//String output4State = "off";
//String output6State = "off";




void setup() {
  
  Serial.begin(115200);
  secondTick.attach(1, ISRwatchdog);
  EEPROM.begin(512);  //512 bytes it can store
  pinMode(LED1pin,OUTPUT);  //  the LED1pin to the gpio as output
  pinMode(LED2pin,OUTPUT);
  //digitalWrite(LED1pin,LOW);
  //digitalWrite(LED2pin,LOW); 
  LED1slide = EEPROM.read(0);// 0 address for data stored in flash memory
  LED2slide = EEPROM.read(10);// 1 address for data stored in flash memory
  LED1Button = EEPROM.read(20); //2
  LED1Butslide = EEPROM.read(30); // 3

  val3 = map(LED1slide,0,255,0,1023); 
  val4 = map(LED2slide,0,255,0,1023);
Serial.println("LED1 = ");
Serial.print(val3);
Serial.println("LED2 = ");
Serial.print(val4);

 
analogWrite(LED1pin, val3); 
analogWrite(LED2pin, val4); 
analogWrite(LED1pin, LED1Button);
analogWrite(LED1pin, val3);


  
  WiFi.mode(WIFI_AP);
  IPAddress apLocalIp(2,2,2,40);
  IPAddress apSubnetMask(255,255,255,0);
  WiFi.softAPConfig(apLocalIp,apLocalIp,apSubnetMask);
  WiFi.softAP(ssid, password);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  //Server________________________________
  server.begin();
  
}

void loop(){
  watchdogCount = 0;
  //delay(20);
  yield();
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            


            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
      /*      // Button Code HTML
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
        */    
            
      
            //slider code HTML
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
            client.println("</head><body><h1>LED brightness control</h1>");
            client.println("<p>LEDwhite: <span id=\"brightnessWhite\"></span></p>");          
            client.println("<input type=\"range\" min=\"250\" max=\"1023\" step=\"38.65\" class=\"slider\" id=\"brightLEDW\" oninput=\"LED1(this.value)\" value=\""+valueString+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"brightLEDW\");");
            client.println("var brightLW = document.getElementById(\"brightLvl\"); brightLW.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; brightLW.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function LED1(pos1) { ");
            client.println("$.get(\"/?value=\" + pos1 + \"&\"); {Connection: close};}</script>");
            //slider2
            client.println("<p>LEDwarm: <span id=\"brightnessWarm\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"1023\" step=\"68.2\" class=\"slider\" id=\"brightLEDWa\" oninput=\"LED2(this.value)\" value=\""+valueStringg+"\"/>");
            
            client.println("<script>var slider1 = document.getElementById(\"brightLEDWa\");");
            client.println("var brightLWa = document.getElementById(\"brightLvL\"); brightLWa.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; brightLWa.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function LED2(pos2) { ");
            client.println("$.get(\"/?value(\" + pos2 + \")\"); {Connection: close};}</script>");

//------------------------------------------------------------------------------------//

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            if (output5State=="slidervalue") {
              client.println("<p><a href=\"/5/off\"><button class=\"button\">OFF</button></a></p>");
           } else {
              client.println("<p><a href=\"/5/sldval\"><button class=\"button button2\">Slider</button></a></p>");
             } 


            client.println("");
            
            
           // >>>>>>>>>>>>>LED button code write from here AJAX <<<<<<<<<<<<<<<<<<  
            
            
            
            
            
            client.println("</body></html>");     
           
           //GET /?value=180& HTTP/1.1
            
            if(header.indexOf("GET /?value=")>=0) {
              lvl1 = header.indexOf('=');
              lvl2 = header.indexOf('&');
              valueString = header.substring(lvl1+1, lvl2);
              val23 = map(valueString.toInt(),0,1023,0,255);  // converting string value into int then mapping it into 0-1023 to 0-255 to store value in EEPROM 0-255 value
              //PWM to the LED white brightness
              val1 = valueString.toInt();
              analogWrite(LED1pin,val1);             // always write analog vlue 0-1023 range
              EEPROM.write(0, val23);
              EEPROM.commit();
              
              Serial.print(val23);
            }
            //GET /?value(150) HTTP/1.1 
            if(header.indexOf("GET /?value(")>=0) {
              lvlWa1 = header.indexOf('(');
              lvlWa2 = header.indexOf(')');
              valueStringg = header.substring(lvlWa1+1, lvlWa2);
              val33 = map(valueStringg.toInt(),0,1023,0,255);
              //PWM to the LED white brightness
              val2 = valueStringg.toInt();
              analogWrite(LED2pin,val2);
              EEPROM.write(10, val33);
              EEPROM.commit();
              Serial.println(val33);
              
            }

            
           if (header.indexOf("GET /5/off") >= 0) {
              output5State = "off";
              Serial.println(output5State);
              digitalWrite(LED1pin,LOW);
              EEPROM.write(20, 0);
              EEPROM.commit();
              Serial.println(LED1pin);
              
            } else if (header.indexOf("GET /5/sldval") >= 0) {
              output5State = "slidervalue";
              Serial.println(output5State);
              analogWrite(LED1pin,val1);
              EEPROM.write(30, val23);
              EEPROM.commit();
              Serial.println(valueString);
              
               
            }
            
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
