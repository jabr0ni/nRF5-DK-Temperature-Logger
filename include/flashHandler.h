#ifndef _FLASH_HANDLER_H_
#define _FLASH_HANDLER_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <stdio.h>

#include "temperature.h"

#define NVS_PARTITION		    rolling_buffer
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_ID(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)

// flash last page start address: 7F000
#define FLASH_START             0x0007F000
#define SIZEOF_FLASH_BUFFER     4096                                // bytes in 1 page of flash
#define NUM_ENTRIES_OFFSET      0                                   // first byte of flash stores number of entries
#define DATA_START_OFFSET       4                                   // data starts after 4 bytes (to align to 4-byte boundary)
#define ENTRY_SIZE              16                                  // each entry is 12 bytes (8 bytes for double + 8 bytes for int64_t) 
#define MAX_ENTRIES             (SIZEOF_FLASH_BUFFER / ENTRY_SIZE)  // Max entries based on buffer size and entry size
#define MAX_SAMPLES 			8 		                            // 64 entries of 12 bytes = 768 bytes total

void initFlash(void);
void writeToFlash(uint8_t *data, size_t len, off_t offset);
void readFromFlash(uint8_t *data, size_t len, off_t offset);
void eraseFlash(void);
void printFlashContents(void);

off_t getOffset(void);
void setOffset(off_t offset);
#endif /* _FLASHHANDLER_H_ */