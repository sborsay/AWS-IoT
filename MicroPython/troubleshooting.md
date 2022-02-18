```diff
- Cannot connect MQTT: bytes index out of range bytes index out of range
 
+ Usually casue by certificate creation region not matching the AWS endpoint region you are using


```

If it doesn't work for you check the solutions below:

If you are getting an error: = RESTART = Unable to connect to COM4 Error: could not open port 'COM4': FileNotFoundError(2, 
'The system cannot find the file specified.', None, 2) Check the configuration, select Run -> Stop/Restart or press Ctrl+F2 
to try again. (On some occasions it helps to wait before trying again.) or = RESTART = Could not connect to REPL.Make sure
 your device has suitable firmware and is not in bootloader mode!Disconnecting or = RESTART = Lost connection to the device
 (EOF). Disconnect and connect your ESP32 development board. Then, double-check that you selected correct serial port in the 
Tools -> Options... -> Interpreter -> Port. Click on Stop/Restart backend button to establish a serial communication.
 You should now be able to upload a new script or re-run new code. These errors might also mean that you have your
 serial port being used in another program (like a serial terminal or in the Arduino IDE). Double-check that you
 closed all the programs that might be establishing a serial communication with your ESP32 development board.
 Disconnect and connect your ESP32 development board again. Do restart Thonny IDE.

//from https://acoptex.com/project/11901/basics-project-086p-esp32-development-board-using-thonny-ide-at-alek-c/

1. If you are getting an error: = RESTART = Unable to connect to COM4 Error: could not open port 'COM4': FileNotFoundError
(2, 'The system cannot find the file specified.', None, 2) Check the configuration, select Run -> Stop/Restart or press
Ctrl+F2 to try again. (On some occasions it helps to wait before trying again.) or = RESTART = Could not connect to REPL
.Make sure your device has suitable firmware and is not in bootloader mode!Disconnecting or = RESTART = Lost connectio
n to the device (EOF). Disconnect and connect your ESP32 development board. Then, double-check that you selected correct
serial port in the Tools -> Options... -> Interpreter -> Port. Click on Stop/Restart backend button to establish a serial
communication. You should now be able to upload a new script or re-run new code. These errors might also mean that you
have your serial port being used in another program (like a serial terminal or in the Arduino IDE). Double-check that
you closed all the programs that might be establishing a serial communication with your ESP32 development board.
Disconnect and connect your ESP32 development board again. Do restart Thonny IDE.

2. If Thonny IDE fails to respond or gives an Internal Error window. When it happens, close that window and it will continue to
 work. If it will continue to crash restart the Thonny IDE software.


3. If Thonny IDE hangs when you click on Stop/Restart backend button. When you click on Stop/Restart backend button you need to wait
 a few seconds. The ESP32 development board needs time to restart and establish the serial communication with Thonny IDE. If
 you click on this button multiple times or if you click on this button very quickly, the ESP32 development board will not have
 enough time to restart properly and it's very likely crash Thonny IDE.

4. Problem restarting your ESP32 development board, running a new script or opening the serial port - Brownout detector was triggered
 or if the ESP32 development board keeps restarting and printing the ESP boot information. Brownout detector was triggered error
 message or constant reboots  means that there's some sort of hardware problem. It's often related to one of the following issues:
 poor quality USB cable; USB cable is too long; board with some defect (bad solder joints); bad computer USB port; or not enough
 power provided by the computer USB port. Try a different shorter USB data cable, try a different computer USB port or use a USB
 hub with an external power supply. If you keep having constant problems or error messages we recommend to re-flash your ESP32 
development board with the latest version of MicroPython firmware.


5. When you try to establish a serial communication with the ESP32 development board in Thonny IDE but can not get it to connect.
 When you are running a script in your board, sometimes it's busy running that script and performing the tasks. So, you need
 to try start the connection by click on Stop/Restart backend button multiple times or restart the ESP32 development board to
 catch available to establish the serial communication. Please  do not click on Stop/Restart backend button multiple times very
 quickly. After pressing that button, you need to be patient and wait a few seconds for the command to run. If you are running
 a script that uses Wi-Fi, deep sleep or it's doing multiple tasks we recommend trying 3 or 4 times to establish the communication.
 If you can not we recommend to re-flash the ESP32 development board with MicroPython firmware.


6.  Debug tools are grayed out. Thonny IDE debug tools are not available for MicroPython. The debug tools are only available for the Python
 Interpreter, so being grayed out is the expected behavior. 



