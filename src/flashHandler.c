#include <stdio.h>
#include "flashHandler.h"
#include "sensorsHandler.h"
#include "temperature.h"

#include "main.h"

// static const struct device *flash_dev = NVS_PARTITION_DEVICE;
// static const struct device *flash_device;

/**
 * First byte of flash will be number of entries 
 */
// flash ptr
// uint32_t *flash_addr = (uint32_t *)FLASH_START;

// static struct nvs_fs fs;

struct flash_parameters flash_params;
uint8_t buf[64];
// uint8_t bufRX[64];  // temp test buffer for reading back

void writeToFlash(uint8_t *data, size_t len, off_t offset)
{
    // Write the entire buffer to flash at the specified offset within the partition
    memset(buf, 0xFF, len); // default erased state
    // memset(bufRX, 0xFF, len); // default erased state
    memcpy(buf, data, len);

    temperature_data_t newData[len];

    const struct flash_area *fa;
    
    int rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
        return;
    }

    // erase flash if writing at start of data area
    if (offset == DATA_START_OFFSET) { 
        rc = flash_area_erase(fa, 0, 4096); // erase one page
        if (rc) {
            printk("Failed to erase flash area (%d)\n", rc);
            return;
        }
    }

    rc = flash_area_write(fa, offset, buf, len);
    if (rc) {
        printk("Flash write failed (%d)\n", rc);
    }

    // rc = flash_area_read(fa, offset, bufRX, len);
    // if (rc) {
    //     printk("Flash read failed (%d)\n", rc);
    // }

    // memcpy(newData, bufRX, len * sizeof(temperature_data_t));

    // increment offset for next write
    offset += len;
    if (offset >= SIZEOF_FLASH_BUFFER) {
        offset = DATA_START_OFFSET; // wrap around to start of data area
    }
    
    // store offset back to flash
    // uint32_t numEntries = (offset - DATA_START_OFFSET) / ENTRY_SIZE;
    flash_area_write(fa, NUM_ENTRIES_OFFSET, offset, sizeof(offset));

    flash_area_close(fa);
}

void readFromFlash(uint8_t *data, size_t len, off_t offset)
{
    const struct flash_area *fa;
    int rc;
    rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
        return;
    }

    // Read the entire buffer from flash at the specified offset within the partition
    uint8_t buf[ENTRY_SIZE];
    flash_area_read(fa, offset, buf, len);
    memcpy(data, buf, len);

    flash_area_close(fa);
}


void eraseFlash(void)
{
    const struct flash_area *fa;
    int rc;
    rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
        return;
    }

    rc = flash_area_erase(fa, 0, 2*1024/*fa->fa_size*/);
    if (rc) {
        printk("Failed to erase flash partition (%d)\n", rc);
        return;
    }
    // flash_area_close(fa);
}

off_t getOffset(void)
{
    off_t offset;
    const struct flash_area *fa;
    int rc;
    rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
        return DATA_START_OFFSET; // default to start of data area
    }

    rc = flash_area_read(fa, NUM_ENTRIES_OFFSET, (void*)offset, sizeof(offset));
    if (rc) {
        printk("Failed to read number of entries from flash (%d)\n", rc);
        return DATA_START_OFFSET; // default to start of data area
    }
    // flash_area_close(fa);

    return offset;
}

// test func - not used in main program
// void initFlash(void)
// {
// 	uint32_t offset;
// 	// const struct device *flash_dev = NVS_PARTITION_DEVICE;
// 	// struct flash_parameters flash_params;
// 	// volatile const struct flash_parameters *paramsX = flash_get_parameters(flash_dev);

// 	// // uint8_t test_data[TEST_DATA_SIZE_IN_BYTES];

// 	// memcpy(&flash_params, flash_get_parameters(flash_dev), sizeof(flash_params));
    
// 	// if (!device_is_ready(flash_dev)) {
// 	// 	printf("Internal storage device not ready\n");
// 	// 	return;
// 	// }

// 	int rc = 0, cnt = 0, cnt_his = 0;
// 	char buf[16];
// 	uint8_t key[8], longarray[128];
// 	uint32_t reboot_counter = 0U, reboot_counter_his;
// 	struct flash_pages_info info;

// 	/* define the nvs file system by settings with:
// 	 *	sector_size equal to the pagesize,
// 	 *	3 sectors
// 	 *	starting at NVS_PARTITION_OFFSET
// 	 */
// 	fs.flash_device = NVS_PARTITION_DEVICE;
// 	if (!device_is_ready(fs.flash_device)) {
// 		printk("Flash device %s is not ready\n", fs.flash_device->name);
// 		return 0;
// 	}
// 	fs.offset = 0;;
// 	rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
// 	if (rc) {
// 		printk("Unable to get page info, rc=%d\n", rc);
// 		return;
// 	}
// 	fs.sector_size = info.size;
// 	fs.sector_count = 3U;

// 	rc = nvs_mount(&fs);
// 	if (rc) {
// 		printk("Flash Init failed, rc=%d\n", rc);
// 		return;
// 	}
//     key[0] = 'r';
//     key[1] = 'o';
//     key[2] = 'l';
//     key[3] = 'l';
//     key[4] = 'i';
//     key[5] = 'n';

//     volatile uint8_t test[6] = {0};
//     nvs_write(&fs, 0, key, sizeof(key));
//     nvs_read(&fs, 0, test, sizeof(key));

// }

