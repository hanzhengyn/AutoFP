/*****************************************************************************
 * HError.h
 ***************************************************************************** 
 *
 * Project:     HALCON/libhalcon
 * Description: Error messages
 *
 * (c) 1996-2012 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 * 
 * $Revision: 1.14 $
 * $Date: 2012/04/27 13:16:35 $
 *
 */


#ifndef HERROR_H
#define HERROR_H


#if defined(__cplusplus)
extern "C" {
#endif

extern HLibExport Herror HSetErrText(char *text);

extern HLibExport Herror HErrText(Herror message,char *err_text);

extern HLibExport Herror HMessage(Herror message,char *msg_text);



#if defined(__cplusplus)
}
#endif


#endif
