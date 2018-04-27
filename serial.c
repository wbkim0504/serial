#include <stdio.h>
#include <string.h>
#include <errno.h>
 
#include <wiringPi.h>
#include <wiringSerial.h>
 
#define BAUDRATE	(115200)

int main ()
{
  int fd ;
  int count ;
  unsigned int nextTime ;
 
  fd = serialOpen ("/dev/ttyAMA0", BAUDRATE));
  if (fd < 0) 
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;                   
    return 1 ;
  }
 
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
 
  nextTime = millis () + 300 ;
 
  for (count = 0 ; count < 256 ; )
  {
    if (millis () > nextTime)
    {
      printf ("\nOut: %d: ", count) ;
      fflush (stdout) ;
      serialPutchar (fd, count) ;
      nextTime += 300 ;
      ++count ;
    }
 
    delay (3) ;
 
    while (serialDataAvail (fd))
    {
      printf (" -> %3d", serialGetchar (fd)) ;
      fflush (stdout) ;
    }
  }
 
  printf ("\n") ;
  return 0 ;
}

