#pragma once
#include "Singlton.h"
#include <mytype.h>

class CDwarfOptions : public CSingleton<CDwarfOptions>
{
public:
	CDwarfOptions(void);
	~CDwarfOptions(void);

	int	Load();
	int Save();

public:
	tstring		DirCodecFile;		//解码器文件目录

	tstring		DirPackageFile;		//包文件目录

	tstring		DirMediaFileA;		//媒体文件目录
	tstring		DirMediaFileB;		//媒体文件目录
	tstring		DirMediaFileC;		//媒体文件目录

	tstring		DirDataBase;		//数据库目录
};
