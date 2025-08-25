#include "buttonHandler.h"
#include "timers.h"
#include "main.h"
#include "flashHandler.h"

#define SW0_NODE	DT_ALIAS(sw0)   // Start/stop temperature logging button
#define SW1_NODE	DT_ALIAS(sw1)   // dump flash contents to console button
#define SW2_NODE	DT_ALIAS(sw2)   // clear flash contents button

#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

// BUTTON 1
void logTemperatureButton_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

// BUTTON 2
void dumpFlashButton_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

// BUTTON 3
void eraseFlashButton_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
static const struct gpio_dt_spec logTemperatureButton   = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,{0});
static const struct gpio_dt_spec dumpFlashButton        = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,{0});
static const struct gpio_dt_spec eraseFlashButton       = GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios,{0});

static struct gpio_callback button_cb_data_temperature, button_cb_data_erase, button_cb_data_dump;

static volatile bool _temperatureButton_pressed         = 0;
static volatile bool _eraseFlashButton_pressed          = 0;
static volatile bool _dumpFlashButton_pressed           = 0;

void initButtons()
{
    int ret;

    // Configure button pins
    if (!device_is_ready(logTemperatureButton.port)) {
        printk("Button: device %s not ready.\n", logTemperatureButton.port->name);
        return;
    }

    ret = gpio_pin_configure_dt(&logTemperatureButton, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n",
               ret, logTemperatureButton.port->name, logTemperatureButton.pin);
        return;
    }

    ret = gpio_pin_configure_dt(&dumpFlashButton, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n",
               ret, dumpFlashButton.port->name, dumpFlashButton.pin);
        return;
    }

    ret = gpio_pin_configure_dt(&eraseFlashButton, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n",
               ret, eraseFlashButton.port->name, eraseFlashButton.pin);
        return;
    }

    // Configure interrupt on log temperature button pin
    ret = gpio_pin_interrupt_configure_dt(&logTemperatureButton, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
               ret, logTemperatureButton.port->name, logTemperatureButton.pin);
        return;
    }

    // Configure interrupt on log temperature button pin
    ret = gpio_pin_interrupt_configure_dt(&dumpFlashButton, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
               ret, dumpFlashButton.port->name, dumpFlashButton.pin);
        return;
    }

    // Configure interrupt on log temperature button pin
    ret = gpio_pin_interrupt_configure_dt(&eraseFlashButton, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
               ret, eraseFlashButton.port->name, eraseFlashButton.pin);
        return;
    }

    // init callback for log temperature button
    gpio_init_callback(&button_cb_data_temperature, logTemperatureButton_pressed, BIT(logTemperatureButton.pin));
    gpio_add_callback(logTemperatureButton.port, &button_cb_data_temperature);
    printk("Set up button at %s pin %d\n", logTemperatureButton.port->name, logTemperatureButton.pin);

    // init callback for dump flash button
    gpio_init_callback(&button_cb_data_dump, dumpFlashButton_pressed, BIT(dumpFlashButton.pin));
    gpio_add_callback(dumpFlashButton.port, &button_cb_data_dump);
    printk("Set up button at %s pin %d\n", dumpFlashButton.port->name, dumpFlashButton.pin);

    // init callback for erase flash button
    gpio_init_callback(&button_cb_data_erase, eraseFlashButton_pressed, BIT(eraseFlashButton.pin));
    gpio_add_callback(eraseFlashButton.port, &button_cb_data_erase);
    printk("Set up button at %s pin %d\n", eraseFlashButton.port->name, eraseFlashButton.pin);
}

// Callback for log temperature button
void logTemperatureButton_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // Collect temperature data every 500ms when button pressed to buffer, then write buffer to flash when button pressed again
    if(!_temperatureButton_pressed) {
        _temperatureButton_pressed = 1;
        k_timer_start(&temperature_timer, K_MSEC(500), K_MSEC(500));
    }
    else{
        _temperatureButton_pressed = 0;
        k_timer_stop(&temperature_timer);

        // Get temperature data array and write to flash
        temperature_data_t *temperature_data_array = getTemperatureDataArray();

        // Crashing in here
        // writeToFlash((uint8_t*)&temperature_data_array, 64, getOffset());
    }
}

// Callback for erase flash button
void eraseFlashButton_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    eraseFlash();
}

// Callback for  print contents of flash buffer to console
void dumpFlashButton_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // read flash contents and print to console
    for (int i = 0; i < MAX_ENTRIES; i++) {
        uint8_t data[ENTRY_SIZE];
        readFromFlash(data, ENTRY_SIZE, i * ENTRY_SIZE);
        temperature_data_t *tempData = (temperature_data_t *)data;
        if (tempData->timestamp == 0xFFFFFFFFFFFFFFFF) {
            // reached end of valid data
            break;
        }
        printk("Entry %d: Temp = %.2f C, Timestamp = %lld\n", i, tempData->temperature, tempData->timestamp);
    }
}