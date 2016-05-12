/*
Copyright (c) 2016, Yomei Otani <yomei.otani@gmai.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/


#ifndef SET_UART1_H
#define	SET_UART1_H

#ifdef	__cplusplus
extern "C" {
#endif

/****************************
 * void uart_init(void);
 * initialize uart
 * port setting -> rx:RP5 tx:RP0
*****************************/
void uart1_init(void);

/****************************
 * int read_uart1(char *cmd,int sz);
 * return ret<0 : TIMEOUT or error
 * return ret > 0 : string length 
*****************************/
int read_uart1(char *cmd,int sz);

/******************************
 * write_uart1(char* cmd, int size);
 * 0<timeout
 * 1>readlength
 ******************************/
int write_uart1(char *cmd,int sz);

/****************************
 * void uart_finit(void);
 * initialize uart
 * port setting -> rx:RP5 tx:RP0
*****************************/
void uart1_finit(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SET_UART_H */
