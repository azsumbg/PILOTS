#include "pch.h"
#include "pilots.h"

// RANDIT CLASS **********************************

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

bool dll::TILE::move()
{
	switch (dir)
	{
	case dirs::up:
		start.y -= my_speed;
		set_edges();
		if (end.y <= 0)return false;
		break;

	case dirs::down:
		start.y += my_speed;
		set_edges();
		if (start.y >= scr_height)return false;
		break;

	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (end.x <= -50.0f)return false;
		break;

	case dirs::right:
		start.x += my_speed;
		set_edges();
		if (start.x >= scr_width + 50.0f)return false;
		break;

	case dirs::up_left:
		start.x -= my_speed;
		set_edges();
		if (end.x <= -50.0f)return false;
		break;

	case dirs::up_right:
		start.x += my_speed;
		set_edges();
		if (start.x >= scr_width + 50.0f)return false;
		break;

	case dirs::down_left:
		start.x -= my_speed;
		set_edges();
		if (end.x <= -50.0f)return false;
		break;

	case dirs::down_right:
		start.x += my_speed;
		set_edges();
		if (start.x >= scr_width + 50.0f)return false;
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
		if (end.y < -200.0f)return false;
		break;

	case dirs::down:
		start.y += my_speed;
		set_edges();
		if (start.y > scr_height + 200.0f)return false;
		break;

	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (end.x < -scr_width / 2.0f)return false;
		break;

	case dirs::right:
		start.x += my_speed;
		set_edges();
		if (start.x > scr_width + scr_width / 2.0f)return false;
		break;

	case dirs::up_left:
		start.x -= my_speed;
		start.y -= my_speed;
		set_edges();
		if (end.x < -scr_width / 2.0f || end.y < -200.0f)return false;
		break;

	case dirs::up_right:
		start.x += my_speed;
		start.y -= my_speed;
		set_edges();
		if (start.x > scr_width + 50.0f || end.y < -200.0f)return false;
		break;

	case dirs::down_left:
		start.x -= my_speed;
		start.y -= my_speed;
		set_edges();
		if (end.x < -scr_width / 2.0f || start.y > scr_height + 200.0f)return false;
		break;

	case dirs::down_right:
		start.x += my_speed;
		start.y -= my_speed;
		set_edges();
		if (start.x > scr_width + 50.0f || start.y > scr_height + 200.0f)return false;
		break;
	}

	return true;
}

dll::ASSETS* dll::ASSETS::create(assets what_type, float where_x, float where_y)
{
	return new ASSETS(what_type, where_x, where_y);
}

/////////////////////////////////////////

// SHOT CLASS **************************

dll::SHOT::SHOT(float _where_x, float _where_y, float _to_where_x, float _to_where_y) :PROTON(_where_x, _where_y, 15.0f, 15.0f)
{
	SetPathInfo(_to_where_x, _to_where_y);
}

void dll::SHOT::SHOT::Release()
{
	delete this;
}

void dll::SHOT::SetPathInfo(float to_x, float to_y)
{
	ver_dir = false;
	hor_dir = false;

	move_sx = start.x;
	move_sy = start.y;

	move_ex = to_x;
	move_ey = to_y;

	if (move_ex == move_sx || (move_ex > start.x && move_ex < end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_ey == move_sy || (move_ey > start.y && move_ey < end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - slope * start.x;
}

int dll::SHOT::get_frame()
{
	current_frame++;
	if (current_frame > 63)current_frame = 0;
	return current_frame;
}

bool dll::SHOT::Move(float gear)
{
	float my_speed = _speed + gear / 2.0f;

	if (hor_dir)
	{
		if (move_sx < move_ex)
		{
			start.x += my_speed;
			set_edges();
			if (start.x > scr_width)return false;
		}
		else if (move_sx > move_ex)
		{
			start.x -= my_speed;
			set_edges();
			if (end.x < 0)return false;
		}
	}
	else if (ver_dir)
	{
		if (move_sy < move_ey)
		{
			start.y += my_speed;
			set_edges();
			if (start.y > ground)return false;
		}
		else if (move_sy > move_ey)
		{
			start.y -= my_speed;
			set_edges();
			if (end.y < sky)return false;
		}
	}
	else
	{
		if (move_sx < move_ex)
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (start.x > scr_width || start.y > ground || end.y < sky)return false;
		}
		else if (move_sx > move_ex)
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x < 0 || start.y > ground || end.y < sky)return false;
		}
	}

	return true;
}

dll::SHOT* dll::SHOT::create(float where_x, float where_y, float to_where_x, float to_where_y)
{
	return new SHOT(where_x, where_y, to_where_x, to_where_y);
}

///////////////////////////////////////////////

// HERO CLASS *********************************

dll::HERO::HERO(float sx, float sy) :PROTON(sx, sy, 120.0f, 120.0f)
{
	speed = 0.5f;
}

void dll::HERO::move(float gear)
{
	float my_speed = speed + gear / 5.0f;

	switch (dir)
	{
	case dirs::up_left:
		if (start.x - my_speed >= 0 && start.y - my_speed >= sky)
		{
			start.x -= my_speed;
			start.y -= my_speed;
			set_edges();
		}
		break;

	case dirs::up:
		if (start.y - my_speed >= sky)
		{
			start.y -= my_speed;
			set_edges();
		}
		break;

	case dirs::up_right:
		if (end.x + my_speed <= scr_width && start.y - my_speed >= sky)
		{
			start.x += my_speed;
			start.y -= my_speed;
			set_edges();
		}
		break;

	case dirs::down_left:
		if (start.x - my_speed >= 0 && end.y + my_speed <= ground)
		{
			start.x -= my_speed;
			start.y += my_speed;
			set_edges();
		}
		break;

	case dirs::down:
		if (end.y + my_speed <= ground)
		{
			start.y += my_speed;
			set_edges();
		}
		break;

	case dirs::down_right:
		if (end.x + my_speed <= scr_width && end.y + my_speed <= ground)
		{
			start.x += my_speed;
			start.y += my_speed;
			set_edges();
		}
		break;

	case dirs::right:
		if (end.x + my_speed <= scr_width)
		{
			start.x += my_speed;
			set_edges();
		}
		break;

	case dirs::left:
		if (start.x - my_speed >= 0)
		{
			start.x -= my_speed;
			set_edges();
		}
		break;
	}
}

void dll::HERO::heal(int heal_points)
{
	if (lifes + heal_points > max_lifes)lifes = max_lifes;
	else lifes += heal_points;
}

int dll::HERO::get_frame()
{
	--frame_delay;
	if (frame_delay < 0)
	{
		frame_delay = 20;
		++current_frame;
		if (current_frame >= max_frames)current_frame = 0;
	}

	return current_frame;
}

//////////////////////////////////////////////

// EVILS CLASS ******************************

dll::EVILS::EVILS(planes _what, float _start_x, float _start_y) :PROTON(_start_x, _start_y)
{
	_type = _what;

	switch (_type)
	{
	case planes::evil1:
		new_dims(50.0f, 53.0f);
		strenght = 20;
		lifes = 50;
		speed = 0.7f;
		max_frames = 12;
		frame_delay = 6;
		attack_delay = 50;
		break;

	case planes::evil2:
		new_dims(45.0f, 31.0f);
		strenght = 15;
		lifes = 40;
		speed = 0.9f;
		max_frames = 24;
		frame_delay = 3;
		attack_delay = 40;
		break;

	case planes::evil3:
		new_dims(55.0f, 49.0f);
		strenght = 22;
		lifes = 60;
		speed = 0.6f;
		max_frames = 24;
		frame_delay = 3;
		attack_delay = 45;
		break;

	case planes::evil4:
		new_dims(55.0f, 51.0f);
		strenght = 18;
		lifes = 55;
		speed = 0.5f;
		max_frames = 24;
		frame_delay = 3;
		attack_delay = 55;
		break;

	case planes::boss1:
		new_dims(200.0f, 67.0f);
		strenght = 35;
		lifes = 800;
		speed = 0.4f;
		max_frames = 4;
		frame_delay = 17;
		attack_delay = 80;
		break;

	case planes::boss2:
		new_dims(200.0f, 143.0f);
		strenght = 45;
		lifes = 1000;
		speed = 0.3f;
		max_frames = 2;
		frame_delay = 30;
		attack_delay = 150;
		break;

	case planes::boss3:
		new_dims(250.0f, 118.0f);
		strenght = 40;
		lifes = 1100;
		speed = 0.2f;
		max_frames = 10;
		frame_delay = 6;
		attack_delay = 180;
		break;
	}

	max_lifes = lifes;
	max_attack_delay = attack_delay;
	max_frame_delay = frame_delay;
}

void dll::EVILS::SetPathInfo(float to_x, float to_y)
{
	ver_dir = false;
	hor_dir = false;

	move_sx = start.x;
	move_sy = start.y;

	move_ex = to_x;
	move_ey = to_y;

	if (move_ex == move_sx || (move_ex > start.x && move_ex < end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_ey == move_sy || (move_ey > start.y && move_ey < end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - slope * start.x;
}

void dll::EVILS::Release()
{
	delete this;
}

bool dll::EVILS::move(float gear)
{
	float my_speed = speed + gear / 10.0f;

	if (hor_dir)
	{
		if (move_sx < move_ex)
		{
			start.x += my_speed;
			dir = dirs::right;
			set_edges();
			if (start.x > scr_width + 200.0f)return false;
		}
		else if (move_sx > move_ex)
		{
			start.x -= my_speed;
			set_edges();
			dir = dirs::left;
			if (end.x < -200.0f)return false;
		}
	}
	else if (ver_dir)
	{
		if (move_sy < move_ey)
		{
			start.y += my_speed;
			set_edges();
			dir = dirs::down;
			if (start.y > ground)return false;
		}
		else if (move_sy > move_ey)
		{
			start.y -= my_speed;
			set_edges();
			dir = dirs::up;
			if (end.y < -100.0f)return false;
		}
	}
	else
	{
		if (move_sx < move_ex)
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			if (start.y >= move_ey)dir = dirs::up_right;
			else dir = dirs::down_right;
			set_edges();
			if (start.x > scr_width + 200.0f || start.y > ground || end.y < -100.0f)return false;
		}
		else if (move_sx > move_ex)
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			if (start.y >= move_ey)dir = dirs::up_left;
			else dir = dirs::down_left;
			set_edges();
			if (end.x < -200.0f || start.y > ground || end.y < -100.0f)return false;
		}
	}

	return true;
}

planes dll::EVILS::get_type()const
{
	return _type;
}

int dll::EVILS::get_frame()
{
	--frame_delay;
	if (frame_delay < 0)
	{
		frame_delay = max_frame_delay;
		++current_frame;
		if (current_frame >= max_frames)current_frame = 0;
	}

	return current_frame;
}

void dll::EVILS::heal(int heal_points)
{
	if (lifes + heal_points > max_lifes)lifes = max_lifes;
	else lifes += heal_points;
}

int dll::EVILS::attack()
{
	--attack_delay;
	if (attack_delay < 0)
	{
		attack_delay = max_attack_delay;
		return strenght;
	}

	return 0;
}

dll::EVILS* dll::EVILS::create(planes what, float start_x, float start_y)
{
	return new EVILS(what, start_x, start_y);
}

actions dll::EVILS::AI_move(FPOINT hero_center, BAG<FPOINT>& EvilFleet, BAG<FPOINT>& Shots)
{
	if (_type != planes::boss1 && _type != planes::boss2 && _type != planes::boss3)
	{
		if (!EvilFleet.empty())SortBag(EvilFleet, center);
		if (!Shots.empty())SortBag(EvilFleet, center);

		if (Distance(center, hero_center) <= 200.0f)return actions::shoot;
		else if (Distance(center, hero_center) <= 300.0f)
		{
			SetPathInfo(hero_center.x, hero_center.y);
			return actions::move;
		}
		if (!EvilFleet.empty())
		{
			if (Distance(center, EvilFleet[0]) <= 110.0f)
			{
				if (center.x < EvilFleet[0].x)
				{
					if (center.y < EvilFleet[0].y)SetPathInfo(EvilFleet[0].x - 50.0f, sky);
					else if (center.y > EvilFleet[0].y)SetPathInfo(EvilFleet[0].x - 50.0f, ground);
					else SetPathInfo(EvilFleet[0].x - 50.0f, center.y);
				}
				else if (center.x > EvilFleet[0].x)
				{
					if (center.y < EvilFleet[0].y)SetPathInfo(EvilFleet[0].x + 50.0f, sky);
					else if (center.y > EvilFleet[0].y)SetPathInfo(EvilFleet[0].x + 50.0f, ground);
					else SetPathInfo(scr_width, center.y);
				}
				else
				{
					if (center.y > EvilFleet[0].y)SetPathInfo(center.x, ground);
					else if (center.y < EvilFleet[0].y)SetPathInfo(center.x, sky);
				}

				return actions::move;
			}
		}
		if (!Shots.empty())
		{
			if (Distance(center, Shots[0]) <= 50.0f)
			{
				if (center.x < Shots[0].x)
				{
					if (center.y < Shots[0].y)SetPathInfo(0, sky);
					else if (center.y > Shots[0].y)SetPathInfo(0, ground);
					else SetPathInfo(0, center.y);
				}
				else if (center.x > Shots[0].x)
				{
					if (center.y < Shots[0].y)SetPathInfo(scr_width, sky);
					else if (center.y > Shots[0].y)SetPathInfo(scr_width, ground);
					else SetPathInfo(scr_width, center.y);
				}
				else
				{
					if (center.y > Shots[0].y)SetPathInfo(center.x, ground);
					else SetPathInfo(center.x, sky);
				}

				return actions::flee;
			}
		}

		return actions::move;
	}

	if (Distance(center, hero_center) <= 200.0f)return actions::shoot;
	else if (Distance(center, hero_center) <= 300.0f)
	{
		SetPathInfo(hero_center.x, hero_center.y);
		return actions::move;
	}

	return actions::move;
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