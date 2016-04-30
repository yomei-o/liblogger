#include<stdio.h>
#include "uart1_serial.h"

int main()
{
	char buf[256];
	int ret;
	uart1_init();
	ret=read_uart1(buf, 200);
	ret = read_uart1(buf, 200);
	ret = read_uart1(buf, 200);
	ret = read_uart1(buf, 200);
	uart1_finit();

	return 0;
}