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

  /* Pull IEC_DATA low to signal we exist */
  pinMode(IEC_DATA, OUTPUT);
  
  if (numAxes) {
    fprintf(stderr, "WTF: %i\n", numAxes);
    exit(-1);
  }
  
  if ((joyfd = open("/dev/input/js0", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open joystick\n");
    exit(-1);
  }

  ioctl(joyfd, JSIOCGAXES, &numAxes);
  ioctl(joyfd, JSIOCGBUTTONS, &numButtons);

  fprintf(stderr, "Axes: %i  Buttons: %i\n", numAxes, numButtons);
  
  axis = calloc(numAxes, sizeof(int));
  button = calloc(numButtons, sizeof(int));

#if 0
  wiringPiISR(IEC_CLK, INT_EDGE_RISING, readIEC);
  wiringPiISR(IEC_ATN, INT_EDGE_BOTH, iecAttention);
#else
  readIEC();
#endif
  
  for (;;) {
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
  }
  
  /* FIXME - update console joystick port */
  
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
  char buf[256];
  int pos, val, abort;
  int i;
  struct timespec start, now;
  int elapsed;


  //fprintf(stderr, "Reading IEC\n");
  pos = abort = 0;
  while (!abort) {
    if (digitalRead(IEC_CLK)) {
      //fprintf(stderr, "Reading byte %i\n", pos);
      val = readIECByte();
#if 1
      fprintf(stderr, "%02x ", val);
#else
      if (val < 0) {
	//fprintf(stderr, "No bits %i\n", val);
	abort = 1;
	break;
      }
    
      buf[pos++] = val;
      if (val & DATA_EOI) {
	pinMode(IEC_DATA, INPUT);
	usleep(60);
	pinMode(IEC_DATA, OUTPUT);
	break;
      }
#endif

#if 0
      clock_gettime(CLOCK_MONOTONIC, &start);
      while (!digitalRead(IEC_CLK)) {
	clock_gettime(CLOCK_MONOTONIC, &now);
	elapsed = (now.tv_sec - start.tv_sec) / 1e-6 + (now.tv_nsec - start.tv_nsec) / 1e3;
	if (elapsed >= 10000) {
	  fprintf(stderr, "Abort waiting for next byte\n");
	  abort = 1;
	  break;
	}
      }
#endif
    }
  }

  if (abort)
    fprintf(stderr, "Timeout\n");
  fprintf(stderr, "Read %i bytes\n", pos);
  for (i = 0; i < pos; i++)
    fprintf(stderr, "%02x ", buf[i]);
  fprintf(stderr, "\n");
  
  return;
}

int readIECByte()
{
  int bits, len, eoi, abort;
  struct timespec start, now;
  int elapsed;

  
  pinMode(IEC_DATA, INPUT);

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (eoi = abort = 0; digitalRead(IEC_CLK); ) {
    clock_gettime(CLOCK_MONOTONIC, &now);
    elapsed = (now.tv_sec - start.tv_sec) / 1e-6 + (now.tv_nsec - start.tv_nsec) / 1e3;
    
    if (!eoi && elapsed >= 200) {
      pinMode(IEC_DATA, OUTPUT);
      usleep(60);
      pinMode(IEC_DATA, INPUT);
      eoi = 1;
    }

    if (elapsed > 10000) {
      fprintf(stderr, "First bit not received\n");
      abort = 1;
      break;
    }
  }

  for (len = bits = 0; !abort && len < 8; len++) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    while (!digitalRead(IEC_CLK)) {
      clock_gettime(CLOCK_MONOTONIC, &now);
      elapsed = (now.tv_sec - start.tv_sec) / 1e-6 + (now.tv_nsec - start.tv_nsec) / 1e3;
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
      elapsed = (now.tv_sec - start.tv_sec) / 1e-6 + (now.tv_nsec - start.tv_nsec) / 1e3;
      if (elapsed >= 10000) {
	fprintf(stderr, "Timeout after bit %i\n", len);
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
  if (!digitalRead(IEC_ATN))
    bits |= DATA_ATN;
  
  return bits;
}

void iecAttention()
{
  iecAttentionState = digitalRead(IEC_ATN);
  fprintf(stderr, "Attention: %i\n", iecAttentionState);
  return;
}
