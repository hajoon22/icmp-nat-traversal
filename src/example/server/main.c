#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "../../traversal/icmp/icmp.h"
#include "../../traversal/traversal.h"

int main() {
    uint32_t stun_address = ntohl(inet_addr("74.125.250.129"));
    uint16_t stun_port = 19302;

    struct traversal_session ts;
    if (new_traversal_session(&ts, stun_address, stun_port) < 0) {
        return -1;
    }
    
    struct in_addr a;
    a.s_addr = htonl(ts.public_address);
    printf("udp mapped: public addr=%s, public sport=%d\n", inet_ntoa(a), ts.mapped_port);

    while (1) {
        struct icmp_unreach *rp = traversal_read(&ts, 1000);
        if (!rp) continue;

        printf("%.*s\n", (int)rp->data_len, rp->data);
        deinit_icmp_unreach(rp);

        break;
    }
    deinit_traversal_session(&ts);

    return 0;
}
