#pragma once

#ifndef SINGLETONE_H_
#define SINGLETONE_H_

/*template<typename T>
class SingletonROM<T, typename std::enable_if_t<std::is_base_of<RomObject, T>::value>> {
	public:
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton() = delete;

	static constexpr const T& GetInstance()   {
		return instance;
	}
	private:
	static constexpr T instance{T()};
};
template<typename T>
constexpr T Singleton<T,typename std::enable_if_t<std::is_base_of<RomObject, T>::value>>::instance ;*/

template<typename T, class Enable = void>
class Singleton {
	public:
		Singleton(const Singleton&) = delete;
		Singleton& operator = (const Singleton&) = delete;
		Singleton() =  delete;
	
		constexpr static T& Instance()   {
			return instance;
		}
	private:
		static T instance;
};

template<typename T, class Enable>
T Singleton<T,Enable>::instance;

/*template<typename T>
class Singleton {
	protected:
	Singleton() noexcept = default;

	Singleton(const Singleton&) = delete;

	Singleton& operator=(const Singleton&) = delete;

	virtual ~Singleton() = default; // to silence base class Singleton<T> has a
	// non-virtual destructor [-Weffc++]

	public:
	static T& Instance() noexcept {
		// Guaranteed to be destroyed.
		// Instantiated on first use.
		// Thread safe in C++11
		static T instance;

		return instance;
	}
};*/

#endif