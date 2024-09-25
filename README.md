# ESP8266 Ultrasonic Sensor Distance Measurement with Low-pass Filter

This project uses an ESP8266 microcontroller to measure distance using an ultrasonic sensor. It connects to a WiFi network and hosts a web server that provides the current measured distance. A low-pass filter is used to smooth the data and provide more accurate readings. The system also includes a buzzer that adjusts its sound based on the measured distance.

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

- **ESP8266**: The microcontroller used for WiFi communication and controlling the sensor and buzzer.
- **Ultrasonic Sensor**: Measures the distance to nearby objects by emitting sound waves and measuring the time it takes for the echo to return.
- **Buzzer**: Provides audible feedback based on the distance.
- **Breadboard, wires, and other peripherals** for connections.

## Software Libraries

- `ESP8266WiFi.h`: Handles WiFi connections.
- `ESP8266WebServer.h`: Manages the web server that responds to client requests.

## Low-pass Filter Design

A low-pass filter is used to smooth the noisy distance data from the ultrasonic sensor. This filter reduces high-frequency noise while allowing lower frequency signals (the actual distance data) to pass through. The smoothing is achieved by applying the following formula:

