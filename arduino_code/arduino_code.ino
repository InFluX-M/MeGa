void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

String command;
int T = 1000;

int led_state = 0;
bool blink_state = 0;

int calPeriod()
{
  int i = 7;
  while(i <= 10)
  {
    if(command[i] == '0')
      i++;
    else
      break;
  }
  String numT = command.substring(i, 11);

  int num = 0;
  for(int j = 0; j < numT.length(); j++)
  {
    num *= 10;
    num += (numT[j] - '0');
  }
  return num;
}

void loop() {   
  if(Serial.available() > 0)
  {
    char c = Serial.read();   
    if(c == '\n')
    {
      String msg;   
      if(command.substring(0, 2) == "AT")
          msg = "OK";
      else if(command.substring(0, 6) == "LED=ON")
        led_state = 1, msg = "OK-LED=ON";
      else if(command.substring(0, 7) == "LED=OFF")
        led_state = 0, msg = "OK-LED=OFF";
      else if(command.substring(0, 11) == "BLINKING=ON")
        blink_state = 1, msg = "OK-BLINKING=ON";
      else if(command.substring(0, 12) == "BLINKING=OFF")
        blink_state = 0, msg = "OK-BLINKING=OFF";
      else if(command.substring(0, 7) == "PERIOD=")
        T = calPeriod()/2, msg ="OK-PERIOD";
      else if(command.substring(0, 7) == "STATUS?")
      {
        if(blink_state)
          msg = "BLINKING, PERIOD=" + String(T);
        else
          msg = ((led_state)? "ON" : "OFF");
      }
      else
        msg = "Error";

      Serial.println(msg);
      command = "";
    }
    else
      command.concat(c);
    }
   
  if(blink_state)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(T/2);
    digitalWrite(LED_BUILTIN, LOW);
    delay(T/2);
  }
  else
    digitalWrite(LED_BUILTIN, led_state);
}
