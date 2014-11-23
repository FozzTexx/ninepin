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

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>

#define ATARI_UP	0x01
#define ATARI_DOWN	0x02
#define ATARI_LEFT	0x04
#define ATARI_RIGHT	0x08
#define ATARI_FIRE	0x10

#define SREG_STORE	24
#define SREG_SHIFT	22
#define SREG_DATA	23

static int numAxes = 0, numButtons = 0;
static int *axis, *button;

void joystickWriteBits(int output)
{
  int i, bit;


  /* Write the high bits first */
  for (i = 0; i < 16; i++) {
    bit = (output & 0x8000) >> 15;
    output <<= 1;
    digitalWrite(SREG_DATA, bit);
    digitalWrite(SREG_SHIFT, 1);
    digitalWrite(SREG_SHIFT, 0);
  }

  digitalWrite(SREG_STORE, 1);
  digitalWrite(SREG_STORE, 0);
  return;
}

int initJoystick()
{
  int joyfd;

  
  wiringPiSetupGpio();
  pinMode(SREG_STORE, OUTPUT);
  pinMode(SREG_SHIFT, OUTPUT);
  pinMode(SREG_DATA, OUTPUT);

  digitalWrite(SREG_STORE, LOW);
  digitalWrite(SREG_SHIFT, LOW);
  joystickWriteBits(0);

  if ((joyfd = open("/dev/input/js0", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open joystick\n");
    return -1;
  }

  ioctl(joyfd, JSIOCGAXES, &numAxes);
  ioctl(joyfd, JSIOCGBUTTONS, &numButtons);

  fprintf(stderr, "Axes: %i  Buttons: %i\n", numAxes, numButtons);
  
  axis = calloc(numAxes, sizeof(int));
  button = calloc(numButtons, sizeof(int));

  return joyfd;
}

void joystickHandleIO(int fd)
{
  struct js_event js;
  int deadzone = 16384;
  int output = 0;
  static int last = 0;


  /* FIXME - check for read error, maybe user unplugged joystick */
  read(fd, &js, sizeof(struct js_event));

  switch (js.type & ~JS_EVENT_INIT) {
  case JS_EVENT_AXIS:
    axis[js.number] = js.value;
    break;
	
  case JS_EVENT_BUTTON:
    button[js.number] = js.value;
    break;
  }

  /* Dpad shows up as axes 4&5 */

  output &= ~ATARI_UP;
  output |= (axis[5] < -deadzone) * ATARI_UP;
  output &= ~ATARI_DOWN;
  output |= (axis[5] > deadzone) * ATARI_DOWN;
  output &= ~ATARI_LEFT;
  output |= (axis[4] < -deadzone) * ATARI_LEFT;
  output &= ~ATARI_RIGHT;
  output |= (axis[4] > deadzone) * ATARI_RIGHT;
  output &= ~ATARI_FIRE;
  output |= button[0] * ATARI_FIRE;

  if (output != last) {
    joystickWriteBits(output);
    printf("Output: 0x%02x\r", output);
    fflush(stdout);
    last = output;
  }

  return;
}
