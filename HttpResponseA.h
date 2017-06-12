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
	std::string responseBody;//��Ӧ������
	std::string responseHeader;//��Ӧ����ͷ
	std::string responseTotal;//��Ӧ�ı�
private:
	CHttpRequestA httpRequest;
};

