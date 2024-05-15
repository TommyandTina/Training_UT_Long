
#ifndef OSAL_TEST_H
#define OSAL_TEST_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rcar-xos/osal/r_osal.h"

/* Define */
#define OSAL_TEST_ALL_EXEC (100)
#define OSAL_TEST_APP_EXIT (999)

#define OSAL_TEST_OK (0)
#define OSAL_TEST_NG (1)

#define OSAL_TEST_OPT_INFO_BUFFER   (150)

/* enum */
/* OSAL Manager ID */
typedef enum
{
    OSAL_TEST_CONFIG          = 10,
    OSAL_TEST_ALL             = OSAL_TEST_ALL_EXEC,
    OSAL_TEST_EXIT            = OSAL_TEST_APP_EXIT,
    OSAL_TEST_PARENT_MAX      = OSAL_TEST_CONFIG,
} OSAL_TEST_PARENT_ID;

/* OSAL Manager API ID */
typedef enum
{
/* Configuration table check */
    OSAL_TEST_CONFIG_CHECK = 1,

/* Maximum ID of Manager */
    OSAL_TEST_CONFIG_MAX     = OSAL_TEST_CONFIG_CHECK,
} OSAL_TEST_API_ID;

/* Macro */
/* Additional information create macro for output of evaluation results. */
#define OSAL_TEST_MAKE_INFO(opt_info, exp_str, ret_str) \
    sprintf(opt_info, "Expectation value: %s, Return value: %s", exp_str, ret_str)

/* ErrorLog output macro */
#define OSAL_TEST_ERR(fmt, ...) \
    fprintf(stderr, \
            "error: [%s][%s][line.%d] %02d_%03d_%03d: " fmt "\n", \
            __FILE__, __func__, __LINE__, __VA_ARGS__); \
            test_err = -1;

/* prototype */
int osal_test(int parent_number, int child_number);
void osal_test_configuration_display_menu(void);
void osal_test_configuration_menu(int child);

extern unsigned int test_pass;
extern unsigned int test_fail;
extern unsigned int test_skip;
extern int          test_err;

#endif  /* OSAL_TEST_H */
