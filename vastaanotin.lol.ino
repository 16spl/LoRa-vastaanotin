    //LoRa vastaanotin koodi!!!
    #include <TimeLib.h>
    #include <SPI.h>
    #include <RH_RF95.h> 
    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 7  
    #define RF95_FREQ 916.0
    #include <TimeLib.h>

    // Singleton instance of the radio driver
    RH_RF95 rf95(RFM95_CS, RFM95_INT);
     
    #define LED 13
     
    void setup() 
    {
      pinMode(LED, OUTPUT);     
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
      
      
      while (!Serial);
      Serial.begin(9600);
      delay(100);
     
      Serial.println("Feather LoRa RX Testi!");
      
      // manuaalinen resetointi
      digitalWrite(RFM95_RST, LOW);
      delay(10);
      digitalWrite(RFM95_RST, HIGH);
      delay(10);
     
      while (!rf95.init()) {
        Serial.println("LoRa käyttööntotto epäonnistui");
        while (1);
      }
      Serial.println("LoRa radion käyttöönotto OK");
     
      if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("Taajuuden asettaminen epäonnistui");
        while (1);
      }
      Serial.print("Taajuus asetettu: "); Serial.println(RF95_FREQ);


      // Lähetystehon säätö välillä 5-23 dBm:
      rf95.setTxPower(23, false);
    }
     
    void loop()
    {
      if (rf95.available())
      {
        // Pitäisi olla viesti saatavilla 
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        
        if (rf95.recv(buf); len)
        {
          digitalWrite(LED, HIGH);
          RH_RF95::("Vastaanotettu: ", buf, len);
          //Serial.print("Got: ");
          Serial.println((char*)buf);
          // Serial.print("RSSI: ");
          //Serial.println(rf95.lastRssi(), DEC);
          delay(100);
         
          // Lähettää vastauksen
          //delay(200); // may or may not be needed
          uint8_t data[] = "Moro vaan";
          rf95.send(data, sizeof(data));
         

          rf95.waitPacketSent();
          //Serial.println("Vastaus lähetetty");
          digitalWrite(LED, LOW);
        }
        
        else
        {
          Serial.println("Vastaanottovirhe");
        }
      }
    }
