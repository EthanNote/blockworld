#include "input.h"
#include <GLFW/glfw3.h>

// struct EVENT_HANDLER_LIST default_handler_list;
// struct EVENT_HANDLER_LIST* current_handler_list;

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
// 	/*if (key == GLFW_KEY_E && action == GLFW_PRESS)
// 		activate_airship();
// */

// 	if (action == GLFW_PRESS) {
// 		for (int i = 0; i < current_handler_list->tail_index; i++) {
// 			if (current_handler_list->handles[i] && current_handler_list->handles[i]->on_key_down) {
// 				current_handler_list->handles[i]->on_key_down(key);
// 			}
// 		}
// 	}

// 	if (action == GLFW_RELEASE) {
// 		for (int i = 0; i < current_handler_list->tail_index; i++) {
// 			if (current_handler_list->handles[i] && current_handler_list->handles[i]->on_key_down) {
// 				current_handler_list->handles[i]->on_key_up(key);
// 			}
// 		}
// 	}
// }


// void input_init() {
// 	init_event_handler_list(&default_handler_list);
// 	current_handler_list = &default_handler_list;
// }

// void init_event_handler_list(struct EVENT_HANDLER_LIST* list) {
// 	int init_capacity = 10;
// 	list->handles = malloc(sizeof(struct EVENT_HANDLER*)*init_capacity);
// 	list->count = 0;
// 	list->capacity = init_capacity;
// 	list->tail_index = 0;
// }

// void add_event_handler(struct EVENT_HANDLER* handler, struct EVENT_HANDLER_LIST* handler_list) {
// 	if (!handler_list) {
// 		handler_list = current_handler_list;
// 	}

// 	if (handler_list->tail_index == handler_list->count) {
// 		if (handler_list->capacity <= handler_list->count) {
// 			handler_list->handles = realloc(sizeof(struct EVENT_HANDLER*)*handler_list->count * 2);
// 		}
// 		handler_list->handles[handler_list->tail_index++] = handler;
// 		handler_list->count = handler_list->tail_index;
// 	}
// 	else {
// 		for (int i = 0; i < handler_list->tail_index; i++) {
// 			if (!handler_list->handles[i]) {
// 				handler_list->handles[i] = handler;
// 				handler_list->count++;
// 				break;
// 			}

// 		}
// 	}
// }

// void remove_event_handler(struct EVENT_HANDLER* handler, struct EVENT_HANDLER_LIST* handler_list) {

// 	for (int i = 0;; i < handler_list->count; i++) {
// 		if (handler_list->handles[i] == handler) {
// 			handler_list->handles[i] = 0;
// 			if (i == handler_list->count - 1) {
// 				handler_list->tail_index--;
// 			}
// 			handler_list->count--;
// 			break;
// 		}
// 	}
// }