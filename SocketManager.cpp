// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlconv.h>
//#include "AutoFPDlg.h"
#include "SocketManager.h"
#include <string>
#include <vector>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/*
const UINT EVT_CONSUCCESS = 0x0000;	// Connection established
const UINT EVT_CONFAILURE = 0x0001;	// General failure - Wait Connection failed
const UINT EVT_CONDROP	  = 0x0002;	// Connection dropped
const UINT EVT_ZEROLENGTH = 0x0003;	// Zero length message
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketManager::CSocketManager()
    : m_pMsgCtrl(NULL)
{
    m_pMain = NULL;
}

CSocketManager::~CSocketManager()
{

}
//字符转十六进制
char HexChar(char c)
{
    if ((c >= '0') && (c <= '9'))
        return char(c - 0x30);
    else if ((c >= 'A') && (c <= 'F'))
        return char(c - 'A' + 10);
    else if ((c >= 'a') && (c <= 'f'))
        return char(c - 'a' + 10);
    else if (c == ' ')
        return char(0x00);
    else
        return char(0x10);

}
int str_to_hex(char* string, unsigned char* cbuf, int len)
{
    BYTE high, low;
    int idx, ii = 0;
    for (idx = 0; idx < len; idx += 2)
    {
        high = string[idx];
        low = string[idx + 1];

        if (high >= '0' && high <= '9')
            high = high - '0';
        else if (high >= 'A' &&high <= 'F')
            high = high - 'A' + 10;
        else if (high >= 'a' && high <= 'f')
            high = high - 'a' + 10;
        else
            return -1;

        if (low >= '0' && low <= '9')
            low = low - '0';
        else if (low >= 'A' &&low <= 'F')
            low = low - 'A' + 10;
        else if (high >= 'a' && low <= 'f')
            low = low - 'a' + 10;
        else
            return -1;

        cbuf[ii++] = high << 4 | low;
    }
    return 0;
}
//字符串分割函数
std::vector<std::string> split1(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
void CSocketManager::DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom)
{
    CString strData;
    memcpy(strData.GetBuffer(dwCount), A2CT((LPSTR)lpData), dwCount);
    strData.ReleaseBuffer(dwCount);

    if (!sfrom.IsNull())
    {
        LONG  uAddr = sfrom.GetIPAddr();
        BYTE* sAddr = (BYTE*)&uAddr;
        int nPort = ntohs(sfrom.GetPort()); // show port in host format...
        CString strAddr;
        // Address is stored in network format...
        strAddr.Format(_T("%u.%u.%u.%u (%d)>"),
            (UINT)(sAddr[0]),
            (UINT)(sAddr[1]),
            (UINT)(sAddr[2]),
            (UINT)(sAddr[3]),
            nPort);

        strData = strAddr + strData;
    }

    //初始化进度条。 提供编程器序号，正在进行的动作Download, 适配器的状态: 0F,表示对应的四个适配器正常。进度条的最大值：0x8A5D5, 进度条的当前位置: 0, 进度条的布进：1
    //MT_PBINIT#SITE[0]-DOWNLOAD-Adp:0F-PbStep:00000001-PbMax:0009A5D5-PbPos:00000000
    int nPbInit = strData.Find("MT_PBINIT#SITE");
    if (nPbInit == 0)
    {
        BYTE nProg;
        char ch = strData.GetAt(15);
        nProg = HexChar(ch);
        gm_sBurningStatus[nProg].nAdpStatus[0] = 1;
        gm_sBurningStatus[nProg].nAdpStatus[1] = 1;
        gm_sBurningStatus[nProg].nAdpStatus[2] = 1;
        gm_sBurningStatus[nProg].nAdpStatus[3] = 1;
    }

    //进度条的位置。提供编程器序号，适配器的状态，进度条的位置
    //MT_PBPOS#SITE[0]-Adp:0F-PbPos:0000FFFF
    int nPbPos = strData.Find("MT_PBPOS#SITE");
    if (nPbPos == 0)
    {
        //BYTE nProg;
        //char ch = strData.GetAt(13);
        //nProg = HexChar(ch);
        //int nPos = strData.Find("PbPos:");
        //int length=strData.GetLength();
        //CString strPos=strData.Right(length - nPos - 1);
        //unsigned char cbuf[256];
        //str_to_hex(strPos.GetBuffer(), cbuf, strPos.GetLength());
        //strPos.ReleaseBuffer();

    }
    //这个Checksum值是下载到母片后,母片数据的Cheksum值
    //MT_CHECKSUM#SITE[0]-Checksum:B488C664
    int nCheckSum = strData.Find("MT_CHECKSUM#SITE");
    if (nCheckSum == 0)
    {
        int nPos = strData.Find(':');
        if (nPos >= 0)
        {
            gm_uInfo.strCheckSum = strData.Mid(nPos + 1);//包含$,不想包含时nPos+1 
        }
    }
    //每台设备的序列号，是否在线，SPRJ文件名， Checksum值 用-号分隔。设备间隔为#
    //DeviceList#[SITE 0]-100055051000000X-TRUE-F:\H26MYN.SPRJ-54827D1A
    int nDevList = strData.Find("DeviceList#");
    if (nDevList == 0)
    {
        std::string pattern;
        pattern = "-";
        std::string s(strData.GetBuffer());
        std::vector<std::string> result = split1(s, pattern);
        gm_uInfo.strProcName = result[3].c_str();
        gm_sdInfo.strWorkNo = result[3].c_str();
    }

    //返回执行结果。提供编程器序号， 执行的动作。适配器状态。
    //MT_RESULT#SITE[0]-CHECKSUM-Adp:0F
    int nResult = strData.Find("MT_RESULT#SITE");//返回执行结果
    if (nResult == 0)
    {
        BYTE nProg;
        char ch = strData.GetAt(15);
        nProg = HexChar(ch);
        int nAdp = strData.Find("Adp:");
        ch = strData.GetAt(nAdp + 5);
        BYTE status = HexChar(ch);
        for (int i = 0; i < 4; i++)
        {
            if ((status & (1 << i)) != 0)
                gm_sBurningStatus[nProg].nAdpStatus[i] = 2;
            else
                gm_sBurningStatus[nProg].nAdpStatus[i] = 3;
        }
    }

    //这个Checksum值是文件里数据的Checksum值. 下载成功需要比较MT_CHECKSUM
    //MT_FILE_CHECKSUM:B488C664
    int nFileCheckSum = strData.Find("MT_FILE_CHECKSUM");
    if (nFileCheckSum == 0)
    {
        int nPos = strData.Find(':');
        if (nPos >= 0)
        {
            gm_uInfo.strCheckSum = strData.Mid(nPos + 1);//包含$,不想包含时nPos+1 
        }
    }
    //AfxMessageBox(strData);
}


void CSocketManager::AppendMessage(LPCTSTR strText)
{
    if (NULL == m_pMsgCtrl)
        return;
    /*
        if (::IsWindow( m_pMsgCtrl->GetSafeHwnd() ))
        {
            int nLen = m_pMsgCtrl->GetWindowTextLength();
            m_pMsgCtrl->SetSel(nLen, nLen);
            m_pMsgCtrl->ReplaceSel( strText );
        }
    */
    HWND hWnd = m_pMsgCtrl->GetSafeHwnd();
    DWORD dwResult = 0;
    if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
    {
        int nLen = (int)dwResult;
        if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
        {
            if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)strText, SMTO_NORMAL, 1000L, &dwResult) != 0)
            {
            }
        }

    }
}


void CSocketManager::SetMessageWindow(CEdit* pMsgCtrl)
{
    m_pMsgCtrl = pMsgCtrl;
}

void CSocketManager::SetMainWindow(CWnd* pMain)
{
    m_pMain = pMain;
}

void CSocketManager::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount)
{
    LPBYTE lpData = lpBuffer;
    SockAddrIn origAddr;
    stMessageProxy msgProxy;
    if (IsSmartAddressing())
    {
        dwCount = __min(sizeof(msgProxy), dwCount);
        memcpy(&msgProxy, lpBuffer, dwCount);
        origAddr = msgProxy.address;
        if (IsServer())
        {
            // broadcast message to all
            msgProxy.address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
            WriteComm((const LPBYTE)&msgProxy, dwCount, 0L);
        }
        dwCount -= sizeof(msgProxy.address);
        lpData = msgProxy.byData;
    }

    // Display data to message list
    DisplayData(lpData, dwCount, origAddr);
}

///////////////////////////////////////////////////////////////////////////////
// OnEvent
// Send message to parent window to indicate connection status
void CSocketManager::OnEvent(UINT uEvent, LPVOID lpvData)
{
    /*	if (NULL == m_pMsgCtrl)
            return;

        CWnd* pParent = m_pMsgCtrl->GetParent();
        if (!::IsWindow( pParent->GetSafeHwnd()))
            return;
    */
    switch (uEvent)
    {
    case EVT_CONSUCCESS:
        AppendMessage(_T("Connection Established\r\n"));
        break;
    case EVT_CONFAILURE:
        AppendMessage(_T("Connection Failed\r\n"));
        break;
    case EVT_CONDROP:
        AppendMessage(_T("Connection Abandonned\r\n"));
        break;
    case EVT_ZEROLENGTH:
        AppendMessage(_T("Zero Length Message\r\n"));
        break;
    default:
        TRACE("Unknown Socket event\n");
        break;
    }

    m_pMain->PostMessage(WM_UPDATE_CONNECTION, uEvent, (LPARAM) this);

}
