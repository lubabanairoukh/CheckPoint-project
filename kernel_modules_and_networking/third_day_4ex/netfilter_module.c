#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h> // Core Netfilter definitions
#include <linux/netfilter_ipv4.h>
#include <linux/types.h>
#include <linux/inet.h>
#include <linux/init.h>
#include <linux/ip.h>  // for IPv4 header
#include <linux/tcp.h> // for TCP header
#include <linux/udp.h> // for UDP header
// IPv4 specific Netfilter hooks
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Netfilter Kernel Module Example");
// Hook function definition


struct cidr_block{
    char* ip_str;
    int prefix;
    __be32 first_addr;
    __be32 last_addr;
};
static struct cidr_block cidr_blocks[] = {
    {"1.10.16.0" ,20,0,0},
    {"1.19.0.0" , 16,0,0}
};
static int cidr_blocks_size = sizeof(cidr_blocks)/sizeof(struct cidr_block);
static unsigned int packet_hook(void *priv,
                                struct sk_buff *skb,
                                const struct nf_hook_state *state)
{

    struct iphdr *ip_header = ip_hdr(skb);
    if(ip_header->protocol != IPPROTO_TCP)
    {
        return NF_ACCEPT;
    }
    __be32 src_ip = ip_header->saddr;

    int i = 0;
    for (i=0;i<cidr_blocks_size; i++)
    {
        if (src_ip >= htonl(cidr_blocks[i].first_addr && src_ip <= cidr_blocks[i].last_addr)
        {   
            printk(KERN_INFO "Range: %u - %u\n", cidr_blocks[i].first_addr, cidr_blocks[i].last_addr);
            printk(KERN_INFO "Blocked IP: %u\n", src_ip);
            return NF_DROP;
        }
    }
  
    printk(KERN_INFO "Netfilter Module: Packet intercepted.\n");
    return NF_ACCEPT; // Accept the packet to continue its path
}

// Define the nf_hook_ops structure
static struct nf_hook_ops nfho = {
    .hook = packet_hook,            // Hook function
    .pf = PF_INET,                  // Protocol family (IPv4)
    .hooknum = NF_INET_PRE_ROUTING, // Hook point (Pre-routing)
    .priority = NF_IP_PRI_FIRST,    // Priority
};
static int __init my_netfilter_init(void)
{
    int i=0;
    for (i=0;i<cidr_blocks_size; i++)
    {
        __be32 addr = in_aton(cidr_blocks[i].ip_str);
        __be32 net_mask = htonl(~((1 << (32 - cidr_blocks[i].prefix)) - 1));
        
        cidr_blocks[i].first_addr = (addr & net_mask);
        cidr_blocks[i].last_addr = (cidr_blocks[i].first_addr | (~net_mask));
        printk(KERN_INFO "Last Address: %pI4\n", &cidr_blocks[i].last_addr);
        printk(KERN_INFO "First Address: %pI4\n", &cidr_blocks[i].first_addr);
    }
    // Register the Netfilter hook
    nf_register_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Netfilter Module: Loaded and hook registered.\n");
    return 0;
}

static void __exit my_netfilter_exit(void)
{
    // Unregister the Netfilter hook
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Netfilter Module: Unloaded and hook unregistered.\n");
}
module_init(my_netfilter_init);
module_exit(my_netfilter_exit);