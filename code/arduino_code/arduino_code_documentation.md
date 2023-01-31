argb_eg
Arduino (>=UNO) parses serial commands and convert it to signal changing fan color.
Example of fan color control can be found in notebook Pyserial_arduino_fan.ipynb

For a single fan color update with pyserial, use:
```
arduino.write(('<' + str(color) + ',' + str(p11) + ','  + str(p12) + ','  + str(p21) + ','  + str(p22) + '>').encode('ascii')) #240=>120
```
where:
color is the Hue value of 1st fan color.
p11 is the number of LED on of all 9 LEDS in 2nd fan. (color is default to be white), p12 is the brightness (0-255) of last active LED in 2nd fan.
p21 is the number of LED on of all 9 LEDS in 3rd fan. (color is default to be white), p22 is the brightness (0-255) of last active LED in 3rd fan.