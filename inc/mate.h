#pragma once
#include "common.h"
#include "entity.h"

struct cell {
	uint8_t health;
	uint8_t resistance;
	gene* genome;
};

bool is_attractive(entity male, entity female);
cell* create_sperm(entity male);
bool try_fertilize(cell sperm, cell egg);