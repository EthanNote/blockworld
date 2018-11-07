#include "input.h"
#include <GLFW/glfw3.h>
//#include<stdio.h>
#include<stdlib.h>

static struct HANDLER_FUNC_LIST {
	KEY_EVENT_HANDLER_FUNC func;
	struct HANDLER_FUNC_LIST* next;
};

//struct HANDLER_FUNC_LIST* key_up_event_list = NULL;
//struct HANDLER_FUNC_LIST* key_down_event_list = NULL;
struct HANDLER_FUNC_LIST* key_event_list = NULL;

struct HANDLER_FUNC_LIST* create_func_list_node(KEY_EVENT_HANDLER_FUNC func) {
	struct HANDLER_FUNC_LIST* pnode = malloc(sizeof(struct HANDLER_FUNC_LIST));
	pnode->func = func;
	pnode->next = NULL;
	return pnode;
}

bool check_list(struct HANDLER_FUNC_LIST** list, KEY_EVENT_HANDLER_FUNC func) {
	if (!*list) {
		*list = create_func_list_node(func);
		return false;
	}
	return true;
}

void _add_key_event_handler(KEY_EVENT_HANDLER_FUNC func, struct HANDLER_FUNC_LIST* list) {

	struct HANDLER_FUNC_LIST* p_node = list;
	while (p_node->next)
	{
		p_node = p_node->next;
	}
	p_node->next = create_func_list_node(func);

}

void add_key_event_handler(KEY_EVENT_HANDLER_FUNC func) {
	if (check_list(&key_event_list, func)) {
		_add_key_event_handler(func, key_event_list);
	}
}

//void add_key_up_event_handler(KEY_EVENT_HANDLER_FUNC func) {
//	if (check_list(&key_up_event_list, func)) {
//		_add_key_event_handler(func, key_up_event_list);
//	}
//}
//
//void add_key_down_event_handler(KEY_EVENT_HANDLER_FUNC func) {
//	if (check_list(&key_down_event_list, func)) {
//		_add_key_event_handler(func, key_down_event_list);
//	}
//}


GLFWwindow* input_window = NULL;






// struct EVENT_HANDLER_LIST default_handler_list;
// struct EVENT_HANDLER_LIST* current_handler_list;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	for (struct HANDLER_FUNC_LIST* p_list = key_event_list; p_list != NULL; p_list = p_list->next) {
		p_list->func(key, action, mods);
	}
	/*if (key == GLFW_KEY_E && action == GLFW_PRESS)
		activate_airship();
 */
 //printf("KEY %d %d %d\n", key, action, mods);

 //if (action == GLFW_PRESS) {
 //	/*for (int i = 0; i < current_handler_list->tail_index; i++) {
 //		if (current_handler_list->handles[i] && current_handler_list->handles[i]->on_key_down) {
 //			current_handler_list->handles[i]->on_key_down(key);
 //		}
 //	}*/
 //	for (struct HANDLER_FUNC_LIST* p_list = key_down_event_list; p_list != NULL; p_list = p_list->next) {
 //		p_list->func(key, action, mods);
 //	}
 //}

 //if (action == GLFW_RELEASE) {
 //	/*for (int i = 0; i < current_handler_list->tail_index; i++) {
 //		if (current_handler_list->handles[i] && current_handler_list->handles[i]->on_key_down) {
 //			current_handler_list->handles[i]->on_key_up(key);
 //		}
 //	}*/
 //	for (struct HANDLER_FUNC_LIST* p_list = key_up_event_list; p_list != NULL; p_list = p_list->next) {
 //		p_list->func(key, action, mods);
 //	}
 //}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	/*double x, y;
	input_get_mouse_pos(&x, &y);
	printf("KEY %d %d %d %lf %lf\n", button, action, mods, x, y);*/
	for (struct HANDLER_FUNC_LIST* p_list = key_event_list; p_list != NULL; p_list = p_list->next) {
		p_list->func(button, action, mods);
	}
}

void input_setup(void* window) {
	input_window = window;
	glfwSetKeyCallback(input_window, key_callback);
	glfwSetMouseButtonCallback(input_window, mouse_button_callback);
}

bool input_get_mouse_pos(double* px, double* py) {
	glfwGetCursorPos(input_window, px, py);
}


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