#include "Profile.h"


CProfile::CProfile(void)
{
    char buff[1024]="/tmp/misamino";
    m_errcode = 0;
    m_path = buff;
    char back = m_path[m_path.size()-1];
    if ( back != '\\' && back != '/' ) {
        m_path += '\\';
    }
}


CProfile::~CProfile(void)
{
}
