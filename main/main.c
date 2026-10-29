/*
 * Experimental Color Kinetics Chromasic ESP32-S3 transmitter.
 * NOT hardware-verified. Do not enable output until cable pinout and
 * single-ended/differential interface have been established.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt_encoder.h"
#include "driver/rmt_tx.h"
#include "chromasic_protocol.h"

#define CK_OUTPUT_ENABLED 0   /* Must remain off until electrical verification */
#define CK_DATA_GPIO GPIO_NUM_18
#define CK_RMT_RESOLUTION_HZ 10000000U
#define CK_RMT_DMA_SYMBOLS 1024U
#define CK_ZERO_HIGH_TICKS 6U
#define CK_ZERO_LOW_TICKS 16U
#define CK_ONE_HIGH_TICKS 16U
#define CK_ONE_LOW_TICKS 6U

static const char *TAG = "chromasic";
/* App task stores values; encoder ISR reads atomically. */
static uint16_t s_brightness[CK_NODE_COUNT];
typedef struct {
    size_t frame_bit_index;
    uint16_t snapshot[CK_NODE_COUNT];
} ck_rmt_state_t;
static ck_rmt_state_t s_rmt_state;
#if CK_OUTPUT_ENABLED
static const uint8_t s_dummy_payload = 0;
#endif

static void ck_set_node(size_t index, uint16_t level)
{
    if (index >= CK_NODE_COUNT) return;
    if (level > CK_MAX_BRIGHTNESS) level = CK_MAX_BRIGHTNESS;
    __atomic_store_n(&s_brightness[index], level, __ATOMIC_RELAXED);
}
static void ck_fill(uint16_t level)
{
    for (size_t i = 0; i < CK_NODE_COUNT; ++i) ck_set_node(i, level);
}

/* ISR callback, bounded per invocation; must not log, allocate or block.
   Continuous streaming and underrun behavior remain to be scope-verified. */
static size_t ck_rmt_encode(const void *data, size_t data_size,
                            size_t symbols_written, size_t symbols_free,
                            rmt_symbol_word_t *symbols, bool *done, void *arg)
{
    (void)data;
    (void)data_size;
    (void)symbols_written;
    ck_rmt_state_t *state = (ck_rmt_state_t *)arg;
    for (size_t i = 0; i < symbols_free; ++i) {
        if (state->frame_bit_index == 0U) {
            for (size_t n = 0; n < CK_NODE_COUNT; ++n) {
                state->snapshot[n] = __atomic_load_n(&s_brightness[n],
                                                     __ATOMIC_RELAXED);
            }
        }
        const uint8_t bit = ck_frame_bit(state->snapshot,
                                         state->frame_bit_index);
        if (bit) {
            symbols[i] = (rmt_symbol_word_t){
                .level0 = 1, .duration0 = CK_ONE_HIGH_TICKS,
                .level1 = 0, .duration1 = CK_ONE_LOW_TICKS,
            };
        } else {
            symbols[i] = (rmt_symbol_word_t){
                .level0 = 1, .duration0 = CK_ZERO_HIGH_TICKS,
                .level1 = 0, .duration1 = CK_ZERO_LOW_TICKS,
            };
        }
        if (++state->frame_bit_index == CK_FRAME_BIT_COUNT) {
            state->frame_bit_index = 0;
        }
    }
    *done = false;
    return symbols_free;
}

#if CK_OUTPUT_ENABLED
static void ck_start_stream(void)
{
    rmt_channel_handle_t tx = NULL;
    rmt_encoder_handle_t encoder = NULL;
    const rmt_tx_channel_config_t cfg = {
        .gpio_num = CK_DATA_GPIO, .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = CK_RMT_RESOLUTION_HZ,
        .mem_block_symbols = CK_RMT_DMA_SYMBOLS, .trans_queue_depth = 1,
        .flags.with_dma = true,
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&cfg, &tx));
    const rmt_simple_encoder_config_t ecfg = {
        .callback = ck_rmt_encode, .arg = &s_rmt_state, .min_chunk_size = 1,
    };
    ESP_ERROR_CHECK(rmt_new_simple_encoder(&ecfg, &encoder));
    ESP_ERROR_CHECK(rmt_enable(tx));
    const rmt_transmit_config_t tcfg = {.loop_count = 0};
    ESP_ERROR_CHECK(rmt_transmit(tx, encoder, &s_dummy_payload,
                                 sizeof(s_dummy_payload), &tcfg));
    ESP_LOGI(TAG, "Chromasic stream on GPIO %d", (int)CK_DATA_GPIO);
}
#endif

void app_main(void)
{
    const double frame_ms = 1000.0 * CK_FRAME_BIT_COUNT * 22.0 / 10000000.0;
    ESP_LOGI(TAG, "eW Flex Compact HE, %u nodes, 24V rated", CK_NODE_COUNT);
    ESP_LOGI(TAG, "Chromasic: %u bits/frame, %.2f ms/frame",
             (unsigned)CK_FRAME_BIT_COUNT, frame_ms);
#if !CK_OUTPUT_ENABLED
    ESP_LOGW(TAG, "SAFE MODE: GPIO output disabled.");
    ESP_LOGW(TAG, "Verify connector voltage, pinout and line driver first.");
    return;
#else
    ck_fill(0);
    ck_start_stream();
    vTaskDelay(pdMS_TO_TICKS(1000));
    const uint16_t demo_level = 700; /* ~17% */
    while (true) {
        for (size_t n = 0; n < CK_NODE_COUNT; ++n) {
            ck_fill(0);
            ck_set_node(n, demo_level);
            ESP_LOGI(TAG, "Node %u / %u", (unsigned)(n+1), CK_NODE_COUNT);
            vTaskDelay(pdMS_TO_TICKS(200));
        }
        ck_fill(0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif
}
