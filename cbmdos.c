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
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

extern void dosHandleIO(int fd)
{
  int len;
  iec_data header;
  unsigned char *data = NULL;
  int dlen = 0;
  int cmd, dev, sub, chan;
  char filename[1024], exten[10];
  FILE *file;
  char buf[256];
  static struct timeval start;
  static int sentLen;


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
      {
	int delay;
	struct timeval now;
	double bps, seconds;


	gettimeofday(&now, NULL);
	delay = now.tv_sec - start.tv_sec;
	delay *= 1000000;
	delay += now.tv_usec - start.tv_usec;
	seconds = delay;
	seconds /= 1000000;

	bps = sentLen;
	bps /= seconds;
	    
	fprintf(stderr, "Sent %i bytes in %.2f seconds, %.2f bytes/sec\n",
		sentLen, seconds, bps);
      }
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
	gettimeofday(&start, NULL);
	sentLen = header.len;
	fprintf(stderr, "Sending %s\n", filename);
	write(fd, data, header.len);
      }
      else
	fprintf(stderr, "No such file %s\n", filename);
    }
    break;
  }

  return;
}
