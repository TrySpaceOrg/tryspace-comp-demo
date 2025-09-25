/*******************************************************************************
** File: demo_cli.h
**
** Purpose:
**   This is the header file for the demo checkout.
**
*******************************************************************************/
#ifndef _DEMO_CHECKOUT_H_
#define _DEMO_CHECKOUT_H_

/*
** Includes
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#include "hwlib.h"
#include "device_cfg.h"
#include "demo_device.h"

#if TGTNAME == cpu1
#include "simulith_transport.h"
#endif

/*
** Standard Defines
*/
#define PROMPT               "demo> "
#define MAX_INPUT_BUF        512
#define MAX_INPUT_TOKENS     64
#define MAX_INPUT_TOKEN_SIZE 50
#define TELEM_BUF_LEN        8

/*
** Command Defines
*/
#define CMD_UNKNOWN -1
#define CMD_HELP     0
#define CMD_EXIT     1
#define CMD_NOOP     2
#define CMD_HK       3
#define CMD_DEMO     4
#define CMD_CFG      5

/*
** Prototypes
*/
void print_help(void);
int  get_command(const char *str);
int  process_command(int cc, int num_tokens, char tokens[MAX_INPUT_TOKENS][MAX_INPUT_TOKEN_SIZE]);
int  main(int argc, char *argv[]);

/*
** Generic Prototypes
*/
int  check_number_arguments(int actual, int expected);
void to_lower(char *str);

#endif /* _DEMO_CHECKOUT_H_ */
