// DataPos.cpp: implementation of the CDataPos class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataPos.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataPos::CDataPos()
{
    basePoint.x = 0;
    basePoint.y = 0;
    downCamera.x = 0;
    downCamera.y = 0;
    nozzle1.x = 0;
    nozzle1.y = 0;
    offset_nozzle1.x = 0;
    offset_nozzle1.y = 0;
    nozzle2.x = 0;
    nozzle2.y = 0;
    offset_nozzle2.x = 0;
    offset_nozzle2.y = 0;
    nozzle3.x = 0;
    nozzle3.y = 0;
    offset_nozzle3.x = 0;
    offset_nozzle3.y = 0;
    nozzle4.x = 0;
    nozzle4.y = 0;
    offset_nozzle4.x = 0;
    offset_nozzle4.y = 0;

    for (int i = 0; i < MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET; i++)
    {
        socket[i].x = 0;
        socket[i].y = 0;
        socketHeight[i] = 0;
    }

    for (int i = 0; i < 3; i++)
    {
        NGBin[i].x = 0;
        NGBin[i].y = 0;
        tray[i].x = 0;
        tray[i].y = 0;
        supply[i].x = 0;
        supply[i].y = 0;
    }

    pitchx = 0;
    pitchy = 0;
    trayHeight = 0;
    trayHeight_1 = 0;
    trayHeight_2 = 0;
    supplyHeight = 0;
    supplyHeight_1 = 0;
    supplyHeight_2 = 0;

    ngbinHeight = 0;
    tapeIn.x = 0;
    tapeIn.y = 0;
    tapeOut.x = 0;
    tapeOut.y = 0;
    tapeInHeight = 0;
    tapeOutHeight = 0;
}

CDataPos::~CDataPos()
{
}

void CDataPos::Init(LPCTSTR lpszPath)
{
    CIniFile file;
    file.SetIniFileName(lpszPath);
    basePoint.x = file.GetFloat("BasePoint", "X", 0);
    basePoint.y = file.GetFloat("BasePoint", "Y", 0);
    downCamera.x = file.GetFloat("DownCamera", "X", 0);
    downCamera.y = file.GetFloat("DownCamera", "Y", 0);
    nozzle1.x = file.GetFloat("Nozzle1", "X", 0);
    nozzle1.y = file.GetFloat("Nozzle1", "Y", 0);
    nozzle2.x = file.GetFloat("Nozzle2", "X", 0);
    nozzle2.y = file.GetFloat("Nozzle2", "Y", 0);
    nozzle3.x = file.GetFloat("Nozzle3", "X", 0);
    nozzle3.y = file.GetFloat("Nozzle3", "Y", 0);
    nozzle4.x = file.GetFloat("Nozzle4", "X", 0);
    nozzle4.y = file.GetFloat("Nozzle4", "Y", 0);
    //offset_nozzle1.x = nozzle1.x - basePoint.x;
    //offset_nozzle1.y = nozzle1.y - basePoint.y;
    //offset_nozzle2.x = nozzle2.x - basePoint.x;
    //offset_nozzle2.y = nozzle2.y - basePoint.y;
    //offset_nozzle3.x = nozzle3.x - basePoint.x;
    //offset_nozzle3.y = nozzle3.y - basePoint.y;
    //offset_nozzle4.x = nozzle4.x - basePoint.x;
    //offset_nozzle4.y = nozzle4.y - basePoint.y;
    offset_nozzle1.x = file.GetFloat("offset_nozzle1", "X", 0);
    offset_nozzle1.y = file.GetFloat("offset_nozzle1", "Y", 0);
    offset_nozzle2.x = file.GetFloat("offset_nozzle2", "X", 0);
    offset_nozzle2.y = file.GetFloat("offset_nozzle2", "Y", 0);
    offset_nozzle3.x = file.GetFloat("offset_nozzle3", "X", 0);
    offset_nozzle3.y = file.GetFloat("offset_nozzle3", "Y", 0);
    offset_nozzle4.x = file.GetFloat("offset_nozzle4", "X", 0);
    offset_nozzle4.y = file.GetFloat("offset_nozzle4", "Y", 0);
    CString socStr;
    for (int i = 0; i < MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET; i++)
    {
        socStr.Format("Socket%d", i);
        socket[i].x = file.GetFloat(socStr.operator LPCTSTR(), "X", 0);
        socket[i].y = file.GetFloat(socStr.operator LPCTSTR(), "Y", 0);
        socketHeight[i] = file.GetFloat("Height", socStr.operator LPCTSTR(), 0);
    }

    NGBin[0].x = file.GetFloat("NGBinPos1", "X", 0);
    NGBin[0].y = file.GetFloat("NGBinPos1", "Y", 0);
    NGBin[1].x = file.GetFloat("NGBinPos2", "X", 0);
    NGBin[1].y = file.GetFloat("NGBinPos2", "Y", 0);
    NGBin[2].x = file.GetFloat("NGBinPos3", "X", 0);
    NGBin[2].y = file.GetFloat("NGBinPos3", "Y", 0);

    tray[0].x = file.GetFloat("TrayPos1", "X", 0);
    tray[0].y = file.GetFloat("TrayPos1", "Y", 0);
    tray[1].x = file.GetFloat("TrayPos2", "X", 0);
    tray[1].y = file.GetFloat("TrayPos2", "Y", 0);
    tray[2].x = file.GetFloat("TrayPos3", "X", 0);
    tray[2].y = file.GetFloat("TrayPos3", "Y", 0);

    supply[0].x = file.GetFloat("SupplyPos1", "X", 0);
    supply[0].y = file.GetFloat("SupplyPos1", "Y", 0);
    supply[1].x = file.GetFloat("SupplyPos2", "X", 0);
    supply[1].y = file.GetFloat("SupplyPos2", "Y", 0);
    supply[2].x = file.GetFloat("SupplyPos3", "X", 0);
    supply[2].y = file.GetFloat("SupplyPos3", "Y", 0);

    pitchx = file.GetFloat("Pitch", "X", 0);
    pitchy = file.GetFloat("pitch", "Y", 0);

    tapeIn.x = file.GetFloat("TapeIn", "X", 0);
    tapeIn.y = file.GetFloat("TapeIn", "Y", 0);
    tapeOut.x = file.GetFloat("TapeOut", "X", 0);
    tapeOut.y = file.GetFloat("TapeOut", "Y", 0);

    trayHeight = file.GetFloat("Height", "TrayHeight1", 0);
    trayHeight_1 = file.GetFloat("Height", "TrayHeight2", 0);
    trayHeight_2 = file.GetFloat("Height", "TrayHeight3", 0);

    supplyHeight = file.GetFloat("Height", "SupplyHeight1", 0);
    supplyHeight_1 = file.GetFloat("Height", "SupplyHeight2", 0);
    supplyHeight_2 = file.GetFloat("Height", "SupplyHeight3", 0);

    ngbinHeight = file.GetFloat("Height", "NGBinHeight", 0);

    tapeInHeight = file.GetFloat("Height", "TapeIn", 0);
    tapeOutHeight = file.GetFloat("Height", "TapeOut", 0);

    //chipModel.phi = file.GetFloat("Model", "Chip_Angle", 0);
    //chipModel.x = file.GetFloat("Model", "Chip_X", 0);
    //chipModel.y = file.GetFloat("Model", "Chip_Y", 0);

    //tapeInModel.phi = file.GetFloat("Model", "TapeIn_Angle", 0);
    //tapeInModel.x = file.GetFloat("Model", "TapeIn_X", 0);
    //tapeInModel.y = file.GetFloat("Model", "TapeIn_Y", 0);

    //tapeOutModel.phi = file.GetFloat("Model", "TapeOut_Angle", 0);
    //tapeOutModel.x = file.GetFloat("Model", "TapeOut_X", 0);
    //tapeOutModel.y = file.GetFloat("Model", "TapeOut_Y", 0);
}
void CDataPos::Save(LPCTSTR lpszFile)
{
    CIniFile file;
    file.SetIniFileName(lpszFile);
    file.WriteFloatNumber("BasePoint", "X", basePoint.x);
    file.WriteFloatNumber("BasePoint", "Y", basePoint.y);
    file.WriteFloatNumber("DownCamera", "X", downCamera.x);
    file.WriteFloatNumber("DownCamera", "Y", downCamera.y);
    file.WriteFloatNumber("Nozzle1", "X", nozzle1.x);
    file.WriteFloatNumber("Nozzle1", "Y", nozzle1.y);
    file.WriteFloatNumber("Nozzle2", "X", nozzle2.x);
    file.WriteFloatNumber("Nozzle2", "Y", nozzle2.y);
    file.WriteFloatNumber("Nozzle3", "X", nozzle3.x);
    file.WriteFloatNumber("Nozzle3", "Y", nozzle4.y);
    file.WriteFloatNumber("Nozzle4", "X", nozzle4.x);
    file.WriteFloatNumber("Nozzle4", "Y", nozzle4.y);
    //offset_nozzle1.x = nozzle1.x - basePoint.x;
    //offset_nozzle1.y = nozzle1.y - basePoint.y;
    file.WriteFloatNumber("offset_nozzle1", "X", offset_nozzle1.x);
    file.WriteFloatNumber("offset_nozzle1", "Y", offset_nozzle1.y);
    //offset_nozzle2.x = nozzle2.x - basePoint.x;
    //offset_nozzle2.y = nozzle2.y - basePoint.y;
    file.WriteFloatNumber("offset_nozzle2", "X", offset_nozzle2.x);
    file.WriteFloatNumber("offset_nozzle2", "Y", offset_nozzle2.y);
    //offset_nozzle3.x = nozzle3.x - basePoint.x;
    //offset_nozzle3.y = nozzle3.y - basePoint.y;
    file.WriteFloatNumber("offset_nozzle3", "X", offset_nozzle3.x);
    file.WriteFloatNumber("offset_nozzle3", "Y", offset_nozzle3.y);
    //offset_nozzle4.x = nozzle4.x - basePoint.x;
    //offset_nozzle4.y = nozzle4.y - basePoint.y;
    file.WriteFloatNumber("offset_nozzle4", "X", offset_nozzle4.x);
    file.WriteFloatNumber("offset_nozzle4", "Y", offset_nozzle4.y);

    CString socStr;
    for (int i = 0; i < MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET; i++)
    {
        socStr.Format("Socket%d", i);
        file.WriteFloatNumber(socStr.operator LPCTSTR(), "X", socket[i].x);
        file.WriteFloatNumber(socStr.operator LPCTSTR(), "Y", socket[i].y);
        file.WriteFloatNumber("Height", socStr.operator LPCTSTR(), socketHeight[i]);
    }

    file.WriteFloatNumber("NGBinPos1", "X", NGBin[0].x);
    file.WriteFloatNumber("NGBinPos1", "Y", NGBin[0].y);
    file.WriteFloatNumber("NGBinPos2", "X", NGBin[1].x);
    file.WriteFloatNumber("NGBinPos2", "Y", NGBin[1].y);
    file.WriteFloatNumber("NGBinPos3", "X", NGBin[2].x);
    file.WriteFloatNumber("NGBinPos3", "Y", NGBin[2].y);

    file.WriteFloatNumber("TrayPos1", "X", tray[0].x);
    file.WriteFloatNumber("TrayPos1", "Y", tray[0].y);
    file.WriteFloatNumber("TrayPos2", "X", tray[1].x);
    file.WriteFloatNumber("TrayPos2", "Y", tray[1].y);
    file.WriteFloatNumber("TrayPos3", "X", tray[2].x);
    file.WriteFloatNumber("TrayPos3", "Y", tray[2].y);

    file.WriteFloatNumber("SupplyPos1", "X", supply[0].x);
    file.WriteFloatNumber("SupplyPos1", "Y", supply[0].y);
    file.WriteFloatNumber("SupplyPos2", "X", supply[1].x);
    file.WriteFloatNumber("SupplyPos2", "Y", supply[1].y);
    file.WriteFloatNumber("SupplyPos3", "X", supply[2].x);
    file.WriteFloatNumber("SupplyPos3", "Y", supply[2].y);

    file.WriteFloatNumber("Pitch", "X", pitchx);
    file.WriteFloatNumber("Pitch", "Y", pitchy);

    file.WriteFloatNumber("TapeIn", "X", tapeIn.x);
    file.WriteFloatNumber("TapeIn", "Y", tapeIn.y);
    file.WriteFloatNumber("TapeOut", "X", tapeOut.x);
    file.WriteFloatNumber("TapeOut", "Y", tapeOut.y);

    file.WriteFloatNumber("Height", "TrayHeight1", trayHeight);
    file.WriteFloatNumber("Height", "TrayHeight2", trayHeight_1);
    file.WriteFloatNumber("Height", "TrayHeight3", trayHeight_2);

    file.WriteFloatNumber("Height", "SupplyHeight1", supplyHeight);
    file.WriteFloatNumber("Height", "SupplyHeight2", supplyHeight_1);
    file.WriteFloatNumber("Height", "SupplyHeight3", supplyHeight_2);

    file.WriteFloatNumber("Height", "NGBinHeight", ngbinHeight);

    file.WriteFloatNumber("Height", "TapeIn", tapeInHeight);
    file.WriteFloatNumber("Height", "TapeOut", tapeOutHeight);

    //file.WriteFloatNumber("Model", "Chip_Angle", gm_hv_Angle_Chip);
    //file.WriteFloatNumber("Model", "Chip_X", gm_hv_Column_Chip);
    //file.WriteFloatNumber("Model", "Chip_Y", gm_hv_Row_Chip);

    //file.WriteFloatNumber("Model", "TapeIn_Angle", gm_hv_Angle_In);
    //file.WriteFloatNumber("Model", "TapeIn_X", gm_hv_Column_In);
    //file.WriteFloatNumber("Model", "TapeIn_Y", gm_hv_Row_In);

    //file.WriteFloatNumber("Model", "TapeOut_Angle", gm_hv_Angle_Out);
    //file.WriteFloatNumber("Model", "TapeOut_X", gm_hv_Column_Out);
    //file.WriteFloatNumber("Model", "TapeOut_Y", gm_hv_Row_Out);
}



void CDataPos::SetPlacePoint(LPCTSTR place, double x, double y)
{
    CString chPlace = place;
    if (chPlace.Compare("BasePoint") == 0)
    {
        basePoint.x = x;
        basePoint.y = y;
    }
    else if (chPlace.Compare("DownCamera") == 0)
    {
        downCamera.x = x;
        downCamera.y = y;
    }
    else if (chPlace.Compare("Nozzle1") == 0)
    {
        nozzle1.x = x;
        nozzle1.y = y;
    }
    else if (chPlace.Compare("offset_nozzle1") == 0)
    {
        offset_nozzle1.x = x;
        offset_nozzle1.y = y;
    }
    else if (chPlace.Compare("Nozzle2") == 0)
    {
        nozzle2.x = x;
        nozzle2.y = y;
    }
    else if (chPlace.Compare("offset_nozzle2") == 0)
    {
        offset_nozzle2.x = x;
        offset_nozzle2.y = y;
    }
    else if (chPlace.Compare("Nozzle3") == 0)
    {
        nozzle3.x = x;
        nozzle3.y = y;
    }
    else if (chPlace.Compare("offset_nozzle3") == 0)
    {
        offset_nozzle3.x = x;
        offset_nozzle3.y = y;
    }
    else if (chPlace.Compare("Nozzle4") == 0)
    {
        nozzle4.x = x;
        nozzle4.y = y;
    }
    else if (chPlace.Compare("offset_nozzle4") == 0)
    {
        offset_nozzle4.x = x;
        offset_nozzle4.y = y;
    }
    else if (chPlace.Compare("Socket0") == 0)
    {
        socket[0].x = x;
        socket[0].y = y;
    }
    else if (chPlace.Compare("Socket1") == 0)
    {
        socket[1].x = x;
        socket[1].y = y;
    }
    else if (chPlace.Compare("Socket2") == 0)
    {
        socket[2].x = x;
        socket[2].y = y;
    }
    else if (chPlace.Compare("Socket3") == 0)
    {
        socket[3].x = x;
        socket[3].y = y;
    }
    else if (chPlace.Compare("Socket4") == 0)
    {
        socket[4].x = x;
        socket[4].y = y;
    }
    else if (chPlace.Compare("Socket5") == 0)
    {
        socket[5].x = x;
        socket[5].y = y;
    }
    else if (chPlace.Compare("Socket6") == 0)
    {
        socket[6].x = x;
        socket[6].y = y;
    }
    else if (chPlace.Compare("Socket7") == 0)
    {
        socket[7].x = x;
        socket[7].y = y;
    }
    else if (chPlace.Compare("Socket8") == 0)
    {
        socket[8].x = x;
        socket[8].y = y;
    }
    else if (chPlace.Compare("Socket9") == 0)
    {
        socket[9].x = x;
        socket[9].y = y;
    }
    else if (chPlace.Compare("Socket10") == 0)
    {
        socket[10].x = x;
        socket[10].y = y;
    }
    else if (chPlace.Compare("Socket11") == 0)
    {
        socket[11].x = x;
        socket[11].y = y;
    }
    else if (chPlace.Compare("Socket12") == 0)
    {
        socket[12].x = x;
        socket[12].y = y;
    }
    else if (chPlace.Compare("Socket13") == 0)
    {
        socket[13].x = x;
        socket[13].y = y;
    }
    else if (chPlace.Compare("Socket14") == 0)
    {
        socket[14].x = x;
        socket[14].y = y;
    }
    else if (chPlace.Compare("Socket15") == 0)
    {
        socket[15].x = x;
        socket[15].y = y;
    }
    else if (chPlace.Compare("Socket16") == 0)
    {
        socket[16].x = x;
        socket[16].y = y;
    }
    else if (chPlace.Compare("Socket17") == 0)
    {
        socket[17].x = x;
        socket[17].y = y;
    }
    else if (chPlace.Compare("Socket18") == 0)
    {
        socket[18].x = x;
        socket[18].y = y;
    }
    else if (chPlace.Compare("Socket19") == 0)
    {
        socket[19].x = x;
        socket[19].y = y;
    }
    else if (chPlace.Compare("Socket20") == 0)
    {
        socket[20].x = x;
        socket[20].y = y;
    }
    else if (chPlace.Compare("Socket21") == 0)
    {
        socket[21].x = x;
        socket[21].y = y;
    }
    else if (chPlace.Compare("Socket22") == 0)
    {
        socket[22].x = x;
        socket[22].y = y;
    }
    else if (chPlace.Compare("Socket23") == 0)
    {
        socket[23].x = x;
        socket[23].y = y;
    }
    else if (chPlace.Compare("Socket24") == 0)
    {
        socket[24].x = x;
        socket[24].y = y;
    }
    else if (chPlace.Compare("Socket25") == 0)
    {
        socket[25].x = x;
        socket[25].y = y;
    }
    else if (chPlace.Compare("Socket26") == 0)
    {
        socket[26].x = x;
        socket[26].y = y;
    }
    else if (chPlace.Compare("Socket27") == 0)
    {
        socket[27].x = x;
        socket[27].y = y;
    }
    else if (chPlace.Compare("Socket28") == 0)
    {
        socket[28].x = x;
        socket[28].y = y;
    }
    else if (chPlace.Compare("Socket29") == 0)
    {
        socket[29].x = x;
        socket[29].y = y;
    }
    else if (chPlace.Compare("Socket30") == 0)
    {
        socket[30].x = x;
        socket[30].y = y;
    }
    else if (chPlace.Compare("Socket31") == 0)
    {
        socket[31].x = x;
        socket[31].y = y;
    }
    else if (chPlace.Compare("Socket32") == 0)
    {
        socket[32].x = x;
        socket[32].y = y;
    }
    else if (chPlace.Compare("Socket33") == 0)
    {
        socket[33].x = x;
        socket[33].y = y;
    }
    else if (chPlace.Compare("Socket34") == 0)
    {
        socket[34].x = x;
        socket[34].y = y;
    }
    else if (chPlace.Compare("Socket35") == 0)
    {
        socket[35].x = x;
        socket[35].y = y;
    }
    else if (chPlace.Compare("Socket36") == 0)
    {
        socket[36].x = x;
        socket[36].y = y;
    }
    else if (chPlace.Compare("Socket37") == 0)
    {
        socket[37].x = x;
        socket[37].y = y;
    }
    else if (chPlace.Compare("Socket38") == 0)
    {
        socket[38].x = x;
        socket[38].y = y;
    }
    else if (chPlace.Compare("Socket39") == 0)
    {
        socket[39].x = x;
        socket[39].y = y;
    }
    else if (chPlace.Compare("NGBinPos1") == 0)
    {
        NGBin[0].x = x;
        NGBin[0].y = y;
    }
    else if (chPlace.Compare("NGBinPos2") == 0)
    {
        NGBin[1].x = x;
        NGBin[1].y = y;
    }
    else if (chPlace.Compare("NGBinPos3") == 0)
    {
        NGBin[2].x = x;
        NGBin[2].y = y;
    }
    else if (chPlace.Compare("TrayPos1") == 0)
    {
        tray[0].x = x;
        tray[0].y = y;
    }
    else if (chPlace.Compare("TrayPos2") == 0)
    {
        tray[1].x = x;
        tray[1].y = y;
    }
    else if (chPlace.Compare("TrayPos3") == 0)
    {
        tray[2].x = x;
        tray[2].y = y;
    }
    else if (chPlace.Compare("SupplyPos1") == 0)
    {
        supply[0].x = x;
        supply[0].y = y;
    }
    else if (chPlace.Compare("SupplyPos2") == 0)
    {
        supply[1].x = x;
        supply[1].y = y;
    }
    else if (chPlace.Compare("SupplyPos3") == 0)
    {
        supply[2].x = x;
        supply[2].y = y;
    }
    else if (chPlace.Compare("TapeIn") == 0)
    {
        tapeIn.x = x;
        tapeIn.y = y;
    }
    else if (chPlace.Compare("TapeOut") == 0)
    {
        tapeOut.x = x;
        tapeOut.y = y;
    }
}
void CDataPos::SetPitch(double x, double y)
{
    pitchx = x;
    pitchy = y;
}
double CDataPos::GetPitchX()
{
    return pitchx;
}
double CDataPos::GetPitchY()
{
    return pitchy;
}
Point CDataPos::GetPlacePoint(LPCTSTR place)
{
    Point temp;
    temp.x = 0;
    temp.y = 0;
    CString chPlace = place;
    if (chPlace.Compare("BasePoint") == 0)
    {
        return basePoint;
    }
    else if (chPlace.Compare("DownCamera") == 0)
    {
        return downCamera;
    }
    else if (chPlace.Compare("Nozzle1") == 0)
    {
        return nozzle1;
    }
    else if (chPlace.Compare("offset_nozzle1") == 0)
    {
        return offset_nozzle1;
    }
    else if (chPlace.Compare("Nozzle2") == 0)
    {
        return nozzle2;
    }
    else if (chPlace.Compare("offset_nozzle2") == 0)
    {
        return offset_nozzle2;
    }
    else if (chPlace.Compare("Nozzle3") == 0)
    {
        return nozzle3;
    }
    else if (chPlace.Compare("offset_nozzle3") == 0)
    {
        return offset_nozzle3;
    }
    else if (chPlace.Compare("Nozzle4") == 0)
    {
        return nozzle4;
    }
    else if (chPlace.Compare("offset_nozzle4") == 0)
    {
        return offset_nozzle4;
    }
    else if (chPlace.Compare("Socket0") == 0)
    {
        return socket[0];
    }
    else if (chPlace.Compare("Socket1") == 0)
    {
        return socket[1];
    }
    else if (chPlace.Compare("Socket2") == 0)
    {
        return socket[2];
    }
    else if (chPlace.Compare("Socket3") == 0)
    {
        return socket[3];
    }
    else if (chPlace.Compare("Socket4") == 0)
    {
        return socket[4];
    }
    else if (chPlace.Compare("Socket5") == 0)
    {
        return socket[5];
    }
    else if (chPlace.Compare("Socket6") == 0)
    {
        return socket[6];
    }
    else if (chPlace.Compare("Socket7") == 0)
    {
        return socket[7];
    }
    else if (chPlace.Compare("Socket8") == 0)
    {
        return socket[8];
    }
    else if (chPlace.Compare("Socket9") == 0)
    {
        return socket[9];
    }
    else if (chPlace.Compare("Socket10") == 0)
    {
        return socket[10];
    }
    else if (chPlace.Compare("Socket11") == 0)
    {
        return socket[11];
    }
    else if (chPlace.Compare("Socket12") == 0)
    {
        return socket[12];
    }
    else if (chPlace.Compare("Socket13") == 0)
    {
        return socket[13];
    }
    else if (chPlace.Compare("Socket14") == 0)
    {
        return socket[14];
    }
    else if (chPlace.Compare("Socket15") == 0)
    {
        return socket[15];
    }
    else if (chPlace.Compare("Socket16") == 0)
    {
        return socket[16];
    }
    else if (chPlace.Compare("Socket17") == 0)
    {
        return socket[17];
    }
    else if (chPlace.Compare("Socket18") == 0)
    {
        return socket[18];
    }
    else if (chPlace.Compare("Socket19") == 0)
    {
        return socket[19];
    }
    else if (chPlace.Compare("Socket20") == 0)
    {
        return socket[20];
    }
    else if (chPlace.Compare("Socket21") == 0)
    {
        return socket[21];
    }
    else if (chPlace.Compare("Socket22") == 0)
    {
        return socket[22];
    }
    else if (chPlace.Compare("Socket23") == 0)
    {
        return socket[23];
    }
    else if (chPlace.Compare("Socket24") == 0)
    {
        return socket[24];
    }
    else if (chPlace.Compare("Socket25") == 0)
    {
        return socket[25];
    }
    else if (chPlace.Compare("Socket26") == 0)
    {
        return socket[26];
    }
    else if (chPlace.Compare("Socket27") == 0)
    {
        return socket[27];
    }
    else if (chPlace.Compare("Socket28") == 0)
    {
        return socket[28];
    }
    else if (chPlace.Compare("Socket29") == 0)
    {
        return socket[29];
    }
    else if (chPlace.Compare("Socket30") == 0)
    {
        return socket[30];
    }
    else if (chPlace.Compare("Socket31") == 0)
    {
        return socket[31];
    }
    else if (chPlace.Compare("Socket32") == 0)
    {
        return socket[32];
    }
    else if (chPlace.Compare("Socket33") == 0)
    {
        return socket[33];
    }
    else if (chPlace.Compare("Socket34") == 0)
    {
        return socket[34];
    }
    else if (chPlace.Compare("Socket35") == 0)
    {
        return socket[35];
    }
    else if (chPlace.Compare("Socket36") == 0)
    {
        return socket[36];
    }
    else if (chPlace.Compare("Socket37") == 0)
    {
        return socket[37];
    }
    else if (chPlace.Compare("Socket38") == 0)
    {
        return socket[38];
    }
    else if (chPlace.Compare("Socket39") == 0)
    {
        return socket[39];
    }
    else if (chPlace.Compare("NGBinPos1") == 0)
    {
        return NGBin[0];
    }
    else if (chPlace.Compare("NGBinPos2") == 0)
    {
        return NGBin[1];
    }
    else if (chPlace.Compare("NGBinPos3") == 0)
    {
        return NGBin[2];
    }
    else if (chPlace.Compare("TrayPos1") == 0)
    {
        return tray[0];
    }
    else if (chPlace.Compare("TrayPos2") == 0)
    {
        return tray[1];
    }
    else if (chPlace.Compare("TrayPos3") == 0)
    {
        return tray[2];
    }
    else if (chPlace.Compare("SupplyPos1") == 0)
    {
        return supply[0];
    }
    else if (chPlace.Compare("SupplyPos2") == 0)
    {
        return supply[1];
    }
    else if (chPlace.Compare("SupplyPos3") == 0)
    {
        return supply[2];
    }
    else if (chPlace.Compare("TapeIn") == 0)
    {
        return tapeIn;
    }
    else if (chPlace.Compare("TapeOut") == 0)
    {
        return tapeOut;
    }
    return temp;
}

void CDataPos::SetHeight(LPCTSTR place, double height)
{
    CString chPlace = place;
    if (chPlace.Compare("Socket0") == 0)
        socketHeight[0] = height;
    else if (chPlace.Compare("Socket1") == 0)
        socketHeight[1] = height;
    else if (chPlace.Compare("Socket2") == 0)
        socketHeight[2] = height;
    else if (chPlace.Compare("Socket3") == 0)
        socketHeight[3] = height;
    else if (chPlace.Compare("Socket4") == 0)
        socketHeight[4] = height;
    else if (chPlace.Compare("Socket5") == 0)
        socketHeight[5] = height;
    else if (chPlace.Compare("Socket6") == 0)
        socketHeight[6] = height;
    else if (chPlace.Compare("Socket7") == 0)
        socketHeight[7] = height;
    else if (chPlace.Compare("Socket8") == 0)
        socketHeight[8] = height;
    else if (chPlace.Compare("Socket9") == 0)
        socketHeight[9] = height;
    else if (chPlace.Compare("Socket10") == 0)
        socketHeight[10] = height;
    else if (chPlace.Compare("Socket11") == 0)
        socketHeight[11] = height;
    else if (chPlace.Compare("Socket12") == 0)
        socketHeight[12] = height;
    else if (chPlace.Compare("Socket13") == 0)
        socketHeight[13] = height;
    else if (chPlace.Compare("Socket14") == 0)
        socketHeight[14] = height;
    else if (chPlace.Compare("Socket15") == 0)
        socketHeight[15] = height;
    else if (chPlace.Compare("Socket16") == 0)
        socketHeight[16] = height;
    else if (chPlace.Compare("Socket17") == 0)
        socketHeight[17] = height;
    else if (chPlace.Compare("Socket18") == 0)
        socketHeight[18] = height;
    else if (chPlace.Compare("Socket19") == 0)
        socketHeight[19] = height;
    else if (chPlace.Compare("Socket20") == 0)
        socketHeight[20] = height;
    else if (chPlace.Compare("Socket21") == 0)
        socketHeight[21] = height;
    else if (chPlace.Compare("Socket22") == 0)
        socketHeight[22] = height;
    else if (chPlace.Compare("Socket23") == 0)
        socketHeight[23] = height;
    else if (chPlace.Compare("Socket24") == 0)
        socketHeight[24] = height;
    else if (chPlace.Compare("Socket25") == 0)
        socketHeight[25] = height;
    else if (chPlace.Compare("Socket26") == 0)
        socketHeight[26] = height;
    else if (chPlace.Compare("Socket27") == 0)
        socketHeight[27] = height;
    else if (chPlace.Compare("Socket28") == 0)
        socketHeight[28] = height;
    else if (chPlace.Compare("Socket29") == 0)
        socketHeight[29] = height;
    else if (chPlace.Compare("Socket30") == 0)
        socketHeight[30] = height;
    else if (chPlace.Compare("Socket31") == 0)
        socketHeight[31] = height;
    else if (chPlace.Compare("Socket32") == 0)
        socketHeight[32] = height;
    else if (chPlace.Compare("Socket33") == 0)
        socketHeight[33] = height;
    else if (chPlace.Compare("Socket34") == 0)
        socketHeight[34] = height;
    else if (chPlace.Compare("Socket35") == 0)
        socketHeight[35] = height;
    else if (chPlace.Compare("Socket36") == 0)
        socketHeight[36] = height;
    else if (chPlace.Compare("Socket37") == 0)
        socketHeight[37] = height;
    else if (chPlace.Compare("Socket38") == 0)
        socketHeight[38] = height;
    else if (chPlace.Compare("Socket39") == 0)
        socketHeight[39] = height;
    else if (chPlace.Compare("TrayHeight1") == 0)
        trayHeight = height;
    else if (chPlace.Compare("TrayHeight2") == 0)
        trayHeight_1 = height;
    else if (chPlace.Compare("TrayHeight3") == 0)
        trayHeight_2 = height;
    else if (chPlace.Compare("SupplyHeight1") == 0)
        supplyHeight = height;
    else if (chPlace.Compare("SupplyHeight2") == 0)
        supplyHeight_1 = height;
    else if (chPlace.Compare("SupplyHeight3") == 0)
        supplyHeight_2 = height;
    else if (chPlace.Compare("NGBinHeight") == 0)
        ngbinHeight = height;
    else if (chPlace.Compare("TapeIn") == 0)
        tapeInHeight = height;
    else if (chPlace.Compare("TapeOut") == 0)
        tapeOutHeight = height;
}
double CDataPos::GetHeight(LPCTSTR place)
{
    CString chPlace = place;
    if (chPlace.Compare("Socket0") == 0)
        return socketHeight[0];
    else if (chPlace.Compare("Socket1") == 0)
        return socketHeight[1];
    else if (chPlace.Compare("Socket2") == 0)
        return socketHeight[2];
    else if (chPlace.Compare("Socket3") == 0)
        return socketHeight[3];
    else if (chPlace.Compare("Socket4") == 0)
        return socketHeight[4];
    else if (chPlace.Compare("Socket5") == 0)
        return socketHeight[5];
    else if (chPlace.Compare("Socket6") == 0)
        return socketHeight[6];
    else if (chPlace.Compare("Socket7") == 0)
        return socketHeight[7];
    else if (chPlace.Compare("Socket8") == 0)
        return socketHeight[8];
    else if (chPlace.Compare("Socket9") == 0)
        return socketHeight[9];
    else if (chPlace.Compare("Socket10") == 0)
        return socketHeight[10];
    else if (chPlace.Compare("Socket11") == 0)
        return socketHeight[11];
    else if (chPlace.Compare("Socket12") == 0)
        return socketHeight[12];
    else if (chPlace.Compare("Socket13") == 0)
        return socketHeight[13];
    else if (chPlace.Compare("Socket14") == 0)
        return socketHeight[14];
    else if (chPlace.Compare("Socket15") == 0)
        return socketHeight[15];
    else if (chPlace.Compare("Socket16") == 0)
        return socketHeight[16];
    else if (chPlace.Compare("Socket17") == 0)
        return socketHeight[17];
    else if (chPlace.Compare("Socket18") == 0)
        return socketHeight[18];
    else if (chPlace.Compare("Socket19") == 0)
        return socketHeight[19];
    else if (chPlace.Compare("Socket20") == 0)
        return socketHeight[20];
    else if (chPlace.Compare("Socket21") == 0)
        return socketHeight[21];
    else if (chPlace.Compare("Socket22") == 0)
        return socketHeight[22];
    else if (chPlace.Compare("Socket23") == 0)
        return socketHeight[23];
    else if (chPlace.Compare("Socket24") == 0)
        return socketHeight[24];
    else if (chPlace.Compare("Socket25") == 0)
        return socketHeight[25];
    else if (chPlace.Compare("Socket26") == 0)
        return socketHeight[26];
    else if (chPlace.Compare("Socket27") == 0)
        return socketHeight[27];
    else if (chPlace.Compare("Socket28") == 0)
        return socketHeight[28];
    else if (chPlace.Compare("Socket29") == 0)
        return socketHeight[29];
    else if (chPlace.Compare("Socket30") == 0)
        return socketHeight[30];
    else if (chPlace.Compare("Socket31") == 0)
        return socketHeight[31];
    else if (chPlace.Compare("Socket32") == 0)
        return socketHeight[32];
    else if (chPlace.Compare("Socket33") == 0)
        return socketHeight[33];
    else if (chPlace.Compare("Socket34") == 0)
        return socketHeight[34];
    else if (chPlace.Compare("Socket35") == 0)
        return socketHeight[35];
    else if (chPlace.Compare("Socket36") == 0)
        return socketHeight[36];
    else if (chPlace.Compare("Socket37") == 0)
        return socketHeight[37];
    else if (chPlace.Compare("Socket38") == 0)
        return socketHeight[38];
    else if (chPlace.Compare("Socket39") == 0)
        return socketHeight[39];
    else if (chPlace.Compare("TrayHeight1") == 0)
        return trayHeight;
    else if (chPlace.Compare("TrayHeight2") == 0)
        return trayHeight_1;
    else if (chPlace.Compare("TrayHeight3") == 0)
        return trayHeight_2;
    else if (chPlace.Compare("SupplyHeight1") == 0)
        return supplyHeight;
    else if (chPlace.Compare("SupplyHeight2") == 0)
        return supplyHeight_1;
    else if (chPlace.Compare("SupplyHeight3") == 0)
        return supplyHeight_2;
    else if (chPlace.Compare("NGBinHeight") == 0)
        return ngbinHeight;
    else if (chPlace.Compare("TapeIn") == 0)
        return tapeInHeight;
    else if (chPlace.Compare("TapeOut") == 0)
        return tapeOutHeight;
    return 0;
}