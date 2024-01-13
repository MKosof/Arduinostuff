#include <Keypad_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include "SSD1306Wire.h"
#define SS_PIN 15
#define RST_PIN 0
#define keypad_addr 0x20
#define delay_after_new 2500 //delay before screen clears after new balance is shown;
// rows , columns
char keys[4][4] = { 
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[4] = {0,1,2,3};
byte colPins[4] = {4,5,6,7};

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
SSD1306Wire display(0x3c, SDA, SCL);
Keypad_I2C keyp(makeKeymap(keys),rowPins,colPins,4,4,keypad_addr, PCF8574); 

void printBuffer(void);

void setup() {
  Serial.begin(9600);
  Serial.println();
  keyp.begin();
  display.init();
  display.setContrast(255);
  display.clear();
  delay (100);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  display.setFont(ArialMT_Plain_16);

}
 
// Init balances
String uids[12];
int balance[12];
int difference=0;
uint8_t current_card;
bool exists=false;
uint8_t num_of_cards=0;
char msg[30];
char key_pressed;

//keypad.getKey(); to get what's pressed;
void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  String uid = getHexId(rfid.uid.uidByte, rfid.uid.size); // get the hexadecimal uid
  exists=false;
    // Halt PICC (no idea what this does but it doesn't work without it so I will roll with it)
  rfid.PICC_HaltA();

  // Stop encryption on PCD (same as above)
  rfid.PCD_StopCrypto1();

  for (uint8_t i=0;i<12;i++){
    if (uid == uids[i]){
      display.clear();
      snprintf(msg,30,"Balance:%d £",balance[i]);
      display.drawString(0,0,msg);
      display.drawString(0,16,"A to Deposit.");
      display.drawString(0,32,"B to Withdraw.");
      display.drawString(0,48,"* to Cancel.");
      display.display();
      current_card=i;
      exists=true;
      break;
    }
    
  }
  if (exists==false){
    uids[num_of_cards]=uid;
    balance[num_of_cards]=1500;
    display.clear();
    snprintf(msg,30,"Welcome!");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0,0,msg);
    display.setFont(ArialMT_Plain_16);
    snprintf(msg,30,"Balance:%d£.",balance[num_of_cards]);
    display.drawString(0,26,msg);
    num_of_cards+=1;
    snprintf(msg,30,"Num of players:%d.",num_of_cards);
    display.drawString(0,44,msg);
    display.display();
    delay(400);
    return;
  }
  

  key_pressed=getKABH();
  change_bal(&key_pressed,&balance[current_card],&difference);

}

String uid;
String hexa;

// just reads the uid and sends it off
String getHexId(byte *buffer, byte bufferSize) {
  uid = "";
  for (byte i = 0; i < bufferSize; i++) {
    hexa = String(buffer[i], HEX);
    uid = uid + hexa + " ";
  }
  return uid;
}
char getKABH()
{
  while(true)
  {
    {
      if(keyp.getKeys())
      {
        if (keyp.isPressed('A'))
        {
          return 'A';          
        }

        if (keyp.isPressed('B'))
        {
          return 'B';          
        }

        if (keyp.isPressed('*'))
        {
          display.clear();
          display.display();
          return '*';          
        }

        if (keyp.isPressed('#'))
        {
          return '#';          
        }
      }
    }
    yield();
  }
}

char getKCDNum()
{
  while(true)
  {
    {
      if(keyp.getKeys())
      {
        if (keyp.isPressed('C'))
        {
          return 'C';          
        }

        if (keyp.isPressed('D'))
        {
          return 'D';          
        }

        if (keyp.isPressed('*'))
        {
          return '*';          
        }

        if (keyp.isPressed('0'))
        {
          return '0';          
        }

        if (keyp.isPressed('1'))
        {
          return '1';          
        }

        if (keyp.isPressed('2'))
        {
          return '2';          
        }

        if (keyp.isPressed('3'))
        {
          return '3';          
        }

        if (keyp.isPressed('4'))
        {
          return '4';          
        }

        if (keyp.isPressed('5'))
        {
          return '5';          
        }

        if (keyp.isPressed('6'))
        {
          return '6';          
        }

        if (keyp.isPressed('7'))
        {
          return '7';          
        }

        if (keyp.isPressed('8'))
        {
          return '8';          
        }

        if (keyp.isPressed('9'))
        {
          return '9';          
        }

      }
    }
    yield();
  }
}


void change_bal(char *key,int *balance,int *difference){
  char temp=*key;
  char amount[6]={'\0','\0','\0','\0','\0','\0'};
  String change;
  uint8_t i=0;
  if (temp=='A')
  {
    while(true)
    {
      display.clear();
      snprintf(msg,30,"Depositing...");
      display.drawString(0, 0, msg);
      snprintf(msg,30,"Amount: %s £", amount);
      display.drawString(0, 16, msg);
      snprintf(msg,30,"C to confirm");
      display.drawString(0, 32, msg);
      snprintf(msg,30,"D to delete");
      display.drawString(0, 48, msg);
      display.display();
      *key=getKCDNum();
      if (*key=='*')
      {
        display.clear();
        display.display();
        break;
      }
      else if(*key == 'C')
      {
        change=amount;
        *difference=change.toInt();
        *balance+=*difference;
        *key=temp;
        display.clear();
        snprintf(msg,30,"New balance:",*balance);
        display.drawString(0,0,msg);
        display.setFont(ArialMT_Plain_24);
        snprintf(msg,30,"%d £",*balance);
        display.drawString(0,32,msg);
        display.setFont(ArialMT_Plain_16);
        display.display();
        delay(delay_after_new);
        display.clear();
        display.display();
        break;
      }
      else if(*key == 'D' && i >=0)
      {
        i-=1;
        amount[i]='\0';   
      }
      else
      {
        amount[i]=*key;
        i++;
      }
      yield();
    }
  }

  if (temp=='B')
  {
    while(true)
    {
      display.clear();
      snprintf(msg,30,"Withdrawing...");
      display.drawString(0, 0, msg);
      snprintf(msg,30,"Amount: %s £", amount);
      display.drawString(0, 16, msg);
      snprintf(msg,30,"C to confirm");
      display.drawString(0, 32, msg);
      snprintf(msg,30,"D to delete");
      display.drawString(0, 48, msg);
      display.display();
      *key=getKCDNum();
      if (*key=='*')
      {
        display.clear();
        display.display();
        break;
      }
      else if(*key == 'C')
      {
        change=amount;
        *difference=change.toInt();
        *balance-=*difference;
        *difference= -*difference;
        *key=temp;
        display.clear();
        snprintf(msg,30,"New balance:",*balance);
        display.drawString(0,0,msg);
        display.setFont(ArialMT_Plain_24);
        snprintf(msg,30,"%d £",*balance);
        display.drawString(0,32,msg);
        display.setFont(ArialMT_Plain_16);
        display.display();
        delay(delay_after_new);
        display.clear();
        display.display();
        break;
      }
      else if(*key == 'D' && i >=0)
      {
        i-=1;
        amount[i]='\0';   
      }
      else
      {
        amount[i]=*key;
        i++;
      }
      yield();
    }
  }
  if (temp=='#')
  {
    *balance-=*difference;
    display.clear();
    snprintf(msg,30,"New balance:",*balance);
    display.drawString(0,0,msg);
    display.setFont(ArialMT_Plain_24);
    snprintf(msg,30,"%d £",*balance);
    display.drawString(0,32,msg);
    display.setFont(ArialMT_Plain_16);
    display.display();
    delay(delay_after_new);
    display.clear();
    display.display();
  }
}