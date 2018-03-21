#include "../graphics.h"

/* TODO:
 *  - FIX BMP LOADING, TOTALLY FUCKED ATM
 *  - Colour escapes for print()
 *  - Wayland window code
 *  - Window resize events
 *  - Fullscreening
 *  - Cursor lock/hide
 *  - Any other window events
 *  - AA functions for line & circle
 *  - Documentation & comments
 *  - Extended surface functions, resize, rotate, filters, etc
 *  - stb_image/stb_truetype tests
 *  - Windows/X11/Wayland OpenGL backend
 *  - Independent screen & window size
 */

#define TICK_INTERVAL 30
static long next_time;

long time_left(void) {
  long now = ticks();
  long ret = (next_time <= now ? 0 : next_time - now);
  next_time += TICK_INTERVAL;
  return ret;
}

static int mx = 0, my = 0, running = 1;

int invert(int x, int y, int c) {
  return RGB(255 - ((c >> 16) & 0xFF), 255 - ((c >> 8) & 0xFF), 255 - (c & 0xFF));
}

#define RND_255 (rand() % 256)

int rnd(int x, int y, int c) {
  return RGB(RND_255, RND_255, RND_255);
}

#define PRINT_CHAR_MAP(fn, min_i, max_i, c) \
for (i = min_i; i < max_i; ++i, x += 8) { \
  if (x + 8 > win->w) { \
    x  = 5; \
    y += 8; \
  } \
  fn(win, i, x, y, c); \
} \
x  = 5;\
y += 8;

int main(int argc, const char* argv[]) {
  surface_t* win = screen("test", 640, 480);
  if (!win) {
    fprintf(stderr, "%s\n", get_last_error());
    return 1;
  }

  surface_t* a = surface(50, 50);
#if defined(__APPLE__)
  surface_t* c = bmp("/Users/roryb/Documents/git/graphics/Uncompressed-24.bmp");
#elif defined(_WIN32)
  surface_t* c = bmp("C:\\Users\\DESKTOP\\Documents\\git\\graphics.h\\x64\\Debug\\Uncompressed-24.bmp");
#else
  surface_t* c = bmp("/home/reimu/Desktop/graphics.h/Uncompressed-24.bmp");
#endif
  surface_t* e = copy(c);
  iterate(e, invert);

  rect_t  tmpr  = { 150, 50, 50, 50 };
  point_t tmpp  = { 10, 150 };
  point_t tmpp2 = { 5, 227 };
  point_t tmpp3 = { 350, 125 };
  point_t tmpp4 = { tmpp2.x + c->w + 5, tmpp2.y };
  point_t tmpp5 = { 10, 110 };
  point_t tmpp6 = { 425, 110 };
  point_t tmpp7 = { 482, 170 };

  surface_t* d = string_f(RED, LIME, "cut from the\nimage below\nx: %d y: %d\nw: %d h: %d", tmpr.x, tmpr.y, tmpr.w, tmpr.h);
  surface_t* h = string(RED, LIME, "NO\nGREEN\nHERE");
  surface_t* k = string(LIME, BLACK, "WOW");
  surface_t* l = surface(50, 50);
  fill(l, BLACK);
  point_t tmmp8 = { 13, 20 };
  blit(l, &tmmp8, k, NULL, -1);
  destroy(&k);

  surface_t* f = surface(100, 100);
  rect(f, 0,  0,  50, 50, RED, true);
  rect(f, 50, 50, 50, 50, LIME, true);
  rect(f, 50, 0,  50, 50, BLUE, true);
  rect(f, 0,  50, 50, 50, YELLOW, true);

  int col, i, x, y;
  long sine_i = 0;
  user_event_t ue;
  next_time = ticks() + TICK_INTERVAL;
  while (running) {
    fill(win, WHITE);

    for (int x = 32; x < win->w; x += 32)
      yline(win, x, 0, win->h, GRAY);
    for (int y = 32; y < win->h; y += 32)
      xline(win, y, 0, win->w, GRAY);

    blit(win, &tmpp5, d, NULL, LIME);
    blit(win, &tmpp, c, &tmpr, LIME);

    blit(win, &tmpp2, c, NULL, LIME);
    blit(win, &tmpp4, e, NULL, LIME);

    blit(win, &tmpp6, f, NULL, LIME);
    blit(win, &tmpp7, h, NULL, LIME);

    iterate(a, rnd);
    blit(a, NULL, l, NULL, LIME);
    blit(win, &tmpp3, a, NULL, LIME);

    circle(win, 352, 32, 30, RED,    true);
    circle(win, 382, 32, 30, ORANGE, true);
    circle(win, 412, 32, 30, YELLOW, true);
    circle(win, 442, 32, 30, LIME,   true);
    circle(win, 472, 32, 30, BLUE,   true);
    circle(win, 502, 32, 30, INDIGO, true);
    circle(win, 532, 32, 30, VIOLET, true);

    print_f(win, 400, 88, BLACK, "mouse x,y: (%d, %d)", mx, my);

    x = 5;
    y = 5;
    PRINT_CHAR_MAP(letter, 33, 128, MAROON);
    PRINT_CHAR_MAP(letter_block, 0, 32, DARK_RED);
    PRINT_CHAR_MAP(letter_box, 0, 128, BROWN);
    PRINT_CHAR_MAP(letter_extra, 0, 132, FIREBRICK);
    PRINT_CHAR_MAP(letter_greek, 0, 58, CRIMSON);
    PRINT_CHAR_MAP(letter_hiragana, 0, 96, RED);

    get_mouse_pos(&mx, &my);
    col = pget(win, mx, my);
    rect(win, 150, 50,  100, 100, col, false);
    rect(win, 200, 100, 100, 100, col, false);
    line(win, 150, 50,  200, 100, col);
    line(win, 250, 50,  300, 100, col);
    line(win, 150, 150, 200, 200, col);
    line(win, 250, 150, 300, 200, col);

    line(win, 0, 0, mx, my, col);
    circle(win, mx, my, 30, col, false);
    
    int last_x = 0, last_y = 240;
    for (long i = sine_i; i < (sine_i + 641); ++i) {
      float x = (float)(i - sine_i);
      float y = 240.f + (100.f * sin(i * (3.141 / 180.f)));
      line(win, last_x, last_y, x, y, col);
      last_x = x;
      last_y = y;
    }
    sine_i += 5;

    while (poll_events(&ue)) {
      switch (ue.type) {
        case WINDOW_CLOSED:
          running = 0;
          break;
        case KEYBOARD_KEY_DOWN:
          switch (ue.sym) {
#if defined(__APPLE__)
            case KB_KEY_Q:
              if (ue.mod & KB_MOD_SUPER)
                running = 0;
              break;
#else
            case KB_KEY_F4
              if (ue.mod & KB_MOD_ALT)
                running = 0;
              break;
#endif
            case KB_KEY_SPACE:
              save_bmp(win, "test.bmp");
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
    }
    
    render();
    delay(time_left());
  }

  destroy(&c);
  destroy(&d);
  destroy(&e);
  destroy(&a);
  destroy(&h);
  destroy(&l);
  release();
  return 0;
}