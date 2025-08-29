Simple temperature logger for the nRF52 PCA10040 series. Logging internal die temperature to internal flash memory.

Console output can be viewed in any serial terminal such as TeraTerm at a baud of 115200.

Memory partitioned from last page address 0x0007F000 with a partition size of 4KB (0x1000) with name `rolling_buffer`.
Partition defined in `pm_static.yml` file.

Nunber of samples buffered and written to flash can be modified with `MAX_SAMPLES` in flashHandler.h

**Button 1:** Start/Stop temperature logging
**Button 2:** Print contents of flash buffer to console
**Button 3:** Erase flash
