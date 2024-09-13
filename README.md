# Laser Projector Robot

## Summary

This repo contains all of the firmware to operate the laser projector!

The following is a overview of the work performed by [Ryan Manak](https://github.com/rmanak22):

- Overall design of firmware package and use of submodules
- Design and testing PID controllers to position two DC motors
- Design and testing of encoder position filters
- Preliminary design of laser position path planning
- Configuration and testing of STM32 hardware and peripherals
- Configuration of basic CI/CD system (Github Actions)

https://github.com/user-attachments/assets/f7988f82-3e74-4d4a-a1a3-aba7bfd9b3aa

https://github.com/user-attachments/assets/97ee21f7-2a8c-4a54-b086-ee989aa87727

## Firmware Layout
The Firmware in the source folder can be broken down into three distinct sections:
- App
    - Laser path planning
    - Motor control and position monitoring
    - H-bridge controller
- Device
    - Configuration of HAL API function callbacks
    - GPIO configuration and methods
    - Interrupt and timer configurations
    - UART configuration and methods
- Lib
    - PID controller
    - First order IIR lowpass filter

<img width="346" alt="Screenshot 2024-09-13 at 10 45 09" src="https://github.com/user-attachments/assets/fe70fa2d-7432-45bf-b6bb-7f72ba20b2d8">





