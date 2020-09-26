# WifiDustSensor
Hosting the code in progress for connecting the Waveshare Dust Sensor Module with my Arduino MKR 1000 board.

**********************************
9/26/2020:
This project started on a desire to track indoor air pollution at a low-cost and find practical applications for a micro controller board, and then evolved into my first IoT project. This code will enable you to take readings from the Waveshare Dust Sensor Module and trigger a warning message to turn on your air filter if the dust density readings are too high. 

Waveshare hosts a "wiki" for their dust sensor module here: https://www.waveshare.com/wiki/Dust_Sensor
I used their code as a base template for readings from the sensor. However, while the code works when applied to an Arduino UNO, 
the delay() function used to take a reading from the AOUT port of the dust sensor causes error with a MKR 1000 board.

Using an Oscilliscope demonstrated that the digital pin out (D7) of the MKR board was not responding to code input if the wifi on the board was enabled. 
Upon further invesitgation I found that using a delay() function on a board using WiFi causes a communication conflict, since delay() stops all serial communication.

I'm currently in the process of testing the use of interrupts as a way to take readings at the precise time recommended by the 
data sheet for the dust sensor, a Sharp GP2Y1014AU0F (https://www.mouser.com/datasheet/2/365/GP2Y1014AU0F_SPEC-1288381.pdf)

This code and readme will be updated as frequently as possible, until a working solution is found.

***********************************
Setup:

Waveshare dust sensor is a fully integrated product with a simple plug-and-play type installation. Simply connect the V_CC, GND to your board's power and ground pins. The AOUT and ILED are connected to a GPIO pin capable of analog input conversion and a digital enabled GPIO for the ILED to be toggled on and off. 

***********************************
About:
The ILED pulses a light signal and it's diffraction due to dust particles is picked up by an infrared sensor, which gives a corresponding voltage output to an amplifier circuit and is output at AOUT. Timing of the pulse width and sample collection are critical for obtaining accurate readings. Sharp recommends a pulse width of 300-340 micro seconds and that sampling occur at roughly 280 microseconds into the pulse. Ideally the time between pulses would be roughly 10 milliseconds to prevent interferences between pulses.

The Waveshare code provides a rudimentary form of this sampling using a simple delayMicroseconds() function to create a 280 microsecond delay after the ILED pin is turned HIGH.  The AOUT input is then recorded and the ILED pin is set back to LOW.

The useful parts of the Waveshare code I preserved were the average function to even out the signal readings; the original "NO DUST VOLTAGE" levels, as I assume this was a calibration done by Waveshare with better equipment than what I have available; and the conversion from the AOUT voltage reading into the dust density. Waveshare also kindly provided a table at the end of their module's "User Manual" to show the correlation of density reading and rated air quality, as shown below:

Density:      |    Air Quality:           |     AQI:
0   - 35            Excellent                   0   - 50
35  - 75            Average                     51  - 100
75  - 115           Light Pollution             101 - 150
115 - 150           Moderate Pollution          151 - 200
150 - 250           Heavy Pollution             201 - 300
250 - 500           Serious Pollution           >= 300

Arduino has a host of built-in libraries to configure the connection of the MKR 1000 board to you wifi network. Basic information on this can be found on Arduino's website. I used the tutorial for the "I Love You Pillow" project as the basis for the communications between the MKR board and my phone (https://create.arduino.cc/projecthub/arduino/love-you-pillow-f08931?ref=platform&ref_id=424_trending___&offset=3)
I highly recommend you view that tutorial if this is your first time building an IoT project. 

After you create the Telegram bot, using the Bot Father API, enter the requisite wifi and bot token information. To get the chat ID to enable sending messages, I recommend using the Echobot sample code found in the TelegramBot library, and using "Serial.println(m.chatID) after the check messages line. 

After this configuration, place the dust sensor in your deisred location. For power, either a power bank or a direct USB to wall AC adapter.
