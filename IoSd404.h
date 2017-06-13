#pragma once
void OpenIOBoard();
void CloseIOBoard();

BOOL CheckDoorOpen();
BOOL CheckNegtiveMeter(INT nNozzleNum);//负压表
BOOL CheckOutTapeIO();
BOOL CheckOutTapeIOFilm();
BOOL CheckTrayArrive();
BOOL CheckPositiveMeter();//正压表
BOOL CheckFeederOutError();
BOOL CheckModelCylinderPressed(INT nModelNum);
BOOL CheckNozzleUpStatus(INT nNozzleNum);

VOID TrayComplete();
VOID SetFeederOut();
VOID SetFeedIn();
VOID SetCylinderDown(INT nModelNum);
VOID SetCylinderUp(INT nModelNum);
VOID SetNozzleVaccumOn(INT nNozzleNum);
VOID SetNozzleVaccumOff(INT nNozzleNum);
VOID SetNozzleBlowOn(INT nNozzleNum);
VOID SetNozzleBlowOff(INT nNozzleNum);
VOID SetRedLightOn();
VOID SetRedLightOff();
VOID SetBeepOn();
VOID SetBeepOff();
VOID SetGreenLightOn();
VOID SetGreenLightOff();
VOID SetUpCameraLightOn();
VOID SetUpCameraLightOff();
VOID SetDownCameraLightOn();
VOID SetDownCameraLightOff();
VOID SetNozzleDownOn(INT nNozzleNum);
VOID SetNozzleDownOff(INT nNozzleNum);
