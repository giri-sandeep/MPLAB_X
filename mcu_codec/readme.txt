				PIC32 Audio Development Board Demo
***********************************************************************************************************   
This test uses SPI channels 2 to send the audio sine wave in I2S format using SPI interrupt. 
I2C 2 channel is used to set the codec register .

The master channel (SPI2) sends data to a slave device (codec).

Sampling rate and volume can be adjusted using the codec header file functions.
We are implementing 16khz in our application.

If we want to change the sampling rate the we need to change the codec sampling rate and the 
BCLK from microcontroller by selecting the serial clk divider.

In the same way if we want to change the bit resolution, we need to change the word length from codec 
and specific spi flags from microcontroller.

This way we can verify that the connection to the slave is ok.
Hardware connections have to be made:
	- SCK2 <-> BCLK
	- SDO2 <-> DACDAT
	- SDI2 <-> ADCDAT
	- SS2  <-> FRAME (needed only if we use the framed mode)

This demo works on Audio Development Board for PIC32 (DM320011).
