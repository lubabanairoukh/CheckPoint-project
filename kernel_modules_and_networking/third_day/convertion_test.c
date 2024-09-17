#include <stdio.h>
#include <arpa/inet.h> // Header where htonl() and ntohl() are defined

int main() {
    uint32_t host_order_number = 0xAABBCCDD;
    uint32_t network_order_number;

    // Convert from host order to network order
    network_order_number = htonl(host_order_number);
    printf("Host Order: 0x%X, Network Order: 0x%X\n", host_order_number, network_order_number);

    // Convert back from network order to host order
    host_order_number = ntohl(network_order_number);
    printf("Network Order: 0x%X, Host Order: 0x%X\n", network_order_number, host_order_number);

    return 0;
}
