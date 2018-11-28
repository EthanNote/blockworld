#include "input.h"
#include <GLFW/glfw3.h>
//#include<stdio.h>
#include<stdlib.h>

GLFWwindow* input_window = NULL;
struct HANDLER_FUNC_LIST* key_event_list = NULL;

struct MOUSE_DRAG_CONTROL {

	bool is_active;

	double dx;
	double dy;

	double _last_x;
	double _last_y;
};

struct MOUSE_DRAG_CONTROL drag_control;



struct HANDLER_FUNC_LIST {
	KEY_EVENT_HANDLER_FUNC func;
	struct HANDLER_FUNC_LIST* next;
};


struct HANDLER_FUNC_LIST* create_func_list_node(KEY_EVENT_HANDLER_FUNC func) {
	struct HANDLER_FUNC_LIST* pnode = malloc(sizeof(struct HANDLER_FUNC_LIST));
	pnode->func = func;
	pnode->next = NULL;
	return pnode;
}

struct CURSOR_LOCK
{
	double x;
	double y;
	double dx;
	double dy;
	unsigned char enable;
};


struct CURSOR_LOCK _cursor_lock;

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



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	for (struct HANDLER_FUNC_LIST* p_list = key_event_list; p_list != NULL; p_list = p_list->next) {
		p_list->func(key, action, mods);
	}

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
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
	return true;
}






void drag_init() {
	
	drag_control.is_active = true;
	drag_control.dx = 0;
	drag_control.dy = 0;
	glfwGetCursorPos(input_window, &drag_control._last_x, &drag_control._last_y);
}

void drag_stop() {
	drag_control.is_active = false;
	drag_control.dx = 0;
	drag_control.dy = 0;
}

void drag_update() {
	if (!drag_control.is_active) {
		return;
	}
	double x, y;
	glfwGetCursorPos(input_window, &x, &y);
	drag_control.dx = x - drag_control._last_x;
	drag_control.dy = y - drag_control._last_y;
	drag_control._last_x = x;
	drag_control._last_y = y;
}


void drag_get_vector(double *dx, double *dy) {
	*dx = drag_control.dx;
	*dy = drag_control.dy;
}




//void cursor_lock(double x, double y) {
//	glfwSetCursorPos(input_window, x, y);
//	glfwGetCursorPos(input_window, &_cursor_lock.x, &_cursor_lock.y);
//	glfwSetInputMode(input_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	/*_cursor_lock.x = x;
//	_cursor_lock.y = y;*/
//	_cursor_lock.dx = 0.0;
//	_cursor_lock.dy = 0.0;
//	_cursor_lock.enable = 1;
//}
//void cursor_unlock() {
//	_cursor_lock.enable = 0;
//	_cursor_lock.dx = 0;
//	_cursor_lock.dy = 0;
//	glfwSetInputMode(input_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//}
//
//unsigned char cursor_islocked() {
//	return _cursor_lock.enable;
//}
//
//void cursor_get_lock_drag(double *dx, double *dy) {
//	*dx = _cursor_lock.dx;
//	*dy = _cursor_lock.dy;
//}
//
//void cursor_update() {
//	if (_cursor_lock.enable) {
//		glfwSetCursor(input_window, GLFW_CURSOR_NORMAL);
//		double x, y;
//		glfwGetCursorPos(input_window, &x, &y);
//		_cursor_lock.dx = x - _cursor_lock.x;
//		_cursor_lock.dy = y - _cursor_lock.y;
//		glfwSetCursorPos(input_window, _cursor_lock.x, _cursor_lock.y);
//	}
//}