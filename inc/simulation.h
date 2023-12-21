#pragma once
#include <iostream>
#include <fstream>
#include <random>
using namespace std;

typedef struct {
	bool console_print;
	uint8_t start_size;
	int max_size;
	float mut_rate;
	uint8_t max_life;
	uint8_t avg_life;
	uint8_t gen_length;
	uint8_t max_childs;
	int max_gens;
	float nat_sel;
	float env_change;
	float env_dominance;
	uint16_t initial_food;
} simOptions;

simOptions getCFG();