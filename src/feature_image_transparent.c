#include "pebble.h"

static Window *s_main_window;
static TextLayer *s_text_layer;
static GBitmap *s_bitmap;
static BitmapLayer *s_layer;

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // We do this to account for the offset due to the status bar
  // at the top of the app window.
  GRect layer_frame_description = layer_get_frame(window_layer);
  layer_frame_description.origin.x = 0;
  layer_frame_description.origin.y = 0;

  // Add some background content to help demonstrate transparency.
  s_text_layer = text_layer_create(layer_frame_description);
  text_layer_set_text(s_text_layer,
      "pandamonium pandamon\n"
      "ium pandamonium panda\n"
      "monium     pandamonium\n"
      "pandamonium pandamon\n"
      "ium pandamonium panda\n"
      "monium     pandamonium\n"
      "pandamonium pandamon\n"
      "ium pandamonium panda\n"
      "monium pandamonium p\n"
      "andamonium pandamoni\n");
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
#ifdef PBL_ROUND
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 2);
#endif
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PANDA);

  GPoint center = grect_center_point(&bounds);

  GSize image_size = gbitmap_get_bounds(s_bitmap).size;

  GRect image_frame = GRect(center.x, center.y, image_size.w, image_size.h);
  image_frame.origin.x -= image_size.w / 2;
  image_frame.origin.y -= image_size.h / 2;

  // Use GCompOpOr to display the white portions of the image
  s_layer = bitmap_layer_create(image_frame);
  bitmap_layer_set_bitmap(s_layer, s_bitmap);
  bitmap_layer_set_compositing_mode(s_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_layer));
}

static void main_window_unload(Window *window) {
  bitmap_layer_destroy(s_layer);
  text_layer_destroy(s_text_layer);
  gbitmap_destroy(s_bitmap);
}

static void init(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
}

static void deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
