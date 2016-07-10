#include <pebble.h>
#include "main.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_layer2;

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
    GRect(0, 60, bounds.size.w, 50));
  s_time_layer2 = text_layer_create(
    GRect(0, 0,bounds.size.w,60));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK ));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //Try to set my own text layer
  text_layer_set_background_color(s_time_layer2, GColorBlack);
  text_layer_set_text_color(s_time_layer2, GColorClear);
  text_layer_set_font(s_time_layer2, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK ));
  text_layer_set_text_alignment(s_time_layer2, GTextAlignmentCenter);
  text_layer_set_text(s_time_layer2, "Hi Mike!");

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer2));
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[50];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_unload(Window *window) {
 // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
