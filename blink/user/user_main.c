#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"

static os_timer_t ptimer;


#if (SPI_FLASH_SIZE_MAP == 2)
    // 1M
#   define FLASH_SIZE                                          0x100000
#elif (SPI_FLASH_SIZE_MAP == 3)
    // 2M
#   define FLASH_SIZE                                          0x200000
#elif (SPI_FLASH_SIZE_MAP == 4)
    // 4M
#   define FLASH_SIZE                                          0x400000
#else
#   error "The flash map is not supported"
#endif


#define EAGLE_FLASH_BIN_ADDR                (SYSTEM_PARTITION_CUSTOMER_BEGIN + 1)
#define EAGLE_IROM0TEXT_BIN_ADDR            (SYSTEM_PARTITION_CUSTOMER_BEGIN + 2)

static const partition_item_t partition_table[] = {
    { EAGLE_FLASH_BIN_ADDR,                 0x00000,             0x10000 },
    { EAGLE_IROM0TEXT_BIN_ADDR,             0x10000,             0x60000 },
    { SYSTEM_PARTITION_RF_CAL,              FLASH_SIZE - 0x5000, 0x1000 },
    { SYSTEM_PARTITION_PHY_DATA,            FLASH_SIZE - 0x4000, 0x1000 },
    { SYSTEM_PARTITION_SYSTEM_PARAMETER,    FLASH_SIZE - 0x3000, 0x3000 },
};


void ICACHE_FLASH_ATTR user_pre_init(void)
{
    os_printf("system_partition_table_regist start\r\n");

    if (!system_partition_table_regist(
        partition_table,
        sizeof(partition_table) / sizeof(partition_table[0]),
        SPI_FLASH_SIZE_MAP
    )) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}

    os_printf("system_partition_table_regist done\r\n");
}


void blinky(void *arg)
{
	static uint8_t state = 0;

    os_printf("blinky start, state=%d\r\n", state);
	if (state) {
		GPIO_OUTPUT_SET(2, 1);
	} else {
		GPIO_OUTPUT_SET(2, 0);
	}
	state ^= 1;
    os_printf("blinky end, state=%d\r\n", state);
}

void ICACHE_FLASH_ATTR user_init(void)
{
    os_printf("user_init start\r\n");
    uart_init(115200, 115200);

    os_printf("SDK version:%s\n", system_get_sdk_version());
    gpio_init();


    // Disable WiFi
    wifi_set_opmode(NULL_MODE);

    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    os_timer_disarm(&ptimer);
    os_timer_setfn(&ptimer, (os_timer_func_t *)blinky, NULL);
    os_timer_arm(&ptimer, 1000, 1);
    os_printf("user_init end\r\n");
}
