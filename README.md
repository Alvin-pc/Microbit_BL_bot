## Abstract- Bot controlled via Bluetooth mobile app
The project makes use of the BBC micro:bit, a pocket size computer with 25 red LED lights, 2 programmabe tactile buttons, microphone, speaker, accelerometer, magnetometer and much more. We used the Kitronik mini buggy kit, to build a moving bot that can be controlled via a Bluetooth app on a mobile device.
![image](https://github.com/Alvin-pc/Microbit_BL_bot/assets/105581851/cfbbaa56-13e0-4ad4-bc89-bf3a0c954e78)
The SoC is nRF52, and we built the Board Support Package(BSP) and BLE(Bluetooth Interface) out of the nRF52 SDK. RTX Real-Time Operating system(ARM Core M Series) based on CMSIS2 is used to syrochronise several threads that run on the bot. 
![image](https://github.com/Alvin-pc/Microbit_BL_bot/assets/105581851/bb98f8f1-a2fd-4469-a604-bec2ee92dc6b)
The bot can be to some extent be controlled to move with open loop control, but we added a simple feedback mechanism(Proportional Gain) that makes use of the magnetometer and accelerometer readings(Complimentary Filter) to maintain consistent angle(direction).
Additional features like Neopixels, LEDs, and speaker were used to enunciate the state of the bot. These call for multiple threads that have to be synchronised and queued appropriately to give a funtioning bot.

## Bluetooth MICROBIT


[SDK Setup](nRF5_SDK_17.1.0_ddde560/README.md)

[Bluetooth Setup](ble/README.md)

[RTX Setup](rtx/README.md)

[BSP Setup](bsp/README.md)
