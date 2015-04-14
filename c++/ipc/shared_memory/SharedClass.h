#pragma once


#include <AclAPI.h>


/************************************************************
 *
 * Shared Class
 *
 ************************************************************/
template <class ClassType> class CSharedClass
{
public:
	// Constructor
	CSharedClass(void);

	// Destructor
	~CSharedClass(void);


	// Attatch
	BOOL Attatch(TCHAR* pszSharedName);

	// Detach
	VOID Detach(void);


	// Get Shared Data
	ClassType* GetSharedData(void);

	// Shared Data Property
	__declspec(property(get = GetSharedData)) ClassType* SharedData;


private:
	// Shared Data Handle
	HANDLE		m_hSharedData;

	// Shared Data
	ClassType*	m_pSharedData;
};


/************************************************************
 *	@brief		Constructor
 *	@retval		None
 ************************************************************/
template <class ClassType> CSharedClass<ClassType>::CSharedClass()
{
   m_hSharedData	= NULL;
}


/************************************************************
 *	@brief		Destructor
 *	@retval		None
 ************************************************************/
template <class ClassType> CSharedClass<ClassType>::~CSharedClass()
{
   Detach();
}


/************************************************************
 *	@brief		Attatch
 *	@retval		None
 ************************************************************/
template <class ClassType> BOOL CSharedClass<ClassType>::Attatch(TCHAR* pszSharedName)
{
   m_hSharedData	= CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(ClassType), pszSharedName);

   if (m_hSharedData == NULL)
   {
	  return FALSE;
   }


   SetNamedSecurityInfo(pszSharedName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL) NULL, NULL);


   m_pSharedData	= (ClassType *) MapViewOfFile( m_hSharedData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

   if (m_pSharedData == NULL)
   {
      CloseHandle(m_hSharedData);

	  m_hSharedData		= NULL;

	  return FALSE;
   }


   return TRUE;
}


/************************************************************
 *	@brief		Detach
 *	@retval		None
 ************************************************************/
template <class ClassType> VOID CSharedClass<ClassType>::Detach()
{
   if (m_pSharedData)
   {
      UnmapViewOfFile(m_pSharedData);

	  m_pSharedData		= NULL;
   }


   if (m_hSharedData)
   {
      CloseHandle(m_hSharedData);

	  m_hSharedData		= NULL;
   }
}


/************************************************************
 *	@brief		Get Shared Data
 *	@retval		None
 ************************************************************/
template <class ClassType> ClassType* CSharedClass<ClassType>::GetSharedData(void)
{
   return m_pSharedData;
}

