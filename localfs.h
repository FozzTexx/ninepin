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

#ifndef _LOCALFS_H
#define _LOCALFS_H

#include "cbmdos.h"

extern char *localFindPath(const char *directory, const char *path);
extern FILE *localFindFile(const char *directory, const char *path, const char *mode);
extern CBMDOSChannel localOpenFile(CBMDriveData *data, const char *path, const char *mode);
extern CBMDOSChannel localGetDirectory(CBMDriveData *data, int driveNum);
extern int localChangeDirectory(CBMDriveData *data, const char *path);

#endif /* _LOCALFS_H */
