#ifndef _DEMO_CHECKOUT_DEVICE_CFG_H_
#define _DEMO_CHECKOUT_DEVICE_CFG_H_

/*
** DEMO Checkout Configuration
*/
#define DEMO_CFG
/* Note: NOS3 uart requires matching handle and bus number */
#define DEMO_CFG_STRING      "/dev/usart_5"
#define DEMO_CFG_HANDLE      5
#define DEMO_CFG_BAUDRATE_HZ 115200
#define DEMO_CFG_MS_TIMEOUT  250
#define DEMO_CFG_DEBUG

#endif /* _DEMO_CHECKOUT_DEVICE_CFG_H_ */
