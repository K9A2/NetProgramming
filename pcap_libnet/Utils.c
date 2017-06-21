#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <time.h>

//Bool in C
typedef int bool;
#define true  1
#define false 0

//typedef struct EtherHdr {
//    unsigned char ether_dst[6];
//    unsigned char ether_src[6];
//    unsigned short ether_type[6];
//};

void PrintErrorAndExit(char *errorStr) {
    printf("Error: %s\n", errorStr);
    printf("Program Exited With Error.\n");
    exit(-1);
}

void printCurrentTime() {
    time_t localTime;
    localTime = time(NULL);
    printf("%s", ctime(&localTime));
}

void dumpPackets(u_char *user, const struct pcap_pkthdr *h, const u_char *pkt_data) {

    /*
    int length = h->len;
    int captureLength = h->caplen;
    int ethType;

    EtherHdr *ep;

    printf("Ethernet Header(%u.%06u)\n", (u_int32_t) h->ts.tv_sec, (u_int32_t) h->ts.tv_usec);

    if (captureLength < sizeof(struct EtherHdr)) {
        printf("Error: The captured the Ethernet packet header is too short.\n");
        return;
    }

    ep = (EtherHdr *) p;

    ethType = ntohs(ep->ether_type);

    printf("Hardware source: %s.\n", )

    printf("I get one packet!\n");
     */

//    EtherHdr *ep = (EtherHdr *) pkt_data;

    struct tm *ltime;
    char timestr[16];
    time_t local_tv_sec;

    local_tv_sec = h->ts.tv_sec;
    ltime = localtime(&local_tv_sec);
    strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

    printf("%s, %.6d, len:%d\n", timestr, (int) h->ts.tv_usec, h->len);
}

void packet_handler(u_char *user, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
    pcap_dump(user, pkt_header, pkt_data);// 输出数据到文件
    printf("Jacked a packet with length of [%d]\n", pkt_header->len);// 打印抓到的包的长度
}
