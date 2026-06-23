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

## Verified Environments
- Linux netfilter NAT (Ubuntu, Kernel 7.0.0-15-generic)
- TELUS Mobility LTE/5G
- TELUS Wi-Fi Hub (Firmware v3.26.01 build11)
