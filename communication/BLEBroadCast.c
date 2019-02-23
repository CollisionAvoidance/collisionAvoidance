// before running the below code, remember to turn on the bluetooth module
// 1. login to raspberry pi
// 2. hciconfig -h 
// 3. sudo hciconfig hci0 up // enable bluetooth device
// 4. sudo hciconfig hci0 leadv 3 // set bluetooth device to advertise and not-connectable

#include <stdlib.h>
#include <stdio.h>

// for now this should broadcast an url
#define BROADCASTSCRIPT "\
	sudo hcitool -i hci0 cmd 0x08 0x0008 17 02 01 06 03 03 aa fe 30 16 aa fe of 16 aa fe 10 00 02 77 65 62 67 61 7a 65 72 08 00 00 00 00 00 00 00 00 \n\
"

int main(void){
	// this parts converts position, velocity, origin, intent from string to hex so it can be broadcasted
	// unsigned char str[100], strH[200];
	// int i, j;
	// memset(trH, 0, sizeof(strH))
	// for (i=0,j=0;i<strlen(str);i++,j+=2){
	// 	sprintf((char*)strH+j, "%02X", str[i]);
	// }
	// strH[j] = "\0" // end the string
	// starting the broadcast
	puts("Starting Broadcast");
	puts(BROADCASTSCRIPT);
	puts("Starting now:");
	system(BROADCASTSCRIPT);
	return 0;
}

// next step: read the bluetooth manual, figure out what 2 byte code I need to use