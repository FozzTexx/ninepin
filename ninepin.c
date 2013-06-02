/* Copyright 2013 by Chris Osborn <fozztexx@fozztexx.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <time.h>

#define ATARI_UP	0x01
#define ATARI_DOWN	0x02
#define ATARI_LEFT	0x04
#define ATARI_RIGHT	0x08
#define ATARI_FIRE	0x10

#define SREG_STORE	21
#define SREG_SHIFT	22
#define SREG_DATA	23

#define IEC_ATN		25
#define IEC_CLK		8
#define IEC_DATA	7

#define DATA_EOI	0x100
#define DATA_ATN	0x200

int iecAttentionState = 1;

void writeBits(int output);
void readIEC();
int readIECByte();
void iecAttention();

int main(int argc, char *argv[])
{
  int joyfd;
  int numAxes = 0, numButtons = 0;
  int *axis, *button;
  struct js_event js;
  int deadzone = 16384;
  int output = 0, last = 0;

  
  /* FIXME - read CLI arguments */

#if 0
  {
    char buf[256];

  
    sprintf(buf, "gpio export %i out", SREG_STORE);
    system(buf);
    sprintf(buf, "gpio export %i out", SREG_SHIFT);
    system(buf);
    sprintf(buf, "gpio export %i out", SREG_DATA);
    system(buf);
    wiringPiSetupSys();
  }
#else
  wiringPiSetupGpio();
  pinMode(SREG_STORE, OUTPUT);
  pinMode(SREG_SHIFT, OUTPUT);
  pinMode(SREG_DATA, OUTPUT);

  pinMode(IEC_ATN, INPUT);
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, INPUT);
#endif

  digitalWrite(SREG_STORE, LOW);
  digitalWrite(SREG_SHIFT, LOW);
  writeBits(0);

  digitalWrite(IEC_CLK, LOW);
  digitalWrite(IEC_DATA, LOW);

  if (numAxes) {
    fprintf(stderr, "WTF: %i\n", numAxes);
    exit(-1);
  }

#if 0
  if ((joyfd = open("/dev/input/js0", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open joystick\n");
    exit(-1);
  }

  ioctl(joyfd, JSIOCGAXES, &numAxes);
  ioctl(joyfd, JSIOCGBUTTONS, &numButtons);

  fprintf(stderr, "Axes: %i  Buttons: %i\n", numAxes, numButtons);
  
  axis = calloc(numAxes, sizeof(int));
  button = calloc(numButtons, sizeof(int));
#endif

  //wiringPiISR(IEC_CLK, INT_EDGE_RISING, readIEC);
  wiringPiISR(IEC_ATN, INT_EDGE_FALLING, readIEC);
  
  for (;;) {
#if 0
    /* FIXME - check for read error, maybe user unplugged joystick */
    read(joyfd, &js, sizeof(struct js_event));

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
      writeBits(output);
      printf("Output: 0x%02x\r", output);
      fflush(stdout);
      last = output;
    }
#endif
  }
  
  /* FIXME - update console joystick port */

  fprintf(stderr, "Out of infinite loop!\n");
  
  exit(0);
}

void writeBits(int output)
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

void readIEC()
{
  static short buf[256];
  static int pos = 0;
  static int listen = 31;
  int val, abort, atn;
  int i;
  struct timespec start, now;
  int elapsed;
  int cmd, dev;
  

  //fprintf(stderr, "Reading IEC\n", spinlock);
  abort = 0;
  atn = !digitalRead(IEC_ATN);
  //fprintf(stderr, "ATN: %i\n", atn);
  if (atn) {
    pinMode(IEC_CLK, INPUT);
    pinMode(IEC_DATA, OUTPUT);
  }
  else if (listen != 8)
    abort = 1;
  
  while (!abort) {
    atn = !digitalRead(IEC_ATN);
    if (!atn && listen != 8) {
      //fprintf(stderr, "Nothing for me %i %i\n", atn, listen);
      break;
    }
    
    if (digitalRead(IEC_CLK)) {
      //fprintf(stderr, "Atn: %i\n", atn);
      //fprintf(stderr, "Reading byte %i\n", pos);
      val = readIECByte();
      if (digitalRead(IEC_CLK))
	fprintf(stderr, "Why is it still high?\n");

      if (val < 0) {
	//fprintf(stderr, "No bits %i\n", val);
	abort = 1;
	break;
      }

      buf[pos++] = val;
      if (val & DATA_EOI) {
	pinMode(IEC_CLK, INPUT);
	pinMode(IEC_DATA, INPUT);
	break;
      }

      if (atn) {
	cmd = val & 0xe0;
	dev = val & 0x1f;

	switch (cmd) {
	case 0x20: /* Listen */
	  listen = dev;
	  //fprintf(stderr, "Listen %i\n", listen);
	  if (listen != 8) {
	    pinMode(IEC_CLK, INPUT);
	    pinMode(IEC_DATA, INPUT);

	    /* Ignore rest of commands sent during ATN */
	    clock_gettime(CLOCK_MONOTONIC, &start);
	    while (!digitalRead(IEC_ATN)) {
	      clock_gettime(CLOCK_MONOTONIC, &now);
	      elapsed = (now.tv_sec - start.tv_sec) * 1000000 +
		(now.tv_nsec - start.tv_nsec) / 1000;
	      if (elapsed >= 100000) {
		fprintf(stderr, "Timeout waiting for end of ATN\n");
		abort = 1;
		break;
	      }
	    }
	  }
	}
      }

      //if (val > 255)
      //break;
    }
  }

  if (pos) {
    fprintf(stderr, "Read %i bytes: ", pos);
    for (i = 0; i < pos; i++)
      fprintf(stderr, "%02x ", buf[i]);
    fprintf(stderr, "\n");
    pos = 0;
  }

  return;
}

int readIECByte()
{
  int bits, len, eoi, abort, atn;
  struct timespec start, now;
  int elapsed;


  atn = !digitalRead(IEC_ATN);
  pinMode(IEC_DATA, INPUT);

  abort = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);
  while (!digitalRead(IEC_CLK)) {
    clock_gettime(CLOCK_MONOTONIC, &now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 +
      (now.tv_nsec - start.tv_nsec) / 1000;
    if (elapsed >= 100000) {
      fprintf(stderr, "Timeout waiting for talk\n");
      abort = 1;
      break;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (eoi = 0; !abort &&  digitalRead(IEC_CLK); ) {
    clock_gettime(CLOCK_MONOTONIC, &now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 +
      (now.tv_nsec - start.tv_nsec) / 1000;
    
    if (!eoi && elapsed >= 200) {
      pinMode(IEC_DATA, OUTPUT);
      usleep(80);
      pinMode(IEC_DATA, INPUT);
      eoi = 1;
    }

    if (elapsed > 1000) {
      pinMode(IEC_DATA, OUTPUT);
      usleep(80);
      pinMode(IEC_DATA, INPUT);
      if (elapsed > 10000) {
	fprintf(stderr, "First bit not received %i\n", elapsed);
	abort = 1;
	break;
      }
    }
  }

  for (len = bits = 0; !abort && len < 8; len++) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    while (!digitalRead(IEC_CLK)) {
      clock_gettime(CLOCK_MONOTONIC, &now);
      elapsed = (now.tv_sec - start.tv_sec) * 1000000 +
	(now.tv_nsec - start.tv_nsec) / 1000;
      if (elapsed >= 100000) {
	fprintf(stderr, "Timeout waiting for bit %i\n", len);
	abort = 1;
	break;
      }
    }

    if (abort)
      break;
    
    bits |= digitalRead(IEC_DATA) << len;

    while (digitalRead(IEC_CLK)) {
      clock_gettime(CLOCK_MONOTONIC, &now);
      elapsed = (now.tv_sec - start.tv_sec) * 1000000 +
	(now.tv_nsec - start.tv_nsec) / 1000;
      if (elapsed >= 10000) {
	fprintf(stderr, "Timeout after bit %i %i %i %02x\n", len, eoi, atn, bits);
	if (len < 7)
	  abort = 1;
	break;
      }
    }
  }

  pinMode(IEC_DATA, OUTPUT);

  if (abort)
    return -1;
  
  if (eoi)
    bits |= DATA_EOI;
  if (atn)
    bits |= DATA_ATN;
  
  return bits;
}

void iecAttention()
{
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, OUTPUT);
  //readIEC();
  return;
}
