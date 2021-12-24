#pragma once
#include "Microchip/usb_config.h"


// USB read buffer
#define USB_READ_BUFFER_MAX  CDC_DATA_IN_EP_SIZE
uint8_t usb_ReadBuffer[USB_READ_BUFFER_MAX];

// USB write buffer
#define USB_WRITE_BUFFER_MAX  CDC_DATA_OUT_EP_SIZE
uint8_t usb_WriteBuffer[USB_WRITE_BUFFER_MAX];


// Input buffer - max is maximum line length
#define USB_INPUT_BUFFER_MAX  80
uint8_t usb_InputBuffer[USB_INPUT_BUFFER_MAX];
uint8_t usb_InputBufferCount = 0;
bool usb_InputBufferCorrupted = false;

// Output buffer - max needs to be on a large size to prevent running out of it
#define USB_OUTPUT_BUFFER_MAX 20
uint8_t usb_OutputBuffer[USB_OUTPUT_BUFFER_MAX];
uint8_t usb_OutputBufferCount = 0;

#define usb_outputBufferAppend(X)  usb_OutputBuffer[usb_OutputBufferCount] = (X); usb_OutputBufferCount++;


void usb_bufferCopy(uint8_t* destination, const uint8_t* source, const uint8_t count);
