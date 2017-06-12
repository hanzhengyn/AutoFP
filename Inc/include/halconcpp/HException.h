/*****************************************************************************
 * HException.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Error handling
 *
 * (c) 2010-2012 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.6 $
 * $Date: 2012/04/27 13:16:35 $
 *
 *****************************************************************************/


#ifndef HCPP_EXCEPTION_H
#define HCPP_EXCEPTION_H


#include "HString.h"

#include "HTuple.h"


namespace HalconCpp
{


// Base class for all exception that is raised for generic
// errors within the HALCON/C++ interface

class LIntExport HException 
{

public:

  // Constructors / Destructor
               
  HException(const char *proc_name, Herror err);

  HException(const char *proc_name, const char *msg, Herror err = H_MSG_OK);
             
  HException(const HException &except);
                          
  HException(const HTuple &tuple);
  
  virtual ~HException();


  // Raising exceptions (synchronization-aware)
  
  void ThrowInstance();

  static void Throw(const char *proc_name, const char *msg, Herror err = H_MSG_OK);

  static void Throw(const char *proc_name, Herror err);


  // Conversion and copying
    
  static void    GetExceptionData(const HTuple &exception, 
                                  const HTuple &name, HTuple *value);  

  void           ToHTuple(HTuple *exception) const;
                                  
  HException&    operator = (const HException &except);
  

  // Accessors
  
  const HString& ProcName()    const {return mProcName;}
  const HString& ErrorText()   const {return mErrorText;}
  Herror         ErrorNumber() const {return mErrorNumber;}
  
  const HTuple&  UserData()    const {return mUserData;}
  
  // Legacy
#ifdef HCPP_LEGACY_EXCEPTION
  // Handler for user defined exception handling.
  // Used with InstallHHandler()
  typedef void (*Handler)(const HException &exception);
  
  void           React(Hproc_handle proc_handle);

  static Handler InstallHHandler(Handler proc);
  // default exception handler
  void           PrintException(void);
  static Handler handler;   /* handler in use */
#endif
  
protected:
  
  HString        mProcName;    // Name of procedure/operator
  HString        mErrorText;   // Error text
  Herror         mErrorNumber; // Error number
  
  HTuple         mUserData;    // User defined error data

private:

  void InitException(HString proc_name, HString msg,
                     Herror err, const HTuple& user_data);
                                          
};


// Exception that is raised if operator call in HALCON library fails 

class LIntExport HOperatorException : public HException
{
public:

  // Failure during operator call
  HOperatorException(Hproc_handle proc, Herror err);
  
  // Precall failure before procedure handle is available
  HOperatorException(INT proc_index, Herror err);

  // Failure during operator-like call, e.g. emulated dev_* operators
  HOperatorException(const char* name, Herror err);
           
  HOperatorException(const HOperatorException &except);
  
  static void Throw(Hproc_handle proc, Herror err);

  static void Throw(INT proc_index, Herror err);

  static void Throw(const char* name, Herror err);  
};


// Exception that is raised from user code (code export)

class LIntExport HUserException : public HException
{
public:

  HUserException(Herror err, const char *msg);
             
  HUserException(const HUserException &except);
                          
  HUserException(const HTuple &tuple);

};


// Exception that is raised for tuple-related errors

class LIntExport HTupleAccessException: public HException
{
public:

  HTupleAccessException(void) : HException("","Illegal operation on tuple") {}
  HTupleAccessException(const char *m) : HException("",m) {}
  HTupleAccessException(Herror err) : HException("",err) {}
  static void Throw(void);
  static void Throw(const char* m);  
  static void Throw(Herror err);  
  ~HTupleAccessException();
};


}

#endif
