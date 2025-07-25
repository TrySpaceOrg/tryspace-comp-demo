#ifndef _DEMO_DEVICE_CFG_H_
#define _DEMO_DEVICE_CFG_H_

/*
** Demo Device Configuration
*/
#ifndef DEMO_CFG
/* Notes:
**   NOS3 uart requires matching handle and bus number
*/
#define DEMO_CFG_STRING      "/dev/usart_5"
#define DEMO_CFG_HANDLE      5
#define DEMO_CFG_BAUDRATE_HZ 115200
#define DEMO_CFG_MS_TIMEOUT  250
/* Note: To disable debug prints, comment out the following line */
#define DEMO_CFG_DEBUG
#endif

#endif /* _DEMO_DEVICE_CFG_H_ */
