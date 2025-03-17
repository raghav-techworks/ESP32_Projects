  
//LED Blinking
#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/ledc.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "soc/gpio_struct.h"
#include "rom/ets_sys.h"
#include "esp_err.h"
#include "esp_timer.h"


#define LED2_PIN GPIO_NUM_2
#define LED18_PIN GPIO_NUM_18

#define LED_PIN GPIO_NUM_5               // GPIO for PWM
#define LEDC_TIMER LEDC_TIMER_0 // Timer 0 for PWM
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_10_BIT // 10-bit resolution (0-1023)
#define LEDC_FREQUENCY 5000             // PWM Frequency in Hz

void Led2Blink(void)
{
    printf("LED2 Blink\n\r");
    gpio_set_level(LED2_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(LED2_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void Led18blink(void)
{
    printf("LED8 Blink\n\r");
    gpio_set_level(LED18_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(LED18_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void pwm_init(void)
{
    // Configure PWM Timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    // Configure PWM Channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER,
        .duty = 0, // Start with 0 duty cycle
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);
}

void pwm_set_duty(uint32_t duty)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void ChangeDutyCycle(void)
{
    printf("Increasing brightness\n");
    for (int duty = 0; duty <= 1023; duty += 50) // Increase brightness
    {
        pwm_set_duty(duty);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    printf("Decreasing brightness\n");
    for (int duty = 1023; duty >= 0; duty -= 50) // Decrease brightness
    {
        pwm_set_duty(duty);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_reset_pin(LED2_PIN);
    gpio_reset_pin(LED18_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED18_PIN, GPIO_MODE_OUTPUT);

    printf("Hello ESP32 Wroom32 MCU....\n");

    pwm_init();

    do
    {
        Led2Blink();
        ChangeDutyCycle();
        Led18blink();
    } while (1);
}






