#include <stdio.h>
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

    struct in_addr a;
    a.s_addr = htonl(ts.public_address);
    printf("udp mapped: public addr=%s, public sport=%d\n", inet_ntoa(a), ts.mapped_port);

    struct icmp_unreach icmpun;
    while (1) {
        int n = traversal_read(&ts, &icmpun, 1000);
        if (n < 0) {
            printf("%d\n", n);
            continue;
        }
        printf("%.*s\n", (int)icmpun.data_len, icmpun.data);
        deinit_icmp_unreach(&icmpun);

        break;
    }
    deinit_traversal_session(&ts);

    return 0;
}
