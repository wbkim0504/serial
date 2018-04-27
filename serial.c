#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
 
#include <wiringPi.h>
#include <wiringSerial.h>
 
#define BAUDRATE	(115200)

int serial_fd;

int init_uart()
{
	int fd;
	int count ;
	unsigned int nextTime ;

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
	return fd;
}

#define buff_size	(256)

void timer_10ms(int sig_num)
{
	if (sig_num == SIGALRM)
	{
		int ndata;

		ndata = serialDataAvail (serial_fd);
		if (ndata > 0)
		{
			int i;
			char buff[buff_size];

			fprintf(stdout, "ndata = %d \n", ndata);
			if (ndata > (buff_size-1))
				ndata = buff_size - 1;

			for (int i=0; i<ndata; i++)
			{
				buff[i] = serialGetchar(serial_fd); 
			}
			buff[ndata] = '\0';
			fprintf(stdout, "RX -> %s", buff);
		}	
	}
}


int main ()
{
	if (serial_fd = init_uart()< 0)
	{
		fprintf (stdout, "Unable to init uart device \n");
		fflush(stdout);
		return 1;
	}

	//signal(SIGALRM, timer_10ms);
	//ualarm(10000,10000);
	
	while(1)
	{
		char buff[256];

		fgets(buff, 256, stdin);
		if (buff[0] == '\n')
		{
			break;
		}
		else
		{
			serialPuts(serial_fd, buff);
		}
	}
	return 0 ;
}

