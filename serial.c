#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
 
#include <wiringPi.h>
#include <wiringSerial.h>
 
#define BAUDRATE	(115200)
#define buff_size	(256)

int fd_uart;
int timer_count = 0;

void check_rx(int fd)
{
	int ndata;

	ndata = serialDataAvail (fd);
	if (ndata > 0)
	{
		int i;
		char buff[buff_size];

		//fprintf(stdout, "ndata = %d \n", ndata);
		if (ndata > (buff_size-1))
			ndata = buff_size - 1;

		for (int i=0; i<ndata; i++)
		{
			buff[i] = serialGetchar(fd); 
		}
		buff[ndata] = '\0';
		fprintf(stdout, "RX -> %s", buff);
	}
}


void timer_100ms()
{
	check_rx(fd_uart);
}

void timer_10ms(int sig_num)
{
	if (sig_num == SIGALRM)
	{
		timer_count++;
		if (timer_count >= 10)
		{
			timer_count = 0;
			timer_100ms();
		}
	}
}

int main ()
{
	int fd;
	int count ;

	fd = serialOpen ("/dev/ttyAMA0", BAUDRATE);
	if (fd < 0) 
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
		return -1 ;
	}

	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return -1 ;
	}

	fd_uart = fd;
	signal(SIGALRM, timer_10ms);
	ualarm(10000,10000);
	
	while(1)
	{
		int ndata;
		char buff[256];

		fgets(buff, 256, stdin);
		ndata = strlen(buff);
		if (ndata <= 1)
		{
			break;
		}
		else
		{
			/*
			for (int i=0; i<ndata; i++)
				serialPutchar(fd, buff[i]);
			*/
			serialPuts(fd, buff);

			//delay(3);
		}
	}
	return 0 ;
}

