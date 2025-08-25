#ifndef _FLASH_HANDLER_H_
#define _FLASH_HANDLER_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#include "temperature.h"

#define NVS_PARTITION		    rolling_buffer
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)

// flash alst page start address: 7F000
#define FLASH_START             0x0007F000
// #define SIZEOF_FLASH_BUFFER 	16 		// 8 bytes for double + 8 bytes for int64_t
#define SIZEOF_FLASH_BUFFER     1200  // bytes
#define NUM_ENTRIES_OFFSET      0  // first byte of flash stores number of entries
#define DATA_START_OFFSET       4  // data starts after 4 bytes (to align to 4-byte boundary)
#define ENTRY_SIZE              16 // each entry is 12 bytes (8 bytes for double + 8 bytes for int64_t) 
#define MAX_ENTRIES             (SIZEOF_FLASH_BUFFER / ENTRY_SIZE) // 100 entries of
#define MAX_ENTRIES 			64 		// 64 entries of 12 bytes = 768 bytes total
#define FCB_SECTOR_COUNT        1

// static struct fcb _fcb;
// static struct flash_sector sectors[FCB_SECTOR_COUNT];

// Buffer to hold data to write to flash
// uint8_t xflashBuffer[SIZEOF_FLASH_BUFFER * MAX_ENTRIES]; // 4 bytes for float + 8 bytes for int64_t

void initFlash(void);
void writeToFlash(uint8_t *data, size_t len, off_t offset);
void readFromFlash(uint8_t *data, size_t len, off_t offset);
void eraseFlash(void);

off_t getOffset(void);

#endif /* _FLASHHANDLER_H_ */