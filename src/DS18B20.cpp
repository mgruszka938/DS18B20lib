#include "DS18B20.hpp"

DS18B20::DS18B20(TIM_HandleTypeDef *tim, GPIO_TypeDef *port, uint16_t pin) : _tim(tim), _port(port), _pin()
{
    HAL_TIM_Base_Start(_tim);
}

void DS18B20::delayMicro(uint16_t us)
{
    _tim->Instance->CNT = 0;
    while (_tim->Instance->CNT < us) /* Loop lasts until counter value until it equals or exceedes the value of us */
        ;
}

void DS18B20::setInput()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    return HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

void DS18B20::setOutput()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    return HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

void DS18B20::setPin(bool on)
{
    return HAL_GPIO_WritePin(_port, _pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void DS18B20::togglePin()
{
    return HAL_GPIO_TogglePin(_port, _pin);
}

GPIO_PinState DS18B20::readPin()
{
    return HAL_GPIO_ReadPin(_port, _pin);
}

void DS18B20::writeData(uint8_t data)
{
    setOutput();
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1 << i))
        {
            setOutput();
            setPin(false);
            delayMicro(1);
            setInput();
            delayMicro(60);
            continue;
        }

        setOutput();
        setPin();
        delayMicro(60);
        setInput();
    }
}

uint8_t DS18B20::readData()
{
    uint8_t val = 0;
    setInput();
    for (uint8_t i = 0; i < 8; i++)
    {
        setOutput();
        setPin(false);
        delayMicro(2);
        setInput();

        if (readData())
        {
            val |= 1 << 1;
        }

        delayMicro(60);
    }

    return val;
}