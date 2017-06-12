// DataPos.h: interface for the CDataPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPOS_H__69E4A714_3D7E_4F49_845B_26BBB2DB606A__INCLUDED_)
#define AFX_DATAPOS_H__69E4A714_3D7E_4F49_845B_26BBB2DB606A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CDataPos
{
public:
	CDataPos();
	virtual ~CDataPos();
	//获取点坐标
	Point GetPlacePoint(LPCTSTR place);
	//设置点坐标
	void  SetPlacePoint(LPCTSTR place, double x, double y);
	//获取高度
	double GetHeight(LPCTSTR place);
	//设置高度
	void   SetHeight(LPCTSTR place, double height);
	void Init(LPCTSTR lpszPath);
	//设置tray盘上芯片间距
	void SetPitch(double x, double y);
	//获取tray盘上芯片x间距
	double GetPitchX();
	//获取tray盘上芯片y间距
	double GetPitchY();
	//保存坐标
	void Save(LPCTSTR lpszFile);
private:
	Point basePoint;//基准点坐标
	Point downCamera;//下相机坐标
	Point nozzle1;//吸嘴1坐标
	Point nozzle2;//吸嘴2坐标
	Point nozzle3;//吸嘴3坐标
	Point nozzle4;//吸嘴4坐标
	Point offset_nozzle1;//吸嘴1与上相机偏差
	Point offset_nozzle2;//吸嘴2与上相机偏差
	Point offset_nozzle3;//吸嘴3与上相机偏差
	Point offset_nozzle4;//吸嘴4与上相机偏差
	Point socket[MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET];//烧录座坐标
	double socketHeight[MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET];//烧录座高度
	Point NGBin[3];//废料盘坐标
	Point tray[3];//Tray盘坐标
	Point supply[3];//补料盘坐标
	Point tapeIn;//编带进料坐标
	Point tapeOut;//编带出料坐标
	double pitchx;//tray盘上芯片x间距
	double pitchy;//tray盘上芯片y间距
	double trayHeight;//tray盘左上角高度
	double trayHeight_1;//tray盘右上角高度
	double trayHeight_2;//tray盘右下角高度
	double supplyHeight;//补料盘左上角高度
	double supplyHeight_1;//补料盘右上角高度
	double supplyHeight_2;//补料盘右下角高度
	double ngbinHeight;//废料盘高度
	double tapeInHeight;//编带进料高度
	double tapeOutHeight;//编带出料高度
};
#endif // !defined(AFX_DATAPOS_H__69E4A714_3D7E_4F49_845B_26BBB2DB606A__INCLUDED_)
