#include <avr/wdt.h>  // Include the watchdog timer library

const unsigned long interval = 30000; // 30 seconds in milliseconds
unsigned long previousMillis = 0;
int readCountA0 = 0;
int readCountA4 = 0;
long totalValueA0 = 0;
long totalValueA4 = 0;

const int resetThreshold = 3; // Number of times the threshold can be exceeded in a minute
int thresholdExceededCount = 0; // Counter for threshold exceedances
unsigned long thresholdTimeWindow = 60000; // 1 minute in milliseconds
unsigned long lastThresholdTime = 0;

bool restartFlag = false; // Flag to indicate if a restart occurred

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Set pin A2 as an output
  pinMode(A2, OUTPUT);

  // Check if the board has restarted due to threshold being met
  if (restartFlag) {
    // Set pin A2 to HIGH for 1 minute
    digitalWrite(A2, HIGH);
    Serial.println("Pin A2 is HIGH for 1 minute due to reset");

    // Delay for 1 minute (60000 milliseconds)
    delay(60000);
    
    // Reset the flag
    restartFlag = false;

    // Set pin A2 back to LOW
    digitalWrite(A2, LOW);
    Serial.println("Pin A2 is LOW after 1 minute");
  }
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
  if (analogValueA0 > 500 || analogValueA4 > 550) { // Thresholds for A0 and A4
    // Set pin A2 to HIGH
    digitalWrite(A2, HIGH);
    // Print the state of pin A2 (HIGH)
    Serial.println("Pin A2 state: HIGH (A0 > 500 or A4 > 550)");
    
    // Increment the threshold exceeded count
    thresholdExceededCount++;
    
    // Record the time of this exceedance
    lastThresholdTime = currentMillis;
  } else {
    // Set pin A2 to LOW
    digitalWrite(A2, LOW);
    // Print the state of pin A2 (LOW)
    Serial.println("Pin A2 state: LOW (A0 <= 500 and A4 <= 550)");
  }
  
  // Check if the threshold exceeded count is more than resetThreshold within the time window
  if (thresholdExceededCount > resetThreshold && (currentMillis - lastThresholdTime <= thresholdTimeWindow)) {
    Serial.println("Threshold exceeded more than 3 times in a minute. Restarting...");
    restartFlag = true;  // Set the restart flag before restarting
    wdt_enable(WDTO_15MS);  // Enable watchdog timer to reset the board
    while (true) {}  // Wait for the watchdog timer to reset the board
  }
  
  // Reset the counter if the time window has passed
  if (currentMillis - lastThresholdTime > thresholdTimeWindow) {
    thresholdExceededCount = 0;
  }
  
  // Add a short delay before the next reading
  delay(2000);  // delay in milliseconds
}

