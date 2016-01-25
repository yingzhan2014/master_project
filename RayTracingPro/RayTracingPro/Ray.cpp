//
//  Ray.cpp
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#include "Ray.h"
Ray::Ray() {
    
}

Ray::~Ray() {
    
}

void Ray::setSign(int sign) {
    ray_sign = sign;
}

int Ray::getSign() {
    return ray_sign;
}

void Ray::setEnergy(float energy) {
    ray_energy = energy;
}

float Ray::getEnergy() {
    return ray_energy;
}

void Ray::setRayDirection(Eigen::VectorXd direction_vector) {
    ray_direction = direction_vector;
}

Eigen::VectorXd Ray::getRayDirection() {
    return ray_direction;
}

void Ray::setStartTracingPoint(Eigen::VectorXd point) {
    start_tracing_point = point;
}

Eigen::VectorXd Ray::getStartTracingPoint() {
    return start_tracing_point;
}

void Ray::setRayDistance(float distance) {
    ray_distance = distance;
}

float Ray::getRayDistance() {
    return ray_distance;
}
