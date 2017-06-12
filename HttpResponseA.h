#pragma once
#include "HttpRequestA.h"
class CHttpResponseA
{
public:
	CHttpResponseA();
	CHttpResponseA(CHttpRequestA &httpRequest);
	~CHttpResponseA();
public:
	void InitResponseBody();
	void InitResponseHeader();
	std::string responseBody;//响应报文体
	std::string responseHeader;//响应报文头
	std::string responseTotal;//响应文本
private:
	CHttpRequestA httpRequest;
};

