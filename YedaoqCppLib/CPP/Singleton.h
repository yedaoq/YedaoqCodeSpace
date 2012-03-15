#pragma once

template<typename T>
class CSingleton
{
public :
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};