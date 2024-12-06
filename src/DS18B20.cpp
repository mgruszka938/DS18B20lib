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

void DS18B20::setOutput()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    return HAL_GPIO_Init(_port, &GPIO_InitStruct);
}
