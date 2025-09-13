#include "app.h"

void user_main()
{
    while (1) {
        HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
        HAL_Delay(1000);
    }
}
