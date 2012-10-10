// Win32Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "FileEnumerator.h"

using namespace std;

int TestFileEnum(int argc, _TCHAR* argv[])
{
	TCHAR cur_dir[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, cur_dir);

	cout<<"file in current directory : \n\n";
	CFileEnumerator file_i(cur_dir, 0, CFileEnumerator::SUBFILE);
	while(file_i.MoveNext())
	{
		wcout<<file_i.Current().cFileName<<endl;
	}

	cout<<"\n\nsub directory in current directory : \n\n";
	file_i.Reset(cur_dir, 0, CFileEnumerator::SUBDIR);
	while(file_i.MoveNext())
	{
		wcout<<file_i.Current().cFileName<<endl;
	}

	cout<<"\n\nsub item in current directory : \n\n";
	file_i.Reset(cur_dir, 0, CFileEnumerator::SUBALL);
	while(file_i.MoveNext())
	{
		wcout<<file_i.Current().cFileName<<endl;
	}

	cout<<"\n\nsub file in current directory (recusive) : \n\n";
	file_i.Reset(cur_dir, 0, CFileEnumerator::SUBFILE | CFileEnumerator::RECUSIVE);
	while(file_i.MoveNext())
	{
		wcout<<file_i.CurrentDir().c_str() + _tcslen(cur_dir)<<'\\'<<file_i.Current().cFileName<<endl;
	}

	cout<<"\n\nsub dir in current directory (recusive) : \n\n";
	file_i.Reset(cur_dir, 0, CFileEnumerator::SUBDIR | CFileEnumerator::RECUSIVE);
	while(file_i.MoveNext())
	{
		wcout<<file_i.CurrentDir().c_str() + _tcslen(cur_dir)<<'\\'<<file_i.Current().cFileName<<endl;
	}

	cout<<"\n\nsub item in current directory (recusive) : \n\n";
	file_i.Reset(cur_dir, 0, CFileEnumerator::SUBALL | CFileEnumerator::RECUSIVE);
	while(file_i.MoveNext())
	{
		wcout<<file_i.CurrentDir().c_str() + _tcslen(cur_dir)<<'\\'<<file_i.Current().cFileName<<endl;
	}

	getchar();
	return 0;
}

