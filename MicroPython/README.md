─=≡Σ((( つ◕ل͜◕)つ

1.   Download and Install Thonny:
     https://thonny.org
  
2.   Erase your ESP's memory and upload MicroPython Environment to your device:

```
 pip install esptool
 ```

Download latest Binary:

ESP32: 

> https://micropython.org/download/esp32/

ESP8266:

> https://micropython.org/download/esp8266/

Erase your ESP Chip:

ESP32:    
```
python -m esptool –-chip esp32 erase_flash
```
or

ESP8266: 
```
python -m esptool --chip esp8266 erase_flash
```

Upload the ESP MicroPython Firmware:


ESP32:
```
> python -m esptool --chip esp32 --port COM4 write_flash -z 0x1000 esp32-2019-Latest-g5064dfxxx.bin
```
or

ESP8266: 
```
python -m esptool --chip esp8266 --port COM4 write_flash --flash_mode dio --flash_size detect 0x0 esp8266-Latest-vx.x.x.bin
```

* Replace COM4 with whatever Com port your device is connected to

```
 Thonny --> tools-->options-->Interpreter -> Set ESP device type and serial port
 ```
 Paste in Code and change indicated fields to personalize your device
 
 Program and connect to AWS IoT Core
 
 Congrats, now PayPal me or at least star my repo
