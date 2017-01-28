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

#include "d64fs.h"
#include "localfs.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct __attribute__ ((__packed__)) {
  unsigned char track, sector;
  unsigned char filetype;
  unsigned char firstTrack, firstSector;
  char filename[16];
  unsigned char sideTrack, sideSector;
  unsigned char recordLength;
  unsigned char reserved[6];
  uint16_t sectorCount; /* FIXME - endianness danger */
} CBMDirectoryEntry;

typedef struct __attribute__ ((__packed__)) {
  unsigned char nextTrack, nextSector;
  unsigned char data[254];
} CBMFileSector;

typedef struct __attribute__ ((__packed__)) {
  unsigned char freeCount;
  unsigned char used[3];
} CBMBAMEntry;

typedef struct __attribute__ ((__packed__)) {
  unsigned char dirTrack, dirSector;
  unsigned char version;
  unsigned char unused;
  CBMBAMEntry entries[35];
  char diskname[16];
  unsigned char padding[2];
  char diskID[2];
  unsigned char unknown;
  unsigned char dosType[2];
  unsigned char padding2[4];
  unsigned char unused2;
  CBMBAMEntry dolphin[5];
  CBMBAMEntry speedDOS[5];
  unsigned char unused3[44];
} CBMBAM;

#if 0
static int d64SectorCount[] = {
  21, 21, 21, 21, 21, 21, 21, 21, 21,	/*  1 -  9 */
  21, 21, 21, 21, 21, 21, 21, 21,	/* 10 - 17 */
  19, 19, 19, 19, 19, 19, 19,		/* 18 - 24 */
  18, 18, 18, 18, 18, 18,		/* 25 - 30 */
  17, 17, 17, 17, 17			/* 31 - 35 */
};
#endif

static int d64TrackOffset[] = {
  0x00000, 0x01500, 0x02A00, 0x03F00, 0x05400, 0x06900, 0x07E00, 0x09300,
  0x0A800, 0x0BD00, 0x0D200, 0x0E700, 0x0FC00, 0x11100, 0x12600, 0x13B00,
  0x15000, 0x16500, 0x17800, 0x18B00, 0x19E00, 0x1B100, 0x1C400, 0x1D700,
  0x1EA00, 0x1FC00, 0x20E00, 0x22000, 0x23200, 0x24400, 0x25600, 0x26700,
  0x27800, 0x28900, 0x29A00, 0x2AB00, 0x2BC00, 0x2CD00, 0x2DE00, 0x2EF00
};

int d64MountDisk(CBMDriveData *data, const char *filename)
{
  FILE *file;
  int len;
  char *d64image = NULL;
  char *altname;


  if (!(file = fopen(filename, "r")) &&
      strlen(filename) > 4 && !strcasecmp(&filename[strlen(filename) - 4], ".d64")) {
    altname = alloca(strlen(filename) + 5);
    strcpy(altname, filename);
    strcat(altname, ".d64");
    file = fopen(altname, "r");
  }
    
  if (file) {
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);

    d64image = malloc(len);
    fread(d64image, 1, len, file);
    fclose(file);
  }

  if (d64image) {
    if (data->image)
      free(data->image);
    data->image = d64image;
    return 1;
  }
  
  return 0;
}

const char *d64Extension(int filetype)
{
  int ft;
  const char *exten = NULL;
  

  /* FIXME - look at other bits in filetype to set additional info */
  ft = filetype & 0x0f;
  switch (ft) {
  case 0x00:
    exten = "DEL";
    break;
  case 0x01:
    exten = "SEQ";
    break;
  case 0x02:
    exten = "PRG";
    break;
  case 0x03:
    exten = "USR";
    break;
  case 0x04:
    exten = "REL";
    break;
  }

  return exten;
}
  
CBMDirectoryEntry *d64FindFile(char *d64image, const char *path)
{
  int curTrack, curSect;
  CBMDirectoryEntry *entry;
  int count, ft;
  char filename[32];
  int i;


  curTrack = 18;
  curSect = 1;

  do {
    entry = (CBMDirectoryEntry *) (d64image + d64TrackOffset[curTrack - 1] + curSect * 256);
    for (count = 0; count < 8; count++) {
      ft = entry[count].filetype & 0x0f;
      if (ft >= 1 && ft <= 4) {
	strncpy(filename, entry[count].filename, 16);
	for (i = 0; i < 16; i++)
	  if (filename[i] == 0xa0)
	    break;
	filename[i] = 0;

	/* Add .PRG to filename since dosOpenFile will automatically append it */
	if (ft == 0x02)
	  strcat(filename, ".PRG");
	
	if (dosWildcardMatch(path, filename)) {
	  fprintf(stderr, "Found \"%s\"\n", filename);
	  return &entry[count];
	}
      }
    }

    curTrack = entry->track;
    curSect = entry->sector;
  } while (curTrack);

  return NULL;
}

void d64BufferFile(char *d64image, CBMDirectoryEntry *entry, char **buffer, size_t *length)
{
  char *data;
  CBMFileSector *sector;
  size_t len;
  int curTrack, curSect;

  
  /* Don't trust sectorCount, loop through sectors building up file */
  curTrack = entry->firstTrack;
  curSect = entry->firstSector;
  len = 0;
  do {
    sector = (CBMFileSector *) (d64image + d64TrackOffset[curTrack - 1] + curSect * 256);
    len += 254;
    curTrack = sector->nextTrack;
    curSect = sector->nextSector;
  } while (curTrack);

  *length = len - 254 + curSect - 1;
  *buffer = data = malloc(len);
  
  curTrack = entry->firstTrack;
  curSect = entry->firstSector;
  do {
    sector = (CBMFileSector *) (d64image + d64TrackOffset[curTrack - 1] + curSect * 256);
    memcpy(data, sector->data, 254);
    data += 254;
    curTrack = sector->nextTrack;
    curSect = sector->nextSector;
  } while (curTrack);
  
  return;
}

CBMDOSChannel d64OpenFile(CBMDriveData *data, const char *path, const char *mode)
{
  CBMDirectoryEntry *entry;
  CBMDOSChannel aChan;
  

  aChan.file = NULL;
  aChan.buffer = NULL;
  aChan.length = 0;
  aChan.sent = 0;
  aChan.cpos = aChan.clen = 0;

  entry = d64FindFile(data->image, path);
  if (entry)
    d64BufferFile(data->image, entry, &aChan.buffer, &aChan.length);

  return aChan;
}

CBMDOSChannel d64GetDirectory(CBMDriveData *data, int driveNum)
{
  size_t len;
  char filename[64];
  off_t blocks;
  int bw, nw, qc;
  const char *exten;
  CBMBAM *bam;
  CBMDOSChannel aChan;
  int curTrack, curSect;
  int count;
  CBMDirectoryEntry *entry;


  aChan.file = open_memstream(&aChan.buffer, &len);
  aChan.buffer = NULL;
  aChan.length = 0;
  aChan.sent = 0;
  aChan.cpos = aChan.clen = 0;

  bam = (CBMBAM *) (data->image + d64TrackOffset[17]);
  fwrite("\001\004\001\001", 4, 1, aChan.file);
  fprintf(aChan.file, "%c%c", driveNum, 0);

  strncpy(filename, bam->diskname, 16);
  for (nw = 15; nw >= 0; nw--)
    if (filename[nw] != 0xa0)
      break;
  filename[nw+1] = 0;
  strcat(filename, "                ");
  filename[16] = 0;
  exten = "FZ";
  if (bam->version == 0x41)
    exten = "2A";
  else if (bam->version == 0x50)
    exten = "2P";
  else if (bam->version == 0x00)
    exten = ".C";
  fprintf(aChan.file, "\022\"%s\" %c%c %s", filename, bam->diskID[0], bam->diskID[1], exten);
  fputc(0x00, aChan.file);

  curTrack = 18;
  curSect = 1;

  do {
    entry = (CBMDirectoryEntry *) (data->image + d64TrackOffset[curTrack - 1] + curSect * 256);
    for (count = 0; count < 8; count++) {
      if (entry[count].filetype &&
	  (entry[count].filetype & 0x0f) >= 0 &&
	  (entry[count].filetype & 0x0f) <= 4) {
	strncpy(filename, entry[count].filename, 16);
	for (nw = 16; nw >= 0; nw--)
	  if (filename[nw] != 0xa0)
	    break;
	filename[nw+1] = 0;
	for (nw = 0, qc = '"'; filename[nw]; nw++)
	  if (filename[nw] == 0xa0 || filename[nw] == '"') {
	    filename[nw] = qc;
	    if (!qc)
	      break;
	    qc = 0;
	  }
	if (qc)
	  strcat(filename, "\"");
	filename[17] = 0;

	fwrite("\001\001", 2, 1, aChan.file);
	blocks = entry[count].sectorCount;
	for (bw = 1, nw = 9; blocks > nw; bw++, nw = nw * 10 + 9)
	  ;
	exten = d64Extension(entry[count].filetype);
	nw = strlen(filename);
	fprintf(aChan.file, "%c%c%*s\"%s%*s%s%c",
		(int) blocks & 0xff, (int) (blocks >> 8) & 0xff,
		4 - bw, " ", filename, 18 - nw, " ", exten, 0x00);
      }
    }
    curTrack = entry->track;
    curSect = entry->sector;
  } while (curTrack);

  for (count = blocks = 0; count < 35; count++) {
    if (count == 17)
      continue;
    blocks += bam->entries[count].freeCount;
  }
  
  fprintf(aChan.file, "\001\001%c%cBLOCKS FREE.              %c%c%c",
	  (int) blocks & 0xff, (int) (blocks >> 8) & 0xff, 0x00, 0x00, 0x00);

  fflush(aChan.file);
  aChan.length = len;
  fclose(aChan.file);
  aChan.file = NULL;

  return aChan;
}
