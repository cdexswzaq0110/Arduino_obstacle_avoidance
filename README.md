# ESP8266 Ultrasonic Sensor Distance Measurement with Low-pass Filter

This project utilizes the LOLIN (WeMos) D1 WiFi Arduino UNO development board to measure distance using an ultrasonic sensor. The system connects to a WiFi network and hosts a web server that provides real-time distance data. A low-pass filter is applied to smooth the sensor readings, providing more accurate and stable data. Additionally, the system includes a buzzer that alters its sound pattern based on the measured distance, offering feedback in real-time.

## Features

- **Distance Measurement**: Measures distance using an ultrasonic sensor.
- **Low-pass Filter**: Smooths the distance data to reduce noise and provide more stable readings.
- **WiFi Connectivity**: Connects to a WiFi network and hosts a simple web server.
- **Web Interface**: Responds to HTTP requests with the current distance.
- **Buzzer Feedback**: Adjusts the buzzer sound based on the measured distance:
  - Continuous sound for distances between 1 and 80 cm.
  - Intermittent sound for distances between 80 and 150 cm.
  - Slower intermittent sound for distances between 151 and 180 cm.

## Hardware Components

- **LOLIN (WeMos) D1 WiFi Arduino UNO development board**: The core microcontroller responsible for WiFi communication, sensor reading, and controlling the buzzer.
- **Ultrasonic Sensor (HC-SR04)**: Used to measure the distance to nearby objects by emitting sound waves and calculating the echo time.
- **Buzzer**: Provides audible feedback based on the distance.
- **Breadboard, wires, and other peripherals**: For connecting the components.

## Software Libraries

- `ESP8266WiFi.h`: Handles WiFi connections.
- `ESP8266WebServer.h`: Manages the web server that responds to client requests.

## Low-pass Filter Design

A low-pass filter is used to smooth the noisy distance data from the ultrasonic sensor. This filter reduces high-frequency noise while allowing lower frequency signals (the actual distance data) to pass through. The smoothing is achieved by applying the following formula:

prev_value = (time_constant / (time_constant + 1)) * prev_value + (1 / (time_constant + 1)) * input;


- **time_constant**: Controls how much past values influence the current filtered value. A smaller time constant makes the filter more responsive to new data, while a larger time constant makes the filter smoother but slower to respond.

In this project, the time constant is set to `0.1`, which balances responsiveness and smoothing.

## Getting Started

1. Clone or download the project repository.
2. Open the project in Arduino IDE.
3. Upload the code to your ESP8266.
4. Connect the ultrasonic sensor and buzzer to the appropriate pins.
5. Modify the WiFi credentials in the code.
6. Once the ESP8266 is connected to WiFi, you can access the web server by navigating to `http://<ESP8266_IP_Address>/getPosition.asp`.
