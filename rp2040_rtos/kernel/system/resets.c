#include "resets.h"

struct resets_hw {
	unsigned int reset;
	unsigned int wdsel;
	unsigned int reset_done;
};

#define rst	((volatile struct resets_hw*)RESETS_BASE)


void reset_set(unsigned int hw) {
	rst->reset |= hw;
}

void reset_release(unsigned int hw) {
	rst->reset &= (~hw);
}

void reset_release_wait(unsigned int hw) {
	rst->reset &= (~hw);
	while (!(rst->reset_done & hw));
}
