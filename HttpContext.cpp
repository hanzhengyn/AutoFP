#include "stdafx.h"
#include "HttpContext.h"

CHttpContext::CHttpContext()
{
}

CHttpContext::CHttpContext(const char* reqStr)
{
    //1.�Դ��ݹ����ı��Ľ��н���
    //������ֱ𱣴浽Requset��Response������
    //�������ļ������ݸ�HttpRequset������HttpRequest����Ա��Ľ��н���
    request = CHttpRequestA(reqStr);

    //����һ��Response����Ӧ
    response = CHttpResponseA(request);
}

CHttpContext::~CHttpContext()
{

}
