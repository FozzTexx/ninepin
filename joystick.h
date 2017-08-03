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

#define ATARI_FIRE	0x01
#define ATARI_LEFT	0x02
#define ATARI_RIGHT	0x04
#define ATARI_UP	0x08
#define ATARI_DOWN	0x10

#define APPLE_FIRE0	ATARI_FIRE
#define APPLE_FIRE1	ATARI_LEFT

#define MSX_FIRE0	ATARI_FIRE
#define MSX_FIRE1	0x20

extern int initJoystick();
extern void joystickHandleIO(int fd);

extern int potx, poty, xmax, ymax, joy_state;
extern void joystickWriteBits(int output);
extern void updatePaddles();

extern void calibrateJoystick();
