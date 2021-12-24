#pragma once
#include "Microchip/usb_config.h"

// USB read buffer
#define USB_READ_BUFFER_MAX  CDC_DATA_IN_EP_SIZE
uint8_t UsbReadBuffer[USB_READ_BUFFER_MAX];

// USB write buffer
#define USB_WRITE_BUFFER_MAX  CDC_DATA_OUT_EP_SIZE
uint8_t UsbWriteBuffer[USB_WRITE_BUFFER_MAX];


// Input buffer - max is maximum line length
#define USB_INPUT_BUFFER_MAX  80
uint8_t UsbInputBuffer[USB_INPUT_BUFFER_MAX];
uint8_t UsbInputBufferCount = 0;
bool UsbInputBufferCorrupted = false;

// Output buffer - max needs to be on a large size to prevent running out of it
#define USB_OUTPUT_BUFFER_MAX 20
uint8_t UsbOutputBuffer[USB_OUTPUT_BUFFER_MAX];
uint8_t UsbOutputBufferCount = 0;

#define UsbOutputBufferAppend(X)  UsbOutputBuffer[UsbOutputBufferCount] = (X); UsbOutputBufferCount++;


void usbBufferCopy(uint8_t* destination, const uint8_t* source, const uint8_t count);
