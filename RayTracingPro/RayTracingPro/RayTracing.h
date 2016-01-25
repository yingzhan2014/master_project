//
//  RayTracing.h
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#ifndef __RayTracingPro__RayTracing__
#define __RayTracingPro__RayTracing__

#include <iostream>
#include "/Users/cherry1991/Documents/GT/Ying_MasterProject/Source/Eigen/Core"
#include "Ray.h"
#include "RoomModel.h"
// This class will take care the whole final IR
// Since intersection will return the one ray's final IR,
// What have to do here is the aggregation of all IRs.
class RayTracing
{
private:
    //int num_rays;
    //Eigen::MatrixXd generateRays(int num_rays);
    //float final_energy;
    //float travelled_distance;
    //Eigen::VectorXd calReflection(Ray*);
    //bool isInListenerArea(Ray*);
    //int getCurrentValidSurfaceID(Eigen::VectorXd direction_vector, Eigen::VectorXd point);
    RoomModel* room;
    
    //Ray* ray = new Ray[num_rays];
    
public:
   
    
    RayTracing(float radius, const Eigen::VectorXd& source_point, const Eigen::VectorXd& room_dim, const Eigen::VectorXd& listener_point) {
        room = new RoomModel(room_dim);
        room->setListenerRadius(radius);
        room->setSourcePosition(source_point);
        //room->setRoomDimension(room_dim);
        room->setListenerPosition(listener_point);
        
    };
    ~RayTracing() {
        delete room;
    };
    
    void processRay(Ray*);
    
};



#endif /* defined(__RayTracingPro__RayTracing__) */
