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

#define STDENUM(containertype, container, iter) for ( containertype::iterator iter = container.begin(); iter != container.end(); ++iter )