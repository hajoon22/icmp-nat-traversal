## ICMP NAT Traversal
NAT traversal using ICMP Destination Unreachable packets.

## Overview
1. Creates and maintains a UDP mapping.
2. Sends ICMP Destination Unreachable packets referencing the mapped UDP flow.
3. Delivers data through the ICMP payload.
4. Receives data through the NAT mapping.

## Packet Format
**ICMP Destination Unreachable**

`[ICMP Header (8 bytes)][IPv4 Header (20 bytes)][UDP Header (8 bytes)][Data (n bytes)]`

## Workflow
1. Server creates a UDP mapping.
2. Server keeps the mapping alive.
3. Client crafts an ICMP Destination Unreachable packet referencing the mapped UDP flow.
4. The ICMP payload contains the original IPv4 header, UDP header, and data.
5. The ICMP packet appears to describe an error for the mapped UDP flow.
6. Server receives the data.

## How to use another project
server:
```c
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "../../traversal/icmp/icmp.h" // include icmp header for using icmp_unreach structure
#include "../../traversal/traversal.h" // include traversal header for using traversal api

int main() {
    // stun server
    uint32_t stun_address = ntohl(inet_addr("74.125.250.129"));
    uint16_t stun_port = 19302;

    // create a new traversal session
    struct traversal_session ts;
    if (new_traversal_session(&ts, stun_address, stun_port) < 0) {
        return -1;
    }

    // debug
    struct in_addr a;
    a.s_addr = htonl(ts.public_address);
    printf("udp mapped: public addr=%s, public sport=%d\n", inet_ntoa(a), ts.mapped_port);

    while (1) {
        // read icmp destination unreachable packets (timeout: 1000ms)
        struct icmp_unreach *rp = traversal_read(&ts, 1000);
        if (!rp) continue;

        printf("%.*s\n", (int)rp->data_len, rp->data);
        deinit_icmp_unreach(rp);

        break;
    }
    deinit_traversal_session(&ts);

    return 0;
}
```

client:
```c
#include <stdint.h>
#include <arpa/inet.h>

#include "../../traversal/icmp/icmp.h" // include icmp header for using icmp_unreach structure
#include "../../traversal/traversal.h" // include traversal header for using traversal api

int main() {
    // stun server
    uint32_t stun_address = ntohl(inet_addr("74.125.250.129"));
    uint16_t stun_port = 19302;

    // create a new traversal session
    struct traversal_session ts;
    if (new_traversal_session(&ts, stun_address, stun_port) < 0) {
        return -1;
    }

    // destination info
    uint32_t dst = ntohl(inet_addr("1.1.1.1")); // target's public ip
    uint16_t dst_port = 33836; // target's mapped public port

    // send data
    traversal_send(&ts, dst, dst_port, "helloicmp", 9);
    deinit_traversal_session(&ts);
}
```

## How to Test
Build and run the server on the device behind the NAT:

Build server:
```bash
make example
```

Run the server on the device behind the NAT:
```bash
sudo ./server
```

The server prints:
`udp mapped: public addr=PUBLIC_IP, public sport=MAPPED_PORT`

Edit client:
`src/client/main.c`:
```c
uint32_t dst = ntohl(inet_addr("PUBLIC_IP"));
uint16_t dst_port = MAPPED_PORT;
```

Build client:
```bash
make example
```
Run Client:
```bash
sudo ./client
```

## Test Results
Additional test results are welcome.   
Please send NAT type/device, firmware/version (if available), and whether traversal worked to <hajoon@hajoon.me>.

## Verified Environments
- Linux netfilter NAT (Ubuntu, Kernel 7.0.0-15-generic)
- TELUS Mobility LTE/5G
- TELUS Wi-Fi Hub (Firmware v3.26.01 build11)
- Vancouver International Airport Authority Public Wi-Fi
- U+ GAPD-7500R (Software Version 1.03.10)
- LGTELECOM Mobility LTE/5G
