#pragma once

#include "stm32l4xx_hal.h"
#include "st32l4xx_hal_tim.h" /* Before using the library you need to set up the timer, so that it has a resolution of 1us */
#include <stdint.h>

class DS18B20
{
public:
    DS18B20(TIM_HandleTypeDef *tim, GPIO_TypeDef *port, uint16_t pin);
    float readTemp();

private:
    TIM_HandleTypeDef *m_tim;
    GPIO_TypeDef *m_port;
    uint16_t m_pin;

    void delayMicro(uint16_t us);

    void setInput();
    void setOutput();
    void setPin(bool on);
    void togglePin();
    GPIO_PinState readPin();

    void writeData(uint8_t data);
    uint8_t readData();

    void startSensor();
};
