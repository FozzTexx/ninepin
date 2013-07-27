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

typedef enum {
  CBMOkError = 0,
  CBMFilesScratchedError = 1,
  CBMReadHeaderError = 20,
  CBMReadSyncError = 21,
  CBMReadChecksumError = 22,
  CBMReadBlockError = 23,
  CBMReadOtherError = 24,
  CBMWriteError = 25,
  CBMWriteProtectError = 26,
  CBMRead27Error = 27,
  CBMWriteSyncError = 28,
  CBMDiskIDMismatchError = 29,
  CBMSyntaxError = 30,
  CBMIllegalCommandError = 31,
  CBMCommandOverrunError = 32,
  CBMWildcardError = 33,
  CBMMissingFilenameError = 34,
  CBMUSRFileNotFoundError = 39,
  CBMRecordNotPresentError = 50,
  CBMRecordOverflowError = 51,
  CBMFileTooLargeError = 52,
  CBMWriteFileOpenError = 60,
  CBMFileNotOpenError = 61,
  CBMFileNotFoundError = 62,
  CBMFileExistsError = 63,
  CBMFileTypeMismatchError = 64,
  CBMNoBlockError = 65,
  CBMIllegalBlockError = 66,
  CBMIllegalTrackSector = 67,
  CBMNoChannelError = 70,
  CBMDirectoryError = 71,
  CBMDiskFullError = 72,
  CBMVersionError = 73,
  CBMDriveNotReadyError = 74,
  CBMSpeedError = 75,
} CBMDOSError;

typedef struct {
  CBMDOSError err;
  char *str;
} CBMDOSErrorString;

static CBMDOSErrorString dosErrorStrings[] = {
  {CBMOkError, " OK"},
  {CBMFilesScratchedError, "FILES SCRATCHED"},
  {CBMReadHeaderError, "READ ERROR"},
  {CBMReadSyncError, "READ ERROR"},
  {CBMReadChecksumError, "READ ERROR"},
  {CBMReadBlockError, "READ ERROR"},
  {CBMReadOtherError, "READ ERROR"},
  {CBMWriteError, "WRITE ERROR"},
  {CBMWriteProtectError, "WRITE PROTECT ON"},
  {CBMRead27Error, "READ ERROR"},
  {CBMWriteSyncError, "WRITE ERROR"},
  {CBMDiskIDMismatchError, "DISK ID MISMATCH"},
  {CBMSyntaxError, "SYNTAX ERROR"},
  {CBMIllegalCommandError, "SYNTAX ERROR"},
  {CBMCommandOverrunError, "SYNTAX ERROR"},
  {CBMWildcardError, "SYNTAX ERROR"},
  {CBMMissingFilenameError, "SYNTAX ERROR"},
  {CBMUSRFileNotFoundError, "FILE NOT FOUND"},
  {CBMRecordNotPresentError, "RECORD NOT PRESENT"},
  {CBMRecordOverflowError, "OVERFLOW IN RECORD"},
  {CBMFileTooLargeError, "FILE TOO LARGE"},
  {CBMWriteFileOpenError, "WRITE FILE OPEN"},
  {CBMFileNotOpenError, "FILE NOT OPEN"},
  {CBMFileNotFoundError, "FILE NOT FOUND"},
  {CBMFileExistsError, "FILE EXISTS"},
  {CBMFileTypeMismatchError, "FILE TYPE MISMATCH"},
  {CBMNoBlockError, "NO BLOCK"},
  {CBMIllegalBlockError, "ILLEGAL TRACK OR SECTOR"},
  {CBMIllegalTrackSector, "ILLEGAL TRACK OR SECTOR"},
  {CBMNoChannelError, "NO CHANNEL"},
  {CBMDirectoryError, "DIR ERROR"},
  {CBMDiskFullError, "DISK FULL"},
  {CBMVersionError, "CBM DOS V.26 1541"},
  {CBMDriveNotReadyError, "DRIVE NOT READY"},
  {CBMSpeedError, "FORMAT SPEED ERROR"},
  {0, NULL},
};

/* FIXME - don't use global variables */
static int dosError = 0, dosTrackError = 0, dosSectorError = 0;

void dosFilenameToC64(const char *original, char *c64)
{
  static char *buf = NULL;
  static int buflen = 0;
  int len, elen;
  char *exten;
  int i;


  len = strlen(original);
  if (len+1 > buflen) {
    buflen = ((len + 35) / 32) * 32;
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

int dosWildcardMatch(const char *pattern, const char *str)
{
  const char *s, *p;
  int star = 0;

  
 loopStart:
  for (s = str, p = pattern; *s; ++s, ++p) {
    switch (*p) {
    case '?':
      if (*s == '.')
	goto starCheck;
      break;
    case '*':
      star = 1;
      str = s, pattern = p;
      if (!*++pattern)
	return 1;
      goto loopStart;
    default:
      if (tolower(*s) != tolower(*p))
	goto starCheck;
      break;
    }
  }
  if (*p == '*')
    ++p;
  return (!*p);

 starCheck:
  if (!star)
    return 0;
  str++;
  goto loopStart;
}

FILE *dosFindFile(const char *path, const char *mode)
{
  DIR *dir;
  struct dirent *dp;
  FILE *file = NULL;


  if (!(dir = opendir(".")))
    return NULL;

  for (dp = readdir(dir); dp; dp = readdir(dir))
    if (dosWildcardMatch(path, dp->d_name)) {
      file = fopen(dp->d_name, mode);
      break;
    }

  closedir(dir);
  return file;
}

FILE *dosSendError(char **bufPtr)
{
  FILE *file;
  int i;
  size_t len;


  file = open_memstream(bufPtr, &len);
  for (i = 0; dosErrorStrings[i].str && dosErrorStrings[i].err != dosError; i++)
    ;
  fprintf(file, "%02i,%s,%02i,%02i\r", dosError, dosErrorStrings[i].str,
	  dosTrackError, dosSectorError);
  rewind(file);
  return file;
}
  
FILE *dosOpenFile(const char *path, int channel, const char *mode, char **bufPtr)
{
  int special = 0;
  int driveNum = 0;
  const char *fn, *cn;
  char *filespec;
  
  
  /* Prefix: $#/
     Drive number: decimal
     Colon
     Path with wildcards
     comma followed by type
     comma followed by mode*/

  if (channel == 15 && !strcmp(mode, "r"))
    return dosSendError(bufPtr);
  
  filespec = alloca(strlen(path) + 5);
  strcpy(filespec, path);
  if ((channel == 0 && !strcmp(mode, "r")) || (channel == 1 && !strcmp(mode, "w")))
    strcat(filespec, ".PRG");
  fn = filespec;
  if (*fn == '$' || *fn == '#' || *fn == '/') {
    special = *fn;
    fn++;
  }

  cn = strchr(fn, ':');
  if ((special || cn) && *fn >= '0' && *fn <= '9') {
    driveNum = atoi(fn);
    while (isdigit(*fn))
      fn++;
    if (*fn == ':')
      fn++;
  }

  /* FIXME - remap PETSCII characters */
  /* FIXME - map 16 char names to full names */

  *bufPtr = NULL;
  if (special == '$') {
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


    /* FIXME - check driveNum */
    
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
	dosFilenameToC64(dp->d_name, filename);
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
  else if (special == '#') {
    /* FIXME - load a d64 image into driveNum */
  }
  else if (special == '/') {
    /* FIXME - change directory */
  }
  else
    return dosFindFile(fn, mode);
  
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
      /* FIXME - check command here to set error code. How do I know
	 if it's a read or write though? Oh right, I check the channel
	 or the comma stuff.*/
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
	  
      if ((file = dosOpenFile(filename, chan, "r", &mem))) {
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

