#pragma once
#include "common.h"
#include "entity.h"

struct cell {
	uint8_t health;
	uint8_t resistance;
	gene* genome;
};

struct embryo {
	cell body;
	bool gender;
};

bool is_attractive(entity male, entity female);
cell* create_sperm(entity male);
cell* create_eggs(entity female);
embryo* try_fertilize(cell* sperm, cell* eggs);
void mate(entity male, entity female);
void gestation(embryo* child, entity father, entity mother);