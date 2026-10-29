#pragma once
#include <stddef.h>
#include <stdint.h>
/* eW Flex Compact HE 60 fixed-white nodes. */
#define CK_NODE_COUNT 60U
#define CK_FLUSH_NODE_COUNT 6U
#define CK_IDLE_BIT_COUNT 6000U
#define CK_START_BIT_COUNT 21U
#define CK_BITS_PER_NODE 61U
#define CK_FRAME_BIT_COUNT (CK_IDLE_BIT_COUNT + CK_START_BIT_COUNT + \
                            (CK_NODE_COUNT + CK_FLUSH_NODE_COUNT) * CK_BITS_PER_NODE)
#define CK_MAX_BRIGHTNESS 4094U
/* Return one logical bit of the repeating experimental Chromasic frame. */
uint8_t ck_frame_bit(const uint16_t brightness[CK_NODE_COUNT], size_t position);
