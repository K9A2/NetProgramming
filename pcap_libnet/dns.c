#include <stdio.h>
#include <libnet.h>
#include <pcap.h>

#include "Utils.c"

typedef int bool;
#define true  1
#define false 0

#define DEFAULT_SNAPLEN 192
#define DEFAULT_TIMEOUT_MS 1000
#define DEFAULT_QUERY_STRING_LENGTH 128

void usage(char *prog)
{
    fprintf(stderr, "Usage: %s -d dst_ip -q query_host [-s src_ip] [-t]\n", prog);
    exit(1);
}

void capturePackets(){
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

    //Close and return
    pcap_close(pd);
    pcap_dump_close(out_pcap);

    return;

}

void getQueryString(char *queryString) {

    char link[] = "www.baidu.com";

    char payload[DEFAULT_QUERY_STRING_LENGTH];

    char *former = link;
    char *latter = link;
    char *result = payload;

    int count = 0;

    while (true) {
        if (*former == '\0') {
            *result++ = (char) (count + '0');
            while (*latter != *former) {
                *result++ = *latter++;
            }
            *result = '0';
            break;
        } else if (*former == '.') {
            *result++ = (char) (count + '0');
            count = 0;
            while (*latter != *former) {
                *result++ = *latter++;
            }
            former++;
            latter++;
            continue;
        }
        count++;
        former++;
    }

    char *p1 = queryString;
    char *p2 = payload;

    while (*p2 != '\0') {
        *p1++ = *p2++;
    }

}

int main(int argc, char *argv[])
{
    char c;
    u_long src_ip = 0, dst_ip = 0;
    u_short type = LIBNET_UDP_DNSV4_H;
    libnet_t *l;

    libnet_ptag_t ip;
    libnet_ptag_t ptag4; /* TCP or UDP ptag */
    libnet_ptag_t dns;

    char errbuf[LIBNET_ERRBUF_SIZE];
    char *query = NULL;
    char payload[1024];
    u_short payload_s;

    printf("libnet 1.1 packet shaping: DNSv4[raw]\n");

    /*
     *  Initialize the library.  Root priviledges are required.
     */
    l = libnet_init(
            LIBNET_RAW4, /* injection type */
            NULL,        /* network interface */
            errbuf);     /* error buffer */

    if (!l)
    {
        fprintf(stderr, "libnet_init: %s", errbuf);
        exit(EXIT_FAILURE);
    }

    /*
     * parse options
     */
    while ((c = getopt(argc, argv, "d:s:q:t")) != EOF)
    {
        switch (c)
        {

            case 'd':
                if ((dst_ip = libnet_name2addr4(l, optarg, LIBNET_RESOLVE)) == -1)
                {
                    fprintf(stderr, "Bad destination IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                if ((src_ip = libnet_name2addr4(l, optarg, LIBNET_RESOLVE)) == -1)
                {
                    fprintf(stderr, "Bad source IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'q':
                query = optarg;
                break;
            case 't':
                type = LIBNET_TCP_DNSV4_H;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (!src_ip)
    {
        src_ip = libnet_get_ipaddr4(l);
    }

    if (!dst_ip || !query)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * build dns payload
     */
    getQueryString(payload);
    printf("%s", payload);
    printf("%s", query);
    payload_s = (u_short) snprintf(payload, sizeof(payload), "%c%s%c%c%c%c%c", (char)(strlen(query)&0xff), query, 0x00, 0x00, 0x01, 0x00, 0x01);

    /*
     * build packet
     */
    dns = libnet_build_dnsv4(
            type,   /* TCP or UDP */
            0x7777, /* id */
            0x0100, /* request */
            1,      /* num_q */
            0,      /* num_anws_rr */
            0,      /* num_auth_rr */
            0,      /* num_addi_rr */
            payload,
            payload_s,
            l,
            0);

    if (dns == -1)
    {
        fprintf(stderr, "Can't build  DNS packet: %s\n", libnet_geterror(l));
        goto bad;
    }

    if (type == LIBNET_TCP_DNSV4_H) /* TCP DNS */
    {
        ptag4 = libnet_build_tcp(
                0x6666,                                        /* source port */
                53,                                            /* destination port */
                0x01010101,                                    /* sequence number */
                0x02020202,                                    /* acknowledgement num */
                TH_PUSH | TH_ACK,                              /* control flags */
                32767,                                         /* window size */
                0,                                             /* checksum */
                0,                                             /* urgent pointer */
                LIBNET_TCP_H + LIBNET_TCP_DNSV4_H + payload_s, /* TCP packet size */
                NULL,                                          /* payload */
                0,                                             /* payload size */
                l,                                             /* libnet handle */
                0);                                            /* libnet id */

        if (ptag4 == -1)
        {
            fprintf(stderr, "Can't build UDP header: %s\n", libnet_geterror(l));
            goto bad;
        }

        ip = libnet_build_ipv4(
                LIBNET_IPV4_H + LIBNET_TCP_H + type + payload_s, /* length */
                0,                                               /* TOS */
                242,                                             /* IP ID */
                0,                                               /* IP Frag */
                64,                                              /* TTL */
                IPPROTO_TCP,                                     /* protocol */
                0,                                               /* checksum */
                src_ip,                                          /* source IP */
                dst_ip,                                          /* destination IP */
                NULL,                                            /* payload */
                0,                                               /* payload size */
                l,                                               /* libnet handle */
                0);                                              /* libnet id */

        if (ip == -1)
        {
            fprintf(stderr, "Can't build IP header: %s\n", libnet_geterror(l));
            exit(EXIT_FAILURE);
        }
    }
    else /* UDP DNS */
    {
        ptag4 = libnet_build_udp(
                0x6666,                                        /* source port */
                53,                                            /* destination port */
                LIBNET_UDP_H + LIBNET_UDP_DNSV4_H + payload_s, /* packet length */
                0,                                             /* checksum */
                NULL,                                          /* payload */
                0,                                             /* payload size */
                l,                                             /* libnet handle */
                0);                                            /* libnet id */

        if (ptag4 == -1)
        {
            fprintf(stderr, "Can't build UDP header: %s\n", libnet_geterror(l));
            goto bad;
        }

        ip = libnet_build_ipv4(
                LIBNET_IPV4_H + LIBNET_UDP_H + type + payload_s, /* length */
                0,                                               /* TOS */
                242,                                             /* IP ID */
                0,                                               /* IP Frag */
                64,                                              /* TTL */
                IPPROTO_UDP,                                     /* protocol */
                0,                                               /* checksum */
                src_ip,                                          /* source IP */
                dst_ip,                                          /* destination IP */
                NULL,                                            /* payload */
                0,                                               /* payload size */
                l,                                               /* libnet handle */
                0);                                              /* libnet id */

        if (ip == -1)
        {
            fprintf(stderr, "Can't build IP header: %s\n", libnet_geterror(l));
            exit(EXIT_FAILURE);
        }
    }

    /*
     * write to the wire
     */
    c = libnet_write(l);
    if (c == -1)
    {
        fprintf(stderr, "Write error: %s\n", libnet_geterror(l));
        goto bad;
    }
    else
    {
        fprintf(stderr, "Wrote %d byte DNS packet; check the wire.\n", c);
    }

    //Capture replies
    capturePackets();

    libnet_destroy(l);
    return (EXIT_SUCCESS);
    bad:
    libnet_destroy(l);
    return (EXIT_FAILURE);
}
