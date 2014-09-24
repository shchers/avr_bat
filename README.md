# Simple battary monitor device
This device developed for monitoring car accumulator battary and notifying in case of problems with batt or generator.
It has a four LEDs with the next states:
  * Red (high - LED4) - power is over 14.5V means that something wromg woth generator
  * Red (high - LED4) + Green (LED3) - generator works and battary in charging mode
  * Green (LED3) - battary charged and power in range of 99-40%
  * Green (LED3) + Yellow (LED2) - battary discharged and power in range of 40-30%
  * Yellow (LED2) - battary discharged and power in range of 30-20%
  * Yellow (LED2) + Red (low - LED1) - battary heavy discharged, i.e. power in range of 20-10%
  * Red (low - LED1) - critical discharging, power less than 10%

To build firmware on Linux shuld be installed next tools (example for Ubuntu):
```
sudo apt-get install gcc-avr avr-libc avrdude
```

To use AVR dragon for flashing (for now make file hardcoded only for this programmer) should be added Udev rules:
  * Create new rules file in Udev folder:
```
sudo vim /etc/udev/rules.d/51-avr.rules
```
  * Add these string to rule file
```
ATTR{idVendor}=="03eb", ATTR{idProduct}=="2107", MODE="666"
```
  * Restart Udev daemon
```
sudo service udev restart
```

Build rules:
  * just "make" or "make all" or "make firmware" - will be created firmware for ATTiny13
  * "make fuses" - device will be flashed via SPI with correct fuses
  * "make flash" - firmware shall be uploaded to ATTiny13
  * "make clean" - all generated files will be removed
