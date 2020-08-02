//=================================================ESP8266 WEB SERVER UYGULAMASI LED KONTROL==============================================
#include <SoftwareSerial.h>
#define DEBUG true
 
SoftwareSerial esp8266(2,3); //2 yazılımsal RX , 3 yazılımsal TX pinini işaret eder. Esp8266 bağlanırken,
               //TX pini Arduino 2 nolu pine, RX pini Arduino 3 nolu pinine bağlanır.

//====================================================HAZIRLIK AŞAMASI=====================================================================
void setup()
{
  pinMode(13, OUTPUT); //Arduino üzerindeki led çıkış olarak ayarlanır.
  digitalWrite(13,LOW); //Led söndürülmesi sağlanır.
  Serial.begin(9600);  // Seri iletişim 9600 baud hızında başlatılır.
  esp8266.begin(9600); // Yazılımsal seri iletişim 9600 baud hızında başlatılır.
  sendData("AT+RST\r\n",2000,DEBUG); // AT komutu ile esp8266 tekrar başlatılır.
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // Bu komutla esp8266 wifi dağıtıcısı konumundadır. Wifi üzerinden esp8266'ya bağlantı sağlanır.
  sendData("AT+CIFSR\r\n",1000,DEBUG); // Bu komut ile esp8266 ip adresi öğrenilir.
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // Bu komut ile esp8266 üzerine birden çok bağlantı sağlanır.
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // 80 portu üzeründen yayın yapması sağlanır. 
}
//=================================================KONTROL VE WEB SAYFASI KISIMLARI========================================================
void loop()
{
  if(esp8266.available()) // bağlantı durumu kontrol edilir. 
  {
 
    if(esp8266.find("+IPD,")) // ESP8266 data okunduğuna dair bilginin bulunmasına bakılır.
    {
     delay(1000);
     int connectionId = esp8266.read()-48; // Okunan değer ASCII değerlerdir. Bu Değerden 48 çıkarılarak decimal değer elde edilir.
     String websayfasi = "<head> WEB SERVER UYGULAMASI </head>";
     websayfasi += "<br><a href=\" ?pin=on\"><button type='button'>LED ON</button></a>"; 
     websayfasi += "<br><a href=\" ?pin=off\"><button type='button'>LED OFF</button></a>";
     String cipSend = "AT+CIPSEND="; // Veri gönderilmesi için esp8266'ya komut gönderilir.AT+CIPSEND ile veriler gönderilmeye başlanır.
     cipSend += connectionId; //Yukarıdaki connection id değişkene eklenir.
     cipSend += ","; // Eklenen değerler arasına "," konulur.
     cipSend +=websayfasi.length(); //websayfasi değişkeni uzunluğu eklenir.
     cipSend +="\r\n"; //Bir alt satıra geçer.
     sendData(cipSend,1000,DEBUG); //Veri esp8266 ya gönderilir.
     sendData(websayfasi,1000,DEBUG); //Veri esp8266 ya gönderilir.
     String balantiSonlandirma = "AT+CIPCLOSE="; //Bağlantının sonlanması için AT+CIPCLOSE komutu bağlantı sonlandırma komutuna eklenir.
     balantiSonlandirma +=connectionId; // Bağlantı sonlanması için connection id bilgisi değişkene eklenri..
     balantiSonlandirma+="\r\n";
     sendData(balantiSonlandirma,3000,DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug) // veri gönderme fonksiyonu
{
    String response = "";
    esp8266.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        char c = esp8266.read();//ESP8266 üzerinden veriler c icerisine yazılır.
        response+=c; //response değişkenine c içerisindeki veriler eklenir.
      }  
    }
    
    if(debug)
    {
      Serial.print(response); //Veriler seriport ekranına yazılır.
      if((response.indexOf(":GET /?pin=on")>1)){  //on butonuna basıldığında server adresinde /?pin=on var ise
      digitalWrite(13,HIGH);                  //ledi yakar
      }
     if((response.indexOf(":GET /?pin=off")>1)){ // off butonuna basıldığında server adresinde /?pin=off var ise
     digitalWrite(13,LOW);                    //ledi söndürür
      }
    }
    
    return response; 
}