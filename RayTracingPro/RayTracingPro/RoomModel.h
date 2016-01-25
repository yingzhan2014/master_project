//
//  RoomModel.h
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#ifndef __RayTracingPro__RoomModel__
#define __RayTracingPro__RoomModel__

#include <iostream>
#include "/Users/cherry1991/Documents/GT/Ying_MasterProject/Source/Eigen/Core"
#include "Ray.h"

//   room : container for room dimension
//   surface
//   listener position and listener sphere

class RoomModel
{
private:
    Eigen::VectorXd room_dimension;
    Eigen::VectorXd source_position;
    Eigen::VectorXd listener_position;
    float listener_radius;
    Eigen::MatrixXd normal_surfaces;
    Eigen::MatrixXd surfaces;
    float TOLERANCE;
    
    
    
public:
    RoomModel(const Eigen::VectorXd& room_dim):normal_surfaces(6,3), surfaces(6,4){
        
        surfaces<<0,0,1,0,0,0,1,-room_dim[2],1,0,0,0,1,0,0,-room_dim[0],0,1,0,0,0,1,0,-room_dim[1];
        normal_surfaces<<0,0,1,0,0,-1,1,0,0,-1,0,0,0,1,0,0,-1,0;
        room_dimension = room_dim;
        TOLERANCE = 0.0001;
        
    };
    ~RoomModel();
    // set and get members
    void setRoomDimension(const Eigen::VectorXd&);
    
    Eigen::VectorXd getRoomDimension();
    
    void setSourcePosition(const Eigen::VectorXd&);
    
    Eigen::VectorXd getSourcePosition();
    
    void setListenerPosition(const Eigen::VectorXd&);
    
    Eigen::VectorXd getListenerPosition();
    
    void setListenerRadius(float);
    
    float getListenerRadius();
    
    float wall_absorbent_filter(float,int);
    
    bool isPointOnEdge(const Eigen::VectorXd& point);
    
    // calculate ray reflection direction vector
    Eigen::VectorXd calReflection(const Eigen::VectorXd&, int);
   
    bool isInListenerArea(Ray*);
    int getCurrentValidSurfaceID(Ray*, const Eigen::VectorXd&);
    bool isValidIntersectionPoint(Eigen::VectorXd&);
    
    
};



#endif /* defined(__RayTracingPro__RoomModel__) */
