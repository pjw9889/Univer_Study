#include <pcap.h>
#include <unistd.h>
#include "temp_header"
#include <stdio.h>
#include <stdlib.h>

void pcap_fatal(const char *, const char *); // error message print function
void decode_ethernet(const u_char *); // ethernet layer information print function
u_int decode_ip(const u_char *); // ip layer information print function
u_int decode_tcp(const u_char *); // transport layer tcp information function
u_int decode_udp(const u_char *); // transport layer udp information function

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *); // pakcet capture function

int main(int argc, char* argv[]) 
{
	struct pcap_pkthdr cap_header; // pcap.h pcap_pkthdr struct
	const u_char *packet, *pkt_data;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *device;
	
	bpf_u_int32 netp;	//typedef bpf_u_int32
	bpf_u_int32 maskp;

	pcap_t *pcap_handle;
	struct bpf_program fp;
	
	int return_check; // return value check
	
	
	device = pcap_lookupdev(errbuf); // network device print
	if(device == NULL) // network device not exist
		pcap_fatal("pcap_lookupdev", errbuf);

	printf("Sniffing on device %s\n", device); 
	
	return_check = pcap_lookupnet(device, &netp, &maskp, errbuf); // network address, subnetmask for device
	if(return_check == -1) // pcap_lookupnet error
	{
		printf("Error: %s\n", errbuf);
		exit(1);
	}
	
	pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf); // open device, packet size, 1 = all network packet
	if(pcap_handle == NULL) // pcap_open_live error
		pcap_fatal("pcap_open_live", errbuf);
		
	return_check = pcap_compile(pcap_handle, &fp, argv[2], 0, maskp); // argv[2] = filter_rule, netmask = subnet
	if(return_check == -1) // pcap_compile error
	{
		printf("pcap_compile() Error.\n");
		exit(1);
	}
	
	//Make packet capture filtering option
	return_check = pcap_setfilter(pcap_handle, &fp); // packet_filter 
	if(return_check == -1)
	{
		printf("pcap_setfilter() Error.\n");
		exit(1);
	}	
	
	pcap_loop(pcap_handle, argv[1], caught_packet, NULL); // from 0 to argv[1] value loop

	pcap_close(pcap_handle);
	printf("\n");
}

void caught_packet(u_char *user_args/*Useless*/, const struct pcap_pkthdr *cap_header, const u_char *packet) 
{
	int ip_type, tcp_header_length, udp_header_length, total_header_size, pkt_data_len;
	u_char *pkt_data; // unsigned char
	
	printf("\n");
	printf("==== Got a %d byte packet ====\n", cap_header->len);
	printf("\n");

	decode_ethernet(packet); // datalink layer inforamtion decode
	ip_type = decode_ip(packet+ETHER_HDR_LEN); // network layer information decode
	
	if(ip_type == 17) // ip_type = udp
	{
		udp_header_length = decode_udp((packet+ETHER_HDR_LEN+sizeof(struct ip_hdr)));	
		total_header_size = ETHER_HDR_LEN+sizeof(struct ip_hdr)+udp_header_length;
		pkt_data = (u_char *)packet + total_header_size;  
		pkt_data_len = (cap_header->len - total_header_size);
	}
	
	else // ip_type = tcp
	{
		tcp_header_length = decode_tcp(packet+ETHER_HDR_LEN+sizeof(struct ip_hdr));
		total_header_size = ETHER_HDR_LEN+sizeof(struct ip_hdr)+tcp_header_length;
		pkt_data = (u_char *)packet + total_header_size;  // pkt_data points to the data portion
		pkt_data_len = (cap_header->len - total_header_size);
			
	}
	
	
	if(pkt_data_len > 0)
	{
		printf("\t\t\t%u bytes of packet data\n", pkt_data_len);
		dump(pkt_data, pkt_data_len);
	}
	
	else
		printf("\t\t\tNo Packet Data\n");
	
}

void pcap_fatal(const char *failed_in, const char *errbuf) 
{
	printf("Fatal Error in %s: %s\n", failed_in, errbuf);
	exit(1);
}

void decode_ethernet(const u_char *header_start) // ethernet layer information print function 
{
	int i;
	const struct ether_hdr *ethernet_header; // ethernet_header struct

	ethernet_header = (const struct ether_hdr *)header_start; 
	printf("[[  Layer 2 :: Ethernet Header  ]]\n");
	printf("[ Source: %02x", ethernet_header->ether_src_addr[0]); // source mac address print
	for(i=1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ethernet_header->ether_src_addr[i]);

	printf("\tDest: %02x", ethernet_header->ether_dest_addr[0]); // destination mac address print
	for(i=1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ethernet_header->ether_dest_addr[i]);
	printf("\tType: %hu ]\n", ethernet_header->ether_type); // ethernet type print
}

u_int decode_ip(const u_char *header_start) // ip layer information print function
{
	const struct ip_hdr *ip_header; // ip_header struct

	ip_header = (const struct ip_hdr *)header_start;
	printf("\t((  Layer 3 ::: IP Header  ))\n");
	printf("\t( Source: %s\t", inet_ntoa(ip_header->ip_src_addr)); // source ip address print
	printf("Dest: %s )\n", inet_ntoa(ip_header->ip_dest_addr)); // Destination ip address print
	printf("\t( Type: %u\t", (u_int) ip_header->ip_type); // ip type print
	printf("ID: %hu\tLength: %hu )\n", ntohs(ip_header->ip_id), ntohs(ip_header->ip_len)); // ip_header lenght print
	
	return ((u_int) ip_header->ip_type);
}


u_int decode_tcp(const u_char *header_start) // transport layer tcp information function
{
	u_int header_size; // unsigned int type
	const struct tcp_hdr *tcp_header; // tcp_header struct

	tcp_header = (const struct tcp_hdr *)header_start;
	header_size = 4 * tcp_header->tcp_offset; // header size calculration
	
	printf("\t\t{{  Layer 4 :::: TCP Header  }}\n");
	printf("\t\t{ Src Port: %hu\t", ntohs(tcp_header->tcp_src_port)); // source port print
	printf("Dest Port: %hu }\n", ntohs(tcp_header->tcp_dest_port)); // destination port print
	printf("\t\t{ Seq #: %u\t", ntohl(tcp_header->tcp_seq)); // seq number print
	printf("Ack #: %u }\n", ntohl(tcp_header->tcp_ack)); // ack number print
	printf("\t\t{ Header Size: %u\tFlags: ", header_size); // header size, flags print
	
	// flags mode case
	if(tcp_header->tcp_flags & TCP_FIN)
		printf("FIN ");
		
	if(tcp_header->tcp_flags & TCP_SYN)
		printf("SYN ");
		
	if(tcp_header->tcp_flags & TCP_RST)
		printf("RST ");
		
	if(tcp_header->tcp_flags & TCP_PUSH)
		printf("PUSH ");
		
	if(tcp_header->tcp_flags & TCP_ACK)
		printf("ACK ");
		
	if(tcp_header->tcp_flags & TCP_URG)
		printf("URG ");
		
	printf(" }\n");

	return header_size;
}
	
u_int decode_udp(const u_char *header_start) // transport layer udp information function
{
	u_int header_size; // unsigned int type
	const struct tcp_hdr *tcp_header; // tcp header reference

	tcp_header = (const struct tcp_hdr *)header_start; // tcp header type information
	header_size = 8;
	
	printf("\t\t{{  Layer 4 :::: UDP Header  }}\n");
	printf("\t\t{ Src Port: %hu\t", ntohs(tcp_header->tcp_src_port)); // source port print
	printf("Dest Port: %hu }\n", ntohs(tcp_header->tcp_dest_port)); // destination port print
	printf("\t\t{ Header Size: %u\t", header_size); // header size print
		
	printf(" }\n");

	return header_size;	
}


