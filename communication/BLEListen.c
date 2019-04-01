#include<stdio.h>
#include <stdlib.h>

 
int main()
{
    FILE *fp;
    char var[1035];

    /* Open the command for reading. */
    setup_whitelist_1 = popen("sudo hcitool lescan lewladd B8:27:EB:16:89:BB", "r");
    setup_whitelist_2 = popen("sudo hcitool lescan lewladd B8:27:EB:D3:61:02", "r")

    fp = popen("sudo hcitool lescan --whitelist", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    while (fgets(var, sizeof(var), fp) != NULL) 
    {
      printf("%s", var);
    }
    pclose(fp);

    outputfile = fopen("text.txt", "a");
    fprintf(outputfile,"%s\n",var);
    fclose(outputfile);
    /* close */
    pclose(fp);

 
    return 0;
}