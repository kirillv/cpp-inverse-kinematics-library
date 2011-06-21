#include "CConfigLoader.h"
#include <iostream>
#include "lexer.h"


dh_table & CConfigLoader::GetTable()
{
    return for_load;
}

bool CConfigLoader::LoadXml()
{
    dh_parametrs temp_struct;

    std::string  temp_string;
    std::string  out_temp_string;

    Token tkn;

    temp_struct.z_joint_type = NOTSET;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fname.c_str());
    //Show robot name
#ifdef DEBUGOUTPUTFORXML
    std::cout << "Load result: " << result.description() <<std::endl;
    std::cout << "Robot name: " << doc.child("Robot").attribute("name").value() << std::endl;
#endif
    pugi::xml_node tools = doc.child("Robot").child("Joints");
    //Second method
    for (pugi::xml_node tool = tools.child("joint"); tool; tool = tool.next_sibling("joint"))
    {
        //Get joint name
#ifdef DEBUGOUTPUTFORXML
        std::cout << "Name: " << tool.attribute("name").value()<<std::endl;
#endif
        temp_struct.joint_name  = tool.attribute("name").value();
        //Id is currently not needed
#ifdef DEBUGOUTPUTFORXML
        std::cout << "id  : " << tool.attribute("id").value()<<std::endl;
#endif
        //Get alphai of joint
#ifdef DEBUGOUTPUTFORXML
        std::cout << "alphai    :" << tool.attribute("alphai").value()<<std::endl;
#endif
        temp_struct.alpha       = tool.attribute("alphai").as_float();
        //Get a of joint
#ifdef DEBUGOUTPUTFORXML
        std::cout << "ai        :" << tool.attribute("ai").value()<<std::endl;
#endif
        temp_struct.a           = tool.attribute("ai").as_float();
        //Get d of joint
#ifdef DEBUGOUTPUTFORXML
        std::cout << "di        :" << tool.attribute("di").value()<<std::endl;
#endif
        temp_string             = tool.attribute("di").value();
        tkn = scan_string(temp_string.c_str(),temp_string.size(),out_temp_string);
        switch (tkn)
        {
        case INT:
        case FLOAT:
            temp_struct.d = tool.attribute("di").as_float();
        	break;
        case VAR:
            temp_struct.d = (float)strtod(out_temp_string.c_str(), 0);
            temp_struct.z_joint_type = PRISMATIC;
            break;
        case ERROR:
            std::cout<<"Error. Cannot convert :"<< temp_string<<std::endl;
            return false;
        }
        //Get theta of joint
#ifdef DEBUGOUTPUTFORXML
        std::cout << "theta     :" << tool.attribute("theta").value()<<std::endl;
#endif
        temp_string             = tool.attribute("theta").value();
        tkn = scan_string(temp_string.c_str(),temp_string.size(),out_temp_string);
        switch (tkn)
        {
        case INT:
        case FLOAT:
            temp_struct.theta = tool.attribute("di").as_float();
            break;
        case VAR:
            temp_struct.theta = (float)strtod(out_temp_string.c_str(), 0);

            if (temp_struct.z_joint_type == PRISMATIC)
            {
                std::cout<<"Error. Joint cannot be REVOLUTE and PRISMATIC at the same time."<<std::endl;
                return false;
            }

            temp_struct.z_joint_type = REVOLUTE;
            break;
        case ERROR:
            std::cout<<"Error. Cannot convert :"<< temp_string<<std::endl;            
            return false;
        }

        if (temp_struct.z_joint_type == NOTSET)
        {
            temp_struct.z_joint_type = CONSTANTJOINT;
        }

        //Save DH table field
        for_load.push_back(temp_struct);
        //Clear temporary string 
        temp_string.clear();
    }

    return true;
}