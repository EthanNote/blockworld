#pragma once
#include <stdbool.h>
//check key down event, frame related
bool input_get_key_down(int key);

//check key up event, frame related
bool input_get_key_up(int key);

//check key hold state
bool input_get_key(int key);

//get key state
bool input_get_key_state(int key, int* state);

//get key stroke event, down and up
bool input_get_key_stroked(int key, bool* stroked);

//get current cursor position
bool input_get_mouse_pos(double* px, double* py);

//get cursor move vector, frame related
bool input_get_mouse_move(double* dx, double* dy);


//typedef void(*ON_KEY_DOWN)(int key);
//typedef void(*ON_KEY_UP)(int key);

void bind_input_window(void* window);


typedef void(*KEY_EVENT_HANDLER_FUNC)(int, int, int);

//void add_key_down_event_handler(KEY_EVENT_HANDLER_FUNC);
//
//void add_key_up_event_handler(KEY_EVENT_HANDLER_FUNC);

void add_key_event_handler(KEY_EVENT_HANDLER_FUNC);

void input_setup(void* window);

// typedef void(*ON_MOUSE_DRAG)(double dx, double dy);
// typedef void(*ON_MOUSE_MOVE)(double x, double y);

// struct EVENT_HANDLER {
// 	ON_KEY_DOWN on_key_down;
// 	ON_KEY_UP on_key_up;
// 	ON_MOUSE_DRAG on_mouse_drag;
// 	ON_MOUSE_MOVE on_mouse_move;
// };

// struct EVENT_HANDLER_LIST {
// 	struct EVENT_HANDLER** handles;
// 	int count;
// 	int tail_index;
// 	int capacity;
// };

// void input_init();

// void init_event_handler_list(struct EVENT_HANDLER_LIST* list);

// void add_event_handler(struct EVENT_HANDLER* handler, struct EVENT_HANDLER_LIST* handler_list);

// void remove_event_handler(struct EVENT_HANDLER* handler, struct EVENT_HANDLER_LIST* handler_list);

// typedef struct MESSAGE{
//     int type;
//     int params[4];
// }

// typedef struct MESSAGE_BUFFER{
//     MESSAGE* data;
//     int count;
//     int capacity;
// }

// struct MESSAGE_BUFFER* get_global_message_buffer();

// void clear_global_message();

// typedef bool (*MESSAGE_FILTER_FUNC)(struct MESSAGE*);

// typedef void (*MESSAGE_INTERPRATE_FUNC)(struct MESSAGE*, struct MESSAGE*);

// typedef struct MESSAGE_FILTER_FAMILY{
//     MESSAGE_FILTER_FUNC* functions;
//     int filter_count;
//     MESSAGE_BUFFER output;
// }


//typedef void(*KEY_DOWN_EVENT_HANDLER)(int key);
//typedef void(*KEY_UP_EVENT_HANDLER)(int key);
//typedef void(*MOUSE_DRAG_EVENT_HANDLER(int dx, int dy));
//
//union HANDLER_FUNCTION
//{
//	KEY_DOWN_EVENT_HANDLER key_down_event_handler;
//	KEY_UP_EVENT_HANDLER key_up_event_handler;
//	MOUSE_DRAG_EVENT_HANDLER mouse_drag_event_handler;
//};
//
//
//struct HANDLER_COLLECTION {
//	union HANDLER_FUNCTION* handlers;
//	int type;
//	int count;
//	int capacity;
//};
//
//
//void init_HANDLER_COLLECTION(HANDLER_COLLECTION* collection);
//
//
//
//
//struct KEY_DOWN_STATE {
//	int* key_buffer;
//	int buffer_size;
//};
//
//void init_key_down_state(KEY_DOWN_STATE* state);
//
//void push_key_down_event(KEY_DOWN_STATE* state);
//
//void push_key_up_event(KEY_DOWN_STATE* state);