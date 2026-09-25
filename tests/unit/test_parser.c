#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t cmd_id;
    uint16_t length;
    uint8_t payload[64];
} command_pkt_t;

static bool parse_command(const uint8_t *raw, size_t len, command_pkt_t *pkt) {
    if (raw == NULL || pkt == NULL || len < 3U) {
        return false;
    }
    pkt->cmd_id = raw[0];
    pkt->length = (uint16_t)(((uint16_t)raw[1] << 8) | raw[2]);
    if (pkt->length > sizeof(pkt->payload) || len < (3U + (size_t)pkt->length)) {
        return false;
    }
    if (pkt->length > 0U) {
        (void)memcpy(pkt->payload, &raw[3], pkt->length);
    }
    return true;
}

int main(void) {
    uint8_t valid_frame[] = {0x01, 0x00, 0x04, 'T', 'E', 'S', 'T'};
    command_pkt_t pkt = {0};

    assert(parse_command(valid_frame, sizeof(valid_frame), &pkt) == true);
    assert(pkt.cmd_id == 0x01);
    assert(pkt.length == 4);
    assert(memcmp(pkt.payload, "TEST", 4) == 0);

    /* Null pointer safety check */
    assert(parse_command(NULL, sizeof(valid_frame), &pkt) == false);

    /* Buffer overflow safety check */
    uint8_t invalid_frame[] = {0x02, 0x00, 0x80}; /* length 128 > payload max 64 */
    assert(parse_command(invalid_frame, sizeof(invalid_frame), &pkt) == false);

    printf("test_parser passed.\n");
    return 0;
}
