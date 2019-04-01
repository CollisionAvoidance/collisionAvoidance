
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	FILE *fp;
	char command[900] = "sudo hcitool -i hci0 cmd 0x08 0x0008 1e 02 01 1a 0c ff 18 01 48 45 4c 4c 4f 57 4f 52 4c 44 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
	if argc == 1{
		fp = popen(command, "r")
	    if (fp == NULL) {
        	printf("Failed to run command\n" );
        	exit(1);
        }
    }else if argc >= 2{
    	char speed[] = argv[0];
    	char space[] = " "
    	char postion[] = argv[1];
    	char base_command[900] = "sudo hcitool -i hci0 cmd 0x08 0x0008 1e 02 01 1a 0c ff 18 01 ";
    	strcat(base_command, speed)
    	strcat(base_command, postion)
    	fp = popen(base_command, "r")
	    if (fp == NULL) {
        	printf("Failed to run command\n" );
        	exit(1);
        }else{
        	printf("Broadcasting")
        }

    }
	return 0;
}

