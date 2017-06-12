#pragma once
class CHttpRequestA
{
public:
	CHttpRequestA();
	CHttpRequestA(const char* httpRequestMessage);
	~CHttpRequestA();
public
	:
	std::string method;//请求类型
	std::string requestUrl;//请求函数
	std::string httpRequesetBody;//请求实体
};

