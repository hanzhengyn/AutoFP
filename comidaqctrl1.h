#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装器类

// 注意:  不要修改此文件的内容。  如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CComidaqctrl1 包装器类

class CComidaqctrl1 : public CWnd
{
protected:
    DECLARE_DYNCREATE(CComidaqctrl1)
public:
    CLSID const& GetClsid()
    {
        static CLSID const clsid
            = { 0xD638FBDD, 0x9AFD, 0x4C6D, { 0xB7, 0x9, 0x32, 0x46, 0x17, 0x9B, 0xB1, 0x16 } };
        return clsid;
    }
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
        const RECT& rect, CWnd* pParentWnd, UINT nID,
        CCreateContext* pContext = NULL)
    {
        return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
    }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
        UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
        BSTR bstrLicKey = NULL)
    {
        return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
            pPersist, bStorage, bstrLicKey);
    }

    // 特性
public:
    ////enum
    ////{
    ////    COMI_CP101 = 49409,
    ////    COMI_CP201 = 49665,
    ////    COMI_CP301 = 49921,
    ////    COMI_CP302 = 49922,
    ////    COMI_CP401 = 50177,
    ////    COMI_CP501 = 50433,
    ////    COMI_SD101 = 45313,
    ////    COMI_SD102 = 45314,
    ////    COMI_SD103 = 45315,
    ////    COMI_SD104 = 45316,
    ////    COMI_SD201 = 45569,
    ////    COMI_SD202 = 45570,
    ////    COMI_SD203 = 45571,
    ////    COMI_SD301 = 45825,
    ////    COMI_SD401 = 46081,
    ////    COMI_SD402 = 46082,
    ////    COMI_SD403 = 46083,
    ////    COMI_SD404 = 46084,
    ////    COMI_SD414 = 46100,
    ////    COMI_SD424 = 46116,
    ////    COMI_SD501 = 46337,
    ////    COMI_SD502 = 46338,
    ////    COMI_LX101 = 41217,
    ////    COMI_LX102 = 41218,
    ////    COMI_LX103 = 41219,
    ////    COMI_LX201 = 41473,
    ////    COMI_LX202 = 41474,
    ////    COMI_LX203 = 41475,
    ////    COMI_LX301 = 41729,
    ////    COMI_LX401 = 41985,
    ////    COMI_LX402 = 41986,
    ////    COMI_ST101 = 53505,
    ////    COMI_ST201 = 53761,
    ////    COMI_ST202 = 53762,
    ////    COMI_ST203 = 53763,
    ////    COMI_ST301 = 54017,
    ////    COMI_ST401 = 54273,
    ////    COMI_ST402 = 54274,
    ////    COMI_MU101 = 57601,
    ////    COMI_MU201 = 57857,
    ////    COMI_MU301 = 58113,
    ////    COMI_MU401 = 58369,
    ////    COMI_MU402 = 58370,
    ////    COMI_MU403 = 58371,
    ////    COMI_MU501 = 58625,
    ////    COMI_MU701 = 59137,
    ////    MB_DAC101 = 257,
    ////    MB_DAC201 = 513,
    ////    MB_DAC301 = 769,
    ////    MB_DAC401 = 1025,
    ////    MB_DAC501 = 1281,
    ////    MB_DAC601 = 1537,
    ////    AFA50 = 64513
    ////}TCdDeviceID;
    enum
    {
        cmAI_DIFF = 0,
        cmAI_SINGLE = 1
    }TCdAiInputType;
    enum
    {
        cmTRS_SINGLE = 1,
        cmTRS_BLOCK = 2
    }TCdAiScanTrs;
    enum
    {
        cmTS_NONE = 0,
        cmTS_ANALOG = 1,
        cmTS_DIGITAL = 2
    }TCdAiEventSrc;
    enum
    {
        cmTE_POSITIVE = 0,
        cmTE_NEGATIVE = 1
    }TCdEdgeType;
    enum
    {
        cmDI_ONLY = 0,
        cmDI_DO = 1,
        cmDO_DI = 2,
        cmDO_ONLY = 3
    }TCdDioUsage;
    enum
    {
        cmdFALSE = 0,
        cmdTRUE = 1
    }TCdBool;
    enum
    {
        cmCMODE0 = 0,
        cmCMODE1 = 1,
        cmCMODE2 = 2,
        cmCMODE3 = 3,
        cmCMODE4 = 4,
        cmCMODE5 = 5
    }TCnt16Mode;
    enum
    {
        cmCOUNTER_LATCH = 0,
        cmREAD_LOAD_MSB = 1,
        cmREAD_LOAD_LSB = 2,
        cmREAD_LOAD_WORD = 3
    }TCdRwMode;
    enum
    {
        cmBINARY = 0,
        cmBCD = 1
    }TBcdBin;
    enum
    {
        cmENCODER_1X = 0,
        cmENCODER_2X = 1,
        cmENCODER_4X = 2
    }TEncMode;


    // 操作
public:

    // _DComiDaq

    // Functions
    //

    short GnDeviceLoad()
    {
        short result;
        InvokeHelper(0x6, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short GnDeviceUnload()
    {
        short result;
        InvokeHelper(0x7, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short AiSetInputType(long InputMode)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x8, DISPATCH_METHOD, VT_I2, (void*)&result, parms, InputMode);
        return result;
    }
    short AiSetRange(long Channel, double vmin, double vmax)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8;
        InvokeHelper(0x9, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, vmin, vmax);
        return result;
    }
    short AiGetRangeVolt(long Channel, double * vmin, double * vmax)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PR8;
        InvokeHelper(0xa, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, vmin, vmax);
        return result;
    }
    short AiGetRangeDigit(long Channel, long * Dmin, long * Dmax)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4;
        InvokeHelper(0xb, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, Dmin, Dmax);
        return result;
    }
    long AiGetDigit(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    double AiGetVolt(long Channel)
    {
        double result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0xd, DISPATCH_METHOD, VT_R8, (void*)&result, parms, Channel);
        return result;
    }
    long AiScanStart(long NumChannel, long * ChanList, long ScanFreq, long BufSize, long TrsMethod, long IsPauseAtFull)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_PI4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NumChannel, ChanList, ScanFreq, BufSize, TrsMethod, IsPauseAtFull);
        return result;
    }
    short AiScanStop(long IsReleaseBuf)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0xf, DISPATCH_METHOD, VT_I2, (void*)&result, parms, IsReleaseBuf);
        return result;
    }
    short AiScanSetTrgEvent(long InputSource, long EdgeType, long TrgMode, double AiRef, double AiRefBand)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_R8;
        InvokeHelper(0x10, DISPATCH_METHOD, VT_I2, (void*)&result, parms, InputSource, EdgeType, TrgMode, AiRef, AiRefBand);
        return result;
    }
    long AiScanCurCount()
    {
        long result;
        InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    long AiScanChangeFreq(long ScanFreq)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ScanFreq);
        return result;
    }
    short AiScanReleaseBuf()
    {
        short result;
        InvokeHelper(0x13, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short AiScanResume()
    {
        short result;
        InvokeHelper(0x14, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    double AiScanRetrOne(long ChannelOrder, long ScanCount)
    {
        double result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x15, DISPATCH_METHOD, VT_R8, (void*)&result, parms, ChannelOrder, ScanCount);
        return result;
    }
    long AiScanRetrChannelI2(long ChannelOrder, long StartCount, long MaxNumData, short * DestBuf)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI2;
        InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ChannelOrder, StartCount, MaxNumData, DestBuf);
        return result;
    }
    long AiScanRetrChannelF4(long ChannelOrder, long StartCount, long MaxNumData, float * DestBuf)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PR4;
        InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ChannelOrder, StartCount, MaxNumData, DestBuf);
        return result;
    }
    long AiScanRetrChannelF8(long ChannelOrder, long StartCount, long MaxNumData, double * DestBuf)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PR8;
        InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ChannelOrder, StartCount, MaxNumData, DestBuf);
        return result;
    }
    long AiScanRetrBlockI2(long StartCount, long MaxNumData, short * DestBuf)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI2;
        InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, StartCount, MaxNumData, DestBuf);
        return result;
    }
    long AiScanRetrBlockF4(long StartCount, long MaxNumData, float * DestBuf)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR4;
        InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, StartCount, MaxNumData, DestBuf);
        return result;
    }
    long AiScanRetrBlockF8(long StartCount, long MaxNumData, double * DestBuf)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8;
        InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, StartCount, MaxNumData, DestBuf);
        return result;
    }
    short AoOut(long Channel, double OutVolt)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_R8;
        InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, OutVolt);
        return result;
    }
    long AoWfmStart(long Channel, double * DataBuffer, long NumData, long PPS, long MaxLoops)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_PR8 VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel, DataBuffer, NumData, PPS, MaxLoops);
        return result;
    }
    short AoWfmStop(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x1e, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    long AoWfmGetCurLoops(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long AoWfmGetCurPos(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long AoWfmRateChange(long Channel, long PPS)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel, PPS);
        return result;
    }
    short DioSetUsage(long Usage)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x22, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Usage);
        return result;
    }
    long DioGetUsage()
    {
        long result;
        InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    long DiGetOne(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long DiGetAll()
    {
        long result;
        InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    long DiGetAllEx(long GroupIdx)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms, GroupIdx);
        return result;
    }
    long DoGetOne(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long DoGetAll()
    {
        long result;
        InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    long DoGetAllEx(long GroupIdx)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms, GroupIdx);
        return result;
    }
    short DoPutOne(long Channel, long OutState)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x2a, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, OutState);
        return result;
    }
    short DoPutAll(long OutStates)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x2b, DISPATCH_METHOD, VT_I2, (void*)&result, parms, OutStates);
        return result;
    }
    short DoPutAllEx(long GroupIdx, long OutStates)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x2c, DISPATCH_METHOD, VT_I2, (void*)&result, parms, GroupIdx, OutStates);
        return result;
    }
    short SdioInitComm()
    {
        short result;
        InvokeHelper(0x2d, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short SdioCheckModule(long ModuleNo)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x2e, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ModuleNo);
        return result;
    }
    short SdioSetDioUsage(long ModuleNo, long Usage)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x2f, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ModuleNo, Usage);
        return result;
    }
    long SdioReadLowByte(long ModuleNo)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ModuleNo);
        return result;
    }
    long SdioReadHighByte(long ModuleNo)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ModuleNo);
        return result;
    }
    long SdioReadWord(long ModuleNo)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ModuleNo);
        return result;
    }
    short SdioWriteLowByte(long ModuleNo, long Value8)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x33, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ModuleNo, Value8);
        return result;
    }
    short SdioWriteHighByte(long ModuleNo, long Value8)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x34, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ModuleNo, Value8);
        return result;
    }
    short SdioWriteWord(long ModuleNo, long Value16)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x35, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ModuleNo, Value16);
        return result;
    }
    short Cnt16Setup(long Channel, long RwMode, long OpMode, long BcdBin, long LoadValue)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x36, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, RwMode, OpMode, BcdBin, LoadValue);
        return result;
    }
    short Cnt16LoadCount(long Channel, long LoadValue)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x37, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, LoadValue);
        return result;
    }
    long Cnt16ReadCount(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long Cnt32ReadCount(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x39, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    short Cnt32ClearCount(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x3a, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    short EncConfig(long Channel, long Mode, long IsResetByZ)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x3b, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, Mode, IsResetByZ);
        return result;
    }
    short EncReset(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x3c, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    short EncLoad(long Channel, long Count)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x3d, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, Count);
        return result;
    }
    long EncRead(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x3e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    short EncResetZ(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x3f, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    short EncLoadZ(long Channel, long Count)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x40, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, Count);
        return result;
    }
    long EncReadZ(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    double PgStart(long Channel, double Freq, long NumPulses)
    {
        double result;
        static BYTE parms[] = VTS_I4 VTS_R8 VTS_I4;
        InvokeHelper(0x42, DISPATCH_METHOD, VT_R8, (void*)&result, parms, Channel, Freq, NumPulses);
        return result;
    }
    double PgChangeFreq(long Channel, double Freq)
    {
        double result;
        static BYTE parms[] = VTS_I4 VTS_R8;
        InvokeHelper(0x43, DISPATCH_METHOD, VT_R8, (void*)&result, parms, Channel, Freq);
        return result;
    }
    short PgIsActive(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x44, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    short PgStop(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x45, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    short FcSetCounter(long Channel, long Mode, long ClkSource)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x46, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel, Mode, ClkSource);
        return result;
    }
    long FcReadNowCount(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long FcReadOldCount(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x48, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    long FcGetGateState(long Channel)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x49, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel);
        return result;
    }
    double FcGetClkFreq(long ClkSourceIdx)
    {
        double result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x4a, DISPATCH_METHOD, VT_R8, (void*)&result, parms, ClkSourceIdx);
        return result;
    }
    short FcClear(long Channel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x4b, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Channel);
        return result;
    }
    short FcClearMulti(long CntrSel)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x4c, DISPATCH_METHOD, VT_I2, (void*)&result, parms, CntrSel);
        return result;
    }
    long GnGetErrorCode()
    {
        long result;
        InvokeHelper(0x4d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    CString GnGetErrorString(long ErrorCode)
    {
        CString result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x4e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, ErrorCode);
        return result;
    }
    long GnBitShift(long Value, long ShiftOption)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value, ShiftOption);
        return result;
    }
    short DlogSetup(long Level, LPCTSTR szLogFile)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_BSTR;
        InvokeHelper(0x50, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Level, szLogFile);
        return result;
    }
    short DlogSetSkipThis()
    {
        short result;
        InvokeHelper(0x51, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short DlogAddComment(LPCTSTR szComment)
    {
        short result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x52, DISPATCH_METHOD, VT_I2, (void*)&result, parms, szComment);
        return result;
    }
    short GnShowLastError()
    {
        short result;
        InvokeHelper(0x53, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short AiScanIsBufFull()
    {
        short result;
        InvokeHelper(0x54, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    short DiEventSetup(long ChannelMask1, long ChannelMask2)
    {
        short result;
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x55, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ChannelMask1, ChannelMask2);
        return result;
    }
    short GnDeviceIsLoaded()
    {
        short result;
        InvokeHelper(0x56, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    long AoWfmLoad(long Channel, double * DataBuffer, long NumData, long PPS, long MaxLoops)
    {
        long result;
        static BYTE parms[] = VTS_I4 VTS_PR8 VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x57, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Channel, DataBuffer, NumData, PPS, MaxLoops);
        return result;
    }
    short AoWfmStartEx(long ChanMask)
    {
        short result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x58, DISPATCH_METHOD, VT_I2, (void*)&result, parms, ChanMask);
        return result;
    }

    // Properties
    //

    long GetDeviceId()
    {
        long result;
        GetProperty(0x1, VT_I4, (void*)&result);
        return result;
    }
    void SetDeviceId(long propVal)
    {
        SetProperty(0x1, VT_I4, propVal);
    }
    long GetDevInstance()
    {
        long result;
        GetProperty(0x2, VT_I4, (void*)&result);
        return result;
    }
    void SetDevInstance(long propVal)
    {
        SetProperty(0x2, VT_I4, propVal);
    }
    BOOL GetEnableErrorMsgDlg()
    {
        BOOL result;
        GetProperty(0x3, VT_BOOL, (void*)&result);
        return result;
    }
    void SetEnableErrorMsgDlg(BOOL propVal)
    {
        SetProperty(0x3, VT_BOOL, propVal);
    }
    long GetDlogLevel()
    {
        long result;
        GetProperty(0x4, VT_I4, (void*)&result);
        return result;
    }
    void SetDlogLevel(long propVal)
    {
        SetProperty(0x4, VT_I4, propVal);
    }
    CString GetDlogFile()
    {
        CString result;
        GetProperty(0x5, VT_BSTR, (void*)&result);
        return result;
    }
    void SetDlogFile(CString propVal)
    {
        SetProperty(0x5, VT_BSTR, propVal);
    }


};
