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

// PROTON CLASS ***************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + _width / 2.0f;
	center.y = start.y + _height / 2.0f;

	radius_x = _width / 2.0f;
	radius_y = _height / 2.0f;
}
dll::PROTON::PROTON(float _sx, float _sy)
{
	start.x = _sx;
	start.y = _sy;

	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + _width / 2.0f;
	center.y = start.y + _height / 2.0f;

	radius_x = _width / 2.0f;
	radius_y = _height / 2.0f;
}
dll::PROTON::PROTON(float _sx, float _sy, float _s_width, float _s_height)
{
	start.x = _sx;
	start.y = _sy;

	_width = _s_width;
	_height = _s_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + _width / 2.0f;
	center.y = start.y + _height / 2.0f;

	radius_x = _width / 2.0f;
	radius_y = _height / 2.0f;
}

dll::PROTON::~PROTON()
{
	if (in_heap)delete this;
}

dll::PROTON* dll::PROTON::create(float _start_x, float _start_y, float _start_width, float _start_height)
{
	PROTON* temp{ new PROTON(_start_x, _start_y, _start_width, _start_height) };
	temp->in_heap = true;
	return temp;
}

void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + _width / 2.0f;
	center.y = start.y + _height / 2.0f;
}
void dll::PROTON::set_width(float new_width)
{
	_width = new_width;

	end.x = start.x + _width;
	
	center.x = start.x + _width / 2.0f;
	
	radius_x = _width / 2.0f;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;

	end.y = start.y + _height;

	center.y = start.y + _height / 2.0f;

	radius_y = _height / 2.0f;
}
void dll::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + _width / 2.0f;
	center.y = start.y + _height / 2.0f;

	radius_x = _width / 2.0f;
	radius_y = _height / 2.0f;
}

float dll::PROTON::get_width() const
{
	return _width;
}
float dll::PROTON::get_height() const
{
	return _height;
}

//////////////////////////////////////////

// TILE CLASS ****************************

dll::TILE::TILE(float _s_x, float _s_y) :PROTON(_s_x, _s_y, 50.0f, 50.0f) {};

bool dll::TILE::move(float gear)
{
	float my_speed = _speed + gear / 7.0f;

	switch (dir)
	{
	case dirs::up:
		start.y -= my_speed;
		set_edges();
		if (start.y < 0)return false;
		break;

	case dirs::down:
		start.y += my_speed;
		set_edges();
		if (end.y > scr_height)return false;
		break;

	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (start.x < -50.0f)return false;
		break;

	case dirs::right:
		start.x += my_speed;
		set_edges();
		if (end.x > scr_width + 50.0f)return false;
		break;

	case dirs::up_left:
		start.x -= my_speed;
		start.y -= my_speed;
		set_edges();
		if (start.x < -50.0f || start.y < 0)return false;
		break;

	case dirs::up_right:
		start.x += my_speed;
		start.y -= my_speed;
		set_edges();
		if (end.x > scr_width + 50.0f || start.y < 0)return false;
		break;

	case dirs::down_left:
		start.x -= my_speed;
		start.y -= my_speed;
		set_edges();
		if (start.x < -50.0f || end.y > scr_height)return false;
		break;

	case dirs::down_right:
		start.x += my_speed;
		start.y -= my_speed;
		set_edges();
		if (end.x > scr_width + 50.0f || end.y > scr_height)return false;
		break;
	}

	return true;
}

void dll::TILE::Release()
{
	delete this;
}

dll::TILE* dll::TILE::create(float _first_x, float _first_y)
{
	return new TILE(_first_x, _first_y);
}

//////////////////////////////////////////

// ASSETS CLASS **************************

dll::ASSETS::ASSETS(assets _what, float _s_x, float _s_y) :PROTON(_s_x, _s_y)
{
	type = _what;

	switch (type)
	{
	case assets::cloud1:
		new_dims(100.0f, 53.0f);
		_speed = 0.8f;
		break;

	case assets::cloud2:
		new_dims(100.0f, 52.0f);
		_speed = 0.7f;
		break;

	case assets::riff1:
		new_dims(200.0f, 115.0f);
		_speed = 0.5;
		break;

	case assets::riff2:
		new_dims(200.0f, 112.0f);
		_speed = 0.5;
		break;

	case assets::riff3:
		new_dims(180.0f, 94.0f);
		_speed = 0.5;
		break;
	}
}

assets dll::ASSETS::get_type() const
{
	return type;
}
void dll::ASSETS::Release()
{
	delete this;
}
bool dll::ASSETS::move(float gear)
{
	float my_speed = _speed + gear / 7.0f;

	switch (dir)
	{
	case dirs::up:
		start.y -= my_speed;
		set_edges();
		if (end.y < 0)return false;
		break;

	case dirs::down:
		start.y += my_speed;
		set_edges();
		if (start.y > scr_height)return false;
		break;

	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (end.x < -50.0f)return false;
		break;

	case dirs::right:
		start.x += my_speed;
		set_edges();
		if (start.x > scr_width + 50.0f)return false;
		break;

	case dirs::up_left:
		start.x -= my_speed;
		start.y -= my_speed;
		set_edges();
		if (end.x < -50.0f || end.y < 0)return false;
		break;

	case dirs::up_right:
		start.x += my_speed;
		start.y -= my_speed;
		set_edges();
		if (start.x > scr_width + 50.0f || end.y < 0)return false;
		break;

	case dirs::down_left:
		start.x -= my_speed;
		start.y -= my_speed;
		set_edges();
		if (end.x < -50.0f || start.y > scr_height)return false;
		break;

	case dirs::down_right:
		start.x += my_speed;
		start.y -= my_speed;
		set_edges();
		if (start.x > scr_width + 50.0f || start.y > scr_height)return false;
		break;
	}

	return true;
}

dll::ASSETS* dll::ASSETS::create(assets what_type, float where_x, float where_y)
{
	return new ASSETS(what_type, where_x, where_y);
}

/////////////////////////////////////////








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