#include "stm32f1xx_hal.h"

class Timer {
public:
	Timer(uint16_t period_ms, bool enable = true) {
		setPeriod(period_ms);
		en = enable;
	}

	void setPeriod(uint16_t period_ms) {
		prd = period_ms;
	}

	uint16_t getPeriod() {
		return prd;
	}

	void disable() {
		en = false;
	}

	void enable() {
		en = true;
		tmr = HAL_GetTick();
	}

	bool ready() {
		if (en && HAL_GetTick() - tmr >= prd) {
			tmr = HAL_GetTick();
			return true;
		}
		return false;
	}
private:
	uint32_t tmr = 0;
	uint16_t prd = 0;
	bool en = true;
};
