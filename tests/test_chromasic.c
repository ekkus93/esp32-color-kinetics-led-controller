#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "chromasic_protocol.h"

static void test_start_and_idle(void)
{
    uint16_t vals[CK_NODE_COUNT] = {0};
    assert(CK_FRAME_BIT_COUNT == 10047U);
    for (size_t i = 0; i < CK_IDLE_BIT_COUNT; ++i)
        assert(ck_frame_bit(vals, i) == 0U);
    for (size_t i = 0; i < CK_START_BIT_COUNT; ++i)
        assert(ck_frame_bit(vals, CK_IDLE_BIT_COUNT+i) == (i < 13U));
}
static void test_first_node_lsb_first(void)
{
    uint16_t vals[CK_NODE_COUNT] = {0};
    vals[0] = 0x0A55U;
    const size_t base = CK_IDLE_BIT_COUNT + CK_START_BIT_COUNT;
    assert(ck_frame_bit(vals, base) == 1U);
    for (size_t b = 0; b < 12U; ++b)
        assert(ck_frame_bit(vals, base+1U+b) == ((0x0A55U >> b) & 1U));
    for (size_t b = 13U; b < 20U; ++b)
        assert(ck_frame_bit(vals, base+b) == 0U);
    assert(ck_frame_bit(vals, base+20U) == 1U);
    assert(ck_frame_bit(vals, base+40U) == 1U);
    for (size_t b = 21U; b < CK_BITS_PER_NODE; ++b) {
        if (b == 40U) continue;
        assert(ck_frame_bit(vals, base+b) == 0U);
    }
}
static void test_last_node_and_flush(void)
{
    uint16_t vals[CK_NODE_COUNT] = {0};
    vals[CK_NODE_COUNT-1U] = CK_MAX_BRIGHTNESS;
    const size_t pos = CK_IDLE_BIT_COUNT + CK_START_BIT_COUNT +
                       (CK_NODE_COUNT-1U)*CK_BITS_PER_NODE;
    for (size_t b = 0; b < 12U; ++b)
        assert(ck_frame_bit(vals, pos+1U+b) == ((CK_MAX_BRIGHTNESS >> b) & 1U));
    for (size_t n = CK_NODE_COUNT; n < CK_NODE_COUNT+CK_FLUSH_NODE_COUNT; ++n) {
        const size_t base = CK_IDLE_BIT_COUNT+CK_START_BIT_COUNT+n*CK_BITS_PER_NODE;
        for (size_t b = 0; b < CK_BITS_PER_NODE; ++b) {
            const uint8_t expected = b == 0U || b == 20U || b == 40U;
            assert(ck_frame_bit(vals, base+b) == expected);
        }
    }
    assert(ck_frame_bit(vals, CK_FRAME_BIT_COUNT) == 0U);
}
int main(void)
{
    test_start_and_idle();
    test_first_node_lsb_first();
    test_last_node_and_flush();
    puts("PASS: idle, start, LSB-first channels, last node, six flush nodes");
    return 0;
}
