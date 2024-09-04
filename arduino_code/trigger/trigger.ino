
int pin=13;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

}

int input=0;
bool trigger=false;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(input);
  
  if(Serial.available()>0){
    input=Serial.read();
    
    if(isDigit(input)){
      int val=(char) input-48;
      
      if(val==5){
        trigger=true;
        
      }
    }
  }

  // the rest 
  if(trigger){
    digitalWrite(pin, HIGH);
    delayMicroseconds(100);
    
    digitalWrite(pin, LOW);
    delay(10);
    trigger=false;
  }
}
