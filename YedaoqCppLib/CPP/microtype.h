/* ___________________________________________________________________________
@ 
@ file - mytype.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-10
@ info -
@     type identify
/* ___________________________________________________________________________*/
#pragma once

#ifndef interface
#define interface struct
#endif

typedef unsigned char byte;

typedef unsigned int index_t;
typedef unsigned int flag_t;

#define SINGLELOOP for(bool __anonym = true;  __anonym; __anonym = false)

#define STDENUM(containertype, container, iter) for ( containertype::iterator iter = container.begin(); iter != container.end(); ++iter )
#define STDCENUM(containertype, container, iter) for ( containertype::const_iterator iter = container.begin(); iter != container.end(); ++iter )

#ifndef UNALIGNED
#define UNALIGNED
#endif

template <typename _CountofType, size_t _SizeOfArray>
char (*__countof_helper(UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
#define count_of(_Array) sizeof(*__countof_helper(_Array))