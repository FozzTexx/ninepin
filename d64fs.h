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

#ifndef _D64FS_H
#define _D64FS_H

#include "cbmdos.h"

extern int d64MountDisk(CBMDriveData *data, const char *path);
extern CBMDOSChannel d64OpenFile(CBMDriveData *data, const char *path, const char *mode);
extern CBMDOSChannel d64GetDirectory(CBMDriveData *data, int driveNum);

#endif /* _D64FS_H */
