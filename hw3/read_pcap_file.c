#define __FAVOR_BSD
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#define MAC_ADDRSTRLEN 2*6+5+1
void dump(u_int32_t length, const u_char *content);
void dump_ip(u_int32_t length, const u_char *content, int ip_type);
void dump_udp(u_int32_t length, const u_char *content);
void dump_tcp(u_int32_t length, const u_char *content);
void callback(u_char *arg, const struct pcap_pkthdr *header, const u_char *content);
char *ntoa(u_char *d);
char *ip_ntoa(void *i);
int ip_num = 0;

int main(int argc, const char * argv[]) {
    char error[PCAP_ERRBUF_SIZE];
    pcap_t *p_pcap_file = NULL;


    if(strcmp(argv[1], "-r") == 0 && argc == 3)
    {
        p_pcap_file = pcap_open_offline(argv[2], error);
        if(!p_pcap_file)
        {
            printf("file didn't exist");
            exit(1);
        }

        //read file
        if(-1 == pcap_loop(p_pcap_file, -1, callback, NULL)) {
            fprintf(stderr, "pcap_loop: %s\n", pcap_geterr(p_pcap_file));
        }

        pcap_close(p_pcap_file);

        printf("IP封包數量: %d\n", ip_num);
    }
    else
    {
        printf("error command");
    }

    return 0;
}


char *ntoa(u_char *c) {
    static char string[MAC_ADDRSTRLEN];
    snprintf(string, sizeof(string), "%02x:%02x:%02x:%02x:%02x:%02x", c[0], c[1], c[2], c[3], c[4], c[5]);

    return string;
}

char *ip_ntoa(void *s) {
    static char string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, s, string, sizeof(string));

    return string;
}

void callback(u_char *arg, const struct pcap_pkthdr *header, const u_char *content) {
    static int d = 0;
    struct tm *time;
    char timestamp[24];
    time_t raw;

    raw = header->ts.tv_sec;
    time = localtime(&raw);
    strftime(timestamp, sizeof timestamp, "%Y/%b/%d - %H:%M:%S", time);

    printf("No. %d\n", ++d);
    printf("Time: %s\n", timestamp);

    dump(header->caplen, content);
    printf("\n");
}

void dump(u_int32_t length, const u_char *content) {
    struct ether_header *ethernet = (struct ether_header *)content;
    char dst_mac[MAC_ADDRSTRLEN] = {};
    char src_mac[MAC_ADDRSTRLEN] = {};
    u_int16_t type;

    strlcpy(dst_mac, ntoa(ethernet->ether_dhost), sizeof(dst_mac));
    strlcpy(src_mac, ntoa(ethernet->ether_shost), sizeof(src_mac));
    type = ntohs(ethernet->ether_type);

    printf("目的MAC位址:            %17s\n", dst_mac);
    printf("來源的MAC位址:          %17s\n", src_mac);

    switch (type) {
        case ETHERTYPE_IP:
            dump_ip(length, content, 4);
            ip_num ++;
            break;
        case ETHERTYPE_IPV6:
            dump_ip(length, content, 6);
            ip_num ++;
            break;
        default:
            break;
    }

}

void dump_ip(u_int32_t length, const u_char *content, int ip_type) {
    struct ip *ip;
    u_char protocol;

    if(ip_type == 4)
    {
        ip = (struct ip *)(content + ETHER_HDR_LEN);
        printf("Protocol: IPv4\n");
        protocol = ip->ip_p;
    }
    else
    {
        ip = (struct ip *)(content + ETHER_HDR_LEN - 3);
        printf("Protocol: IPv6\n");
        protocol = ip->ip_p;
    }
    printf("來源IP:       %15s\n",  ip_ntoa(&ip->ip_src));
    printf("目的IP:       %15s\n", ip_ntoa(&ip->ip_dst));

    switch (protocol) {
        case IPPROTO_UDP:
            dump_udp(length, content);
            break;

        case IPPROTO_TCP:
            dump_tcp(length, content);
            break;

        default:
            break;
    }
}

void dump_udp(u_int32_t length, const u_char *content) {
    struct ip *ip = (struct ip *)(content + ETHER_HDR_LEN);
    struct udphdr *udp = (struct udphdr *)(content + ETHER_HDR_LEN + (ip->ip_hl << 2));

    u_int16_t s_port = ntohs(udp->uh_sport);
    u_int16_t d_port = ntohs(udp->uh_dport);

    printf("Protocol: UDP\n");
    printf("來源Port:       %5u\n目的Port:  %5u\n", s_port, d_port);

}

void dump_tcp(u_int32_t length, const u_char *content) {
    struct ip *ip = (struct ip *)(content + ETHER_HDR_LEN);
    struct tcphdr *tcp = (struct tcphdr *)(content + ETHER_HDR_LEN + (ip->ip_hl << 2));

    u_int16_t s_port = ntohs(tcp->th_sport);
    u_int16_t d_port = ntohs(tcp->th_dport);

    printf("Protocol: TCP\n");
    printf("來源Port:       %5u\n目的Port:  %5u\n", s_port, d_port);
}
