#include <stdio.h> 
#include <tchar.h> 
#include <windows.h> 
#include <iostream>
#include<thread>
using namespace std;

int main() {
	HANDLE hCom; //ȫ�ֱ��������ھ��
	hCom = CreateFile(_T("COM3"),//COM3��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hCom == (HANDLE)-1)
	{
		std::cout << ("��COMʧ��!") << endl;
	}
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONE5STOPBITS;
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	char run[8] = {1, 5, 0, 0, 0xff, 0, 0x8c, 0x3a};
	char stop[8] = {1, 5, 0, 0, 0, 0, 0xcd, 0xca};
	DWORD dwBytesWrite = 8;
	while (true) {
		Sleep(2000);
		WriteFile(hCom, run, dwBytesWrite, &dwBytesWrite, NULL);
		Sleep(2000);
		WriteFile(hCom, stop, dwBytesWrite, &dwBytesWrite, NULL);
	}
	
	int unnecessary_num;
	cin >> unnecessary_num;
	return 0;
}