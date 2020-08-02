#include <ESP8266WiFi.h>
//==============================================DEFINITIONS========================================================
const char* ssid     = "----------"; // Please write your wifi connect name
const char* password = "----------"; // Please write your wifi password
WiFiServer server(80);// This command open number of port 80, you connect nodenmcu web server with this port number
String header; // This command definition variable for html command
int greenled = D1; //Variable of green led. You should connect green led this pin
int redled = D2; //Variable of red led. You should connect red led this pin
String greenstate = "off";// Definition State of Green Led
String redstate = "off";// Definition State of Red Led
//==============================================SETUPS========================================================
void setup() {
  Serial.begin(9600);
  pinMode(greenled, OUTPUT); // Set this port output
  pinMode(redled, OUTPUT); // Set this port output
  digitalWrite(greenled, LOW); // Set state of port low
  digitalWrite(redled, LOW); // Set state of port low
  WiFi.begin(ssid, password); //This command connet to your wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) { //waits until it is connected and write "."
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());// This command show ip address in arduino serial editor. You will connect with this ip address to webserver interface
  server.begin();
}
//==============================================CODES========================================================
void loop(){
  WiFiClient client = server.available();   // This command listen for incoming clients
  if (client) {                             // If a new client connects,
    String currentLine = "";                // This variable is set up for connected users
    while (client.connected()) {            // If user connects this loop start working
      if (client.available()) {             // If there's bytes to read from the client,
        char c = client.read();             // This comand register to read value
        Serial.write(c);                    // Print character on Serial Monitor
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
			//Below commands give information for client. If clinent connects with success turn http OK code.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            //Below commands decide for reads variable.
			if (header.indexOf("GET /green/on") >= 0) {
              Serial.println("green on");
              greenstate = "on";
              digitalWrite(greenled, HIGH);
            } else if (header.indexOf("GET /green/off") >= 0) {
              Serial.println("green off");
              greenstate = "off";
              digitalWrite(greenled, LOW);
            } else if (header.indexOf("GET /red/on") >= 0) {
              Serial.println("red on");
              redstate = "on";
              digitalWrite(redled, HIGH);
            } else if (header.indexOf("GET /red/off") >= 0) {
              Serial.println("red off");
              redstate = "off";
              digitalWrite(redled, LOW);
            }
       
            // Belows command shows html page.
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the ON and OFF buttons 
            // With below commands you can change style of buttons your prefers.
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {border-radius: 50px; background-color: #4CAF50; border: none; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none;display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");           
            client.println(".button1 { background-color: #4CAF50;border-radius: 50px;}");
            client.println(".button2 { background-color: #555555;border-radius: 50px;}");
            client.println(".button3 { background-color: #f44336;border-radius: 50px;}");
            client.println(".button4 { background-color: #555555;border-radius: 50px;}</style></head>");
			// Web Page Heading
            client.println("<body><h1>NodeMCU V3 WEB SERVER</h1>");            
            // Below commands shows State of green led. 
            client.println("<p>GREEN - STATE: " + greenstate + "</p>");
            // If state of green led is off, on button fills green, off button fills gray.       
            if (greenstate == "off") {
              client.println("<a href=\"/green/on\"><button class=\"button\">ON</button></a>");
              client.println("<button class=\"button button2\">OFF</button>");
			} else {
              client.println("<button class=\"button button2\">ON</button>");
              client.println("<a href=\"/green/off\"><button class=\"button button1\">OFF</button></a>");
			} 
           
            client.println("<p>RED - STATE: " + redstate + "</p>");
            // If state of red led is off, on button fills red, off button fills gray.       
            if (redstate == "off") {
              client.println("<a href=\"/red/on\"><button class=\"button button3\">ON</button></a>");
              client.println("<button class=\"button button4\">OFF</button>");
            } else {
              client.println("<button class=\"button button4\">ON</button>");
			  client.println("<a href=\"/red/off\"><button class=\"button button3\">OFF</button></a>");
            }
			  client.println("<p style=font-family:'Courier New'>Version 1.0</p>");
              client.println("</body></html>");
				client.println();
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
		} else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }    
    header = "";
    client.stop();// Close connection
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}