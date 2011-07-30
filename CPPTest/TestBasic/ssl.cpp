#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/safestack.h>
#define sk_Student_new(st) SKM_sk_new(Student, (st))
#define sk_Student_new_null() SKM_sk_new_null(Student)
#define sk_Student_free(st) SKM_sk_free(Student, (st))
#define sk_Student_num(st) SKM_sk_num(Student, (st))
#define sk_Student_value(st, i) SKM_sk_value(Student, (st), (i))
#define sk_Student_set(st, i, val) SKM_sk_set(Student, (st), (i), (val))
#define sk_Student_zero(st) SKM_sk_zero(Student, (st))
#define sk_Student_push(st, val) SKM_sk_push(Student, (st), (val))
#define sk_Student_unshift(st, val) SKM_sk_unshift(Student, (st), (val))
#define sk_Student_find(st, val) SKM_sk_find(Student, (st), (val))
#define sk_Student_delete(st, i) SKM_sk_delete(Student, (st), (i))
#define sk_Student_delete_ptr(st, ptr) SKM_sk_delete_ptr(Student, (st), (ptr))
#define sk_Student_insert(st, val, i) SKM_sk_insert(Student, (st), (val), (i))
#define sk_Student_set_cmp_func(st, cmp) SKM_sk_set_cmp_func(Student, (st), (cmp))
#define sk_Student_dup(st) SKM_sk_dup(Student, st)
#define sk_Student_pop_free(st, free_func) SKM_sk_pop_free(Student, (st), (free_func))
#define sk_Student_shift(st) SKM_sk_shift(Student, (st))
#define sk_Student_pop(st) SKM_sk_pop(Student, (st))
#define sk_Student_sort(st) SKM_sk_sort(Student, (st))

typedef struct Student_st
{
	char       *name;	
	int           age;	
	char       *otherInfo;	
}Student;
typedef  STACK_OF(Student)  Students;

Student *Student_Malloc()
{	
	Student *a=(Student *)malloc(sizeof(Student));	
	a->name=(char*)malloc(20);	
	strcpy(a->name,"zcp");	
	a->otherInfo=(char*)malloc(20);	
	strcpy(a->otherInfo,"no info");	
	return a;	
}

void Student_Free(Student *a)
{	
	free(a->name);	
	free(a->otherInfo);	
	free(a);	
}

static int Student_cmp(Student *a,Student *b)
{
	int           ret;	
	ret=strcmp(a->name,b->name);
	return ret;	
}



int main()

{	
	Students         *s,*snew;	
	Student          *s1,*one,*s2;	
	int                  i,num;	
	s=sk_Student_new_null();	
	snew=sk_Student_new(Student_cmp);	
	s2=Student_Malloc();	
	sk_Student_push(snew,s2);	
	i=sk_Student_find(snew,s2);	
	s1=Student_Malloc();	
	sk_Student_push(s,s1);	
	num=sk_Student_num(s);	
	for(i=0;i<num;i++)		
	{		
		one=sk_Student_value(s,i);		
		printf("student name :    %s\n",one->name);		
		printf("sutdentage  :     %d\n",one->age);		
		printf("student otherinfo :      %s\n\n\n",one->otherInfo);		
	}
    sk_Student_pop_free(s,Student_Free);	  
    sk_Student_pop_free(snew,Student_Free);
	return 0;	
}