int i;

void setup() {
  // put your setup code here, to run once:
  ledcAttach(10, 250, 16);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  ledcWrite(10, i * 60);
  i++;
}
