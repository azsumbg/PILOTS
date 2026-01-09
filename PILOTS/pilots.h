#pragma once

#ifdef PILOTS_EXPORTS 
#define PILOTS_API __declspec(dllexport)
#else 
#define PILOTS_API __declspec(dllimport)
#endif

#include <random>
#include <type_traits>

constexpr int BAG_BAD_PTR{ 15001 };
constexpr int BAG_BAD_INDEX{ 15002 };
constexpr int BAG_BAD_ARG{ 15003 };
constexpr int BAG_UNKNOWN_ERROR{ 15004 };

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

enum class dirs { stop = 0, up = 1, down = 2, left = 3, right = 4, up_left = 5, up_right = 6, down_left = 7, down_right = 8 };
enum class planes { evil1 = 0, evil2 = 1, evil3 = 2, evil4 = 3, boss1 = 4, boss2 = 5, boss3 = 6 };
enum class assets { cloud1 = 0, cloud2 = 1, riff1 = 2, riff2 = 3, riff3 = 4 };

enum class actions { move = 0, shoot = 1, flee = 2, need_order = 3 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};
struct FRECT
{
	float left{ 0 };
	float up{ 0 };
	float right{ 0 };
	float down{ 0 };
};

namespace dll
{
	class PILOTS_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:

		RANDIT();
		~RANDIT();

		int operator() (int min, int max);
	};

	class PILOTS_API EXCEPTION
	{
	private:
		const wchar_t* bad_ptr = L"BAG base pointer is nullptr !";
		const wchar_t* bad_index = L"BAG provided index is wrong !";
		const wchar_t* bad_arg = L"BAG bad argument provided in constructor !";
		const wchar_t* unknown_err = L"BAG unknown error ocurred !";

		int error{ 0 };

	public:
		EXCEPTION(int what_err)
		{
			error = what_err;
		}

		const wchar_t* GetErr()
		{
			switch (error)
			{
			case BAG_BAD_PTR:
				return bad_ptr;

			case BAG_BAD_INDEX:
				return bad_index;

			case BAG_BAD_ARG:
				return bad_arg;

			case BAG_UNKNOWN_ERROR:
				return unknown_err;
			}

			return unknown_err;
		}

	};

	template<typename T>class BAG
	{
		T* base_ptr{ nullptr };
		size_t max_size{ 0 };
		size_t next_pos{ 0 };

		bool is_valid{ false };

	public:
		BAG() :base_ptr{ reinterpret_cast<T*>(calloc(1,sizeof(T))) }, max_size{ 1 }{}
		BAG(size_t lenght) : base_ptr{ reinterpret_cast<T*>(calloc(lenght,sizeof(T))) }, max_size(lenght) {}

		BAG(BAG& other)
		{
			if (other.base_ptr)
			{
				if (this->base_ptr != other.base_ptr)
				{
					free(this->base_ptr);

					max_size = other.max_size;
					next_pos = other.next_pos;

					base_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
					if (base_ptr)
					{
						for (size_t count = 0; count < next_pos; ++count)base_ptr[count] = other[count];
					}
					else throw EXCEPTION(BAG_BAD_PTR);
				}
				else throw EXCEPTION(BAG_BAD_ARG);
			}
			else throw EXCEPTION(BAG_BAD_PTR);
		}
		BAG(BAG&& other)
		{
			if (this->base_ptr != other.base_ptr)
			{
				if (other.base_ptr)
				{
					if (base_ptr)free(base_ptr);

					base_ptr = other.base_ptr;
					max_size = other.max_size;
					next_pos = other.next_pos;

					other.base_ptr = nullptr;
				}
				else throw EXCEPTION(BAG_BAD_PTR);
			}
			else throw EXCEPTION(BAG_BAD_ARG);
		}

		~BAG()
		{
			if (base_ptr)free(base_ptr);
		}

		bool valid()const
		{
			if (base_ptr && is_valid)return true;

			return false;
		}
		bool empty()const
		{
			if (next_pos == 0)return true;

			return false;
		}
		size_t capacity()const
		{
			return max_size;
		}
		size_t size()const
		{
			return next_pos;
		}

		void push_back(T element)
		{
			if (next_pos + 1 <= max_size)
			{
				if (base_ptr)
				{
					base_ptr[next_pos] = element;
					++next_pos;
				}
				else throw EXCEPTION(BAG_BAD_PTR);
			}
			else
			{
				if (base_ptr)
				{
					++max_size;
					T* temp_ptr{ reinterpret_cast<T*>(realloc(base_ptr,sizeof(T) * max_size)) };
					if (temp_ptr)base_ptr = temp_ptr;
					else throw EXCEPTION(BAG_BAD_PTR);
					base_ptr[next_pos] = element;
					++next_pos;
					is_valid = true;
				}
				else throw EXCEPTION(BAG_BAD_PTR);
			}
		}
		void push_back(T* element)
		{
			if (next_pos + 1 <= max_size)
			{
				if (base_ptr)
				{
					base_ptr[next_pos] = (*element);
					++next_pos;
				}
				else throw EXCEPTION(BAG_BAD_PTR);
			}
			else
			{
				if (base_ptr)
				{
					++max_size;
					T* temp_ptr{ reinterpret_cast<T*>(realloc(base_ptr,sizeof(T) * max_size)) };
					if (temp_ptr)base_ptr = temp_ptr;
					else throw EXCEPTION(BAG_BAD_PTR);
					base_ptr[next_pos] = (*element);
					++next_pos;
					is_valid = true;
				}
				else throw EXCEPTION(BAG_BAD_PTR);
			}
		}

		void push_front(T element)
		{
			if (base_ptr)
			{
				if (next_pos == 0)
				{
					(*base_ptr) = element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t count = 0; count < next_pos; ++count)
						{
							T next_element{ base_ptr[count + 1] };
							base_ptr[count + 1] = base_ptr[count];
							if (count == 0)base_ptr[count] = element;
						}
						++next_pos;
					}
					else
					{
						++max_size;
						++next_pos;
						T* temp_ptr{ reinterpret_cast<T*>(realloc(base_ptr, sizeof(T) * max_size)) };
						free(base_ptr);
						if (temp_ptr)base_ptr = temp_ptr;
						else throw EXCEPTION(BAG_BAD_PTR);

						for (size_t count = 0; count < next_pos; ++count)
						{
							T next_element{ base_ptr[count + 1] };
							base_ptr[count + 1] = base_ptr[count];
							if (count == 0)base_ptr[count] = element;
						}
					}
				}
			}
			else throw EXCEPTION(BAG_BAD_PTR);
		}
		void push_front(T* element)
		{
			if (base_ptr)
			{
				if (next_pos == 0)
				{
					(*base_ptr) = (*element);
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t count = 0; count < next_pos; ++count)
						{
							T next_element{ base_ptr[count + 1] };
							base_ptr[count + 1] = base_ptr[count];
							if (count == 0)base_ptr[count] = (*element);
						}
						++next_pos;
					}
					else
					{
						++max_size;
						++next_pos;
						T* temp_ptr{ reinterpret_cast<T*>(realloc(base_ptr, sizeof(T) * max_size)) };
						free(base_ptr);
						if (temp_ptr)base_ptr = temp_ptr;
						else throw EXCEPTION(BAG_BAD_PTR);

						for (size_t count = 0; count < next_pos; ++count)
						{
							T next_element{ base_ptr[count + 1] };
							base_ptr[count + 1] = base_ptr[count];
							if (count == 0)base_ptr[count] = (*element);
						}
					}
				}
			}
			else throw EXCEPTION(BAG_BAD_PTR);
		}

		void erase(size_t index)
		{
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (!base_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				for (size_t count = index; count < next_pos - 1; ++count)base_ptr[count] = base_ptr[count + 1];
				next_pos--;
			}
		}

		T& operator[] (size_t index)
		{
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (!base_ptr)throw EXCEPTION(BAG_BAD_PTR);
			
			return base_ptr[index];
		}

		BAG& operator = (BAG& other)
		{
			if (this->base_ptr == other.base_ptr)throw EXCEPTION(BAG_BAD_ARG);

			free(base_ptr);
			base_ptr = other.base_ptr;
			max_size = other.max_size;
			next_pos = other.next_pos;

			return (*this);
		}
		BAG& operator = (BAG&& other)
		{
			if (this->base_ptr == other.base_ptr)throw EXCEPTION(BAG_BAD_ARG);

			free(base_ptr);

			base_ptr = other.base_ptr;
			max_size = other.max_size;
			next_pos = other.next_pos;

			other.base_ptr = nullptr;

			return (*this);
		}
	};

	template<typename T>concept primes = (std::is_same<char, T>::value || std::is_same<wchar_t, T>::value
		|| std::is_same<int, T>::value, std::is_same<float, T>::value || std::is_same<double, T>::value
		|| std::is_same<long, T>::value);

	template<primes T> bool PrimeSort(BAG<T>& bag, bool ascending = true)
	{
		if (bag.size() < 2)return false;

		bool is_ok = false;

		while (!is_ok)
		{
			is_ok = true;

			for (size_t count = 0; count < bag.size() - 1; ++count)
			{
				if (ascending)
				{
					if (bag[count] > bag[count + 1])
					{
						T temp{ bag[count] };
						bag[count] = bag[count + 1];
						bag[count + 1] = temp;
						is_ok = false;
					}
				}
				else
				{
					if (bag[count] < bag[count + 1])
					{
						T temp{ bag[count] };
						bag[count] = bag[count + 1];
						bag[count + 1] = temp;
						is_ok = false;
					}
				}
			}
		}

		return true;
	}

	/////////////////////////////////////////////////////

	class PILOTS_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };
		bool in_heap{ false };

	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};
		float radius_x{ 0 };
		float radius_y{ 0 };

		PROTON();
		PROTON(float _sx, float _sy);
		PROTON(float _sx, float _sy, float _s_width, float _s_height);

		virtual ~PROTON() {};

		static PROTON* create(float _start_x, float _start_y, float _start_width, float _start_height);

		void set_edges();
		void set_width(float new_width);
		void set_height(float new_height);
		void new_dims(float new_width, float new_height);
		void Release();

		float get_width() const;
		float get_height() const;
	};

	class PILOTS_API TILE :public PROTON
	{
	private:
		float my_speed{ 1.0f };

		TILE(float _s_x, float _s_y);

	public:
		dirs dir{ dirs::stop };
		bool to_erase = false;

		bool move();

		void Release();

		static TILE* create(float _first_x, float _first_y);
	};

	class PILOTS_API ASSETS :public PROTON
	{
	private:
		float _speed{ 0 };
		assets type{ assets::cloud1 };

		ASSETS(assets _what, float _s_x, float _s_y);

	public:
		dirs dir{ dirs::stop };
		
		assets get_type() const;
		void Release();
		bool move(float gear);

		static ASSETS* create(assets what_type, float where_x, float where_y);
	};

	class PILOTS_API SHOT :public PROTON
	{
	private:
		float _speed = 3.0f;
		int current_frame = 0;

		bool hor_dir{ false };
		bool ver_dir{ false };

		float move_sx{ 0 };
		float move_ex{ 0 };

		float move_sy{ 0 };
		float move_ey{ 0 };

		float slope{ 0 };
		float intercept{ 0 };

		SHOT(float _where_x, float _where_y, float _to_where_x, float _to_where_y);

		void SetPathInfo(float to_x, float to_y);

	public:
		int strenght = 0;

		void Release();

		bool Move(float gear);

		int get_frame();

		static SHOT* create(float where_x, float where_y, float to_where_x, float to_where_y);
	};

	class PILOTS_API HERO :public PROTON
	{
	private:
		float speed;

		int max_frames{ 3 };
		int frame_delay{ 20 };
		int current_frame{ 0 };

		int max_lifes{ 100 };

	public:
		dirs dir = dirs::stop;
		int lifes{ 100 };

		HERO(float sx, float sy);

		void move(float gear = 1.0f);

		void heal(int heal_points);

		int get_frame();

	};

	class PILOTS_API EVILS :public PROTON
	{
	private:
		bool hor_dir{ false };
		bool ver_dir{ false };

		float move_sx{ 0 };
		float move_ex{ 0 };

		float move_sy{ 0 };
		float move_ey{ 0 };

		float slope{ 0 };
		float intercept{ 0 };

		int max_lifes{ 0 };

		float speed{ 0 };

		int current_frame{ 0 };
		int frame_delay{ 0 };
		int max_frame_delay{ 0 };
		int max_frames{ 0 };

		int strenght{ 0 };
		int attack_delay{ 0 };
		int max_attack_delay{ 0 };

		planes _type;
		
		EVILS(planes _what, float _start_x, float _start_y);

	public:
		dirs dir{ dirs::stop };
		int lifes{ 0 };
		
		void SetPathInfo(float to_x, float to_y);

		void Release();

		bool move(float gear);

		planes get_type()const;

		int get_frame();

		void heal(int heal_points);

		int attack();

		static EVILS* create(planes what, float start_x, float start_y);

		actions AI_move(FPOINT hero_center, BAG<FPOINT>& EvilFleet, BAG<FPOINT>& Shots);
	};

	//FUNCTIONS ********************************

	float PILOTS_API Distance(FPOINT first, FPOINT second);

	bool PILOTS_API SortBag(BAG<FPOINT>& Bag, FPOINT ref_point);
	
	bool PILOTS_API Intersect(FRECT first, FRECT second);

	bool PILOTS_API Intersect(FPOINT center1, FPOINT center2, float radius1_x, float radius2_x,
		float radius1_y, float radius2_y);
}