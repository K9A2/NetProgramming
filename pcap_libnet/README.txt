1. ���벢����05-01.c
�����У�gcc -lpcap -o 05-01 -Wall 05-01.c
����ʱʹ��root������С�
root���룺qrupafhl
��05-01.cԴ�����м����ж�����жϷ���ֵ�Ƿ���ȷ��

2. ���벢����dns.c
1�������У�gcc -lnet -o dns -Wall dns.c
2������ʱʹ��root������У�ע��������Ϊ���ļ��� -d dst_ip -q query_host [-s src_ip] [-t]
3��ʹ��tcpdump����dns��ѯ�����ص����ݰ���tcpdump port 53��tcpdump�������λ��/usr/sbin��������Ҫд������·���������С�

3. ������������ϲ����ϲ�����Ϊ����libnet����һ��dns�������ݰ�����ʹ��libpcapץȡ��Ӧ����Ȼ�����dns��Ӧ��ѯ����ip��ַ��
ע�⣺ԭ�е�dns.c�����д��󣬷��͵�dns�������ݰ����ԡ�����
	payload_s = snprintf(payload, sizeof payload, "%c%s%c%c%c%c%c", 
			 (char)(strlen(query)&0xff), query, 0x00, 0x00, 0x01, 0x00, 0x01);
	��䡣
������google dns��ȷ�ĸ�ʽ���Աȵ�2����tcpdump���ӵ����ݰ������д�Ȼ����ܵõ���ȷ����Ӧ����
���ߣ��������windows ��ʹ��sniffer������ȷ��dns��ѯ���鿴��ѯ���ݰ���dns���ݶε����ݣ����dns���İ�ͷ����ȷ�ġ�
	