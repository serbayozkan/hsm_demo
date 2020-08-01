/*
 * main.c
 *
 *  Created on: 1 August 2020
 *      Author: serbay
 */

#include <stdio.h>
#include <assert.h>
#include "hsm.h"

typedef struct
{
	Hsm super;
	Msg msg;
	State led_on;
	State led_off;
}hsm_blink_t;

hsm_blink_t hsm_blink;

enum
{
	NO_EVENT,
	TIMEOUT_EVENT
};

TOP_STATE_HANDLER(hsm_blink_t, top) {
    switch (msg->evt) {
    case START_EVT:
        printf("Top State -> Init...\n");
        TOP_STATE_INIT(led_off);
        return 0;
    case ENTRY_EVT:
        printf("Top State -> Entry\n");
        return 0;
    case EXIT_EVT:
        printf("Top State -> Exit\n");
        return 0;
    }
    return msg;
}

STATE_HANDLER(hsm_blink_t, led_off) {
    switch (msg->evt) {
    case ENTRY_EVT:
        printf("Led Off State -> ENTRY\n");
        printf("Led is OFF now\n");
        return 0;
    case EXIT_EVT:
        printf("Led Off State -> EXIT\n");
        return 0;
    case TIMEOUT_EVENT:
		printf("Led Off State -> SIGNAL_TIMEOUT\n");
		STATE_TRANSIT(led_on);
		return 0;
    }
    return msg;
}

STATE_HANDLER(hsm_blink_t, led_on) {
    switch (msg->evt) {
    case ENTRY_EVT:
        printf("Led On State -> ENTRY\n");
        printf("Led is ON now\n");
        return 0;
    case EXIT_EVT:
        printf("Led On State -> EXIT\n");
        return 0;
    case TIMEOUT_EVENT:
		printf("Led On State -> SIGNAL_TIMEOUT\n");
		STATE_TRANSIT(led_off);
		return 0;
    }
    return msg;
}

void hsm_blink_init(hsm_blink_t *me)
{
	// Create Top State
	TOP_STATE_CTOR(hsm_blink_t);

	// Create Child States
	STATE_CTOR(led_off, top);
	STATE_CTOR(led_on, top);
}

int main() {

	hsm_blink_init(&hsm_blink);

	HSM_START(hsm_blink);

    for (;;)
    {
    	HSM_SET_EVENT(hsm_blink, TIMEOUT_EVENT);

    	HSM_DISPATCH(hsm_blink);
    	HSM_DISPATCH(hsm_blink);
        return 0;
    }
}
