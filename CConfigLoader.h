#ifndef __CCONFIGLOADER__
#define __CCONFIGLOADER__

#include "src\pugixml.hpp"
#include "SharedTypes.h"
#include <string>

class CConfigLoader
{
    std::string fname;
    dh_table    for_load;
public:
    CConfigLoader(std::string & xml_name):fname(xml_name){}
    bool LoadXml();
    OUT dh_table & GetTable();
};

#endif