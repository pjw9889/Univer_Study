# Packet Sniff(Ver. Debian)



네트워크 패킷 데이터 중 OSI 7 layer의 2,3,4,7 계층의 데이터를 스니핑하는 프로그램



## Prepare



1. Kail Linux(attacker)
2. Ubuntu(Target)
3. Pcap lib



## Decode_Sniff_update



temp_header와 Pcap lib를 활용한 스니핑 프로그램



### temp_ Header



1. Ethernet, IP, ICMP, TCP Header 구조체 생성 
2. Dump 함수 구현(패킷 데이터를 읽어와서 Hex로 변환 후 출력)



#### Pcap lib function Explain



- pcap_lookupdev() : 네트워크 장비 탐색 및 출력
- pcap_lookupnet() : 네트워크 장비의 Network Address, Subnet을 가져온다
- pcap_open_live() :  네트워크 패킷을 보기위한 Descriptor를 얻는다
- pcap_complie() : 패킷 필터 룰을 작성한다.
- pcap_setfilter() : 필터 룰을 적용시킨다
- pcap_loop() : 입력된 값만큼 패킷 스니핑을 수행한다
- pcap_close() : 패킷을 닫고 사용한 메모리들을 해제한다.



