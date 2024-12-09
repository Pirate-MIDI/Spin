/*
 Copyright (C) 2024  Greg Smith

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 
*/


#ifndef _USB_COMMS_H
#define _USB_COMMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb/usb_host.h"

#define IK_MULTIMEDIA_USB_VENDOR        0x1963
#define TONEX_ONE_PRODUCT_ID            0x00D1

enum USB_Commands
{
    USB_COMMAND_SET_PRESET,
    USB_COMMAND_NEXT_PRESET,
    USB_COMMAND_PREVIOUS_PRESET
};

typedef struct 
{
    usb_host_client_handle_t client_hdl;
    uint8_t dev_addr;
    usb_device_handle_t dev_hdl;
    uint32_t actions;
} class_driver_t;

typedef struct 
{
    uint8_t Command;
    uint32_t Payload;
} tUSBMessage;

void init_usb_comms(void);

// thread safe public API
void usb_set_preset(uint32_t preset);
void usb_next_preset(void);
void usb_previous_preset(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif