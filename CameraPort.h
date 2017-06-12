#pragma once
class CameraPort
{
public:
	CameraPort();
	~CameraPort();
	void OpenFramegrabUp();
	void OpenFramegrabDown();
	BOOL CheckModel(Position2 & posOffset, LPCTSTR place);
	void CloseFramegrab();
private:
	HWindow m_Window;
	HTuple  m_hv_AcqHandle_Up, m_hv_AcqHandle_Down;
	HObject m_ho_Image, m_ho_ROI_Search_Domain, m_ho_ROI_Model_Domain;
	HObject m_ho_ImagePart;
	HTuple m_hv_Width, m_hv_Height;
	HObject m_ho_ROI_0, m_ho_ImageReduced;
	HObject m_ho_ModelImages, m_ho_ModelRegions, m_ho_ModelContours, m_ho_ImageReducedSearch;
	HTuple m_hv_ModelID;
	HTuple m_hv_Row, m_hv_Column, m_hv_Angle, m_hv_Score;
	HTuple m_hv_CheckRow, m_hv_CheckColumn, m_hv_CheckAngle, m_hv_CheckScore;

	BOOL m_bCameraOpenUp;
	BOOL m_bCameraOpenDown;
	CString m_strCameraNum;
	CString m_strCameraVersion;
};

