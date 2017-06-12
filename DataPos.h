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
	//��ȡ������
	Point GetPlacePoint(LPCTSTR place);
	//���õ�����
	void  SetPlacePoint(LPCTSTR place, double x, double y);
	//��ȡ�߶�
	double GetHeight(LPCTSTR place);
	//���ø߶�
	void   SetHeight(LPCTSTR place, double height);
	void Init(LPCTSTR lpszPath);
	//����tray����оƬ���
	void SetPitch(double x, double y);
	//��ȡtray����оƬx���
	double GetPitchX();
	//��ȡtray����оƬy���
	double GetPitchY();
	//��������
	void Save(LPCTSTR lpszFile);
private:
	Point basePoint;//��׼������
	Point downCamera;//���������
	Point nozzle1;//����1����
	Point nozzle2;//����2����
	Point nozzle3;//����3����
	Point nozzle4;//����4����
	Point offset_nozzle1;//����1�������ƫ��
	Point offset_nozzle2;//����2�������ƫ��
	Point offset_nozzle3;//����3�������ƫ��
	Point offset_nozzle4;//����4�������ƫ��
	Point socket[MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET];//��¼������
	double socketHeight[MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET];//��¼���߶�
	Point NGBin[3];//����������
	Point tray[3];//Tray������
	Point supply[3];//����������
	Point tapeIn;//�����������
	Point tapeOut;//�����������
	double pitchx;//tray����оƬx���
	double pitchy;//tray����оƬy���
	double trayHeight;//tray�����ϽǸ߶�
	double trayHeight_1;//tray�����ϽǸ߶�
	double trayHeight_2;//tray�����½Ǹ߶�
	double supplyHeight;//���������ϽǸ߶�
	double supplyHeight_1;//���������ϽǸ߶�
	double supplyHeight_2;//���������½Ǹ߶�
	double ngbinHeight;//�����̸߶�
	double tapeInHeight;//������ϸ߶�
	double tapeOutHeight;//������ϸ߶�
};
#endif // !defined(AFX_DATAPOS_H__69E4A714_3D7E_4F49_845B_26BBB2DB606A__INCLUDED_)
