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

#ifndef _CBMDOS_H
#define _CBMDOS_H

#include <stdio.h>

typedef struct {
  FILE *file;
  char *buffer;
  size_t length, sent;
  char cache[256];
  int cpos, clen;
} CBMDOSChannel;

typedef struct {
  char *directory;
  void *image;
} CBMDriveData;

typedef CBMDOSChannel (*CBMFileOpener)(CBMDriveData *, const char *, const char *);
typedef CBMDOSChannel (*CBMDirListing)(CBMDriveData *, int);
typedef int (*CBMChangeDir)(CBMDriveData *, const char *);

typedef struct {
  CBMDriveData data;
  CBMFileOpener opener;
  CBMDirListing listDirectory;
  CBMChangeDir cd;
} CBMDrive;

typedef enum {
  CBMOkError = 0,
  CBMFilesScratchedError = 1,
  CBMBlockHeaderError = 20,
  CBMSyncCharacterError = 21,
  CBMDataBlockError = 22,
  CBMDataChecksumError = 23,
  CBMByteDecodingError = 24,
  CBMWriteVerifyError = 25,
  CBMWriteProtectError = 26,
  CBMHeaderChecksumError = 27,
  CBMDataExtendsNextBlockError = 28,
  CBMDiskIDMismatchError = 29,
  CBMSyntaxError = 30,
  CBMInvalidCommandError = 31,
  CBMCommandOverrunError = 32,
  CBMInvalidFilenameError = 33,
  CBMMissingFilenameError = 34,
  CBMCommandFileNotFoundError = 39,
  CBMRecordNotPresentError = 50,
  CBMRecordOverflowError = 51,
  CBMFileTooLargeError = 52,
  CBMFileAlreadyOpenError = 60,
  CBMFileNotOpenError = 61,
  CBMFileNotFoundError = 62,
  CBMFileExistsError = 63,
  CBMFileTypeMismatchError = 64,
  CBMNoBlockError = 65,
  CBMIllegalTrackSectorError = 66,
  CBMIllegalSystemTrackSectorError = 67,
  CBMNoChannelError = 70,
  CBMDirectoryError = 71,
  CBMDiskFullError = 72,
  CBMVersionError = 73,
  CBMDriveNotReadyError = 74,
  CBMSpeedError = 75,
} CBMDOSError;

extern void dosHandleIO(int fd);
extern int dosWildcardMatch(const char *pattern, const char *str);
extern void dosSwapDrive(int newDrive);
extern int dosCurrentDrive();
extern void dosMountDisk(const char *filename, int drive);
void dosInitDrives();

#endif /* _CBMDOS_H */
