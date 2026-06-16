#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/ip_icmp.h>

#include "../stun/stun.h"
#include "../icmp/icmp.h"

int main() {
    uint32_t addr = 0;
    uint16_t port = 0;

    int udp_s = get_stun(ntohl(inet_addr("74.125.250.129")), 19302, &addr, &port);
    if (udp_s < 0) return udp_s;
    
    struct in_addr a;
    a.s_addr = htonl(addr);
    printf("udp mapped: public addr=%s, public sport=%d\n", inet_ntoa(a), port);

    int icmp_s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmp_s < 0) return icmp_s;

	struct pollfd pfd = {
		.fd = icmp_s,
		.events = POLLIN
	};

    while (1) {
        send(udp_s, "aaaaa", 5, 0); // keepalive
        
        int n = poll(&pfd, 1, 1000);
		if (n < 0) break;
        if (n > 0) {
            struct icmp_unreach *rp = read_icmp_unreach(icmp_s);
            if (!rp) continue;
            printf("%.*s\n", (int)rp->data_len, rp->data);
            deinit_icmp_unreach(rp);
        }
    }
    
    close(udp_s);
    close(icmp_s);

    return 0;
}
