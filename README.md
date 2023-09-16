# "Ultrasonic Machine"

## Objective:
The primary objective of this project is to facilitate the mixing of chemicals while exposing them to ultrasonic waves.

## Methodology:
In this project, a small, sealed metal container was designed to hold the chemicals for mixing. This container was placed in the center of a gastronome, which served as a coolant and facilitated the transmission of ultrasonic waves. Four transducers were affixed to the back of the gastronome, generating the ultrasonic waves. This setup was integrated with a standard fruit blender, effectively acting as a mixer for the chemicals.

To ensure the chemicals were mixed while exposed to ultrasonic waves, an Arduino Uno was utilized. Connected to a TFT LCD screen, a 4-Channel Relay module, and two LM35 temperature sensors, the Arduino Uno was programmed to monitor the temperature of the transducer drivers. It would control the activation and deactivation of the ultrasonic waves, synchronized with the blender, based on user-defined timer settings and temperature thresholds displayed on the screen. This protected the drivers from potential damage and ensured their operation in harmony with the blender.

## Results:
The outcome of this system is a user-friendly machine, allowing easy control through the screen interface. Users can operate the system for extended periods without concerns about potential damage to the components.

## Challenges:
The most formidable challenge encountered in this project revolved around calibrating the touch screen. Despite experimenting with various touch screen calibration libraries and codes, achieving precise calibration required extensive testing of multiple parameters to determine the optimal settings.

## Relevance:
This project aligns harmoniously with the principles of Programmable Logic Controllers (PLC), a topic I am keen on further developing in the future.
