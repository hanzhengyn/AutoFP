/*****************************************************************************
 * HToolBase.h
 ***************************************************************************** 
 *
 * Project:     Halcon/C++
 * Description: Smart pointer concept for managing tool class references
 *
 * (c) 2010-2012 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.4 $
 * $Date: 2012/04/27 13:16:35 $
 *
 *****************************************************************************/


#ifndef HCPP_TOOL_BASE_H
#define HCPP_TOOL_BASE_H



namespace HalconCpp
{

// Smart pointer to internal data representation

template<class T> class HSmartPtr;

class HToolRef;

typedef HSmartPtr<HToolRef> HToolRefPtr;


/* Base class for actual tool classes */


class LIntExport HToolBase
{
public:

  // Destructor
  virtual ~HToolBase();
  
public: 

  // Access and Manipulation 
  
  Hlong         GetHandle() const;   
  virtual void  SetHandle(Hlong handle) = 0;   // Must create specialized toolref
  void          InvalidateHandle();  

  void          Clear();
  bool          IsHandleValid() const;

  operator Hlong()  const;
  operator HTuple() const;

  HToolBase& operator=(const HToolBase& tool);
  
protected:

  // Constructor (only for subclass implementation)
  HToolBase();

  // Copy constructor
  HToolBase(const HToolBase& tool);

  //Handle
  HToolRefPtr& mId;
};


class LIntExport HToolArray
{

public:

  // Create empty tool array for use as output parameter
  HToolArray();
  
  virtual ~HToolArray();

  HToolBase* Tools();

  Hlong Length() const;

  void Clear();

  // Create tool array from tuple of handles
  void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  HTuple ConvertToTuple() const;
   
protected:

  void InitFromArray(HToolBase* tools, Hlong length);

  virtual void CreateArray(Hlong length) = 0;
     
  HToolBase* mArray;
  Hlong      mLength;
};

} // namespace


#endif






