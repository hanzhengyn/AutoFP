#pragma once
class CHttpRequestA
{
public:
	CHttpRequestA();
	CHttpRequestA(const char* httpRequestMessage);
	~CHttpRequestA();
public
	:
	std::string method;//��������
	std::string requestUrl;//������
	std::string httpRequesetBody;//����ʵ��
};

