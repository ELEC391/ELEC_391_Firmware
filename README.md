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

## Controller Performance

Position Controller Step Response

<img width="695" height="455" alt="Screenshot 2026-02-21 at 20 38 43" src="https://github.com/user-attachments/assets/921fd394-fbb2-4470-8892-1ab61cf06eb1" />

Filtering Results
<img width="757" height="455" alt="Screenshot 2026-02-22 at 10 06 51" src="https://github.com/user-attachments/assets/67a7e64d-3d1e-493f-af52-b8a72d285f87" />

<img width="490" height="386" alt="Screenshot 2026-02-21 at 20 39 47" src="https://github.com/user-attachments/assets/0289ddd0-ecbf-4b04-aaea-263ff0fef54b" />


