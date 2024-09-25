#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Ultrasonic sensor pins
const int trigPin1 = D10;  // Ultrasonic sensor Trig pin
const int echoPin1 = D11;  // Ultrasonic sensor Echo pin
const int speakerPin = D8; // Buzzer pin

const int delay_time = 200;  // Delay 200ms between each measurement
long duration1, distance1;   // Variables to store the measured time and distance
long avg_distance;           // Averaged distance after applying the filter

// WiFi credentials
const char* ssid = "WiFi SSID";  
const char* password = "WiFi password";  

ESP8266WebServer server(80); // Create a web server listening on port 80

// Low-pass filter class for smoothing distance data
class LowPassFilter {
  public:
    LowPassFilter(float time_constant);
    float filter(float input);
  private:
    float prev_value;
    float time_constant;
};

// Initialize the filter with the time constant
LowPassFilter::LowPassFilter(float time_constant) {
  this->time_constant = time_constant;
  this->prev_value = 0;
}

// Filter algorithm
float LowPassFilter::filter(float input) {
  prev_value = (time_constant / (time_constant + 1)) * prev_value + (1 / (time_constant + 1)) * input;
  return prev_value;
}

// Set filter time constant to 0.1
LowPassFilter distance_filter(0.1);

// Connect to WiFi
void connectToWiFi() {
  Serial.print("Attempting to connect to WiFi...");
  WiFi.begin(ssid, password);  // Start connecting to WiFi

  int attempts = 0;  // Track connection attempts
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    Serial.print(".");
    delay(1000);  // Retry every second
    attempts++;
  }

  // Check if connected
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());  // Display device IP address
    server.on("/getPosition.asp", HTTP_GET, handleGetPosition); // Handle requests to /getPosition.asp
    server.begin();  // Start the server
  } else {
    Serial.println("Unable to connect to WiFi");
    delay(5000);  // Retry in 5 seconds
  }
}

// Measure the distance using the ultrasonic sensor
void measureDistance() {
  digitalWrite(trigPin1, LOW);    // Ensure the Trig pin is low
  delayMicroseconds(5);           // Wait for 5 microseconds
  digitalWrite(trigPin1, HIGH);   // Trigger the ultrasonic sensor
  delayMicroseconds(10);          // Wait for 10 microseconds
  digitalWrite(trigPin1, LOW);    // Set the Trig pin low again

  duration1 = pulseIn(echoPin1, HIGH, 30000);  // Read the Echo pin and measure the time
  distance1 = (duration1 / 2) / 29.1;          // Calculate the distance in cm

  // Ensure the distance is within a valid range
  if (distance1 > 0 && distance1 <= 400) {
    avg_distance = distance_filter.filter(distance1);  // Smooth the data using the low-pass filter
    Serial.print("Distance: ");
    Serial.print(avg_distance);  // Output the distance to the serial monitor
    Serial.println(" cm");
  }

  delay(delay_time);  // Wait for the next measurement
}

// Handle HTTP request to get the distance
void handleGetPosition() {
  Serial.println("Received HTTP request for /getPositi.asp");
  measureDistance();  // Measure the distance on each request
  String message = "distance: ";
  message += avg_distance;  // Return the filtered distance
  message += " cm";
  server.send(200, "text/plain", message);  // Send the distance back to the client
  Serial.println("Sent distance to client: " + message);  // Confirm data sent
}

void setup() {
  Serial.begin(115200);  // Set the baud rate for serial communication
  Serial.println("Initializing ESP8266...");
  connectToWiFi();  // Attempt to connect to WiFi

  pinMode(trigPin1, OUTPUT);    // Set the Trig pin as an output
  pinMode(echoPin1, INPUT);     // Set the Echo pin as an input
  pinMode(speakerPin, OUTPUT);  // Set the buzzer pin as an output

  server.begin();  // Start the web server
  Serial.println("Web server started...");
}

void loop() {
  // Check WiFi connection and attempt to reconnect if needed
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  } else {
    server.handleClient();  // Handle incoming client requests
    measureDistance();      // Measure the distance

    // Control buzzer sound based on the measured distance
    if (avg_distance >= 1 && avg_distance <= 80) {  // Continuous sound for distances between 1 and 80 cm
      digitalWrite(speakerPin, HIGH);  // Buzzer on
    } else if (avg_distance > 80 && avg_distance <= 150) {  // Intermittent sound for distances between 80 and 150 cm
      digitalWrite(speakerPin, HIGH); // Buzzer on
      delay(200);                     // On for 200ms
      digitalWrite(speakerPin, LOW);  // Buzzer off
      delay(200);                     // Off for 200ms
    } else if (avg_distance > 150 && avg_distance <= 180) {  // Slower intermittent sound for distances between 151 and 180 cm
      digitalWrite(speakerPin, HIGH); // Buzzer on
      delay(300);                     // On for 300ms
      digitalWrite(speakerPin, LOW);  // Buzzer off
      delay(500);                     // Off for 500ms
    } else {
      digitalWrite(speakerPin, LOW);  // Buzzer off for distances outside the range
    }
  }
}
