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

FILE *localFindFile(const char *path, const char *mode)
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

CBMDOSChannel localOpenFile(void *cwd, const char *path, const char *mode)
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
    aChan.file = localFindFile(path, mode);
    if (aChan.file && mode[0] == 'r') {
      fseek(aChan.file, 0, SEEK_END);
      aChan.length = ftell(aChan.file);
      rewind(aChan.file);
    }
  }

  return aChan;
}
