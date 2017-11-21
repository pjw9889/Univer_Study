# Packet Sniff(Ver. Debian type)



Packet을 Sniffing 하여 Data 중 일부인  2,3,4,7계층의 데이터를 출력하는 프로그램.



## PrePare



1. Kali Linux (Program development and execution)
2. Ubuntu (Target)
3. Pcap Lib



### Decode_sniff_update



develop program using pcap lib



#### Pcap lib Function Explain



- ```
  pcap_lookupdev() // Find network device and store string type
  ```


- ```
  pcap_lookupnet() // Get Network address, subnet from network device
  ```


- ```
  pcap_open_live() // Get the descriptors needed to view packets
  ```


- ```
  pcap_compile() // Store the filtering rules
  ```


- ```
  pcap_setfilter() // Apply filter
  ```


- ```
  pcap_loop() // loop from 0 to something value 
  ```



### Temp header

1. Ethernet, IP, TCP, icmp Header Make
2. Dump Function - Data Split and Convert to hexadecimal, print



