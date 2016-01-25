//
//  Ray.h
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#ifndef __RayTracingPro__Ray__
#define __RayTracingPro__Ray__

#include <iostream>
#include "/Users/cherry1991/Documents/GT/Ying_MasterProject/Source/Eigen/Core"

class Ray
{
private:
    int ray_sign;
    float ray_energy;
    float ray_distance;
    Eigen::VectorXd ray_direction;
    Eigen::VectorXd start_tracing_point;
    
public:
    Ray();
    ~Ray();
    void setSign(int sign);
    int getSign();
    
    void setEnergy(float);
    float getEnergy();
    
    void setRayDirection(Eigen::VectorXd);
    Eigen::VectorXd getRayDirection();
    
    void setStartTracingPoint(Eigen::VectorXd);
    Eigen::VectorXd getStartTracingPoint();
    
    void setRayDistance(float);
    float getRayDistance();
    
    
};






#endif /* defined(__RayTracingPro__Ray__) */
