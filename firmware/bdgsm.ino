byte cmdlen = 0;

void setup() {                
  DDRC = B11111111; 
  DDRB = B11111111;
  DDRD = B11111110;
  Serial.begin(9600);
  PORTC = B00000000;
  PORTB = B00000000;
  PORTD = B00000000;
}

void loop() {
  ServeConsole();
}

void ServeConsole() {
  static char cmd[16];
  while(Serial.available()) {                     // Read chars from buffer.
    byte c = Serial.read();
    if (cmdlen>=sizeof(cmd)) {
      Serial.println("Troll detected!");
      c = '-';
    }
    Serial.print((char)c);
    if (c == 10 || c == 13 || c==64) {
      for (cmdlen=cmdlen; cmdlen<sizeof(cmd); cmdlen++) {  // clean rest of cmd, because a troll user may occur
        cmd[cmdlen] = 0;                                   // FIXME: better way is correct command terminator handling
      }
      Serial.println("exec");
      ExecCommand(cmd);
      cmd[cmdlen = 0] = 0;
      return;
    }
    if (c == '-') {
      cmd[cmdlen = 0] = 0;
      Serial.println("Aborted.");
      return;
    }
    cmd[cmdlen++] = c;
    cmd[cmdlen] = 0;
  }
  return;
}

void ExecCommand(char *cmd) {
  byte portnum;
  boolean status;
  switch (cmd[0]) {
  case 's':
    /* set bit
        1 → portid (a-)
        2 → state (1/0)
        FIXME: syntax checking
    */
    portnum = cmd[1]-97;
    status = false;
    if (cmd[2] == '1') {
      status = true;
    }
    if (portnum <= 7) {
      PORTD = SetBit(portnum, PORTD, status);
    } else if (portnum <= 15) {
      PORTB = SetBit(portnum-8, PORTB, status);
    } else if (portnum <= 23) {
      PORTC = SetBit(portnum-16, PORTC, status);
    }
    break;
  default:
    Serial.println("WTF?");
    break;
  }
  return;
}

byte SetBit (byte pos, byte packet, boolean state) {
  if(state) {
    bitWrite(packet, pos, 1);
  } 
  else {
    bitWrite(packet, pos, 0);
  }
  return packet;
}
