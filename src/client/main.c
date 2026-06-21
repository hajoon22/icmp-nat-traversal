#include <stdint.h>
#include <arpa/inet.h>

#include "../icmp/icmp.h"
#include "../traversal/traversal.h"

int main() {
    uint32_t stun_address = ntohl(inet_addr("74.125.250.129"));
    uint16_t stun_port = 19302;

    struct traversal_session ts;
    if (new_traversal_session(&ts, stun_address, stun_port) < 0) {
        return -1;
    }

    uint32_t dst = ntohl(inet_addr("255.255.255.255")); // target's public ip
    uint16_t dst_port = 33836; // target's mapped public port

    traversal_send(&ts, dst, dst_port, "helloicmp", 9);
    deinit_traversal_session(&ts);
}
