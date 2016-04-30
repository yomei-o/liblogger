#define DUMMY
#include <stdio.h>
#include <string.h>

#ifndef DUMMY
#include <fcntl.h>
#include <sys/types.h>
#ifdef unix
#include <termios.h>
#include <unistd.h>
#endif
#ifdef _WIN32
#include<windows.h>
#endif
#endif

#include "serialport.h"

#if defined(DUMMY)
#define PORTNAME "COM7"
#elif defined(_WIN32)
#define PORTNAME "COM7"
#elif defined(__CYGWIN__)
#define PORTNAME "/dev/com7"
#else
#define PORTNAME "/dev/ttyO1"
#endif

intptr_t openPort()
{
	intptr_t fd = -1;
#if defined(DUMMY)
	fd = 100;
#elif defined(unix) || defined(__APPLE__) || defined(ANDROID) || defined(__CYGWIN__)
	
	struct termios tty;
    fd = open(PORTNAME,O_RDWR|O_NOCTTY);

	if (fd == -1)
    {
        return -1;
    }

	memset(&tty,0,sizeof(tty));
	tty.c_cflag=CS8|CLOCAL|CREAD;
	tty.c_cc[VMIN]=0;
	tty.c_cc[VTIME]=10;
	cfsetospeed(&tty,B115200);
	cfsetispeed(&tty,B115200);
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&tty);
#elif defined(_WIN32)
	HANDLE hComPort;
	hComPort = CreateFile("COM7", GENERIC_READ | GENERIC_WRITE
		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hComPort == INVALID_HANDLE_VALUE) {
		return -1;
	}
	PurgeComm(hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	DCB dcb;
	GetCommState(hComPort, &dcb);
	dcb.BaudRate = 115200; 
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.EvtChar = 0;
	SetCommState(hComPort, &dcb);
	
	COMMTIMEOUTS cto;
	GetCommTimeouts( hComPort, &cto );
	cto.ReadIntervalTimeout = 1000;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 1000;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts( hComPort, &cto );

	fd = (intptr_t)hComPort;
#endif
	return fd;
}

void closePort(intptr_t fd)
{
	if (fd == -1)return;
#if defined(DUMMY)
#elif defined(unix) || defined(__APPLE__) || defined(ANDROID) || defined(__CYGWIN__)
	close(fd);
#elif defined(_WIN32) 
	CloseHandle((HANDLE)fd);
#endif
}

int writePort(intptr_t fd, void*buf, int sz)
{
	int ret=-1;
	if (fd == -1)return ret;
#if defined(DUMMY)
	return sz;
#elif defined(unix) || defined(__APPLE__) || defined(ANDROID) || defined(__CYGWIN__)
	ret = write(fd, buf, sz);
#elif defined(_WIN32) 
	BOOL R;
	DWORD dwSendSize;
	R = WriteFile((HANDLE)fd, buf, sz, &dwSendSize, NULL);
	if (R == FALSE)return -1;
	ret = dwSendSize;
#endif
	return ret;
}

int readPort(intptr_t fd, void*buf, int sz)
{
	int ret = -1;
	if (fd == -1)return ret;
#if defined(DUMMY)
	{
		static char* a = "ok\r\nok\r\n";
		if (*a == 0)return -1;
		((char*)buf)[0] = *a++;
		return 1;
	}
#elif defined(unix) || defined(__APPLE__) || defined(ANDROID) || defined(__CYGWIN__)
	ret = read(fd, buf, sz);
#elif defined(_WIN32) 
	BOOL R;
	DWORD dwSendSize;
	R = ReadFile((HANDLE)fd, buf, sz, &dwSendSize, NULL);
	if (R == FALSE)return -1;
	ret = dwSendSize;
#endif
	return ret;
}

void sleepPort(int s)
{
#if defined(DUMMY)
#elif defined(unix) || defined(__APPLE__) || defined(ANDROID) || defined(__CYGWIN__)
	sleep(s);
#elif defined(_WIN32)
	Sleep(s * 1000);
#endif
}



#if 0

int main( int c, char ** v )
{
    intptr_t fd = openPort();
    int ret;
    char buffer[200];

	printf("write \n");
    ret=writePort(fd,"0123456789\r\n\r\n\r\n\r\n",20);
	printf("write ret=%d\n",ret);

	while(1){
    	ret = readPort( fd, buffer, 1 );
    	if(ret<1)break;
		printf("read ret=%d\n",ret);
		printf("buyffer=%02x\n",buffer[0]&255);
		sleepPort(1);
	}
	closePort(fd);
	return 0;
}

#endif



