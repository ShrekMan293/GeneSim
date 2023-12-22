#include "mate.h"

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
	int sperm_count = (400000 / average) * (male.fertility * male.health * male.fitness);

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

	uint8_t numberOfEggs = 0;
	int cumulativeProbability = 0;

	for (int i = 0; i < probabilities.size(); i++)
	{
		cumulativeProbability += probabilities[i];
		if (randomNum < (float)(cumulativeProbability / 100)) {
			numberOfEggs += 1;
			break;
		}
	}

	cell* eggs = new cell[numberOfEggs];
	for (int i = 0; i < numberOfEggs; i++)
	{
		eggs[i].genome = meiosis(female);
		eggs[i].health = ((female.health + female.fertility) / 2) / 2;
		std::uniform_real_distribution<> dist(0.00, 1.00);
		eggs[i].resistance = ((female.fitness + female.fertility) / 2) / 2;
		eggs[i].resistance *= dist(gen);
	}

	return eggs;
}