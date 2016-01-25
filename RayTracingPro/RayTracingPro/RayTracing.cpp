
//
//  RayTracing.cpp
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#include "RayTracing.h"
#include "thread"
//#include <cmath>


void RayTracing::processRay(Ray* ray) {
    
    //Eigen::VectorXd ray_info(2,1);
    Eigen::VectorXd point = ray->getStartTracingPoint();
    float energy = ray->getEnergy();
    //std::cout << "left energy: " << energy << std::endl;
    //std::cout << "SHIIIT: "<< energy <<std::endl;
//    std::cout << "SHIIIT: "<< fabs(energy) <<std::endl;
    
    if ( fabs(energy) < 0.001 || room->isPointOnEdge(point)) {
        ray->setEnergy(0);
        ray->setRayDistance(0);
        return;
    }
    
    
    if (room->isInListenerArea(ray)) {
        //float distance = ray->getStartTracingPoint()
       
        //Eigen::VectorXd point = ray->getStartTracingPoint();
        Eigen::VectorXd listen_point = room->getListenerPosition();
        float distance = sqrt((point - listen_point).dot(point-listen_point));
        float total_distance = distance + ray->getRayDistance();
        ray->setRayDistance(total_distance);
//        if (energy == 1) {
//            std::cerr << point << std::endl;
//            std::cerr << distance << std::endl;
//        }
        return;
    }
        
    else {
        Eigen::VectorXd incident_ray_direction = ray->getRayDirection();
        //std::cout << "incident_ray_direction : "<<incident_ray_direction << std::endl;
        
        //std::cout << "point : " << point << std::endl;
//        if (room->isPointOnEdge(point)) {
//            return;
//        }
        int ray_sign = ray->getSign();
            // getCurrentValidSurfaceID(Ray* ray, Eigen::VectorXd point)
        int surface_id = room->getCurrentValidSurfaceID(ray, point);
            // this function has already changed point -> intersectionPoint;
            //Eigen::VectorXd calcReflection(Ray*, int)
        Eigen::VectorXd reflected_direction_vector = room->calReflection(incident_ray_direction, surface_id);
        //std::cout<< "reflected direction: "<< reflected_direction_vector << std::endl;
        
        Eigen::VectorXd intersection_point = ray->getStartTracingPoint();
        
       // std::cout << "intersection_point : " << intersection_point << std::endl;
        float distance_one_reflection = sqrt((point - intersection_point).dot(point-intersection_point));
        float travelled_distance = distance_one_reflection + ray->getRayDistance();
        float energy = room->wall_absorbent_filter(ray->getEnergy(),surface_id);
        float left_energy = ray_sign * energy;
        ray->setStartTracingPoint(intersection_point);
        ray->setEnergy(left_energy);
        ray->setRayDistance(travelled_distance);
        ray->setSign(-ray_sign);
        ray->setRayDirection(reflected_direction_vector);
                                            
        this->processRay(ray);
            
        }
        
}



