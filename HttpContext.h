#pragma once
#include "HttpRequestA.h"
#include "HttpResponseA.h"
class CHttpContext
{
public:
	CHttpContext();
	CHttpContext(const char* reqStr);
	~CHttpContext();
public:
	CHttpRequestA request;
	CHttpResponseA response;
};

