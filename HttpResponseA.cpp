#include "stdafx.h"
#include "HttpResponseA.h"
#include "json\json.h"


CHttpResponseA::CHttpResponseA()
{
}
CHttpResponseA::CHttpResponseA(CHttpRequestA &httpRequest)
{
    this->httpRequest = httpRequest;
    InitResponseBody();
    InitResponseHeader();
    responseTotal = responseHeader + responseBody;
}


CHttpResponseA::~CHttpResponseA()
{
    //free(responseBody);
    //free(responseHeader);
}

void CHttpResponseA::InitResponseBody()
{
    Json::Value json_send;//待发送的字符串
    Json::StyledWriter json_fast_writer;
    std::string str = httpRequest.requestUrl;
    if (str.compare("getWriterInfo.api") == 0)
    {
        json_send["retcode"] = Json::Value("00000");
        json_send["retinfo"] = Json::Value("操作成功");
        json_send["c_ID"] = Json::Value("1234567890");
        json_send["c_manufactor"] = Json::Value("”XXXXXX”");
        json_send["c_type"] = Json::Value("”XXXXX_XXXXXX”");
        json_send["c_info"] = Json::Value("………");
    }
    else if (str.compare("requestWrite.api") == 0)
    {
        json_send["retcode"] = Json::Value("00000");
        json_send["retinfo"] = Json::Value("操作成功");
    }
    else if (str.compare("getProgress.api") == 0)
    {
        json_send["retcode"] = Json::Value("00000");
        json_send["retinfo"] = Json::Value("操作成功");
        json_send["c_state"] = Json::Value("run");
        json_send["c_total"] = Json::Value(gm_nSum);
        json_send["c_stolal"] = Json::Value(gm_nTotalGoodSize);
        json_send["c_ftolal"] = Json::Value(gm_nTotalFailSize);
    }
    responseBody = json_fast_writer.write(json_send);

}

void CHttpResponseA::InitResponseHeader()
{
    const char *strVersion = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n";
    CString strLength;
    strLength.Format("Content-Length: %d\r\n\r\n", sizeof(responseBody));
    const size_t len = strlen(strVersion) + strlen(strLength);
    responseHeader = strVersion + strLength;
}