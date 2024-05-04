#include <xinu.h>

struct resets_hw {
	uint32 reset;
	uint32 wdsel;
	uint32 reset_done;
};

#define rst	((volatile struct resets_hw*)RESETS_BASE)


void reset_set(uint32 hw) {
	rst->reset |= hw;
}

void reset_release(uint32 hw) {
	rst->reset &= (~hw);
}

void reset_release_wait(uint32 hw) {
	rst->reset &= (~hw);
	while (!(rst->reset_done & hw));
}
