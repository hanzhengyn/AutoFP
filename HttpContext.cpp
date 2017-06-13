#include "stdafx.h"
#include "HttpContext.h"

CHttpContext::CHttpContext()
{
}

CHttpContext::CHttpContext(const char* reqStr)
{
    //1.对传递过来的报文进行解析
    //解析后分别保存到Requset和Response对象中
    //把请求报文继续传递给HttpRequset对象，由HttpRequest对象对报文进行解析
    request = CHttpRequestA(reqStr);

    //创建一个Response来响应
    response = CHttpResponseA(request);
}

CHttpContext::~CHttpContext()
{

}
