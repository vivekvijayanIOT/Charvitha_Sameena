#include <SD.h>
#include <Base64.h>

File root;

#define ledPin 7

#define ledPin1 8
int state = 0;
int proceed=0;
String opt="";
String choice;
char a[50];
char b[50];

char input[100];
int index=0;
char input2[100];
int index2=0;
char input4[100];
int index4=0;

int sensorValue ;

float voltage;
int battery;

        

void wait()
{
  while(Serial.available()==0){
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  //  printDirectory(root, 0);

  Serial.println("done!");
  // Default communication rate of the Bluetooth module
}
void loop() { 
  sensorValue = analogRead(A3);
   voltage = sensorValue * (5.0 / 1023.0)*2;
   battery = voltage/5*100;
  if(proceed==0){
    Serial.print("\nBattery level: \t");
    Serial.print(battery);
    Serial.print("%\n Enter the Passkey");
    
    wait();
    if(Serial.available()>0){
      String pswd=Serial.readString();

      if(pswd=="9840"){
        proceed=1;

      }
      else{
        proceed=0;
      }
    }
  }
  else{
    Serial.print("\nBattery level: \t");
    Serial.print(battery);
    Serial.print("%\n");

    Serial.write("\nWelcome \n\t1. List Directory \n\t2. Open a file( ONLY ENRCY FL ) \n\t3. Encrypt a file \n\t4. logout\na. Light A ON \tb. Light A OFF\nc. Light B ON \td. Light B OFF\n\n");
    wait();
    if(Serial.available()>0)
    {
      choice=Serial.readString();
      if(choice=="1"){
        printDirectory(root, 0);
      }

      if(choice=="2"){
         Serial.write("File name to Open :?\n");
        wait();
        if(Serial.available()>0)
        {
          String filenames=Serial.readString();
          filenames.toCharArray(a,50);
          root = SD.open(a,FILE_READ);
          if(root){
            while (root.available()) {
              input2[index2]=root.read();
              index2++;
            }
            
             int input2Len = sizeof(input2);
              
              int decodedLen = base64_dec_len(input2, input2Len);
              char decoded[decodedLen];
              
              base64_decode(decoded, input2, input2Len);
                       
            
            Serial.println(decoded);
                   
            
            
            root.close();
            index2=0;
          }
          
          else{
            Serial.write("No File Found");
          }
        }
      }

/*
      if(choice=="4"){
         Serial.write("File name to Decrypt :?\n");
        wait();
        if(Serial.available()>0)
        {
          String filenames=Serial.readString();
          filenames.toCharArray(b,50);
          root = SD.open(b,FILE_READ);
          if(root){
            while (root.available()) {
              input4[index4]=root.read();
              index4++;
            }
            
           int input4Len = sizeof(input4);
              
              int decodedLen = base64_dec_len(input4, input4Len);
              char decoded[decodedLen];
              
              base64_decode(decoded, input4, input4Len);
                       
            
            Serial.println(decoded);
            Serial.write("** DECRYPTED **\n");
           // Serial.println(encoded);
            SD.remove(b);
            root=SD.open(b,FILE_WRITE);
            if(root){
              root.println(decoded);
            }       
            
            
            root.close();
            index4=0;
          }
          
          else{
            Serial.write("No File Found");
          }
        }
      }
      */
       if(choice=="3"){
         Serial.write("File name to encrypt :?\n");
        wait();
        if(Serial.available()>0)
        {
          String filenames=Serial.readString();
          filenames.toCharArray(a,50);
          root = SD.open(a,FILE_READ);
          if(root){
            while (root.available()) {
              input[index]=root.read();
              index++;
            }
            
            int inputLen = sizeof(input);
  
            int encodedLen = base64_enc_len(inputLen);
            char encoded[encodedLen];
            
           // Serial.print(input); Serial.print(" = ");
            
            // note input is consumed in this step: it will be empty afterwards
            base64_encode(encoded, input, inputLen); 
            Serial.write("** ENCRYPTED **\n");
           // Serial.println(encoded);
            SD.remove(a);
            root=SD.open(a,FILE_WRITE);
            if(root){
              root.println(encoded);
            }
            
            
            
            root.close();
            index=0;
          }
          
          else{
            Serial.write("No File Found");
          }
        }
      }


      if(choice=="4"){
        proceed=0;
        Serial.write("\nLogout Successful\n");
      }
      if (choice == "a") {
    digitalWrite(ledPin, HIGH); // Turn LED OFF
    Serial.println("LED: ON"); // Send back, to the phone, the String "LED: ON"
    state = 0;
  }
  if (choice == "b") {
    digitalWrite(ledPin, LOW); // Turn LED OFF
    Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
    state = 0;
  }
  if (choice == "c") {
    digitalWrite(ledPin1, HIGH); // Turn LED OFF
    Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
    state = 0;
  }
  if (choice == "d") {
    digitalWrite(ledPin1, LOW); // Turn LED OFF
    Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
    state = 0;
  }

    }
  }




  
  
}
void printDirectory(File dir, int numTabs) {
  Serial.write("** LIST **\n");
  while(true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      break;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs+1);
    } 
    else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}




