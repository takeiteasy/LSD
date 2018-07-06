//
//  graphics_image.c
//  graphics_osx_test
//
//  Created by Rory B. Bellows on 04/07/2018.
//  Copyright © 2018 Rory B. Bellows. All rights reserved.
//

#include "graphics_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

static char stb_last_error[1024];

#define SET_LAST_ERROR(MSG, ...) \
memset(stb_last_error, 0, 1024); \
sprintf(stb_last_error, "[ERROR] from %s in %s() at %d -- " MSG, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);

int image(surface_t* out, const char* path, int alpha) {
  int w, h, c, x, y;
  unsigned char* data = stbi_load(path, &w, &h, &c, 0);
  if (!data) {
    SET_LAST_ERROR("stbi_load() failed: %s\n", stbi_failure_reason());
    return 0;
  }
  
  if (!surface(out, w, h)) {
    stbi_image_free(data);
    SET_LAST_ERROR("image() failed: %s\n", last_error());
    return 0;
  }
  
  unsigned char* p = NULL;
  for (x = 0; x < w; ++x) {
    for (y = 0; y < h; ++y) {
      p = data + (x + w * y) * c;
      out->buf[y * w + x] = (c == 4 && !p[3] ? alpha : RGB(p[0], p[1], p[2]));
    }
  }
  
  stbi_image_free(data);
  return 1;
}

const char* get_last_stb_error() {
  return stb_last_error;
}
