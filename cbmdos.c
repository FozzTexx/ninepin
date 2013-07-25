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
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

void dosMakeC64Name(const char *original, char *c64)
{
  static char *buf = NULL;
  static int buflen = 0;
  int len, elen;
  char *exten;
  int i;


  len = strlen(original);
  if (len+1 > buflen) {
    buflen = ((len + 32) / 32) * 32;
    buf = realloc(buf, len);
  }
  strcpy(buf, original);

  /* FIXME - deal with unofficial UTF8 PETSCII mapping */  
  for (i = 0; i < len; i++)
    buf[i] = toupper(buf[i]);
  if (!(exten = strrchr(buf, '.'))) {
    elen = len;
    exten = "USR";
  }
  else {
    elen = exten - buf;
    exten++;

    if (!strcmp(exten, "TXT"))
      strcpy(exten, "SEQ");

    if (strcmp(exten, "SEQ") && strcmp(exten, "USR") &&	strcmp(exten, "PRG")) {
      elen = len;
      exten = "USR";
    }
  }

  if (elen > 16) {
    elen = 14;
    buf[elen] = '#';
    buf[elen+1] = '1';
    /* FIXME - enumerate all files in directory and make sure this isn't a duplicate */
  }

  buf[elen] = '.';
  strncpy(c64, buf, elen);
  c64[elen] = '.';
  c64[elen+1] = 0;
  strcat(c64, exten);
  return;
}

char *dosFindFile(const char *path, int ignoreCase)
{
  /* FIXME - remap characters */
  /* FIXME - search directory if ignoreCase is YES */
  /* FIXME - map 16 char names to full names */
  /* FIXME - deal with wildcards */
  return NULL;
}

FILE *dosOpenFile(const char *path, int channel, int ignoreCase, char *mode, char **bufPtr)
{
  char *fixedPath;


  *bufPtr = NULL;
  if (!strcmp(path, "$") || !strcmp(path, "$0")) {
    DIR *dir;
    struct dirent *dp;
    size_t len;
    FILE *file;
    char *buf, *slash;
    char filename[32];
    struct stat st;
    off_t blocks;
    int bw, nw;
    char *exten;


    /* FIXME - setup buffer */
    if (!(dir = opendir(".")))
      return NULL;

    file = open_memstream(bufPtr, &len);
    fwrite("\001\004\001\001", 4, 1, file);
    fwrite("\000\000", 2, 1, file); /* Drive number */

    buf = getcwd(NULL, 0);
    slash = strrchr(buf, '/') + 1;
    strncpy(filename, slash, 22);
    filename[22] = 0;
    for (bw = 0; filename[bw]; bw++)
      filename[bw] = toupper(filename[bw]);
    fprintf(file, "\022\"%s\"%*s", filename, 22 - strlen(filename), " ");
    fputc(0x00, file);
    free(buf);
    
    for (dp = readdir(dir); dp; dp = readdir(dir))
      if (dp->d_name[0] != '.') {
	fwrite("\001\001", 2, 1, file);
	stat(dp->d_name, &st);
	blocks = (st.st_size + 255) / 256;
	if (blocks > 65535)
	  blocks = 65535;
	for (bw = 1, nw = 9; blocks > nw; bw++, nw = nw * 10 + 9)
	  ;
	dosMakeC64Name(dp->d_name, filename);
	exten = strrchr(filename, '.');
	nw = exten - filename;
	*exten = 0;
	exten++;
	fprintf(file, "%c%c%*s\"%.16s\"%*s%s%*s%c",
		(int) blocks & 0xff, (int) (blocks >> 8) & 0xff,
		5 - bw, " ", filename, 22 - nw, " ", exten, bw, " ", 0x00);
      }

    fprintf(file, "\001\001%c%cBLOCKS FREE.              %c%c%c",
	    0xff, 0xff, 0x00, 0x00, 0x00);

    rewind(file);
    return file;
  }
  else if ((fixedPath = dosFindFile(path, ignoreCase)))
    return fopen(fixedPath, mode);
  
  return NULL;
}

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
  char *mem;
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
      /* FIXME - make sure filename fits */
      strcpy(filename, (char *) data);
      /* Will wait until next command to see if it is a read or
	 write and check if the file exists */
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
      header.command = 0;
      header.channel = chan;
      header.len = 0;
	  
      if ((file = dosOpenFile(filename, chan, 1, "r", &mem))) {
	fprintf(stderr, "Sending %s\n", filename);
	gettimeofday(&start, NULL);
	
	fseek(file, 0, SEEK_END);
	header.len = ftell(file);
	rewind(file);
      }
      else
	fprintf(stderr, "No such file %s\n", filename);
	  
      write(fd, (void *) &header, sizeof(header));
      
      if (file) {
	while ((len = fread(buf, 1, sizeof(buf), file)) > 0)
	  write(fd, buf, len);
	fclose(file);
	if (mem)
	  free(mem);
	sentLen = header.len;
      }
    }
    break;
  }

  return;
}

