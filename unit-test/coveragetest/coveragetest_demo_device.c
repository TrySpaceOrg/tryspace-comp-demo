#include "demo_app_coveragetest_common.h"

void Test_DEMO_ReadData(void)
{
    uart_info_t device;
    uint8_t     read_data[8];
    uint8_t     data_length = 8;
    DEMO_ReadData(&device, read_data, data_length);

    UT_SetDeferredRetcode(UT_KEY(uart_bytes_available), 1, data_length);
    DEMO_ReadData(&device, read_data, data_length);

    UT_SetDeferredRetcode(UT_KEY(uart_bytes_available), 1, data_length + 1);
    DEMO_ReadData(&device, read_data, data_length);
}

void Test_DEMO_CommandDevice(void)
{
    uart_info_t device;
    uint8_t     cmd_code = 0;
    uint32_t    payload  = 0;
    DEMO_CommandDevice(&device, cmd_code, payload);

    UT_SetDeferredRetcode(UT_KEY(uart_flush), 1, UART_ERROR);
    DEMO_CommandDevice(&device, cmd_code, payload);

    UT_SetDeferredRetcode(UT_KEY(uart_write_port), 1, DEMO_DEVICE_CMD_SIZE);
    DEMO_CommandDevice(&device, cmd_code, payload);

    UT_SetDeferredRetcode(UT_KEY(uart_write_port), 1, DEMO_DEVICE_CMD_SIZE);
    UT_SetDeferredRetcode(UT_KEY(uart_bytes_available), 1, 9);
    UT_SetDeferredRetcode(UT_KEY(uart_read_port), 1, 9);
    UT_SetDefaultReturnValue(UT_KEY(DEMO_ReadData), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(DEMO_ReadData), 1, OS_SUCCESS);
    DEMO_CommandDevice(&device, cmd_code, payload);
}

void Test_DEMO_RequestHK(void)
{
    uart_info_t            device;
    DEMO_Device_HK_tlm_t data;
    DEMO_RequestHK(&device, &data);

    uint8_t read_data[] = {0xDE, 0xAD, 0x00, 0x00, 0x00, 0x07, 0x00, 0x06,
                           0x00, 0x0C, 0x00, 0x12, 0x00, 0x00, 0xBE, 0xEF};
    UT_SetDeferredRetcode(UT_KEY(uart_bytes_available), 1, 16);
    UT_SetDeferredRetcode(UT_KEY(uart_read_port), 1, 16);
    UT_SetDataBuffer(UT_KEY(uart_read_port), &read_data, sizeof(read_data), false);
    DEMO_RequestHK(&device, &data);

    UT_SetDeferredRetcode(UT_KEY(uart_flush), 1, OS_ERROR);
    DEMO_RequestHK(&device, &data);
}

void Test_DEMO_RequestData(void)
{
    uart_info_t              device;
    DEMO_Device_Data_tlm_t data;
    DEMO_RequestData(&device, &data);

    uint8_t read_data[] = {0xDE, 0xAD, 0x00, 0x00, 0x00, 0x07, 0x00, 0x06,
                           0x00, 0x0C, 0x00, 0x12, 0x00, 0x00, 0xBE, 0xEF};
    UT_SetDeferredRetcode(UT_KEY(uart_bytes_available), 1, 16);
    UT_SetDeferredRetcode(UT_KEY(uart_read_port), 1, 16);
    UT_SetDataBuffer(UT_KEY(uart_read_port), &read_data, sizeof(read_data), false);
    DEMO_RequestData(&device, &data);

    UT_SetDeferredRetcode(UT_KEY(uart_flush), 1, OS_ERROR);
    DEMO_RequestData(&device, &data);
}

void Test_DEMO_RequestData_Hook(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va) {}

/*
 * Setup function prior to every test
 */
void Demo_UT_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Teardown function after every test
 */
void Demo_UT_TearDown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UT_SetVaHandlerFunction(UT_KEY(Test_DEMO_RequestData), Test_DEMO_RequestData_Hook, NULL);
    ADD_TEST(DEMO_ReadData);
    ADD_TEST(DEMO_CommandDevice);
    ADD_TEST(DEMO_RequestHK);
    ADD_TEST(DEMO_RequestData);
}