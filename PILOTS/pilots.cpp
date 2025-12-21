#include "pch.h"
#include "pilots.h"

// RANDIT **********************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(),rd(),rd() ,rd() ,rd() ,rd() ,rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator() (int min, int max)
{
	std::uniform_int_distribution gen(min, max);
	
	return gen(*twister);
}

////////////////////////////////////////////