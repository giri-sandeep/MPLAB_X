#include "WM8960.h"
PRIVATE INT WM8960Registers[WM8960_TOTAL_REGISTERS];

PRIVATE void WM8960CodecSetRegCopyToDefault(void)
{
    /* If the codec driver needs to keep
     * track about what was written to the
     * codec registers, the following array
     * is available.
     */

    WM8960Registers[WM8960_LEFT_INPUT_VOLUME   ] = 0x97;
    WM8960Registers[WM8960_RIGHT_INPUT_VOLUME  ] = 0x97;
    WM8960Registers[WM8960_LEFT_OUT1_VOLUME    ] = 0;
    WM8960Registers[WM8960_RIGHT_OUT1_VOLUME   ] = 0;
    WM8960Registers[WM8960_CLOCKING_1          ] = 0;
    WM8960Registers[WM8960_ADC_DAC_CTRL1       ] = 0x8;
    WM8960Registers[WM8960_ADC_DAC_CTRL2       ] = 0;
    WM8960Registers[WM8960_AUDIO_INTERFACE_1   ] = 0xA;
    WM8960Registers[WM8960_CLOCKING_2          ] = 0x1C0;
    WM8960Registers[WM8960_AUDIO_INTERFACE_2   ] = 0x0;
    WM8960Registers[WM8960_LEFT_DAC_VOLUME     ] = 0xFF;
    WM8960Registers[WM8960_RIGHT_DAC_VOLUME    ] = 0xFF;
    WM8960Registers[WM8960_RESET               ] = 0x0;
    WM8960Registers[WM8960_3D_CTRL             ] = 0x0;
    WM8960Registers[WM8960_ALC_1               ] = 0x7B;
    WM8960Registers[WM8960_ALC_2               ] = 0x100;
    WM8960Registers[WM8960_ALC_3               ] = 0x32;
    WM8960Registers[WM8960_NOISE_GATE          ] = 0x0;
    WM8960Registers[WM8960_LEFT_ADC_VOLUME     ] = 0xC3;
    WM8960Registers[WM8960_RIGHT_ADC_VOLUME    ] = 0xC3;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_1   ] = 0x1C0;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_2   ] = 0x0;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_3   ] = 0x0;
    WM8960Registers[WM8960_POWER_MGMT_1        ] = 0x0;
    WM8960Registers[WM8960_POWER_MGMT_2        ] = 0x0;
    WM8960Registers[WM8960_ANTI_POP_1          ] = 0x0;
    WM8960Registers[WM8960_ANTI_POP_2          ] = 0x0;
    WM8960Registers[WM8960_ADCL_SIGNAL_PATH    ] = 0x100;
    WM8960Registers[WM8960_ADCR_SIGNAL_PATH    ] = 0x100;
    WM8960Registers[WM8960_LEFT_OUT_MIX      ] = 0x50;
    WM8960Registers[WM8960_RIGHT_OUT_MIX     ] = 0x50;
    WM8960Registers[WM8960_MONO_OUT_MIX_1      ] = 0x0;
    WM8960Registers[WM8960_MONO_OUT_MIX_2      ] = 0x0;
    WM8960Registers[WM8960_LOUT2_VOLUME        ] = 0x0;
    WM8960Registers[WM8960_ROUT2_VOLUME        ] = 0x0;
    WM8960Registers[WM8960_MONO_OUT_VOLUME     ] = 0x40;
    WM8960Registers[WM8960_INPUT_BOOST_MIXER_1 ] = 0x0;
    WM8960Registers[WM8960_INPUT_BOOST_MIXER_2 ] = 0x0;
    WM8960Registers[WM8960_BYPASS_1            ] = 0x50;
    WM8960Registers[WM8960_BYPASS_2            ] = 0x50;
    WM8960Registers[WM8960_POWER_MGMT_3        ] = 0x0;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_4   ] = 0x2;
    WM8960Registers[WM8960_CLASS_D_CTRL_1      ] = 0x37;
    WM8960Registers[WM8960_CLASS_D_CTRL_3      ] = 0x80;
    WM8960Registers[WM8960_PLL_N               ] = 0x8;
    WM8960Registers[WM8960_PLL_K_1             ] = 0x31;
    WM8960Registers[WM8960_PLL_K_2             ] = 0x26;
    WM8960Registers[WM8960_PLL_K_3             ] = 0xE9;

}

INT WM8960Codec_i2c(WM8960_REGISTER commandRegister, void * data)
{
    /* This function writes a value to the
     * codec register. It returns 1 if the
     * function is successful. -1 otherwise
     */

    INT command = *(INT *)data;
    BYTE byte1  = (BYTE) (commandRegister << 1) | (command >> 8);
    BYTE byte2  = (BYTE) (command & 0xFF);

    /* Wait for the bus to go idle */
    while(I2CBusIsIdle(WM8960DRV_I2C_MODULE) == FALSE);

    /* Send a start */

    if(I2CStart(WM8960DRV_I2C_MODULE) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));

        return(-1);
    }

    /* Wait for start to complete */
    while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_START));

    /* Now send the codec I2C address */
    if(I2CSendByte(WM8960DRV_I2C_MODULE,WM8960_I2C_ADDRESS) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));

        return(-1);
    }

    /* Wait till transmission is completed */
    while(!I2CTransmissionHasCompleted(WM8960DRV_I2C_MODULE));

    /* Check if the address was acknowledged */
    if(I2CByteWasAcknowledged(WM8960DRV_I2C_MODULE) == FALSE)
    {
        /* This means some thing went wrong. The address
         * was not acknowledged. Send a stop condition and
         * exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));
        return(-1);
    }

    /* Now send the first byte. This includes bits
     * 7 through 0 of the codec register address and
     * bit 8 of the data to be written to the
     * register */

    if(I2CSendByte(WM8960DRV_I2C_MODULE,byte1) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));

        return(-1);
    }

    /* Wait till transmission is completed */
    while(!I2CTransmissionHasCompleted(WM8960DRV_I2C_MODULE));

    /* Check if the address was acknowledged */
    if(I2CByteWasAcknowledged(WM8960DRV_I2C_MODULE) == FALSE)
    {
        /* This means some thing went wrong. The address
         * was not acknowledged. Send a stop condition and
         * exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));
        return(-1);
    }

    /* Now send the second byte. This includes bits
     * 7 through 0 of the codec register address and
     * bit 8 of the data to be written to the
     * register */

    if(I2CSendByte(WM8960DRV_I2C_MODULE,byte2) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));

        return(-1);
    }

    /* Wait till transmission is completed */
    while(!I2CTransmissionHasCompleted(WM8960DRV_I2C_MODULE));

    /* Check if the byte2 was acknowledged */
    if(I2CByteWasAcknowledged(WM8960DRV_I2C_MODULE) == FALSE)
    {
        /* This means some thing went wrong. The address
         * was not acknowledged. Send a stop condition and
         * exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));
        return(-1);
    }

    /* Send the stop condition */
    I2CStop(WM8960DRV_I2C_MODULE);

    /* Wait for stop to complete */
    while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));

    return(1);

}

INT WM8960CodecConfigSampleRate(WM8960_SAMPLE_RATE sampleRate)
{
    /* This function configures the codec
     * for 48KHz sample rate. This function
     * should be the first function that is called
     * after codec driver is opened. It will
     * configure the digital interface but will
     * not enable it.
     */

    INT command;
    INT result;

    /* Reset the codec */
 //   command = 0;
 //   result = WM8960Codec_ioctl(pCodecHandle, WM8960_RESET, &command);
 //   if (result < 0) return(0);

    /* Disable Left and Right PGA Mute. Other register
     * bits are default. */

    WM8960Registers[WM8960_LEFT_INPUT_VOLUME] = 0x13F;
    result = WM8960Codec_i2c(WM8960_LEFT_INPUT_VOLUME,
            &WM8960Registers[WM8960_LEFT_INPUT_VOLUME]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_RIGHT_INPUT_VOLUME] = 0x13F;
    WM8960Codec_i2c(WM8960_RIGHT_INPUT_VOLUME,
            &WM8960Registers[WM8960_RIGHT_INPUT_VOLUME]);
    if(result < 0) return(-1);

    /* Configure output for the 0dB level. Set bit
     * so that the volume is updated. */

    WM8960Registers[WM8960_LEFT_OUT1_VOLUME] = 0x179;
    result = WM8960Codec_i2c(WM8960_LEFT_OUT1_VOLUME,
            &WM8960Registers[WM8960_LEFT_OUT1_VOLUME]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_RIGHT_OUT1_VOLUME] = 0x179;
    result = WM8960Codec_i2c(WM8960_RIGHT_OUT1_VOLUME,
            &WM8960Registers[WM8960_RIGHT_OUT1_VOLUME]);
    if(result < 0) return(-1);


    /* More ADC and DAC configuration */

    WM8960Registers[WM8960_ADC_DAC_CTRL2] = 0xc;//0;
    result = WM8960Codec_i2c(WM8960_ADC_DAC_CTRL2,
            &WM8960Registers[WM8960_ADC_DAC_CTRL2]);
    if(result < 0) return(-1);

    /* Audio Interface */

    WM8960Registers[WM8960_AUDIO_INTERFACE_1] = 0x4D;
    result = WM8960Codec_i2c(WM8960_AUDIO_INTERFACE_1,
            &WM8960Registers[WM8960_AUDIO_INTERFACE_1]);
    if(result < 0) return(-1);

    /* Configure bit clock to support a word length
     * of 16 bits. This will allow 32 bits to  be
     * transmitted in one frame.
     */

    WM8960Registers[WM8960_CLOCKING_2] = 0x1C7;
    result = WM8960Codec_i2c(WM8960_CLOCKING_2,
            &WM8960Registers[WM8960_CLOCKING_2]);
    if(result < 0) return(-1);

    /* Default values */
    WM8960Registers[WM8960_AUDIO_INTERFACE_2] = 0x0;
    result = WM8960Codec_i2c(WM8960_AUDIO_INTERFACE_2,
            &WM8960Registers[WM8960_AUDIO_INTERFACE_2]);
    if(result < 0) return(-1);

    /* Left DAC volume. Set at 0 db*/
    WM8960Registers[WM8960_LEFT_DAC_VOLUME] = 0x1D7;
    result = WM8960Codec_i2c(WM8960_LEFT_DAC_VOLUME,
            &WM8960Registers[WM8960_LEFT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Right DAC volume. Set at 0 dB */

    WM8960Registers[WM8960_RIGHT_DAC_VOLUME] = 0x1D7;
    result = WM8960Codec_i2c(WM8960_RIGHT_DAC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Left ADC volume. Set at zero dB */

    WM8960Registers[WM8960_LEFT_ADC_VOLUME] = 0x1C3;
    result = WM8960Codec_i2c(WM8960_LEFT_ADC_VOLUME,
            &WM8960Registers[WM8960_LEFT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Right ADC volume. Set at zero dB */

    WM8960Registers[WM8960_RIGHT_ADC_VOLUME] = 0x1C3;
    result = WM8960Codec_i2c(WM8960_RIGHT_ADC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Left Output Mixer */

    WM8960Registers[WM8960_LEFT_OUT_MIX] = 0x100;
    result = WM8960Codec_i2c(WM8960_LEFT_OUT_MIX,
            &WM8960Registers[WM8960_LEFT_OUT_MIX]);
    if(result < 0) return(-1);

    /* Right Output Mixer */

    WM8960Registers[WM8960_RIGHT_OUT_MIX] = 0x100;
    result = WM8960Codec_i2c(WM8960_RIGHT_OUT_MIX,
            &WM8960Registers[WM8960_RIGHT_OUT_MIX]);
    if(result < 0) return(-1);

    /* Set up the Microphone Bias voltage level */

    WM8960Registers[WM8960_ADDITIONAL_CTRL_4   ] = 0x3;
    result = WM8960Codec_i2c(WM8960_ADDITIONAL_CTRL_4 ,
            &WM8960Registers[WM8960_ADDITIONAL_CTRL_4 ]);

    if(result < 0) return(-1);

    if(sampleRate != SAMPLE_RATE_44100_HZ)
    {

        /* If the required sampling rate is 48,32,24,16 or 8
         * KHz, the SYSCLK should ne 12.288MHz.*/

        /* Configure the PLL to provide a 12.288 MHz clock */

        WM8960Registers[WM8960_PLL_N] = 0x28;
        WM8960Registers[WM8960_PLL_K_1] = 0x31;
        WM8960Registers[WM8960_PLL_K_2] = 0x26;
        WM8960Registers[WM8960_PLL_K_3] = 0xE8;

        result = WM8960Codec_i2c(WM8960_PLL_N,
                &WM8960Registers[WM8960_PLL_N]);
        if(result < 0) return(-1);

        result = WM8960Codec_i2c(WM8960_PLL_K_1,
                &WM8960Registers[WM8960_PLL_K_1]);
        if(result < 0) return(-1);

        result = WM8960Codec_i2c(WM8960_PLL_K_2,
                &WM8960Registers[WM8960_PLL_K_2]);
        if(result < 0) return(-1);

        result = WM8960Codec_i2c(WM8960_PLL_K_3,
                &WM8960Registers[WM8960_PLL_K_3]);
        if(result < 0) return(-1);

        /* Configure the ADC and DAC clock dividers */

        switch(sampleRate)
        {
            case SAMPLE_RATE_48000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x5;
                result = WM8960Codec_i2c(WM8960_CLOCKING_1,
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x6;
                result = WM8960Codec_i2c(WM8960_ADC_DAC_CTRL1,
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
            case SAMPLE_RATE_32000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x4D;
                result = WM8960Codec_i2c(WM8960_CLOCKING_1,
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x2;
                result = WM8960Codec_i2c(WM8960_ADC_DAC_CTRL1,
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
/*
            case SAMPLE_RATE_24000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x95;
                result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1,
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x0;
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1,
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
*/
            case SAMPLE_RATE_16000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0xDD;
                result = WM8960Codec_i2c(WM8960_CLOCKING_1,
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x0;
                result = WM8960Codec_i2c(WM8960_ADC_DAC_CTRL1,
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
            case SAMPLE_RATE_8000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x1B5;
                result = WM8960Codec_i2c(WM8960_CLOCKING_1,
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x0;
                result = WM8960Codec_i2c(WM8960_ADC_DAC_CTRL1,
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
            default:
                /* sample rate is not supported */
                return(-1);
                break;
        }

    }
    else if(sampleRate == SAMPLE_RATE_44100_HZ)
    {
        /* The sampling rate is 44100Hz and this needs a
         * SYSCLK of 11.2896 MHz */

        WM8960Registers[WM8960_PLL_N] = 0x27;
        WM8960Registers[WM8960_PLL_K_1] = 0x86;
        WM8960Registers[WM8960_PLL_K_2] = 0xC2;
        WM8960Registers[WM8960_PLL_K_3] = 0x26;

        result = WM8960Codec_i2c(WM8960_PLL_N,
                &WM8960Registers[WM8960_PLL_N]);
        if(result < 0) return(-1);

        result = WM8960Codec_i2c(WM8960_PLL_K_1,
                &WM8960Registers[WM8960_PLL_K_1]);
        if(result < 0) return(-1);

        result = WM8960Codec_i2c(WM8960_PLL_K_2,
                &WM8960Registers[WM8960_PLL_K_2]);
        if(result < 0) return(-1);

        result = WM8960Codec_i2c(WM8960_PLL_K_3,
                &WM8960Registers[WM8960_PLL_K_3]);
        if(result < 0) return(-1);

        WM8960Registers[WM8960_CLOCKING_1] = 0x05;
        result = WM8960Codec_i2c(WM8960_CLOCKING_1,
                &WM8960Registers[WM8960_CLOCKING_1]);
        if(result < 0) return(-1);
        WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x4;
        result = WM8960Codec_i2c(WM8960_ADC_DAC_CTRL1,
                &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
        if(result < 0) return(-1);


    }

    /* Power up all the modules */

    WM8960Registers[WM8960_POWER_MGMT_1] = 0xFE;
    result = WM8960Codec_i2c(WM8960_POWER_MGMT_1,
            &WM8960Registers[WM8960_POWER_MGMT_1]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_POWER_MGMT_2] = 0x1E1;
    result = WM8960Codec_i2c(WM8960_POWER_MGMT_2,
            &WM8960Registers[WM8960_POWER_MGMT_2]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_POWER_MGMT_3] = 0x03C;
    result = WM8960Codec_i2c(WM8960_POWER_MGMT_3,
            &WM8960Registers[WM8960_POWER_MGMT_3]);
    if(result < 0) return(-1);

    return(1);
}

INT WM8960CodecConfigVolume(UINT8 volumePerA, UINT8 volumePerD)
{
    INT result;
    UINT8 volumeA;
    UINT8 volumeD;

	volumePerA=(volumePerA>90)?90 : volumePerA;
	volumePerD=(volumePerD>90)?90 : volumePerD;

	volumeA=0x7F + (UINT8)((long)(0x7F*volumePerA)/100);
	volumeD=0x7F + (UINT8)((long)(0x7F*volumePerD)/100);


    /* Left ADC volume. Set at zero dB */

    WM8960Registers[WM8960_LEFT_ADC_VOLUME] = 0x100;
    result = WM8960Codec_i2c(WM8960_LEFT_ADC_VOLUME,
            &WM8960Registers[WM8960_LEFT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Right ADC volume. Set at zero dB */

    WM8960Registers[WM8960_RIGHT_ADC_VOLUME] = 0x100;
    result = WM8960Codec_i2c(WM8960_RIGHT_ADC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_ADC_VOLUME]);
    if(result < 0) return(-1);


    /* Left DAC volume. Set at 0 db*/
    WM8960Registers[WM8960_LEFT_DAC_VOLUME] = 0x100;
    result = WM8960Codec_i2c(WM8960_LEFT_DAC_VOLUME,
            &WM8960Registers[WM8960_LEFT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Right DAC volume. Set at 0 dB */

    WM8960Registers[WM8960_RIGHT_DAC_VOLUME] = 0x100;
    result = WM8960Codec_i2c(WM8960_RIGHT_DAC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_DAC_VOLUME]);
    if(result < 0) return(-1);



    /* Left ADC volume. Set at zero dB */

    WM8960Registers[WM8960_LEFT_ADC_VOLUME] = 0x100|volumeA;
    result = WM8960Codec_i2c(WM8960_LEFT_ADC_VOLUME,
            &WM8960Registers[WM8960_LEFT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Right ADC volume. Set at zero dB */

    WM8960Registers[WM8960_RIGHT_ADC_VOLUME] = 0x100|volumeA;
    result = WM8960Codec_i2c(WM8960_RIGHT_ADC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_ADC_VOLUME]);
    if(result < 0) return(-1);


    /* Left DAC volume. Set at 0 db*/
    WM8960Registers[WM8960_LEFT_DAC_VOLUME] = 0x100|volumeD;
    result = WM8960Codec_i2c(WM8960_LEFT_DAC_VOLUME,
            &WM8960Registers[WM8960_LEFT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Right DAC volume. Set at 0 dB */

    WM8960Registers[WM8960_RIGHT_DAC_VOLUME] = 0x100|volumeD;
    result = WM8960Codec_i2c(WM8960_RIGHT_DAC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_DAC_VOLUME]);
    if(result < 0) return(-1);

}

INT WM8960CodecOpen()
{

    /* Intialize the the codec control
     * I2C module.
     */

    I2CConfigure(WM8960DRV_I2C_MODULE,0);
    I2CSetFrequency(WM8960DRV_I2C_MODULE, GetPeripheralClock(), WM8960DRV_I2C_BAUD);
    I2CEnable(WM8960DRV_I2C_MODULE, TRUE);

    /* Setup default state of codec registers */
    WM8960CodecSetRegCopyToDefault();

    INT command = 0;
    WM8960Codec_i2c(WM8960_RESET, &command);

}




