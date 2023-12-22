#pragma once
#include "common.h"

enum class gene : uint8_t {
	// Negative
	LowMetabolism = -15, WeakImmune, BrittleBones,
	ShortLifespan, SlowHealing, IncreasedMutation,
	ReducedFertility, ReducedAttractiveness, LimitedMobility,
	LimitedAdaptability, PrematureAging, IncreasedDisease,
	NeurologicalDegeneration, CardioWeakness, PoorVision,

	None,

	// Positive
	HighMetabolism, StrongImmune, StrongBones,
	LongLifespan, QuickHealing, HeightenedSenses,
	IncreasedFertility, IncreasedAttractiveness, IncreasedMobility,
	IncreasedAdaptability, LateAging, DecreasedDisease,
	EnhancedNeurological, StrongCardio, GreatVision
};

struct entity {
	gene* genome;
	uint8_t age;
	uint8_t health;
	uint8_t fitness;
	uint8_t attractiveness;
	uint8_t fertility;
	bool gender;		// Male is false, female is true
	bool can_mate;
	uint8_t mate_attempts;
};