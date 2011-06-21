#include "CRobot.h"
#include "CConfigLoader.h"
#include "CLineFactory.h"
#include "CAlgoAbstract.h"
#include "CJacobianTranspose.h"
#include "CDumpedLeastSquares.h"
#include "CAlgoFactory.h"
#include <iostream>

#define _TESTDLS

int main()
{

#ifdef _TEST1

    std::string str = "robot.xml";
    CConfigLoader cfg(str);
    if(!cfg.LoadXml()) return 1;
    //Set origin at O_zero
    CRobot robot(Vector3f(0.0f,0.0f,0.0f));
    robot.LoadConfig(cfg.GetTable());
    //Print loaded data
    robot.PrintHomogenTransformationMatrix();
    robot.PrintFullTransformationMatrix();
    //Rotating joint 1 for 90 degrees
    robot.RotateJoint(DHINDEX(1),90.0f);
    robot.PrintHomogenTransformationMatrix();
    robot.PrintFullTransformationMatrix();

#endif

#ifdef _TEST2

    CLine3D line;
    Vector3f test2;
    Vector3f vec_end(20,1,0);
    Vector3f vec_start(1,2,3);
    float    displ = 0.01f;

    test2 = line.GetNextPoint(vec_end,vec_start,displ);

    while(true)
    {
        if (test2 == vec_end)
        {
            break;
        }

        std::cout<<test2<<std::endl;

        test2 = line.GetNextPoint(vec_end,test2,displ);
    }   

    std::cout<<test2<<std::endl;

#endif

#ifdef _TESTJACIBIANTRANSPOSE

    std::string str = "robot.xml";
    CConfigLoader cfg(str);
    if(!cfg.LoadXml()) return 1;
    //Set origin at O_zero
    CRobot robot(Vector3f(0.0f,0.0f,0.0f));
    robot.LoadConfig(cfg.GetTable());
    CAlgoFactory factory;
    VectorXf des(6);
    float speccfc = 0.001f;
    des << 200.0f ,200.0f ,0.0f ,0.0f ,0.0f ,0.0f ;

    CAlgoAbstract * pJpt = factory.GiveMeSolver(JACOBIANTRANSPOSE,des,robot);
    pJpt->SetAdditionalParametr(speccfc);
    pJpt->CalculateData();
    robot.PrintConfiguration();

#endif

#ifdef _TESTJACIBIANPSEUDO

    std::string str = "robot.xml";
    CConfigLoader cfg(str);
    if(!cfg.LoadXml()) return 1;
    //Set origin at O_zero
    CRobot robot(Vector3f(0.0f,0.0f,0.0f));
    robot.LoadConfig(cfg.GetTable());
    CAlgoFactory factory;
    VectorXf des(6);
    float speccfc = 0.001f;
    des << 200.0f , 200.0f , 0.0f , 0.0f , 0.0f , 0.0f ;

    CAlgoAbstract * pJpt = factory.GiveMeSolver(JACOBIANPSEVDOINVERSE,des,robot);
    pJpt->CalculateData();
    robot.PrintConfiguration();

#endif

#ifdef _TESTDLS

    std::string str = "robot.xml";
    CConfigLoader cfg(str);
    if(!cfg.LoadXml()) return 1;
    //Set origin at O_zero
    CRobot robot(Vector3f(0.0f,0.0f,0.0f));
    robot.LoadConfig(cfg.GetTable());
    CAlgoFactory factory;
    VectorXf des(6);
    float speccfc = 0.001f;
    des << 200.0f , 200.0f , 0.0f , 0.0f , 0.0f , 0.0f ;

    CAlgoAbstract * pJpt = factory.GiveMeSolver(DUMPEDLEASTSQUARES,des,robot);
    pJpt->SetAdditionalParametr(speccfc);
    pJpt->CalculateData();
    robot.PrintConfiguration();

#endif

    return 0;
}