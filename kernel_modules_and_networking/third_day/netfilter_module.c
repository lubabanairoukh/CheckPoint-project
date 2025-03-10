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

static unsigned int packet_hook(void *priv,
                                struct sk_buff *skb,
                                const struct nf_hook_state *state)
{

    struct iphdr *ip_header = ip_hdr(skb);

    if (ip_header)
    {

        char *ip = "8.8.8.8";
        __be32 block_ip = in_aton(ip);
        if (ip_header->saddr == block_ip)
        {
            __be32 addr = in_aton("1.1.1.1");
            __be32 net_mask = htonl(~((1 << (32 - 20)) - 1));
            
            __be32 first_addr = addr & net_mask;
            __be32 last_addr = first_addr | (~net_mask);
            printk(KERN_INFO "Last Address: %pI4\n", &last_addr);
            printk(KERN_INFO "First Address: %pI4\n", &first_addr);
            printk(KERN_INFO "Network Mask: %pI4\n", &net_mask);
            printk(KERN_INFO "Address: %pI4\n", &addr);

            printk(KERN_INFO "Blocked IP: %pI4\n", &ip_header->saddr);
            return NF_DROP;
        }
    }

    // printk(KERN_INFO "our netfilter is very complicated\n");
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