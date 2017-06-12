#ifndef _TEST_H_XELTEK
#define _TEST_H_XELTEK
#if BOARD_TYPE == ADVANTECH
#include "AdvMotApi.h"

#endif

BOOL OpenMotionBoard();
BOOL GoHome();
double GetPosition(long axis);
BOOL GetPosition(long axis, double &position);
void SetSingleSpeed(LONG axis, double dv, double ac, double del);
void SetAxisSpeed(long axis, int speedtype);
void SetAxisSpeed(long axis, double moverange, int speedratio);
UINT XYMove(double xnextposition, double ynextposition);
BOOL SxMove(long axis, double nextposition);
BOOL SzMove(double nextposition);
BOOL ZComeBack();
BOOL SrMove(double angle, int axis);
void StopMotion();
BOOL MeasureHeight(double &height);
void CloseMotionCard();

/////////////////////////////////////
#endif