#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


intptr_t openPort();
void closePort(intptr_t fd);
int writePort(intptr_t fd, void*buf, int sz);
int readPort(intptr_t fd, void*buf, int sz);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif



