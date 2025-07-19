#ifndef _DEMO_PLATFORM_CFG_H_
#define _DEMO_PLATFORM_CFG_H_

/*
** Default DEMO Configuration
*/
#ifndef DEMO_CFG
/* Notes:
**   NOS3 uart requires matching handle and bus number
*/
#define DEMO_CFG_STRING      "usart_16"
#define DEMO_CFG_HANDLE      16
#define DEMO_CFG_BAUDRATE_HZ 115200
#define DEMO_CFG_MS_TIMEOUT  50 /* Max 255 */
/* Note: Debug flag disabled (commented out) by default */
//#define DEMO_CFG_DEBUG
#endif

#endif /* _DEMO_PLATFORM_CFG_H_ */
