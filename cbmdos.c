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

#include "cbmdos.h"
#include "d64fs.h"
#include "localfs.h"
#include "iec/iec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>

typedef struct {
  CBMDOSError err;
  char *str;
} CBMDOSErrorString;

static CBMDOSErrorString dosErrorStrings[] = {
  {CBMOkError, " OK"},
  {CBMFilesScratchedError, "FILES SCRATCHED"},
  {CBMBlockHeaderError, "READ ERROR"},
  {CBMSyncCharacterError, "READ ERROR"},
  {CBMDataBlockError, "READ ERROR"},
  {CBMDataChecksumError, "READ ERROR"},
  {CBMByteDecodingError, "READ ERROR"},
  {CBMWriteVerifyError, "WRITE ERROR"},
  {CBMWriteProtectError, "WRITE PROTECT ON"},
  {CBMHeaderChecksumError, "READ ERROR"},
  {CBMDataExtendsNextBlockError, "WRITE ERROR"},
  {CBMDiskIDMismatchError, "DISK ID MISMATCH"},
  {CBMSyntaxError, "SYNTAX ERROR"},
  {CBMInvalidCommandError, "SYNTAX ERROR"},
  {CBMCommandOverrunError, "SYNTAX ERROR"},
  {CBMInvalidFilenameError, "SYNTAX ERROR"},
  {CBMMissingFilenameError, "SYNTAX ERROR"},
  {CBMCommandFileNotFoundError, "FILE NOT FOUND"},
  {CBMRecordNotPresentError, "RECORD NOT PRESENT"},
  {CBMRecordOverflowError, "OVERFLOW IN RECORD"},
  {CBMFileTooLargeError, "FILE TOO LARGE"},
  {CBMFileAlreadyOpenError, "WRITE FILE OPEN"},
  {CBMFileNotOpenError, "FILE NOT OPEN"},
  {CBMFileNotFoundError, "FILE NOT FOUND"},
  {CBMFileExistsError, "FILE EXISTS"},
  {CBMFileTypeMismatchError, "FILE TYPE MISMATCH"},
  {CBMNoBlockError, "NO BLOCK"},
  {CBMIllegalTrackSectorError, "ILLEGAL TRACK OR SECTOR"},
  {CBMIllegalSystemTrackSectorError, "ILLEGAL TRACK OR SECTOR"},
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
static CBMDOSChannel dosChannels[16];
static CBMDrive dosDrive[4] = {
  {{NULL, NULL}, NULL, NULL},
  {{NULL, NULL}, NULL, NULL},
  {{NULL, NULL}, NULL, NULL},
  {{NULL, NULL}, NULL, NULL},
};
static int curDrive = 0;

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

CBMDOSChannel dosSendError()
{
  int i;
  size_t len;
  CBMDOSChannel aChan;


  aChan.file = open_memstream(&aChan.buffer, &len);
  aChan.sent = 0;
  for (i = 0; dosErrorStrings[i].str && dosErrorStrings[i].err != dosError; i++)
    ;
  fprintf(aChan.file, "%02i,%s,%02i,%02i\r", dosError, dosErrorStrings[i].str,
	  dosTrackError, dosSectorError);
  fflush(aChan.file);
  aChan.length = len;
  fclose(aChan.file);
  aChan.file = NULL;
  return aChan;
}
  
CBMDOSChannel dosOpenFile(const char *path, int channel)
{
  int special = 0;
  int driveNum = 0;
  char *filespec, *fn, *cn;
  CBMDOSChannel aChan;
  char mode[10];
  

  aChan.file = NULL;
  aChan.buffer = NULL;
  aChan.length = 0;
  aChan.sent = 0;
  aChan.cpos = aChan.clen = 0;
  
  /* Prefix: $#/
     Drive number: decimal
     Colon
     Path with wildcards
     comma followed by type
     comma followed by mode*/

  filespec = alloca(strlen(path) + 5);
  strcpy(filespec, path);
  strcpy(mode, "r");

  fn = filespec;
  if (*fn == '$' || *fn == '#' || *fn == '/') {
    special = *fn;
    fn++;
  }

  if ((channel == 0 || channel == 1) && !special) {
    strcat(fn, ".PRG");
    if (channel == 1)
      strcpy(mode, "w");
  }
  
  cn = strchr(fn, ':');
  if ((special || cn) && (*fn == ':' || (*fn >= '0' && *fn <= '9'))) {
    driveNum = atoi(fn);
    while (isdigit(*fn))
      fn++;
    if (*fn == ':')
      fn++;
  }

  if ((cn = strchr(fn, ','))) {
    /* FIXME - do something with file type? */
    *cn = 0;
    cn++;
    if ((cn = strchr(cn, ','))) {
      cn++;
      if (tolower(*cn) == 'w' || tolower(*cn) == 'a')
	mode[0] = tolower(*cn);
    }
  }
  
  /* FIXME - remap PETSCII characters */
  /* FIXME - map 16 char names to full names */

  if (special == '$')
    aChan = dosDrive[curDrive].listDirectory(&dosDrive[curDrive].data, driveNum);
  else if (special == '#')
    dosMountDisk(fn, curDrive);
  else if (special == '/') {
    /* FIXME - tell user it's an error to change dirs while d64 is
       mounted */
    if (!dosDrive[curDrive].data.image)
      dosDrive[curDrive].cd(&dosDrive[curDrive].data, fn);
    /* FIXME - check if cd was successful */
  }
  else
    aChan = dosDrive[curDrive].opener(&dosDrive[curDrive].data, fn, mode);
  
  return aChan;
}

void dosPrintCommand(iec_data header)
{
  int cmd, dev, sub, chan;

  
  cmd = header.command & 0xe0;
  dev = header.command & 0x1f;
  sub = header.channel & 0xf0;
  chan = header.channel & 0x0f;

  fprintf(stderr, "Device:  %i:%i\n", dev, chan);
  fprintf(stderr, "Command: ");
  switch (cmd) {
  case IECListenCommand:
    fprintf(stderr, "Listen");
    break;
    
  case IECTalkCommand:
    fprintf(stderr, "Talk");
    break;

  default:
    fprintf(stderr, "Unknown %02X", cmd);
    break;
  }

  fprintf(stderr, "/");

  switch (sub) {
  case IECChannelCommand:
    fprintf(stderr, "Channel");
    break;
    
  case IECCloseCommand:
    fprintf(stderr, "Close");
    break;

  case IECOpenCommand:
    fprintf(stderr, "Open");
    break;

  default:
    fprintf(stderr, "Unknown %02X", sub);
    break;
  }
  fprintf(stderr, "\n");

  fprintf(stderr, "Length:  %i\n", header.len);
  fprintf(stderr, "EOI:     %s\n", header.eoi ? "Yes" : "No");

  return;
}

void dosHandleIO(int fd)
{
  int len;
  iec_data header;
  unsigned char *data = NULL;
  int dlen = 0;
  int cmd, /*dev,*/ sub, chan;
  static struct timeval start;
  CBMDOSChannel *aChan;
  static unsigned char serial;


  len = read(fd, &header, sizeof(header));
  if (dlen < header.len + 1) {
    dlen = header.len + 1;
    data = realloc(data, dlen);
  }
  if (header.len) {
    len = read(fd, data, header.len);
    data[header.len] = 0;
  }

  cmd = header.command & 0xe0;
#if 0
  dev = header.command & 0x1f;
#endif
  sub = header.channel & 0xf0;
  chan = header.channel & 0x0f;
  serial = header.serial;
  aChan = &dosChannels[chan];

  dosPrintCommand(header);

  switch (cmd) {
  case IECListenCommand:
    switch (sub) {
    case IECOpenCommand:
      fprintf(stderr, "Opening chan %i \"%s\"\n", chan, data);
      *aChan = dosOpenFile((char *) data, chan);
      if (!aChan->file && !aChan->buffer)
	dosError = CBMFileNotFoundError;
      else
	dosError = CBMOkError;
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

	bps = aChan->sent;
	bps /= seconds;

	if (aChan->file)
	  fclose(aChan->file);
	if (aChan->buffer)
	  free(aChan->buffer);
	aChan->file = NULL;
	aChan->buffer = NULL;
	aChan->length = 0;
	
	fprintf(stderr, "Sent %i bytes in %.2f seconds, %.2f bytes/sec\n",
		aChan->sent, seconds, bps);
      }
      break;

    case IECChannelCommand:
      /* Save data */
      if (aChan->file)
	fwrite(data, header.len, 1, aChan->file);
      else {
	int i;	
	fprintf(stderr, "Trying to write to read-only filesystem\n");
	for (i = 0; i < header.len; i++)
	  fprintf(stderr, " %02x", data[i]);
	fprintf(stderr, "\n");
      }
      break;

    default:
      fprintf(stderr, "Unimplemented subcommand %02x\n", sub);
      break;
    }
    break;

  case IECTalkCommand:
    if (sub == IECChannelCommand) {
      int wlen;


      gettimeofday(&start, NULL);
      if (chan == 15) 
	dosChannels[chan] = dosSendError();

      header.serial = serial;
      if (aChan->buffer || aChan->file) {
	while (aChan->sent < aChan->length) {
	  if (aChan->cpos < aChan->clen)
	    len = aChan->clen - aChan->cpos;
	  else {
	    if (aChan->file) {
	      len = fread(aChan->cache, 1, sizeof(aChan->cache), aChan->file);
	      if (len <= 0)
		break;
	    }
	    else {
	      len = aChan->length - aChan->sent;
	      if (len > sizeof(aChan->cache))
		len = sizeof(aChan->cache);
	      memmove(aChan->cache, aChan->buffer + aChan->sent, len);
	    }
	    aChan->cpos = 0;
	    aChan->clen = len;
	  }

	  header.command = 0;
	  header.channel = chan;
	  header.len = len;
	  header.eoi = aChan->sent+len == aChan->length;
	  
	  write(fd, (void *) &header, sizeof(header));
	  wlen = write(fd, &aChan->cache[aChan->cpos], len);
	  aChan->sent += wlen;
	  aChan->cpos += wlen;
	  fprintf(stderr, "\rSending %u bytes: %u%%", aChan->length,
		  (aChan->sent * 100) / aChan->length);
	  if (wlen < len)
	    break;
	}
	fprintf(stderr, "\n");
      }
      else {
	header.command = 0;
	header.channel = chan;
	header.len = 0;
	header.eoi = 0;
	write(fd, (void *) &header, sizeof(header));
      }
    }
    break;
  }

  return;
}

void dosSwapDrive(int newDrive)
{
  curDrive = newDrive;
  fprintf(stderr, "Swapping to drive %i\n", newDrive + 8);
  return;
}

int dosCurrentDrive()
{
  return curDrive;
}

void dosMountDisk(const char *filename, int drive)
{
  struct stat st;
  char *fullpath;


  if (!filename || !*filename) {
    if (dosDrive[drive].data.image)
      free(dosDrive[drive].data.image);
    dosDrive[drive].data.image = NULL;
    dosDrive[drive].opener = localOpenFile;
    dosDrive[drive].listDirectory = localGetDirectory;
  }
  else {
    fullpath = localFindPath(dosDrive[drive].data.directory, filename);
    if (stat(fullpath, &st) == 0) {
      if (S_ISREG(st.st_mode)) {
	if (d64MountDisk(&dosDrive[drive].data, fullpath)) {
	  dosDrive[drive].opener = d64OpenFile;
	  dosDrive[drive].listDirectory = d64GetDirectory;
	}
      }
      else if (S_ISDIR(st.st_mode)) {
	/* FIXME - tell user it's an error to change dirs while d64 is
	   mounted */
	if (!dosDrive[drive].data.image)
	  dosDrive[drive].cd(&dosDrive[drive].data, filename);
      }
    }
    free(fullpath);
  }

  return;
}

void dosInitDrives()
{
  int drive;


  for (drive = 0; drive < 4; drive++) {
    dosDrive[drive].data.directory = strdup(".");
    dosDrive[drive].data.image = NULL;
    dosDrive[drive].opener = localOpenFile;
    dosDrive[drive].listDirectory = localGetDirectory;
    dosDrive[drive].cd = localChangeDirectory;
  }
  
  return;
}
