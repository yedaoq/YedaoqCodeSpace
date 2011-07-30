//结论：当类包含引用类型成员或常量数据成员时，编译器能够：
//      1. 生成默认拷贝构造函数，但将忽略对引用类型成员和常量数据成员的初始化。
//      2. 不生成默认赋值运算符

#include "StdAfx.h"
#include <string>

using namespace std;

template<class T>
class NamedObject
{
	public:
		NamedObject(std::string& name, const T& val):nameValue(name),objValue(val){}

	private:
		std::string& nameValue;
		const T objValue;
};

int mainRMACD()
{
	std::string strA("strA");
	std::string strB("strB");
	NamedObject<int> objA(strA,2);
	NamedObject<int> objB(strB,3);
	NamedObject<int> objC(objA);
	//objB = objA;
	return 1;
}