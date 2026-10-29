#include "chromasic_protocol.h"
uint8_t ck_frame_bit(const uint16_t brightness[CK_NODE_COUNT], size_t position)
{
    position %= CK_FRAME_BIT_COUNT;
    if (position < CK_IDLE_BIT_COUNT) {
        return 0;
    }
    position -= CK_IDLE_BIT_COUNT;
    /* 21-bit start code: 13 ones followed by 8 zeros. */
    if (position < CK_START_BIT_COUNT) {
        return position < 13U ? 1U : 0U;
    }
    position -= CK_START_BIT_COUNT;

    const size_t node = position / CK_BITS_PER_NODE;
    const size_t bit_in_node = position % CK_BITS_PER_NODE;
    const size_t channel = (bit_in_node < 20U) ? 0U :
                           (bit_in_node < 40U) ? 1U : 2U;
    const size_t bit_in_channel = (channel == 0U) ? bit_in_node :
                                  (channel == 1U) ? bit_in_node - 20U :
                                                    bit_in_node - 40U;
    if (bit_in_channel == 0U) {
        return 1U;
    }
    if (bit_in_channel > 12U) {
        return 0U;
    }
    /* Channel 1 controls fixed-white eW Flex. Remaining channels are zero. */
    uint16_t value = (node < CK_NODE_COUNT && channel == 0U)
                         ? brightness[node] : 0U;
    if (value > CK_MAX_BRIGHTNESS) {
        value = CK_MAX_BRIGHTNESS;
    }
    return (uint8_t)((value >> (bit_in_channel - 1U)) & 1U);
}
