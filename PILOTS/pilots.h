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
enum class planes { evil1 = 0, evil2 = 1, evil3 = 2, evil4 = 3, hero = 4, boss1 = 5, boss2 = 6, boss3 = 7 };
enum class assets { cloud1 = 0, cloud2 = 1, riff1 = 2, riff2 = 3, riff3 = 4 };

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
					T* temp_ptr{ reinterpret_cast<T*>(realloc(base_ptr,sizeof(T) * max_size)) }
					if (temp_ptr)base_ptr = temp_ptr;
					else return false;
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
					T* temp_ptr{ reinterpret_cast<T*>(realloc(base_ptr,sizeof(T) * max_size)) }
					if (temp_ptr)base_ptr = temp_ptr;
					else return false;
					base_ptr[next_pos] = (*element);
					++next_pos;
					is_valid = true;
				}
				else throw EXCEPTION(BAG_BAD_PTR);
			}
		}


		
	};



}