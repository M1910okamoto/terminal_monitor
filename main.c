#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<termios.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"terminal_monitor.h"

#define vtime 50

int main(void){
	connect_device(vtime);
	while(1){
		system("clear");
		scaner();
	}
	close_device();
	return 0;
}
