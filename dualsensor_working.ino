const unsigned long interval = 30000; // 30 seconds in milliseconds
unsigned long previousMillis = 0;
int readCountA0 = 0;
int readCountA4 = 0;
long totalValueA0 = 0;
long totalValueA4 = 0;

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Set pin A2 as an output
  pinMode(A2, OUTPUT);
}

void loop() {
  // Read the analog input from pin A0 and A4
  int analogValueA0 = analogRead(A0);
  int analogValueA4 = analogRead(A4);
  
  // Accumulate the total value and count of readings for A0
  totalValueA0 += analogValueA0;
  readCountA0++;
  
  // Accumulate the total value and count of readings for A4
  totalValueA4 += analogValueA4;
  readCountA4++;
  
  // Get the current time
  unsigned long currentMillis = millis();
  
  // Check if 30 seconds have passed
  if (currentMillis - previousMillis >= interval) {
    // Calculate the average value for A0
    int averageValueA0 = totalValueA0 / readCountA0;
    
    // Calculate the average value for A4
    int averageValueA4 = totalValueA4 / readCountA4;
    
    // Print the average value to the Serial Monitor
    Serial.print("Average analog value from A0 over 30 seconds: ");
    Serial.println(averageValueA0);
    Serial.print("Average analog value from A4 over 30 seconds: ");
    Serial.println(averageValueA4);
    
    // Reset the total value and count for the next interval
    totalValueA0 = 0;
    readCountA0 = 0;
    totalValueA4 = 0;
    readCountA4 = 0;
    
    // Update the previousMillis to the current time
    previousMillis = currentMillis;
  }
  
  // Check if the analog value from A0 or A4 is high (greater than their respective thresholds)
  if (analogValueA0 > 512 || analogValueA4 > 606) { // Thresholds for A0 and A4
    // Set pin A2 to HIGH
    digitalWrite(A2, HIGH);
    // Print the state of pin A2 (HIGH)
    Serial.println("Pin A2 state: HIGH (A0 > 512 or A4 > 606)");
  } else {
    // Set pin A2 to LOW
    digitalWrite(A2, LOW);
    // Print the state of pin A2 (LOW)
    Serial.println("Pin A2 state: LOW (A0 <= 512 and A4 <= 606)");
  }
  
  // Add a short delay before the next reading
  delay(2000);  // delay in milliseconds
}
