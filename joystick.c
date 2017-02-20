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

#include "joystick.h"
#include "cbmdos.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define SREG_STORE	24
#define SREG_SHIFT	23
#define SREG_DATA	22

#define BUTTON_FIRETP	0
#define BUTTON_FIRERT	1
#define BUTTON_FIREBT	2
#define BUTTON_FIRELF	3
#define BUTTON_LSHTOP	6
#define BUTTON_LSHBOT	4
#define BUTTON_RSHTOP	7
#define BUTTON_RSHBOT	5
#define BUTTON_START	9
#define BUTTON_SELECT	8
#define BUTTON_LSTICK	10
#define BUTTON_RSTICK	11

#define DEADZONE	16384

#define calcAccel(x) ({int _x = x; _x / (160 / (1 + abs(_x) / 16384));})

static int numAxes = 0, numButtons = 0;
static int *axis, *button;

typedef enum {
  modeAbsolute = 0,
  modeRelative,
  mode4way,
  modeMax
} joyMode;

typedef enum {
  outputDigital = 0,
  outputAnalog = 1
} outputMode;

int potx, poty;
// Apple II:
int xmax = 135, ymax = 160;
//int xmax = 165, ymax = 180;
//int xmax = 255, ymax = 255;
int joy_state = 0;
joyMode joy_mode = modeAbsolute;
outputMode out_mode = outputAnalog;
int yaxis = 1;
int wrap_x = 0, wrap_y = 0;

void joystickWriteBits(int output)
{
  int i, bit;


  printf("Output: 0x%02x\r", output);
  fflush(stdout);
  
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

  /* modprobe ad525x_dpot  */
  /* echo ad5282 0x2c > /sys/bus/i2c/devices/i2c-1/new_device */
  if ((potx = open("/sys/bus/i2c/devices/1-002c/rdac0", O_RDWR)) >= 0) {
    if ((poty = open("/sys/bus/i2c/devices/1-002c/rdac1", O_RDWR)) < 0) {
      close(potx);
      potx = -1;
    }
  }
  
  return joyfd;
}

void calc4way(int xaxis, int yaxis, int *xpos, int *ypos)
{
  double x, y, angle;


  x = xaxis;
  y = yaxis;
  if (x < -DEADZONE || x > DEADZONE) {
    angle = atan(fabs(y) / fabs(x));
    angle *= 180 / M_PI;
    if (x < 0)
      angle = 180 - angle;
    if (y > 0)
      angle = 360 - angle;
    //fprintf(stderr, "\n%f %f %f\n", angle, x, y);
    if (angle < 45 || angle > (45 * 7)) {
      *xpos = 32767;
      *ypos = 0;
    }
    else if (angle > 45 && angle < (45 * 3)) {
      *xpos = 0;
      *ypos = -32767;
    }
    else if (angle > (45 * 3) && angle < (45 * 5)) {
      *xpos = -32767;
      *ypos = 0;
    }
    else if (angle > (45 * 5) && angle < (45 * 7)) {
      *xpos = 0;
      *ypos = 32767;
    }
  }
  else if (y < -DEADZONE || y > DEADZONE) {
    *xpos = 0;
    *ypos = ((y > 0) - (y < 0)) * 32767;
  }
  else {
    *xpos = 0;
    *ypos = 0;
  }

  return;
}    

void joystickHandleIO(int fd)
{
  struct js_event js;
  int output = 0;
  static int newDrive = 0;
  int xpos, ypos;


  /* FIXME - check for read error, maybe user unplugged joystick */
  read(fd, &js, sizeof(struct js_event));

  switch (js.type & ~JS_EVENT_INIT) {
  case JS_EVENT_AXIS:
    axis[js.number] = js.value;
    //fprintf(stderr, "Axis %i: %i\n", js.number, js.value);
    break;
	
  case JS_EVENT_BUTTON:
    button[js.number] = js.value;
    break;
  }

  if (out_mode == outputDigital) {
    /* MadCatz Dpad shows up as axes 4&5 when in analog mode */
    xpos = axis[0];
    ypos = axis[yaxis];
    if (joy_mode == mode4way)
      calc4way(xpos, ypos, &xpos, &ypos);

    output &= ~ATARI_UP;
    output |= (ypos < -DEADZONE) * ATARI_UP;
    output &= ~ATARI_DOWN;
    output |= (ypos > DEADZONE) * ATARI_DOWN;
    output &= ~ATARI_LEFT;
    output |= (xpos < -DEADZONE) * ATARI_LEFT;
    output &= ~ATARI_RIGHT;
    output |= (xpos > DEADZONE) * ATARI_RIGHT;
    output &= ~ATARI_FIRE;
  }

  output |= button[BUTTON_FIRETP] * ATARI_FIRE;
  output |= button[BUTTON_FIREBT] * ATARI_FIRE;
  output |= button[BUTTON_FIRERT] * APPLE_FIRE1;
  output |= button[BUTTON_FIRELF] * APPLE_FIRE1;

  if (button[BUTTON_SELECT]) {
    if (js.number >= 0 && js.number <= 3 && button[js.number]) {
      newDrive = js.number;
      dosSwapDrive(newDrive);
    }

    if (js.number == BUTTON_RSHTOP && button[BUTTON_RSHTOP])
      xmax -= 10;
    if (js.number == BUTTON_RSHBOT && button[BUTTON_RSHBOT])
      xmax += 10;
    if (xmax < 0)
      xmax = 0;
    if (xmax > 255)
      xmax = 255;

    if (js.number == BUTTON_LSHTOP && button[BUTTON_LSHTOP])
      ymax -= 10;
    if (js.number == BUTTON_LSHBOT && button[BUTTON_LSHBOT])
      ymax += 10;
    if (ymax < 0)
      ymax = 0;
    if (ymax > 255)
      ymax = 255;

    if (js.number == BUTTON_LSTICK && button[BUTTON_LSTICK]) {
      if (button[BUTTON_START]) {
	wrap_x = !wrap_x;
	fprintf(stderr, "\nX-wrap: %i\n", wrap_x);
      }
      else {
	joy_mode = (joy_mode + 1) % modeMax; /* Cycle joy mode */
	fprintf(stderr, "\nJoy mode: %i\n", joy_mode);
      }
    }
    if (js.number == BUTTON_RSTICK && button[BUTTON_RSTICK]) {
      if (button[BUTTON_START]) {
	wrap_y = !wrap_y;
	fprintf(stderr, "\nY-wrap: %i\n", wrap_y);
      }
      else
	yaxis ^= 2; /* Toggle between axis 1 and 3 */
    }
  }
  
  if (output != joy_state) {
    joystickWriteBits(output);
    joy_state = output;
  }

  return;
}

void updatePaddles()
{
  int xval, yval;
  char buf[20];
  static int xpos = 0, ypos = 0;
  static int x_accel = 0, y_accel = 0;


  switch (joy_mode) {
  case modeRelative:
    x_accel = calcAccel(axis[0]);
    y_accel = calcAccel(axis[yaxis]);

    xpos += x_accel;
    if (xpos < -32768) {
      if (!wrap_x)
	xpos = -32768;
      else
	xpos += 65536;
    }
    if (xpos > 32767) {
      if (!wrap_x)
	xpos = 32767;
      else
	xpos -= 65536;
    }

    ypos += y_accel;
    if (ypos < -32768) {
      if (!wrap_y)
	ypos = -32768;
      else
	ypos += 65536;
    }
    if (ypos > 32767) {
      if (!wrap_y)
	ypos = 32767;
      else
	ypos -= 65536;
    }
    break;

  case mode4way:
    calc4way(axis[0], axis[yaxis], &xpos, &ypos);
    break;

  default:
    xpos = axis[0];
    ypos = axis[yaxis];
    break;
  }
  
  xval = (xpos + 32768) * xmax / 65535;
  yval = (ypos + 32768) * ymax / 65535;

  fprintf(stderr, "XPos: %i = %i  YPos: %i = %i  Buttons: %02x       \r",
	  xpos, xval, ypos, yval, joy_state);
  //printf("Pos: %i  Accel: %i\n", xpos, x_accel);

  sprintf(buf, "%i\n", xval);
  write(potx, buf, strlen(buf));
  sprintf(buf, "%i\n", yval);
  write(poty, buf, strlen(buf));
}

void calibrateJoystick()
{
  int xval, yval;
  char buf[20];
  int rval[256];


  memset(rval, 0xff, sizeof(rval));
  
  for (xval = 240; xval >= 0; xval -= 5) {
    fprintf(stderr, "Val: %i  \n", xval);
    sprintf(buf, "%i\n", xval);
    write(potx, buf, strlen(buf));
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == 'u')
      xval += 6;
    else
      rval[xval] = atoi(buf);
  }

  return;
}
