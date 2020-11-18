/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <soc.h>
#include <dtest.h>
#include <pin.h>
#include <test_driver_config.h>


#if (CONFIG_CAN_NUM >= 2)

static void can_test_fun(void)
{




#if  0
    int32_t baud[]         = { 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200};
    int32_t parity[]       = { USART_PARITY_NONE, USART_PARITY_EVEN, USART_PARITY_ODD};
    int32_t stopbits[]     = { USART_STOP_BITS_1, USART_STOP_BITS_2};
    int32_t databits[]     = { USART_DATA_BITS_5, USART_DATA_BITS_6, USART_DATA_BITS_7, USART_DATA_BITS_8};

    uint8_t i, j, k, l;

    for (i = 0; i < sizeof(baud) / sizeof(baud[0]); i++)
        for (j = 0; j < sizeof(parity) / sizeof(parity[0]); j++)
            for (k = 0; k < sizeof(stopbits) / sizeof(stopbits[0]); k++)
                for (l = 0; l < sizeof(databits) / sizeof(databits[0]); l++) {
                    if (test_function(baud[i], parity[j], stopbits[k], databits[l]) < 0) {
                        printf("test_function error\n");
                        return;
                    }
                }
#endif
}
#endif



static void can_test_interfaces(void)
{
#if 0
    usart_handle_t pcsi_usart;
    uint8_t i;
    uint32_t ret;
    uint8_t answer[20];

    test_pin_usart_init();
    pcsi_usart = csi_usart_initialize(TEST_USART_IDX, NULL);
    ASSERT_TRUE(pcsi_usart != NULL);

    for (i = 0; i < sizeof(usart_cases) / sizeof(usart_test_t); i++) {
        ret = csi_usart_config(pcsi_usart, usart_cases[i].baud, usart_cases[i].mode, usart_cases[i].parity, usart_cases[i].stopbits, usart_cases[i].bits);
        ASSERT_TRUE(ret == usart_cases[i].expect_out);
    }

    ASSERT_TRUE(csi_usart_send(NULL, data, 13/*,bool asynch*/) != 0);
    ASSERT_TRUE(csi_usart_receive(NULL, answer, 13/*,bool asynch*/) != 0);
    ASSERT_TRUE(csi_usart_abort_send(NULL) != 0);
    ASSERT_TRUE(csi_usart_abort_receive(NULL) != 0);
    ASSERT_TRUE(csi_usart_uninitialize(NULL) != 0);
#endif
}


int test_can(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_can", NULL, NULL, NULL, NULL
    };
    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);
    dtest_case_info_t test_case_info_array[] = {
#if (CONFIG_CAN_NUM >= 2)
        { "can_test_fun",        can_test_fun,        CAN_TEST_FUN_EN },
#endif
        { "can_test_interfaces", can_test_interfaces, CAN_TEST_INTERFACE_EN },
        { NULL, NULL }
    };
    dtest_add_cases(test_suite, test_case_info_array);
    return 0;
}

