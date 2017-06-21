#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

#include "Utils.c"

#define DEFAULT_SNAPLEN 192
#define DEFAULT_TIMEOUT_MS 1000

int main() {
    char errorBuffer[PCAP_ERRBUF_SIZE];
    char *deviceName = pcap_lookupdev(errorBuffer);

    //Net IP and mask
    bpf_u_int32 localNet;
    bpf_u_int32 netMask;

    pcap_t *pd;

    char rule[] = "port 53";

    struct bpf_program bpf;
    struct pcap_stat status;
    //struct pcap_pkthdr header;

    //Print device information
    if (deviceName == NULL) {
        PrintErrorAndExit(errorBuffer);
    } else {
        printf("The device is: %s.\n", deviceName);
    }

    //Print net information
    printf("In net (IP:Mask): ");
    if (pcap_lookupnet(deviceName, &localNet, &netMask, errorBuffer) == 0) {
        printf("%u.%u.%u.%u", localNet & 0xff, localNet >> 8 & 0xff,
               localNet >> 16 & 0xff, localNet >> 24 & 0xff);
        printf(":%d.%d.%d.%d\n", netMask & 0xff, netMask >> 8 & 0xff,
               netMask >> 16 & 0xff, netMask >> 24 & 0xff);
    } else {
        PrintErrorAndExit(errorBuffer);
    }

    //Open target device, and set capture handle
    if ((pd = pcap_open_live(deviceName, DEFAULT_SNAPLEN, false, DEFAULT_TIMEOUT_MS, errorBuffer)) == NULL) {
        PrintErrorAndExit(errorBuffer);
    }

    //Print data link information
    if (pcap_datalink(pd) == DLT_EN10MB) {
        printf("The network type is: 10Mb Ethernet\n");
    }

    pcap_compile(pd, &bpf, rule, 1, netMask);
    pcap_setfilter(pd, &bpf);

    pcap_dumper_t *out_pcap;
    out_pcap = pcap_dump_open(pd, "/home/pack.pcap");

    //Capture packets
    pcap_loop(pd, 30, packet_handler, (u_char *) out_pcap);
    pcap_dump_flush(out_pcap);

    //Print link status
    pcap_stats(pd, &status);
    printf("Received Packets: %d.\nDropped Packets: %d.\n", status.ps_recv, status.ps_drop);

    //Close and exit
    pcap_close(pd);
    pcap_dump_close(out_pcap);
    exit(0);

}
