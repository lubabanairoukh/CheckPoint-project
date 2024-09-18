#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h> // Core Netfilter definitions
#include <linux/netfilter_ipv4.h>
#include <linux/types.h>
#include <linux/inet.h>
#include <linux/init.h>
#include <linux/ip.h>  // for IPv4 header
#include <linux/tcp.h> // for TCP header

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Netfilter Kernel Module Example");

// CIDR block structure
struct cidr_block {
    char* ip_str;
    int prefix;
    __be32 first_addr;
    __be32 last_addr;
};

// Define CIDR blocks to check against
static struct cidr_block cidr_blocks[] = {
    {"127.0.0.0" , 8, 0, 0},
    {"12.1.2.7" , 16, 0, 0}
};

static int cidr_blocks_size = sizeof(cidr_blocks) / sizeof(struct cidr_block);


static void get_ip_octets(__be32 ip, unsigned char *octets) {
    octets[0] = (ip >> 24) & 0xFF; // A (most significant octet)
    octets[1] = (ip >> 16) & 0xFF; // B
    octets[2] = (ip >> 8) & 0xFF;  // C
    octets[3] = ip & 0xFF;         // D (least significant octet)
}

// Function to calculate IP range (first and last addresses) based on CIDR
static void calculate_ip_range(struct cidr_block *block) {
    __be32 addr = in_aton(block->ip_str);  // Convert IP string to binary form
    __be32 netmask = htonl(~0 << (32 - block->prefix));  // Calculate netmask
    block->first_addr = addr & netmask;  // Calculate first IP (network address)
    block->last_addr = block->first_addr | ~netmask;  // Calculate last IP (broadcast address)
}

#include <linux/icmp.h> // For ICMP header

static unsigned int packet_hook(void *priv,
                                struct sk_buff *skb,
                                const struct nf_hook_state *state)
{
   
    struct iphdr *ip_header = ip_hdr(skb);
    
    // Check if it's a TCP or ICMP packet
    if (ip_header->protocol != IPPROTO_TCP && ip_header->protocol != IPPROTO_ICMP)
    {
        return NF_ACCEPT; // Only process TCP and ICMP packets
    }

    __be32 src_ip = ip_header->saddr;  // Get the source IP in network byte order
    
    unsigned char src_octets[4];
    get_ip_octets(src_ip, src_octets);  // Split source IP into four octets

    // Iterate through CIDR blocks to check if the source IP falls within any range
    for (int i = 0; i < cidr_blocks_size; i++) {
        unsigned char first_octets[4], last_octets[4];
        get_ip_octets(cidr_blocks[i].first_addr, first_octets);  // Split first_addr
        get_ip_octets(cidr_blocks[i].last_addr, last_octets);    // Split last_addr
        if (src_octets[0] < first_octets[0] || src_octets[0] > last_octets[0]) {
            continue;
        } else if (src_octets[1] < first_octets[1] || src_octets[1] > last_octets[1]) {
            continue;
        } else if (src_octets[2] < first_octets[2] || src_octets[2] > last_octets[2]) {
            continue;
        } else if (src_octets[3] >= first_octets[3] && src_octets[3] <= last_octets[3]) {
            printk(KERN_INFO "Blocked IP: %pI4\n", &src_ip);
            return NF_DROP;  // Block the packet
        }
    }

    return NF_ACCEPT; // Accept the packet if not in any CIDR block
}

static struct nf_hook_ops nfho_pre = {
    .hook = packet_hook,            // Hook function
    .pf = PF_INET,                  // Protocol family (IPv4)
    .hooknum = NF_INET_PRE_ROUTING, // Hook point (Pre-routing)
    .priority = NF_IP_PRI_FIRST,    // Priority
};



static int __init my_netfilter_init(void)
{
    // Calculate first and last addresses for each CIDR block
    for (int i = 0; i < cidr_blocks_size; i++) {
        calculate_ip_range(&cidr_blocks[i]);

        // Debugging: Print calculated IP range
        printk(KERN_INFO "CIDR Block: %s/%d, First IP: %pI4, Last IP: %pI4\n", 
               cidr_blocks[i].ip_str, cidr_blocks[i].prefix, 
               &cidr_blocks[i].first_addr, &cidr_blocks[i].last_addr);
    }
    
    // Register the Netfilter hook
    nf_register_net_hook(&init_net, &nfho_pre);
   
    printk(KERN_INFO "Netfilter Module: Loaded and hook registered.\n");
    return 0;
}

static void __exit my_netfilter_exit(void)
{
    // Unregister the Netfilter hook
    nf_unregister_net_hook(&init_net, &nfho_pre);
    printk(KERN_INFO "Netfilter Module: Unloaded and hook unregistered.\n");
}

module_init(my_netfilter_init);
module_exit(my_netfilter_exit);
