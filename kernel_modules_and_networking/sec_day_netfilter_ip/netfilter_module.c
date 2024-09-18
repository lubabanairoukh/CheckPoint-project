#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h> // Core Netfilter definitions
#include <linux/netfilter_ipv4.h>
#include <linux/init.h>
#include <linux/ip.h>     // for IPv4 header
#include <linux/tcp.h>    // for TCP header
#include <linux/udp.h>    // for UDP header
// IPv4 specific Netfilter hooks
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Netfilter Kernel Module Example");
// Hook function definition

static unsigned int packet_hook(void *priv,
                                struct sk_buff *skb,
                                const struct nf_hook_state *state)
{
    
    struct iphdr *ip_header = ip_hdr(skb);
    
    if(ip_header)
    {
        unsigned int source = ip_header->saddr;
        unsigned int dest = ip_header->daddr;
        unsigned short check = ip_header->check;
        unsigned short frag_off = ip_header->frag_off;
        unsigned char ttl = ip_header->ttl;
        unsigned char protocol = ip_header->protocol;
        unsigned short tos = ip_header->tos;
        unsigned short tot_len = ip_header->tot_len;
        unsigned short id = ip_header->id;
        unsigned char ihl = ip_header->ihl;
        unsigned char version = ip_header->version;
        printk(KERN_INFO "/////////IP Header\n");
        printk(KERN_INFO "Source IP: %pI4\n", &source);
        printk(KERN_INFO "Destination IP: %pI4\n", &dest);
        printk(KERN_INFO "Check: %d\n", check);
        printk(KERN_INFO "Frag_off: %d\n", frag_off);
        printk(KERN_INFO "TTL: %d\n", ttl);
        printk(KERN_INFO "Protocol: %d\n", protocol);
        printk(KERN_INFO "TOS: %d\n", tos);
        printk(KERN_INFO "Total Length: %d\n", tot_len);
        printk(KERN_INFO "ID: %d\n", id);
        printk(KERN_INFO "IHL: %d\n", ihl);
        printk(KERN_INFO "Version: %d\n", version);
        
      
    }
    
    printk(KERN_INFO "our netfilter is very complicated\n");
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