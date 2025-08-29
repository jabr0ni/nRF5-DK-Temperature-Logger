#include "flashHandler.h"
#include "sensorsHandler.h"
#include "temperature.h"

#include "main.h"

uint8_t data_buffer[ENTRY_SIZE * MAX_SAMPLES];

void writeToFlash(uint8_t *data, size_t len, off_t offset)
{
    // Write the entire buffer to flash at the specified offset within the partition
    memset(data_buffer, 0xFF, len); // default erased state
    memcpy(data_buffer, data, len);

    const struct flash_area *fa;
    
    int rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
        return;
    }

    // erase flash if writing at start of data area
    if (offset == DATA_START_OFFSET) { 
        rc = flash_area_erase(fa, 0, fa->fa_size); // erase one page
        if (rc) {
            printk("Failed to erase flash area (%d)\n", rc);
            return;
        }
    }

    rc = flash_area_write(fa, offset, data_buffer, len);
    if (rc) {
        printk("Flash write failed (%d)\n", rc);
    }

    setOffset(offset + (off_t)len);

    // flash_area_close(fa);
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
    uint8_t buffer[ENTRY_SIZE];
    flash_area_read(fa, offset, buffer, len);
    memcpy(data, buffer, len);

    // flash_area_close(fa);
}

void setOffset(off_t offset)
{
    int rc;
    const struct flash_area *fa;

    rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc)
    {
        printk("Failed to open flash partition (%d)\n", rc);
        return;
    }

    rc = flash_area_write(fa, NUM_ENTRIES_OFFSET, (void*)&offset, sizeof(offset));
    if (rc)
    {
        printk("Failed to write offset to flash (%d)\n", rc);
        return;
    }
    else
    {
        printk("Wrote new offset %i to flash\n", offset);
    }

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

    rc = flash_area_erase(fa, 0, fa->fa_size);
    if (rc) {
        printk("Failed to erase flash partition (%d)\n", rc);
        return;
    }
    // flash_area_close(fa);
}

off_t getOffset(void)
{
    off_t offset = 0;
    const struct flash_area *fa;
    int rc;
    rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
        return DATA_START_OFFSET; // default to start of data area
    }

    rc = flash_area_read(fa, NUM_ENTRIES_OFFSET, (void*)offset, sizeof(off_t));
    if (rc) {
        printk("Failed to read number of entries from flash (%d)\n", rc);
        return DATA_START_OFFSET; // default to start of data area
    }
    else
    {
        printk("Read offset %lu from flash\n", offset);
    }
    // flash_area_close(fa);

    return offset;
}

void printFlashContents()
{   
    int rc;
    const struct flash_area *fa;
    temperature_data_t RtempData;
    rc = flash_area_open(NVS_PARTITION_DEVICE, &fa);
    if (rc) {
        printk("Failed to open flash partition (%d)\n", rc);
    }
    // read flash contents and print to console
    for (int i = 0; i < MAX_SAMPLES; i++) {
        uint8_t data[ENTRY_SIZE];
        readFromFlash(data, ENTRY_SIZE, DATA_START_OFFSET + i * ENTRY_SIZE);
        memcpy(&RtempData, data, sizeof(temperature_data_t));

        if (RtempData.timestamp == 0xFFFFFFFFFFFFFFFF) {
            // reached end of valid data
            printk("Reached end of valid data. Entry %d: Temp = %.2f C, Timestamp = %lld\n", i, RtempData.temperature, RtempData.timestamp);
            break;
        }

        printk("Entry %d: Temp = %.2f C, Timestamp = %lld\n", i, RtempData.temperature, RtempData.timestamp);
        // printk("Entry %d: Temp = %.2f C, Timestamp = %lld\n", i, RtempData.temperature, RtempData.timestamp);
    }
}