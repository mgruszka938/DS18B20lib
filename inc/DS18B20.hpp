#pragma once

#include "stm32l4xx_hal.h"
#include "st32l4xx_hal_tim.h" /* Before using the library you need to set up the timer, so that it has a resolution of 1us */
#include <stdint.h>

class DS18B20
{
public:
    DS18B20(TIM_HandleTypeDef *tim, GPIO_TypeDef *port, uint16_t pin);
    float read_temperature();

private:
    TIM_HandleTypeDef *_tim;
    GPIO_TypeDef *_port;
    uint16_t _pin;

    void delayMicro(uint16_t us);
    void setInput();
    void setOutput();
    void setPin(bool on);
    void togglePin();
    GPIO_PinState readPin();
    void startSensor();
    void writeData(uint8_t data);
    uint8_t readData();
};
