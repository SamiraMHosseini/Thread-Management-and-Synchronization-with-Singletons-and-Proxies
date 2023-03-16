//----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <conio.h>

class  CtrlSingleton
{
public:
	static CtrlSingleton& GetInstance()
	{
		static CtrlSingleton instance;
		return instance;
	}
	static std::mutex& GetMutex()
	{
		CtrlSingleton& instance = CtrlSingleton::GetInstance();
		return instance.mtx_key;

	}
	static std::condition_variable& GetCV()
	{
		CtrlSingleton& instance = CtrlSingleton::GetInstance();
		std::condition_variable& cv_key = instance.cv_key;
		return cv_key;

	}

private:
	CtrlSingleton() :
		mtx_key(), cv_key() {}
	CtrlSingleton(const CtrlSingleton&) = delete;
	CtrlSingleton& operator=(const CtrlSingleton&) = delete;
	std::mutex				 mtx_key;
	std::condition_variable  cv_key;

};
class ThrdCounter
{

public:
	/*

	When you declare a static variable within a function,
	it is initialized only once, the first time the function is called.
	In the case of a singleton pattern, this ensures that there is only one instance of the class
	created throughout the lifetime of the program. The default constructor is called when the instance is initialized.

	*/

	static ThrdCounter& GetInstance()
	{
		/*
		When you create a reference, it does not allocate new memory for the object.
		Instead, it creates an alias to the original object. In the case of the singleton pattern,
		using a reference ensures that you are working with the same instance of the object throughout the program.

		*/

		static ThrdCounter instance; //This line will call the default constructor to create the instance object of the TCSingleton class.
		return instance;
	}

	static void TCUP()
	{
		ThrdCounter& instance = ThrdCounter::GetInstance();
		std::lock_guard<std::mutex> lock(instance.mtx);
		++instance.count;
		Debug::out("TC:%d \n", instance.count);

	}
	static void TCDown()
	{
		ThrdCounter& instance = ThrdCounter::GetInstance();
		std::lock_guard<std::mutex> lock(instance.mtx);
		--instance.count;
		Debug::out("~TC:%d \n", instance.count);
		if (instance.count == 0)
		{
			instance.cv_thread_count.notify_one();

		}

	}

	static std::condition_variable& GetCVTC()
	{
		ThrdCounter& instance = ThrdCounter::GetInstance();
		//Version 1 

		/*std::condition_variable& cvTC = instance.cv_thread_count;
		return cvTC;*/

		//Version 2
		return instance.cv_thread_count;
	}
	static std::mutex& GetMutextTC()
	{
		ThrdCounter& instance = ThrdCounter::GetInstance();
		return instance.mtx_thread_count;

	}


private:

	ThrdCounter() : mtx(), cv_thread_count(), mtx_thread_count() {}
	ThrdCounter(const ThrdCounter&) = delete;
	ThrdCounter& operator=(const ThrdCounter&) = delete;

	int count;
	std::mutex mtx;

	std::condition_variable  cv_thread_count;
	std::mutex				 mtx_thread_count;
};

struct Proxy
{
	Proxy()
	{
		ThrdCounter::TCUP();
	}
	~Proxy()
	{

		ThrdCounter::TCDown();
	}

};
class A : public BannerBase
{
public:
	A() = delete;
	A(const char* const pName) : BannerBase(pName) {}

	void operator()(std::future<void>& tFuture)
	{
		START_BANNER;


		Proxy a;

		int count = 0;
		while (true)
		{
			Debug::out("%d\n", count);
			count++;


			std::future_status status = tFuture.wait_for(1s);
			if (status == std::future_status::ready)
			{
				// can't read this multiple times
				//tFuture.get();
				break;
			}
		}

		
	}
};

class B : public BannerBase
{
public:
	B() = delete;
	B(const char* const pName) : BannerBase(pName) {}

	void operator()(std::future<void>& tFuture)
	{
		START_BANNER;


		Proxy b;

		int count = 0x10000;
		while (true)
		{
			Debug::out("0x%x\n", count);
			count--;

			std::future_status status = tFuture.wait_for(2s);
			if (status == std::future_status::ready)
			{
				// can't read this multiple times
				//tFuture.get();
				break;
			}
		}
		
	}
};


class C : public BannerBase
{
public:
	C() = delete;
	C(const char* const pName) : BannerBase(pName) {}

	void operator()(std::future<void>& tFuture)
	{
		START_BANNER;


		Proxy c;

		const char* pFruit[] =
		{
			{"apple"},
			{"orange"},
			{"banana"},
			{"lemon"}
		};

		int count = 0;
		while (true)
		{
			int i = count++ % 4;
			Debug::out("%s\n", pFruit[i]);


			std::future_status status =
				//tFuture.wait_until(std::chrono::system_clock::now());
				tFuture.wait_for(500ms);
			if (status == std::future_status::ready)
			{
				break;
			}
		}
		
	}
};

class D : public BannerBase
{
public:
	D() = delete;
	D(const char* const pName) : BannerBase(pName) {}

	void operator()(std::future<void>& tFuture)
	{
		START_BANNER;


		Proxy d;

		const char* pStoryOriginal = "<0><1><2><3><4><5><6><7><8>";
		size_t  len = strlen(pStoryOriginal);
		char* pString = new char[len + 1];
		strcpy_s(pString, len + 1, pStoryOriginal);

		int tmpLen = (int)(len - 1);

		while (true)
		{
			if (tmpLen <= 0)
			{
				tmpLen = (int)(len - 1);
				strcpy_s(pString, len + 1, pStoryOriginal);
			}

			Debug::out("%s\n", pString);
			pString[tmpLen--] = 0;
			pString[tmpLen--] = 0;
			pString[tmpLen--] = 0;

			std::future_status status = tFuture.wait_for(750ms);
			if (status == std::future_status::ready)
			{
				// can't read this multiple times
				//tFuture.get();
				break;
			}
		}

		delete pString;
		std::this_thread::sleep_for(2s);
		
	}
};

class Controller : public BannerBase
{
public:
	Controller() = delete;
	Controller(const char* const pName) : BannerBase(pName) {}

	void operator()(std::promise<void> tPromise)
	{
		START_BANNER;

		/*std::unique_lock<std::mutex> lock_key(mtx_key);
		cv_key.wait(lock_key);*/

		std::unique_lock<std::mutex> lock(CtrlSingleton::GetMutex());
		CtrlSingleton::GetCV().wait(lock);
		Debug::out("key pressed <----  \n");
		// kill them now
		tPromise.set_value();

		// wait until all are done
		std::unique_lock<std::mutex> lock_ct(ThrdCounter::GetMutextTC());
		ThrdCounter::GetCVTC().wait(lock_ct);
	}
};

int main()
{
	// time to move the console window over 
	//std::this_thread::sleep_for(3s);

	START_BANNER_MAIN("main");

	// start threads
	A oA("A");
	B oB("B");
	C oC("C");
	D oD("D");
	Controller oController("Controller");

	// Spawn threads 
	std::promise<void> tPromise;
	std::future<void> tFuture = tPromise.get_future();

	std::thread tA(oA, std::ref(tFuture));
	tA.detach();

	std::thread tB(oB, std::ref(tFuture));
	tB.detach();

	std::thread tC(oC, std::ref(tFuture));
	tC.detach();

	std::thread tD(oD, std::ref(tFuture));
	tD.detach();

	std::future<void> fuController = async(std::launch::async,
		std::move(oController),
		std::move(tPromise));

	// Key press
	_getch();
	CtrlSingleton::GetCV().notify_one();

	// Controller are you done?
	fuController.get();

}

// ---  End of File ---
