�����������˽�Ͳ���Visual Studio��ʹ�á�

�������ˣ�
��linux�б������У�������ȥ��������������������

�ͻ��ˣ�WSAAsyncSelect���򣩱��벽�裺
1. ����MS Visual Studio 2008;
2. File-New-Projects-Win32 Application������Project Name��Ŀ¼��Next��
3. ��What Kind of Windows application�У�ѡ��An empty project;
4. ��File View�����½ǣ��У�
1)ѡ��Header File���Ҽ�-Add files to folder��ѡresource.h��
2)ѡ��Source Files���Ҽ�-Add files to folder��ѡ09-01client.c��
3)ѡ��Resource Files���Ҽ�-Add files to folder��ѡ09-01client.rc
5. Project-Settings��ѡ��Ŀ��
1)Linker�������루��ߣ�����������������ļ�ws2_32.lib;
2) ���棨�󣩣��ַ�����ѡ��δ���á���
3�����ԣ��󣩣�������������������ip��ַ
6. ���У���˵�Server-Start...�����������ȷ���ӵ�linux server�ϣ����ڴ�������ʾһ�����֣����ִ�1-1024�仯��