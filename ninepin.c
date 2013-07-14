/* Copyright 2013 by Chris Osborn <fozztexx@fozztexx.com>
 *
 */

#include "iec/iec.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>
#include <sys/param.h>
#include <string.h>

#define JOYSTICK	0
#define ATARI_UP	0x01
#define ATARI_DOWN	0x02
#define ATARI_LEFT	0x04
#define ATARI_RIGHT	0x08
#define ATARI_FIRE	0x10

#define SREG_STORE	21
#define SREG_SHIFT	22
#define SREG_DATA	23

void writeBits(int output);
void diskDrive();

int main(int argc, char *argv[])
{
#if JOYSTICK
  int joyfd;
  int numAxes = 0, numButtons = 0;
  int *axis, *button;
  struct js_event js;
  int deadzone = 16384;
  int output = 0, last = 0;
#endif

  
  /* FIXME - read CLI arguments */

#if 0
  {
    char buf[256];

  
    sprintf(buf, "gpio export %i out", SREG_STORE);
    system(buf);
    sprintf(buf, "gpio export %i out", SREG_SHIFT);
    system(buf);
    sprintf(buf, "gpio export %i out", SREG_DATA);
    system(buf);
    wiringPiSetupSys();
  }
#else
#if JOYSTICK
  wiringPiSetupGpio();
  pinMode(SREG_STORE, OUTPUT);
  pinMode(SREG_SHIFT, OUTPUT);
  pinMode(SREG_DATA, OUTPUT);
#endif
#endif

#if JOYSTICK
  digitalWrite(SREG_STORE, LOW);
  digitalWrite(SREG_SHIFT, LOW);
  writeBits(0);

  if (numAxes) {
    fprintf(stderr, "WTF: %i\n", numAxes);
    exit(-1);
  }

  if ((joyfd = open("/dev/input/js0", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open joystick\n");
    exit(-1);
  }

  ioctl(joyfd, JSIOCGAXES, &numAxes);
  ioctl(joyfd, JSIOCGBUTTONS, &numButtons);

  fprintf(stderr, "Axes: %i  Buttons: %i\n", numAxes, numButtons);
  
  axis = calloc(numAxes, sizeof(int));
  button = calloc(numButtons, sizeof(int));
#endif

#if !JOYSTICK
  diskDrive();
#endif
  
  for (;;) {
#if JOYSTICK
    /* FIXME - check for read error, maybe user unplugged joystick */
    read(joyfd, &js, sizeof(struct js_event));

    switch (js.type & ~JS_EVENT_INIT) {
    case JS_EVENT_AXIS:
      axis[js.number] = js.value;
      break;
	
    case JS_EVENT_BUTTON:
      button[js.number] = js.value;
      break;
    }

    /* Dpad shows up as axes 4&5 */

    output &= ~ATARI_UP;
    output |= (axis[5] < -deadzone) * ATARI_UP;
    output &= ~ATARI_DOWN;
    output |= (axis[5] > deadzone) * ATARI_DOWN;
    output &= ~ATARI_LEFT;
    output |= (axis[4] < -deadzone) * ATARI_LEFT;
    output &= ~ATARI_RIGHT;
    output |= (axis[4] > deadzone) * ATARI_RIGHT;
    output &= ~ATARI_FIRE;
    output |= button[0] * ATARI_FIRE;

    if (output != last) {
      writeBits(output);
      printf("Output: 0x%02x\r", output);
      fflush(stdout);
      last = output;
    }
#endif
  }
  
  /* FIXME - update console joystick port */

  fprintf(stderr, "Out of infinite loop!\n");
  
  exit(0);
}

void writeBits(int output)
{
  int i, bit;


  /* Write the high bits first */
  for (i = 0; i < 16; i++) {
    bit = (output & 0x8000) >> 15;
    output <<= 1;
    digitalWrite(SREG_DATA, bit);
    digitalWrite(SREG_SHIFT, 1);
    digitalWrite(SREG_SHIFT, 0);
  }

  digitalWrite(SREG_STORE, 1);
  digitalWrite(SREG_STORE, 0);
  return;
}

void diskDrive()
{
  int fd;
  fd_set rd, ex;
  int len;
  iec_data header;
  unsigned char *data = NULL;
  int dlen = 0;
  int cmd, dev, sub, chan;
  char filename[1024], exten[10];
  FILE *file;
  char buf[256];


  fd = open("/dev/iec8", O_RDWR);

  for (;;) {
    FD_ZERO(&rd);
    FD_SET(fd, &rd);
    ex = rd;
    select(NOFILE, &rd, NULL, &ex, NULL);
    if (FD_ISSET(fd, &rd)) {
      len = read(fd, &header, sizeof(header));
      fprintf(stderr, "Data %i\n", header.len);
      if (dlen < header.len + 1) {
	dlen = header.len + 1;
	data = realloc(data, dlen);
      }
      if (header.len) {
	len = read(fd, data, header.len);
	data[header.len] = 0;
      }

      cmd = header.command & 0xe0;
      dev = header.command & 0x1f;
      sub = header.channel & 0xf0;
      chan = header.channel & 0x0f;
      fprintf(stderr, "Command: %02x %02x %02x %02x\n", cmd, dev, sub, chan);
      
      switch (cmd) {
      case IECListenCommand:
	switch (sub) {
	case IECOpenCommand:
	  fprintf(stderr, "Opening %s\n", data);
	  if (header.len == 1 && data[0] == '$') {
	    /* Directory listing */
	  }
	  else {
	    /* FIXME - make sure filename fits */
	    strcpy(filename, (char *) data);
	    /* Will wait until next command to see if it is a read or
	       write and check if the file exists */
	  }
	  break;

	case IECCloseCommand:
	  break;

	case IECChannelCommand:
	  /* Save data */
	  if (chan == 1)
	    strcpy(exten, "PRG");
	  strcat(filename, ".");
	  strcat(filename, exten);
	  file = fopen(filename, "w");
	  fwrite(data, header.len, 1, file);
	  fclose(file);
	  fprintf(stderr, "Saved it to %s\n", filename);
	  break;
	}
	break;

      case IECTalkCommand:
	if (sub == IECChannelCommand) {
	  if (chan == 0)
	    strcpy(exten, "PRG");
	  strcat(filename, ".");
	  strcat(filename, exten);
	  fprintf(stderr, "Sending %s\n", filename);
	  header.command = 0;
	  header.channel = chan;
	  header.len = 0;
	  
	  if ((file = fopen(filename, "r"))) {
	    while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
	      if (dlen < header.len + len) {
		dlen = header.len + len;
		data = realloc(data, dlen);
	      }

	      memmove(&data[header.len], buf, len);
	      header.len += len;
	    }
	    fclose(file);
	  }
	  
	  write(fd, (void *) &header, sizeof(header));
	  if (header.len)
	    write(fd, data, header.len);
	}
	break;
      }
    }

    if (FD_ISSET(fd, &ex)) {
      fprintf(stderr, "Error with IEC\n");
    }
  }

  return;
}
