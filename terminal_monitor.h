#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<termios.h>
#include<signal.h>
#include<time.h>
#include<sys/types.h>
#include<ctype.h>
#include<sys/stat.h>

#define bypass 84
#define deviceport "/dev/ttyACM0"
#define boudrate B115200

static int fd=-1;
static int exp=0;

int connect_device(int vtime){
        struct termios oldtio, newtio;
        fd=open(deviceport,O_RDWR | O_NOCTTY);
        tcgetattr(fd,&oldtio);
        memset(&newtio,0,sizeof(newtio));
        struct termios tio;
        memset(&tio,0,sizeof(tio));
        tio.c_cflag=CS8 | CLOCAL | CREAD;
        tio.c_cc[VTIME]=vtime;
        tio.c_iflag=IGNPAR | ICRNL;
        cfsetispeed(&tio,boudrate);
        cfsetospeed(&tio,boudrate);
        tcsetattr(fd,TCSANOW,&tio);
        return fd;
}

int close_device(){
	int revalue=-1;
	revalue=close(fd);
	return revalue;
}

void scaner(){
	char writing[8]={255,65,bypass,0,0,0,0,255};
	int id,cmd,data,boole;
	int16_t tx_data;
	uint8_t tx_dataL,tx_dataH,tx_sum;
	printf("ID:");
	scanf("%d",&id);
	tx_data=id;
	tx_dataL=tx_data;
	tx_dataH=tx_data >> 8;
	tx_sum=bypass+1+tx_dataL+tx_dataH;
	writing[3]=1;
	writing[4]=tx_dataL;
	writing[5]=tx_dataH;
	writing[6]=tx_sum;
	write(fd,&writing,8);
	printf("CMD:");
        scanf("%d",&cmd);
        tx_data=cmd;
        tx_dataL=tx_data;
        tx_dataH=tx_data >> 8;
        tx_sum=bypass+2+tx_dataL+tx_dataH;
        writing[3]=2;
        writing[4]=tx_dataL;
        writing[5]=tx_dataH;
        writing[6]=tx_sum;
        write(fd,&writing,8);
	printf("DATA:");
        scanf("%d",&data);
        tx_data=data;
        tx_dataL=tx_data;
        tx_dataH=tx_data >> 8;
        tx_sum=bypass+3+tx_dataL+tx_dataH;
        writing[3]=3;
        writing[4]=tx_dataL;
        writing[5]=tx_dataH;
        writing[6]=tx_sum;
        write(fd,&writing,8);
	
	printf("True?");
	scanf("%d",&boole);
	if(boole!=1){
		return;
	}
	tx_data=1;
	tx_dataL=tx_data;
	tx_dataH=tx_data >> 8;
	tx_sum=bypass+4+tx_dataL+tx_dataH;
	writing[3]=4;
	writing[4]=tx_dataL;
	writing[5]=tx_dataH;
	writing[6]=tx_sum;
	write(fd,&writing,8);
}

void send(int id,int cmd,int data){
	int16_t tx_data=data;
	uint8_t tx_dataL=tx_data,tx_dataH=tx_data >> 8;
	uint8_t tx_sum=id+cmd+tx_dataL+tx_dataH;
	char writing[8]={255,65,id,cmd,tx_dataL,tx_dataH,tx_sum,255};
	write(fd,&writing,8);
}
