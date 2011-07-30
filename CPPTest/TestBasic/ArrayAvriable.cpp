#include "stdafx.h"
#include <string.h>
#include <stdio.h>

/* replace the blank in the string with comma */
void StringDivide(char *pInputString, long lInputStringLen, char **pOutputString)
{
	char *pcBegin = NULL, *pcEnd = NULL;

	if(NULL == pInputString || NULL == pOutputString || lInputStringLen <= 0)
		return;

	memcpy(*pOutputString, pInputString, lInputStringLen);
	(*pOutputString)[lInputStringLen] = '\0';

	pcBegin = *pOutputString;
	pcEnd = pcBegin + lInputStringLen;

	while(pcBegin < pcEnd)
	{
		if(' ' == *pcBegin)
			*pcBegin = ',';
		++pcBegin;
	}
}



/* replace the blank in the string with comma */
void StringDivideB(char *pInputString, long lInputStringLen, char pOutputString[])
{
	char *pcBegin = NULL, *pcEnd = NULL;
	
	if(NULL == pInputString || NULL == pOutputString || lInputStringLen <= 0)
		return;
	
	memcpy(pOutputString, pInputString, lInputStringLen);
	pOutputString[lInputStringLen] = '\0';
	
	pcBegin = pOutputString;
	pcEnd = pcBegin + lInputStringLen;
	
	while(pcBegin < pcEnd)
	{
		if(' ' == *pcBegin)
			*pcBegin = ',';
		++pcBegin;
	}
}

/* replace the blank in the string with comma */
void StringDivideA(char *pInputString, long lInputStringLen, char *pOutputString)
{
	char *pcBegin = NULL, *pcEnd = NULL;
	
	if(NULL == pInputString || NULL == pOutputString || lInputStringLen <= 0)
		return;
	
	memcpy(pOutputString, pInputString, lInputStringLen);
	pOutputString[lInputStringLen] = '\0';
	
	pcBegin = pOutputString;
	pcEnd = pcBegin + lInputStringLen;
	
	while(pcBegin < pcEnd)
	{
		if(' ' == *pcBegin)
			*pcBegin = ',';
		++pcBegin;
	}
}

int mainArrayAvriable(int argc, char* argv[])
{
	char pInputString[] = "Hello world ";
	char pOutputString[100] = {0};

	StringDivide(pInputString, strlen(pInputString), (char **)pOutputString);


	char *pOutputStringA = new char[100];
	char *pOutputStringB = pOutputString;
	char **ppOutputString = &pOutputStringA;

	StringDivideA(pInputString, strlen(pInputString), pOutputString);
	StringDivideA(pInputString, strlen(pInputString), pOutputStringA);
	StringDivideA(pInputString, strlen(pInputString), pOutputStringB);
	StringDivideA(pInputString, strlen(pInputString), *ppOutputString);

	StringDivideB(pInputString, strlen(pInputString), pOutputString);

	//StringDivide(pInputString, strlen(pInputString), (char **)pOutputString);

	printf(pOutputString);
	//printf(pOutputStringA);

	getchar();
	
	return 0;
}