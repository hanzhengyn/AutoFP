#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include "HttpRequestA.h"
#include "json\json.h"
using namespace std;
CHttpRequestA::CHttpRequestA()
{
}
//�ַ����ָ��
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
CHttpRequestA::CHttpRequestA(const char* httpRequestMessage)
{
	std::string str;
	std::string pattern;
	str = httpRequestMessage;
	pattern = "\r\n";
	std::vector<std::string> resultLine = split(str, pattern);
	std::string pattern1 = " ";
	std::vector<std::string> resultApi = split(resultLine[0], pattern1);
	//��������
	method = resultApi[0];
	//"/uhome/writedevice1/getWriterInfo.api"
	std::string pattern2 = "/";
	std::vector<std::string> resultUrl = split(resultApi[1], pattern2);

	//����
	requestUrl = resultUrl[3];

	//������յ�������¼ָ�����Ҫ���յ��ı�������н���
	if (requestUrl.compare("requestWrite.api") == 0)
	{
		httpRequesetBody = resultLine[4];//�յ��ı�����

		//��ȡ�ͻ��˷�������д��Ϣ
		Json::Reader json_reader;
		Json::Value json_recv;//���յ����ַ���
		json_reader.parse(httpRequesetBody, json_recv);
		json_recv["c_factory"] = Json::Value("haier");
		json_recv["c_model"] = Json::Value("haier_xxx");
		gm_nSum = json_recv["c_sum"].asInt();
		json_recv["c_file_download"] = Json::Value("d:/temp/haier_xxx.mot");
		json_recv["c_checksum"] = Json::Value("FFFF");
		json_recv["c_filetype"] = Json::Value(0);
		json_recv["c_cache"] = Json::Value(0);
		json_recv["c_start_addr"] = Json::Value("0000");
		json_recv["c_flag"] = Json::Value(2);
		json_recv["c_project_file_updown"] = Json::Value("");
	}
}


CHttpRequestA::~CHttpRequestA()
{

}
