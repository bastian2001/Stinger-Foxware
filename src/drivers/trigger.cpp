#include "global.h"

u8 triggerState = 0;
volatile bool triggerUpdateFlag = false;

void triggerInit() {
	gpio_init(PIN_TRIGGER);
	gpio_set_dir(PIN_TRIGGER, GPIO_IN);
	gpio_set_pulls(PIN_TRIGGER, true, false);
	sleep_us(10); // wait for settling after applying pullup
	u8 read = gpio_get(PIN_TRIGGER);
	triggerState = !read;

	// go to STATE_BOOT_MENU to select boot mode
	if (triggerState && bootReason == BootReason::POR)
		operationState = STATE_BOOT_SELECT;
}

void triggerLoop() {
	if (!triggerState && !gpio_get(PIN_TRIGGER))
	{
		triggerUpdateFlag = false;
		triggerState = 1;
		triggerUpdateFlag = true;
		Serial.println("trigger pressed");
		inactivityTimer = 0;
	} else if (triggerState && gpio_get(PIN_TRIGGER)) {
		triggerUpdateFlag = false;
		triggerState = 0;
		triggerUpdateFlag = true;
		Serial.println("trigger released");
		inactivityTimer = 0;
	}
}