#include "util/tc_dyn_object.h"
#include <string.h>

namespace taf
{
/**********************************TC_DYN_RuntimeClass定义***********************************/

TC_DYN_RuntimeClass* TC_DYN_RuntimeClass::pFirstClass = NULL;

TC_DYN_Object* TC_DYN_RuntimeClass::createObject()
{
    if (m_pfnCreateObject == NULL)
    {
        return NULL;
    }

    TC_DYN_Object* pObject = (*m_pfnCreateObject)();
    {
        return pObject;
    }
}

TC_DYN_RuntimeClass* TC_DYN_RuntimeClass::load(const char *szClassName)
{
    TC_DYN_RuntimeClass* pClass;

    for (pClass = pFirstClass; pClass != NULL; pClass = pClass->m_pNextClass)
    {
        if (strcmp(szClassName, pClass->m_lpszClassName) == 0)
        return pClass;
    }

    return NULL;
}

/**********************************szTC_DYN_Object定义***********************************/

TC_DYN_RuntimeClass TC_DYN_Object::classTC_DYN_Object =
{
    (char*)"TC_DYN_Object", 
    sizeof(TC_DYN_Object), 
    NULL, 
    NULL, 
    NULL 
};

static TC_DYN_Init _init_TC_DYN_Object(&TC_DYN_Object::classTC_DYN_Object);

TC_DYN_RuntimeClass* TC_DYN_Object::GetRuntimeClass() const
{
	return &TC_DYN_Object::classTC_DYN_Object;
}

bool TC_DYN_Object::isKindOf(const TC_DYN_RuntimeClass* pClass) const
{
    TC_DYN_RuntimeClass* pClassThis = GetRuntimeClass();
    while (pClassThis != NULL)
    {
        if (pClassThis == pClass)
        {
            return true;
        }

        pClassThis = pClassThis->m_pBaseClass;
    }
    return false;
}

}
