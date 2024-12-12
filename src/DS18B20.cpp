#include "DS18B20.hpp"

DS18B20::DS18B20(TIM_HandleTypeDef *tim, GPIO_TypeDef *port, uint16_t pin) : m_tim(tim), m_port(port), m_pin()
{
    HAL_TIM_Base_Start(m_tim);
}

void DS18B20::delayMicro(uint16_t us)
{
    m_tim->Instance->CNT = 0;
    while (m_tim->Instance->CNT < us)
        ;
}

void DS18B20::setInput()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = m_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    return HAL_GPIO_Init(m_port, &GPIO_InitStruct);
}

void DS18B20::setOutput()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = m_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    return HAL_GPIO_Init(m_port, &GPIO_InitStruct);
}

void DS18B20::setPin(bool on)
{
    return HAL_GPIO_WritePin(m_port, m_pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void DS18B20::togglePin()
{
    return HAL_GPIO_TogglePin(m_port, m_pin);
}

GPIO_PinState DS18B20::readPin()
{
    return HAL_GPIO_ReadPin(m_port, m_pin);
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

void DS18B20::startSensor()
{
    setOutput();
    setPin(false);

    delayMicro(480);
    setInput();
    delayMicro(80);
    readData();
    delayMicro(400);
}

float DS18B20::readTemp()
{
    startSensor();
    HAL_Delay(1);
    writeData(0xCC);
    writeData(0x44);
    HAL_Delay(800);
    startSensor();
    writeData(0xCC);
    writeData(0xBE);

    uint8_t temp1 = readData();
    uint8_t temp2 = readData();

    uint16_t temp = (temp2 << 8) | temp1;

    return (float)(temp / 16.0);
}