#!/bin/bash

ESP_PORT="/dev/ttyNodeMCU"
FLASH_ARGS="--flash_freq 20m --flash_mode dio --flash_size=4MB"

esptool.py -p $ESP_PORT write_flash $FLASH_ARGS \
    0x00000 ../bin/eagle.flash.bin \
    0x10000 ../bin/eagle.irom0text.bin \
    0x3fb000 ../bin/blank.bin \
    0x3fc000 ../bin/esp_init_data_default_v05.bin \
    0x3fd000 ../bin/blank.bin

picocom $ESP_PORT -b 74880
