void setup() {
    Serial.begin(115200);
}

void loop() {

    if (Serial.available()) {

        String msg = Serial.readStringUntil('\n');

        Serial.print("Arduino received: ");
        Serial.println(msg);
    }
}