#pragma once
#include "HttpContext.h"
class CHttpApplication
{
public:
	CHttpApplication();
	~CHttpApplication();
public:
	void ProcessRequest(CHttpContext context);
};

