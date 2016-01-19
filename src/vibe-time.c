#include <pebble.h>

typedef enum { END, SHORT, LONG } Vibe;

Vibe vibes[13][8] = {
  {SHORT, SHORT, SHORT, SHORT, END}, // 0
  {SHORT, END}, // 1
  {SHORT, SHORT, END}, // 2
  {LONG, END}, // 3
  {LONG, SHORT, END}, // 4
  {LONG, SHORT, SHORT, END}, // 5
  {LONG, LONG, END}, // 6
  {LONG, LONG, SHORT, END}, // 7
  {LONG, LONG, SHORT, SHORT, END}, // 8
  {LONG, LONG, LONG, END}, // 9
  {LONG, LONG, LONG, SHORT, END}, // 10
  {LONG, LONG, LONG, SHORT, SHORT, END}, // 11
  {LONG, LONG, LONG, LONG, END}, // 12
};

uint32_t segments[256];

int vibe_number(int number, int start) {
  for (int i = 0; true; i++) {
    switch (vibes[number][i]) {
      case END:
        return start + 2*i;
      case SHORT:
        segments[start+2*i] = 100;
        segments[start+2*i+1] = 300;
        break;
      case LONG:
        segments[start+2*i] = 500;
        segments[start+2*i+1] = 300;
        break;
    }
  }
}

void vibe_time() {
  time_t tt = time(NULL);
  struct tm *t = localtime(&tt);
  int start = 2;
  segments[0]=0; // Put a buffer at the front.  Otherwise, first vibe is always long.
  segments[1]=100;
  start = vibe_number(t->tm_min / 10, start);
  segments[start-1]=1500;
  start = vibe_number(t->tm_min % 10, start);
  segments[start-1]=1500;
  start = vibe_number(t->tm_hour % 12, start);
  VibePattern pattern;
  pattern.durations = segments;
  pattern.num_segments = start;
  vibes_enqueue_custom_pattern(pattern);
}

int main(void) {
  vibe_time(); // Exit immediately after sending vibes
}
