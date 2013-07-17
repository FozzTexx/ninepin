/* Copyright 2013 by Chris Osborn <fozztexx@fozztexx.com>
 *
 * This file is part of ninepin.
 *
 * ninepin is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * ninepin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ninepin; see the file COPYING. If not see
 * <http://www.gnu.org/licenses/>.
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
  test();
  //diskDrive();
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
	  if (header.len) {
	    fprintf(stderr, "Sending %s\n", filename);
	    write(fd, data, header.len);
	  }
	  else
	    fprintf(stderr, "No such file %s\n", filename);
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

#define IEC_ALLDEV	31
#define IEC_BUFSIZE	1024

typedef struct {
  unsigned char data[IEC_BUFSIZE];
  void *next;
} iec_linkedData;

typedef struct {
  iec_data header;
  iec_linkedData *data;
  void *next;
  int pos;
} iec_io;

typedef struct {
  iec_io *head, *tail, *cur;
} iec_chain;

typedef struct {
  iec_chain in, out;
  int flags;
  //struct semaphore lock;
} iec_device;

static iec_device *iec_openDevices[IEC_ALLDEV];
static int iec_deviceCount = 0, iec_curDevice;

void iec_newIO(int val, int inout)
{
  int dev;
  iec_device *device;
  iec_chain *chain;
  iec_io *io;
  iec_linkedData *data;
  

  dev = val & 0x1f;
  device = iec_openDevices[dev];
  if (!device)
    return;

  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  
  io = malloc(sizeof(iec_io));
  io->header.command = val & 0xff;
  io->header.channel = 0;
  io->header.len = 0;
  data = malloc(sizeof(iec_linkedData));
  io->data = data;
  data->next = NULL;
  io->next = NULL;
  io->pos = 0;
  
  if (!chain->head)
    chain->head = chain->tail = io;
  else {
    chain->tail->next = io;
    chain->tail = io;
  }
  chain->cur = io;

  return;
}

void iec_appendData(char *buf, int buflen, int inout)
{
  iec_device *device;
  iec_chain *chain;
  iec_io *io;
  iec_linkedData *data, *last;
  int pos, count;
  

  if (!(device = iec_openDevices[iec_curDevice]))
    return;
  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  if (!(io = chain->cur))
    return;

  while (buflen) {
    last = data = io->data;
    pos = io->header.len;// - sizeof(io->header);
    while (data && pos >= IEC_BUFSIZE) {
      last = data;
      data = data->next;
      pos -= IEC_BUFSIZE;
    }

    count = buflen;
    if (count > IEC_BUFSIZE - pos)
      count = IEC_BUFSIZE - pos;
    if (!data) {
      data = last->next = malloc(sizeof(iec_linkedData));
      data->next = NULL;
    }

    memcpy(&data->data[pos], buf, count);
    io->header.len += count;
    buflen -= count;
    buf += count;
  }

  return;
}

void test()
{
  FILE *file;
  char buf[IEC_BUFSIZE+10];
  int len, minor = 8;
  iec_device *device;


  device = iec_openDevices[minor] = malloc(sizeof(iec_device));    
  device->in.head = device->in.tail = device->in.cur = NULL;
  device->out.head = device->out.tail = device->out.cur = NULL;
  iec_deviceCount++;
  iec_curDevice = minor;
  
  iec_newIO(minor, OUTPUT);
  file = fopen("CONTACT.PRG", "r");
  while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
    iec_appendData(buf, len, OUTPUT);
  }
  fclose(file);
  exit(0);
}
