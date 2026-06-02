#include <esp_lcd_io_i2c.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <driver/i2c_master.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "font8x8_basic.h"

static i2c_master_bus_handle_t barramento;
static esp_lcd_panel_io_handle_t io;
static esp_lcd_panel_handle_t painel;

//===========================================================
void init_i2c(void) {
    i2c_master_bus_config_t bus_config = {
        .i2c_port             = I2C_NUM_0,
        .sda_io_num           = GPIO_NUM_21,
        .scl_io_num           = GPIO_NUM_22,
        .clk_source           = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt    = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &barramento));
}

//===========================================================
void init_painel(void) {
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr            = 0x3C,
        .scl_speed_hz        = 100000,   // reduzido para 100kHz — mais estável
        .control_phase_bytes = 1,
        .dc_bit_offset       = 6,
        .lcd_cmd_bits        = 8,
        .lcd_param_bits      = 8,
    };

    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
    };

/*Abaixo está a verificação de erro no código, funcionando da seguinte maneira:
O "ESP_ERROR_CHECK" é uma MARO do prórpio ESP-IDF que avalia o retorno de cada função.
Se a função retornar "ESP_OK" o programa segue normalmente.
Mas se retornar qualquer erro, o ESP-IDF interrompe a execução e imprime uma mensagem
de erro no terminal e reinicia o CHIP, ou seja, ele reseta. 

OBS.: Fiz a mesma coisa na função do "init_i2c"*/

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(barramento, &io_config, &io));
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io, &panel_config, &painel));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(painel));
    vTaskDelay(pdMS_TO_TICKS(200));
    ESP_ERROR_CHECK(esp_lcd_panel_init(painel));
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(painel, true));
}

//=========================================================
esp_err_t oled_init(void) {
    init_i2c();
    init_painel();
    return ESP_OK;
}

//=========================================================
void oled_limpar(void) {
    uint8_t buffer[1024] = {0};
    esp_lcd_panel_draw_bitmap(painel, 0, 0, 128, 64, buffer);
}

//=========================================================
void oled_escrever_texto(int x, int y, const char *texto) {
    while (*texto) {
        char c = *texto;
        texto++;
        char *bitmap = font8x8_basic[(int)c];

        for (int linha = 0; linha < 8; linha++) {
            uint8_t byte_linha = bitmap[linha];

            for (int coluna = 0; coluna < 8; coluna++) {
                uint8_t pixel = (byte_linha >> coluna) & 1 ? 0xFF : 0x00;
                esp_lcd_panel_draw_bitmap(painel,
                    x + coluna, y + linha,
                    x + coluna + 1, y + linha + 1,
                    &pixel);
            }
        }
        x += 8;
    }
}

//=========================================================
void oled_atualizar(void) {
    // draw_bitmap já envia direto ao display
}