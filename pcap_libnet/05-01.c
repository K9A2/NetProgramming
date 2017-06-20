#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

void printer(u_char *user, const struct pcap_pkthdr *h, const u_char *p)
{
    printf("I get one packet!\n");
}

#define DEFAULT_SNAPLEN 68

int main()
{
    char ebuf[PCAP_ERRBUF_SIZE];
    char *device = pcap_lookupdev(euf);
    if (device == NULL)
    {
        printf("%s:%s\n", "Cannot lookupdev", ebuf);
        exit(1);
    }
    else
    {
        printf("The device is :%s\n", device);
    }

    bpf_u_int32 localnet, netmask;
    printf("%s\n", "The net:");
    if (pcap_lookupnet(device, &localnet, &netmask, ebuf) == 0)
    {
        printf("%u.%u.%u.%u", localnet & 0xff, localnet >> 8 & 0xff,
               localnet >> 16 & 0xff, localnet >> 24 & 0xff);
        printf(":%d.%d.%d.%d ", netmask & 0xff, netmask >> 8 & 0xff,
               netmask >> 16 & 0xff, netmask >> 24 & 0xff);
    }
    else
    {
        printf("%s\n", ebuf);
    }

    pcap_t *pd = pcap_open_live(device, DEFAULT_SNAPLEN, 0, 1000, ebuf);

    if (pcap_datalink(pd) == DLT_EN10MB)
        printf("10Mb Ethernet\n");

    struct bpf_program fcode;
    pcap_compile(pd, &fcode, NULL, 1, 0);
    pcap_setfilter(pd, &fcode);

    pcap_loop(pd, 10, printer, NULL);

    struct pcap_stat stat;
    pcap_stats(pd, &stat);
    printf("recv %d, drop %d. ", stat.ps_recv, stat.ps_drop);

    pcap_close(pd);
    exit(0);
}
