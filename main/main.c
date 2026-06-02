#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "oled.h"

//==============================================================================================



void app_main(void){
    oled_init();
    oled_limpar();
    oled_escrever_texto(0, 0, "Modo Manual");
    oled_escrever_texto(0, 8, "Modo Gravacao");
    oled_escrever_texto(0, 16, "Modo Reproducao");
    oled_atualizar();
    while(1){
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}
