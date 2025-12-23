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





// FUNCTIONS ******************************

float dll::Distance(FPOINT first, FPOINT second)
{
	float a = (float)(pow((float)(abs(second.x - first.x)), 2));
	float b = (float)(pow((float)(abs(second.y - first.y)), 2));

	return (float)(sqrt(a + b));
}

bool dll::SortBag(BAG<FPOINT>& Bag, FPOINT ref_point)
{
	if (Bag.size() < 2)return false;

	bool is_ok = false;

	while (!is_ok)
	{
		for (size_t count = 0; count < Bag.size() - 1; ++count)
		{
			is_ok = true;

			if (Distance(Bag[count], ref_point) > Distance(Bag[count + 1], ref_point))
			{
				FPOINT temp{ Bag[count] };
				Bag[count] = Bag[count + 1];
				Bag[count + 1] = temp;
				is_ok = false;
			}
		}
	}

	return true;
}

bool dll::Intersect(FRECT first, FRECT second)
{
	if (!(first.left >= second.right || first.right <= second.left || first.up >= second.down || first.down <= second.up))return true;

	return false;
}

bool dll::Intersect(FPOINT center1, FPOINT center2, float radius1_x, float radius2_x,
	float radius1_y, float radius2_y)
{
	if (abs(center2.x - center1.x) <= radius1_x + radius2_x && abs(center2.y - center1.y) <= radius1_y + radius2_y)return true;

	return false;
}