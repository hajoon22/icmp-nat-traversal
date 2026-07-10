#include <stdint.h>
#include <arpa/inet.h>

#include "../../traversal/icmp/icmp.h"
#include "../../traversal/traversal.h"

int main() {
    struct traversal_session ts;
    ts.stun_address = ntohl(inet_addr("74.125.250.129"));
    ts.stun_port = 19302;

    if (new_traversal_session(&ts) < 0) {
        return -1;
    }

    uint32_t dst = ntohl(inet_addr("1.1.1.1")); // target's public ip
    uint16_t dst_port = 54995; // target's mapped public port

    traversal_send(&ts, dst, dst_port, "helloicmp", 9);
    deinit_traversal_session(&ts);
}
