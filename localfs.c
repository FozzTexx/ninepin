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

#include "localfs.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/param.h>

FILE *localFindFile(const char *directory, const char *path, const char *mode)
{
  DIR *dir;
  struct dirent *dp;
  FILE *file = NULL;


  if (!(dir = opendir(directory)))
    return NULL;

  for (dp = readdir(dir); dp; dp = readdir(dir))
    if (dosWildcardMatch(path, dp->d_name)) {
      file = fopen(dp->d_name, mode);
      break;
    }

  closedir(dir);
  return file;
}

CBMDOSChannel localOpenFile(CBMDriveData *data, const char *path, const char *mode)
{
  CBMDOSChannel aChan;

  
  aChan.file = NULL;
  aChan.buffer = NULL;
  aChan.length = 0;
  aChan.sent = 0;
  aChan.cpos = aChan.clen = 0;

  if (mode[0] == 'w') {
    /* FIXME - don't allow writing to existing files */
    aChan.file = fopen(path, mode);
  }
  else {
    aChan.file = localFindFile(data->directory, path, mode);
    if (aChan.file && mode[0] == 'r') {
      fseek(aChan.file, 0, SEEK_END);
      aChan.length = ftell(aChan.file);
      rewind(aChan.file);
    }
  }

  return aChan;
}

void localFilenameToC64(const char *original, char *c64)
{
  static char *buf = NULL;
  static int buflen = 0;
  int len, elen;
  char *exten;
  int i;


  len = strlen(original);
  if (len+1 > buflen) {
    buflen = ((len + 35) / 32) * 32;
    buf = realloc(buf, buflen);
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

CBMDOSChannel localGetDirectory(CBMDriveData *data, int driveNum)
{
  DIR *dir;
  struct dirent *dp;
  size_t len;
  char *buf, *slash;
  char filename[64];
  struct stat st;
  off_t blocks;
  int bw, nw;
  char *exten;
  CBMDOSChannel aChan;

  
  aChan.file = NULL;
  aChan.buffer = NULL;
  aChan.length = 0;
  aChan.sent = 0;
  aChan.cpos = aChan.clen = 0;

  if ((dir = opendir(data->directory))) {
    aChan.file = open_memstream(&aChan.buffer, &len);
    fwrite("\001\004\001\001", 4, 1, aChan.file);
    fprintf(aChan.file, "%c%c", driveNum, 0);

    buf = getcwd(NULL, 0);
    slash = strrchr(buf, '/') + 1;
    strncpy(filename, slash, 22);
    filename[22] = 0;
    for (bw = 0; filename[bw]; bw++)
      filename[bw] = toupper(filename[bw]);
    fprintf(aChan.file, "\022\"%s\"%*s", filename, 22 - strlen(filename), " ");
    fputc(0x00, aChan.file);
    free(buf);

    for (dp = readdir(dir); dp; dp = readdir(dir))
      if (dp->d_name[0] != '.') {
	fwrite("\001\001", 2, 1, aChan.file);
	stat(dp->d_name, &st);
	blocks = (st.st_size + 255) / 256;
	if (blocks > 65535)
	  blocks = 65535;
	for (bw = 1, nw = 9; blocks > nw; bw++, nw = nw * 10 + 9)
	  ;
	localFilenameToC64(dp->d_name, filename);
	exten = strrchr(filename, '.');
	nw = exten - filename;
	*exten = 0;
	exten++;
	fprintf(aChan.file, "%c%c%*s\"%.16s\"%*s%s%*s%c",
		(int) blocks & 0xff, (int) (blocks >> 8) & 0xff,
		5 - bw, " ", filename, 22 - nw, " ", exten, bw, " ", 0x00);
      }

    fprintf(aChan.file, "\001\001%c%cBLOCKS FREE.              %c%c%c",
	    0xff, 0xff, 0x00, 0x00, 0x00);

    fflush(aChan.file);
    aChan.length = len;
    fclose(aChan.file);
    aChan.file = NULL;
    closedir(dir);
  }

  return aChan;
}

int localChangeDirectory(CBMDriveData *data, const char *path)
{
  char *userPath;
  char newdir[MAXPATHLEN];
  int len;
  DIR *dir;
  struct dirent *dp;
  char *sep;


  if (!*path)
    return 1;

  /* FIXME - check for ~ or whatever C64 can type */
  if (*path == '/') {
    strcpy(newdir, "/");
    while (*path && *path == '/')
      path++;
  }
  else
    strcpy(newdir, data->directory);

  len = strlen(path);
  while (len && path[len-1] == '/')
    len--;
  
  if (len) {
    userPath = alloca(len+1);
    strncpy(userPath, path, len);
    userPath[len] = 0;
    sep = userPath;

    while (*userPath) {
      if (!(sep = strchr(sep, '/')))
	sep = userPath + strlen(userPath);
      else {
	*sep = 0;
	sep++;
      }

      if (!(dir = opendir(newdir)))
	return 0;

      for (dp = readdir(dir); dp; dp = readdir(dir))
	if (dosWildcardMatch(userPath, dp->d_name)) {
	  if (newdir[strlen(newdir)-1] != '/')
	    strcat(newdir, "/");
	  strcat(newdir, dp->d_name);
	  break;
	}

      closedir(dir);
      if (!dp)
	return 0;
      userPath = sep;
    }
  }

  free(data->directory);
  data->directory = strdup(newdir);

  return 1;
}
