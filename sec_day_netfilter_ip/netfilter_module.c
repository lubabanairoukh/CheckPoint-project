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
    printk(KERN_INFO "Source IP: %pI4\n", &source);
    printk(KERN_INFO "Destination IP: %pI4\n", &dest);
       
    }
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