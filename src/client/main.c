#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

#include "../icmp/icmp.h"

int main() {
    uint32_t stun_server_addr = ntohl(inet_addr("74.125.250.129"));
    uint16_t stun_server_port = 19302;

    uint32_t dst_addr = ntohl(inet_addr("")); // target's public ip
    uint16_t dst_port = ; // target's mapped public port

    int s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (s < 0) return s;

    send_icmp_unreach(s, dst_addr, dst_port, stun_server_addr, stun_server_port, "hello", 5);
}
