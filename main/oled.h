#ifndef OLED_H
#define OLED_H

#include <esp_lcd_io_i2c.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include "esp_err.h"
#include "driver/i2c_master.h"

esp_err_t oled_init(void);
void oled_limpar(void);
void oled_escrever_texto(int x, int y, const char *texto);
void oled_atualizar(void);


#endif