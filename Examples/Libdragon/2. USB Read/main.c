/***************************************************************
                             main.c
                               
Handles the boot process of the ROM.
***************************************************************/

#include <libdragon.h>
#include <stdio.h>
#include <string.h>
#include "usb.h"


/*==============================
    main
    Initializes the ROM
==============================*/

int main(void)
{
    char echobuffer[1024]; // 1 kilobyte buffer for echoing data back
    char incoming_type = 0;
    int incoming_size = 0;
    
    // Initialize USB and give the user instructions
    usb_initialize();
    usb_write(DATATYPE_TEXT, "Type something into the console!\n", 33+1);
    
    // Run in a loop
    while (1)
    {
        // Check if there's data in USB
        // Needs to be a while loop because we can't write to USB if there's data that needs to be read first
        while (usb_poll() != 0)
        {
            uint32_t header = usb_poll();
            
            // Store the size and type from the header
            incoming_type = USBHEADER_GETTYPE(header);
            incoming_size = USBHEADER_GETSIZE(header);
            
            // If the amount of data is larger than our echo buffer
            if (incoming_size > 1024)
            {
                // Purge the USB data
                usb_purge();
                
                // Write an error message to buffer instead
                sprintf(echobuffer, "Incoming data larger than echo buffer!\n");
                incoming_type = DATATYPE_TEXT;
                incoming_size = strlen(echobuffer)+1;
                
                // Restart the while loop to check if there's more USB data
                continue;
            }
            
            // Read the data from the USB into our echo buffer
            usb_read(echobuffer, incoming_size);
        }
        
        // If there's no more data in the USB buffers and we have new data to echo
        if (incoming_size > 0)
        {
            // Write the data to USB
            usb_write(incoming_type, echobuffer, incoming_size);
            if (incoming_type == DATATYPE_TEXT)
                usb_write(DATATYPE_TEXT, "\n", 1+1);
                
            // Clear everything
            incoming_type = 0;
            incoming_size = 0;
            memset(echobuffer, 0, 1024);
        }
    }
}