#pragma once

typedef void(*ON_KEY_DOWN)(int key);
typedef void(*ON_KEY_UP)(int key);
typedef void(*ON_MOUSE_DRAG)(double dx, double dy);
typedef void(*ON_MOUSE_MOVE)(double x, double y);

struct EVENT_HANDLER {
	ON_KEY_DOWN on_key_down;
	ON_KEY_UP on_key_up;
	ON_MOUSE_DRAG on_mouse_drag;
	ON_MOUSE_MOVE on_mouse_move;
};

struct EVENT_HANDLER_LIST {
	struct EVENT_HANDLER** handles;
	int count;
	int tail_index;
	int capacity;
};

void input_init();

void init_event_handler_list(struct EVENT_HANDLER_LIST* list);

void add_event_handler(struct EVENT_HANDLER* handler, struct EVENT_HANDLER_LIST* handler_list);

void remove_event_handler(struct EVENT_HANDLER* handler, struct EVENT_HANDLER_LIST* handler_list);



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