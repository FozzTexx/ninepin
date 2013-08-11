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
  unsigned char track;
  unsigned char sector;
  unsigned char filetype;
  unsigned char firstTrack;
  unsigned char firstSector;
  char filename[16];
  unsigned char sideTrack;
  unsigned char sideSector;
  unsigned char recordLength;
  unsigned char reserved[6];
  uint16_t sectorCount;
} CBMDirectoryEntry;

typedef struct __attribute__ ((__packed__)) {
  unsigned char nextTrack, nextSector;
  unsigned char data[254];
} CBMFileSector;

#if 0
static int d64SectorCount[] = {
  21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
  19, 19, 19, 19, 19, 19, 19,
  18, 18, 18, 18, 18, 18,
  17, 17, 17, 17, 17
};
#endif

static int d64TrackOffset[] = {
  0x00000, 0x01500, 0x02A00, 0x03F00, 0x05400, 0x06900, 0x07E00, 0x09300,
  0x0A800, 0x0BD00, 0x0D200, 0x0E700, 0x0FC00, 0x11100, 0x12600, 0x13B00,
  0x15000, 0x16500, 0x17800, 0x18B00, 0x19E00, 0x1B100, 0x1C400, 0x1D700,
  0x1EA00, 0x1FC00, 0x20E00, 0x22000, 0x23200, 0x24400, 0x25600, 0x26700,
  0x27800, 0x28900, 0x29A00, 0x2AB00, 0x2BC00, 0x2CD00, 0x2DE00, 0x2EF00
};

void *d64MountDisk(const char *path)
{
  FILE *file;
  int len;
  char *d64image = NULL;


  /* FIXME - check for .d64 extension in path */
  if ((file = localFindFile(path, "r"))) {
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);

    d64image = malloc(len);
    fread(d64image, 1, len, file);
    fclose(file);
  }

  return d64image;
}

CBMDirectoryEntry *d64FindFile(char *d64image, const char *path)
{
  int curTrack, curSect;
  CBMDirectoryEntry *entry;
  int count, ft;
  char filename[20];
  int i;


  curTrack = 18;
  curSect = 1;

  do {
    entry = (CBMDirectoryEntry *) (d64image + d64TrackOffset[curTrack - 1] + curSect * 256);
    for (count = 0; count < 8; count++) {
      ft = entry[count].filetype & 0x0f;
      if (ft >= 1 && ft <= 4) {
	strncpy(filename, entry[count].filename, 16);
	for (i = 15; i >= 0; i--)
	  if (filename[i] != 0xa0)
	    break;
	filename[i+1] = 0;
	if (dosWildcardMatch(path, filename))
	  return &entry[count];
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
  size_t pos;
  int curTrack, curSect;

  
  /* FIXME - don't trust sectorCount, loop through sectors building up file */
  data = malloc(entry->sectorCount * 254);
  curTrack = entry->firstTrack;
  curSect = entry->firstSector;
  pos = 0;
  do {
    sector = (CBMFileSector *) (d64image + d64TrackOffset[curTrack - 1] + curSect * 256);
    memcpy(data + pos, sector->data, 254);
    pos += 254;
    curTrack = sector->nextTrack;
    curSect = sector->nextSector;
  } while (curTrack);

  *buffer = data;
  *length = (entry->sectorCount - 1) * 254 + curSect;

  return;
}

CBMDOSChannel d64OpenFile(void *d64image, const char *path, const char *mode)
{
  CBMDirectoryEntry *entry;
  CBMDOSChannel aChan;
  

  aChan.file = NULL;
  aChan.buffer = NULL;
  aChan.length = 0;
  aChan.sent = 0;
  aChan.cpos = aChan.clen = 0;

  entry = d64FindFile(d64image, path);
  if (entry)
    d64BufferFile(d64image, entry, &aChan.buffer, &aChan.length);

  return aChan;
}
