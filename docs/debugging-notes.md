# Debugging Notes

## STM32CubeIDE / CubeMX Setup
- Initial empty CubeIDE project did not include STM32 CMSIS device headers.
- Switched to STM32CubeMX to generate the project properly for the NUCLEO-F401RE.
- Generated project successfully built in STM32CubeIDE.

## UART Debugging
- USART2 configured for 115200 baud, 8-N-1.
- Used PuTTY through the ST-LINK Virtual COM Port.
- Encountered garbled output when terminal settings did not match the firmware configuration.
- Correcting the baud rate restored normal UART output.

## ADC Debugging
- Potentiometer connected to PA0 / A0 using ADC1_IN0.
- Initial two-channel ADC scan configuration caused the program to hang while waiting for the second conversion.
- Changed approach to manually configure and read ADC channels one at a time.
- PA0 used for potentiometer.
- PA1 used for temperature sensor.

## Temperature Sensor Debugging
- Original MCP9700AE produced unrealistic and unstable readings.
- Verified PA1 ADC path by tying A1 directly to ground; ADC correctly read near zero.
- New sensor is the TMP3600, reads 20 degrees below accurate temp. offset the read value by 20 degrees inside while(1)

## Fan / MOSFET Debugging
- MOSFET module arrived without the control header soldered.
- Temporary header-pin contact was used successfully for testing.
- PA4 / A2 configured as GPIO output for fan control.
- Fan successfully switched ON and OFF through the MOSFET module.
- Control header still needs to be permanently soldered.

## Current Known Issues
- Replacement temperature sensor still needs to be installed and verified.
- MOSFET control header needs permanent soldering.
- External 5 V supply still needs final integration.
- PWM fan-speed control and OLED display are not yet implemented.
