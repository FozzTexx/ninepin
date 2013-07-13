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

#define BCM2708_PERI_BASE   0x20000000
#define GPIO_BASE  (BCM2708_PERI_BASE + 0x200000)

#define INPUT	0
#define OUTPUT	1
#define LOW	0
#define HIGH	1

#define digitalRead(pin)	({int _p = (pin) & 31; (*(gpio + 13) & (1 << _p)) >> _p;})
#define digitalWrite(pin, val)	({int _p = (pin) & 31, _v = !!(val);	\
      *(gpio + 7 + ((_v) ? 0 : 3)) = 1 << _p;})
#define pinMode(pin, mode)	({int _p = (pin) & 31, _m = !!(mode);	\
      *(gpio + _p / 10) = (*(gpio + _p / 10) &		\
			   ~(7 << (_p % 10) * 3)) |	\
	(_m << (_p % 10) * 3);})

