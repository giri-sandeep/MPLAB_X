/* 
 * File:   WM8960.h
 * Author: giri
 *
 * Created on 10. Juni 2014, 08:49
 */

#ifndef _WM8960_H_
#define	_WM8960_H_

#include <p32xxxx.h>
#include <GenericTypeDefs.h>
#include <plib.h>

#ifndef GetPeripheralClock
#define GetPeripheralClock() 10000000
#endif

/**************************************
 * WM8960 Codec I2C Address
 *************************************/

#define WM8960_I2C_ADDRESS 0x34

/**************************************
 * WM8960 I2C Control channel used on this
 * particular hardware.
 *************************************/

#define WM8960DRV_I2C_MODULE    I2C2
#define WM8960DRV_I2C_BAUD      400000

/**************************************
 * WM8960 I2C Data Communication SPI
 * channel used on this particular
 * hardware.
 *************************************/

#define WM8960DRV_SPI_MODULE            SPI_CHANNEL2A
#define WM8960DRV_SPI_MODULE_BUFFER     SPI2ABUF
#define WM8960DRV_SPI_MODULE_TX_IRQ     _SPI2A_TX_IRQ
#define WM8960DRV_SPI_MODULE_RX_IRQ     _SPI2A_RX_IRQ

/**************************************
 * WM8960 Registers and addresses.
 *************************************/

typedef enum
{
    WM8960_LEFT_INPUT_VOLUME    = 0x00,
    WM8960_RIGHT_INPUT_VOLUME   = 0x01,
    WM8960_LEFT_OUT1_VOLUME     = 0x02 ,
    WM8960_RIGHT_OUT1_VOLUME    = 0x03 ,
    WM8960_CLOCKING_1           = 0x04 ,
    WM8960_ADC_DAC_CTRL1        = 0x05,
    WM8960_ADC_DAC_CTRL2        = 0x06,
    WM8960_AUDIO_INTERFACE_1    = 0x07,
    WM8960_CLOCKING_2           = 0x08,
    WM8960_AUDIO_INTERFACE_2    = 0x09,
    WM8960_LEFT_DAC_VOLUME      = 0x0A,
    WM8960_RIGHT_DAC_VOLUME     = 0x0B,
    WM8960_RESET                = 0x0F,
    WM8960_3D_CTRL              = 0x10,
    WM8960_ALC_1                = 0x11,
    WM8960_ALC_2                = 0x12,
    WM8960_ALC_3                = 0x13,
    WM8960_NOISE_GATE           = 0x14,
    WM8960_LEFT_ADC_VOLUME      = 0x15,
    WM8960_RIGHT_ADC_VOLUME     = 0x16,
    WM8960_ADDITIONAL_CTRL_1    = 0x17,
    WM8960_ADDITIONAL_CTRL_2    = 0x18,
    WM8960_ADDITIONAL_CTRL_3    = 0x1B,
    WM8960_POWER_MGMT_1         = 0x19,
    WM8960_POWER_MGMT_2         = 0x1A,
    WM8960_ANTI_POP_1           = 0x1C,
    WM8960_ANTI_POP_2           = 0x1D,
    WM8960_ADCL_SIGNAL_PATH     = 0x20,
    WM8960_ADCR_SIGNAL_PATH     = 0x21,
    WM8960_LEFT_OUT_MIX         = 0x22,
    WM8960_RIGHT_OUT_MIX        = 0x25,
    WM8960_MONO_OUT_MIX_1       = 0x26,
    WM8960_MONO_OUT_MIX_2       = 0x27,
    WM8960_LOUT2_VOLUME         = 0x28,
    WM8960_ROUT2_VOLUME         = 0x29,
    WM8960_MONO_OUT_VOLUME      = 0x2A,
    WM8960_INPUT_BOOST_MIXER_1  = 0x2B,
    WM8960_INPUT_BOOST_MIXER_2  = 0x2C,
    WM8960_BYPASS_1             = 0x2D,
    WM8960_BYPASS_2             = 0x2E,
    WM8960_POWER_MGMT_3         = 0x2F,
    WM8960_ADDITIONAL_CTRL_4    = 0x30,
    WM8960_CLASS_D_CTRL_1       = 0x31,
    WM8960_CLASS_D_CTRL_3       = 0x33,
    WM8960_PLL_N                = 0x34,
    WM8960_PLL_K_1              = 0x35,
    WM8960_PLL_K_2              = 0x36,
    WM8960_PLL_K_3              = 0x37

}WM8960_REGISTER;

#define WM8960_TOTAL_REGISTERS 56

typedef enum
{
    SAMPLE_RATE_48000_HZ,
    SAMPLE_RATE_44100_HZ,
    SAMPLE_RATE_32000_HZ,
    //SAMPLE_RATE_24000_HZ,
    SAMPLE_RATE_16000_HZ,
    SAMPLE_RATE_8000_HZ,
    SAMPLE_RATE_NO_CHANGE

}WM8960_SAMPLE_RATE;

INT WM8960Codec_i2c(WM8960_REGISTER commandRegister, void * data);
INT WM8960CodecConfigSampleRate(WM8960_SAMPLE_RATE sampleRate);
INT WM8960CodecConfigVolume(UINT8 volumePerA, UINT8 volumePerD);
INT WM8960CodecOpen();
INT WM8960CodecDACOutput();

#endif;





