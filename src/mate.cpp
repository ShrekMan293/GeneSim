#include "mate.h"

extern int sperm_count;
extern int egg_count;

bool is_attractive(entity male, entity female) {
	uint8_t male_attscore = ((male.attractiveness / male.fitness) + male.attractiveness) / 5;
	uint8_t female_attscore = ((female.attractiveness / female.fitness) + female.attractiveness) / 5;
	if ((male_attscore / female_attscore) < simoptions.nat_sel) return false;

	return true;
}

gene* meiosis(entity e) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 9);
	gene* genome = new gene[5];
	int alreadyused[4];

	int rnd = dis(gen);

	genome[0] = e.genome[rnd];
	alreadyused[0] = rnd;
	rnd = dis(gen);
	genome[1] = e.genome[rnd];
	alreadyused[1] = rnd;
	rnd = dis(gen);
	genome[2] = e.genome[rnd];
	alreadyused[2] = rnd;
	rnd = dis(gen);
	genome[3] = e.genome[rnd];
	alreadyused[3] = rnd;
	rnd = dis(gen);
	genome[4] = e.genome[rnd];

	return genome;
}

cell* create_sperm(entity male) {			// Wasn't sure if I should use sperm or cell here
	const int average = 75 * 75 * 75;
	sperm_count = (400000 / average) * (male.fertility * male.health * male.fitness);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);

	cell* sperm = new cell[sperm_count];
	for (int i = 0; i < sperm_count; i++)
	{
		sperm[i].genome = meiosis(male);
		sperm[i].health = ((male.health + male.fertility) / 2) / 2;
		sperm[i].resistance = ((male.fitness + male.fertility) / 2) / 2;
	}

	return sperm;
}

cell* create_eggs(entity female) {
	std::vector<int> probabilities = { 6500, 2000, 700, 400, 200, 100, 50, 25, 15, 10 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100);
	int randomNum = dis(gen);

	egg_count = 0;
	int cumulativeProbability = 0;

	for (int i = 0; i < probabilities.size(); i++)
	{
		cumulativeProbability += probabilities[i];
		if ((float)randomNum < (float)(cumulativeProbability / 100)) {
			egg_count += 1;
		}
	}

	cell* eggs = new cell[egg_count];
	for (int i = 0; i < egg_count; i++)
	{
		eggs[i].genome = meiosis(female);
		eggs[i].health = (female.health + female.fertility) / 2;
		std::uniform_real_distribution<> dist(0.00, 1.00);
		eggs[i].resistance = (female.fitness + female.fertility) / 2;
		eggs[i].resistance *= dist(gen);
	}

	return eggs;
}

embryo* try_fertilize(cell* sperm, cell* eggs) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, egg_count - 1);
	std::uniform_int_distribution<> dist(0, 1);

	int deadEggs = 0;
	int currentEgg = 0;

	for (; currentEgg < egg_count; currentEgg++)
	{
		if (eggs[currentEgg].health == 0) deadEggs++;
	}
	if (currentEgg == 0) {
		if (eggs[currentEgg].health == 0 || deadEggs == egg_count) return nullptr;
	}

	cell* egg = &eggs[dis(gen)];

	check_egg:
	if (egg->health == 0) {
		egg = &eggs[dis(gen)];
		goto check_egg;
	}
	
	while (egg->health != 0 && sperm->health != 0) {
		if (dist(gen) == 0) {
			if (dist(gen) == 1) {
				sperm->health -= egg->resistance / 2;
				continue;
			}
			sperm->health -= 1;
			continue;
		}
		else {
			egg->health -= sperm->resistance / 2;
			continue;
		}
	}

	if (egg->health == 0) {
		embryo* e = new embryo[1];
		e->body.health = 50;
		e->body.resistance = 100;
		for (uint8_t i = 0; i < 10; i++)
		{
			if (i < 5) {
				e->body.genome[i] = sperm->genome[i];
			}
			else {
				e->body.genome[i] = egg->genome[i - 5];
			}
		}
		e->gender = dist(gen);
		return e;
	}

	return nullptr;
}

void mate(entity male, entity female) {
	cell* sperm = create_sperm(male);
	cell* eggs = create_eggs(female);
	int currentEmbryo = 0;
	embryo** embryos = new embryo*[egg_count];

	for (int i = 0; i < sperm_count; i++)
	{
		embryo* e = try_fertilize(&sperm[i], eggs);
		if (e == nullptr) {
			continue;
		}
		else {
			embryos[currentEmbryo] = e;
		}
	}

	return;
}

void gestation(embryo* child, entity father, entity mother) {
	std::random_device rd;
	std::mt19937 gen(rd());

	for (uint8_t i = 0; i < 10; i++)
	{
		if (child->body.genome[i] == gene::IncreasedMutation) {
			std::uniform_int_distribution<> dis(0, 9);
			std::uniform_int_distribution<> dist(0, 30);

			child->body.genome[dis(gen)] = (gene)dist(gen);
		}
	}

	const int survival_chance = 95;
	std::uniform_int_distribution<> dis(0, 100);
	if (dis(gen) > survival_chance) return;

	entity e;
	e.age = 0;
	e.attractiveness = (father.attractiveness + mother.attractiveness) / 2;
	e.can_mate = father.can_mate && mother.can_mate;
	e.cell_count = 0;
	e.fertility = (father.fertility + mother.fertility) / 2;
	e.fitness = (father.fitness + mother.fitness) / 2;
	e.gender = child->gender;
	e.genome = child->body.genome;
	e.health = 100;
	e.mate_attempts = 0;

	for (uint8_t i = 0; i < 10; i++)
	{
		switch (e.genome[i])
		{
		case gene::ReducedFertility: {
			if (e.fertility < 45) {
				e.fertility = 0;
				break;
			}
			e.fertility -= 45;
			break;
		}
		case gene::ReducedAttractiveness: {
			if (e.attractiveness < 45) {
				e.attractiveness = 0;
				break;
			}
			e.attractiveness -= 45;
			break;
		}
		case gene::LimitedMobility: {
			if (e.fitness < 15) {
				e.fitness = 0;
				break;
			}
			e.fitness -= 15;
			break;
		}
		case gene::BrittleBones: {
			if (e.fitness < 65) {
				e.fitness = 0;
				break;
			}
			e.fitness -= 65;
			break;
		}
		case gene::NeurologicalDegeneration: {
			e.fitness = 20;
			break;
		}
		case gene::CardioWeakness: {
			if (e.fitness < 10) {
				e.fitness = 0;
				break;
			}
			e.fitness -= 10;
			break;
		}
		case gene::StrongBones: {
			e.fitness = 90;
			break;
		}
		case gene::IncreasedFertility: {
			if (e.fertility > 55) {
				e.fertility = 100;
				break;
			}
			e.fertility += 45;
			break;
		}
		case gene::IncreasedAttractiveness: {
			e.attractiveness = 90;
			break;
		}
		case gene::IncreasedMobility: {
			if (e.fitness > 85) {
				e.fitness = 100;
				break;
			}
			e.fitness += 15;
			break;
		}
		case gene::EnhancedNeurological: {
			if (e.fitness > 75) {
				e.fitness = 100;
				break;
			}
			e.fitness += 25;
			break;
		}
		case gene::StrongCardio: {
			if (e.fitness > 80) {
				e.fitness = 100;
				break;
			}
			e.fitness += 20;
			break;
		}
		case gene::GreatVision: {
			if (e.fitness > 95) {
				e.fitness = 100;
				break;
			}
			e.fitness += 5;
			break;
		}
		default:
			break;
		}
	}

	// addEntity(e);
}
