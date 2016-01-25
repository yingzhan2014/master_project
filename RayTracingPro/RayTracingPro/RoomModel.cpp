//
//  RoomModel.cpp
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#include "RoomModel.h"
// set and get members

RoomModel::~RoomModel() {
    
}

void RoomModel::setRoomDimension(const Eigen::VectorXd& room_dim) {
    room_dimension = room_dim;
    
}

Eigen::VectorXd RoomModel::getRoomDimension() {
    return room_dimension;
    
}

void RoomModel::setSourcePosition(const Eigen::VectorXd& source) {
    source_position = source;
}

Eigen::VectorXd RoomModel::getSourcePosition() {
    return source_position;
    
}

void RoomModel::setListenerPosition(const Eigen::VectorXd& listener_point) {
    listener_position = listener_point;
}

Eigen::VectorXd RoomModel::getListenerPosition() {
    return listener_position;
}

void RoomModel::setListenerRadius(float radius) {
    listener_radius = radius;
}

float RoomModel::getListenerRadius() {
    return listener_radius;
}


float RoomModel::wall_absorbent_filter(float energy,int surface_id) {
    float decay_coeff;
    float left_energy;
    
    if (surface_id == 0) {
        decay_coeff = 0.17;
    }
    
    else if (surface_id == 1) {
        decay_coeff = 0.13;
    }
    
    else if (surface_id == 2) {
        decay_coeff = 0.19;
    }
    
    else if (surface_id == 3) {
        decay_coeff = 0.14;
    }
    
    else if (surface_id == 4) {
        decay_coeff = 0.18;
    }
    
    else
        decay_coeff = 0.16;
    
    left_energy = (1 - decay_coeff) * energy;
    return left_energy;
    
}
bool RoomModel::isPointOnEdge(const Eigen::VectorXd& point) {
    
    Eigen::VectorXd room_dim = getRoomDimension();
    float room_dim_x = room_dim[0];
    float room_dim_y = room_dim[1];
    float room_dim_z = room_dim[2];
    if ( point[0] == 0 && (point[2] == 0 || point[2] == room_dim_z) ) {
        return true;
    }
    
    if ( point[0] == room_dim_x  && (point[2] == 0 || point[2] == room_dim_z) ) {
        return true;
    }
    
    if ( point[0] == 0 && (point[1] == 0 || point[1] == room_dim_y) ) {
        return true;
    }
    
    if ( point[0] == room_dim_x && (point[1] == 0 || point[1] == room_dim_y) ) {
        return true;
    }
    
    if ( point[1] == 0 && (point[2] == 0 || point[2] == room_dim_z) ) {
        return true;
    }
    
    if ( point[1] == room_dim_y && (point[2] == 0 || point[2] == room_dim_z) ) {
        return true;
    }
    return false;
    

}

bool RoomModel::isValidIntersectionPoint(Eigen::VectorXd& point) {
    
    Eigen::VectorXd room_dim = getRoomDimension();
    //std::cout << "room_dim: "<<room_dim << std::endl;
    
    float room_dim_x = room_dim[0];
    float room_dim_y = room_dim[1];
    float room_dim_z = room_dim[2];
    
    float x_diff = point[0] - room_dim_x;
    float y_diff = point[1] - room_dim_y;
    float z_diff = point[2] - room_dim_z;
//    std::cout << "point[0] : " << point[0] << std::endl;
//    std::cout << "point[1] : " << point[1] << std::endl;
//    std::cout << "point[2] : " << point[2] << std::endl;
//    
//    std::cout << "room_dim_x: " << room_dim_x << std::endl;
//    std::cout << "room_dim_y: " << room_dim_y << std::endl;
//    std::cout << "room_dim_z: " << room_dim_z << std::endl;
//    
    //std::cout << "difference 1 : " << x_diff << std::endl;
    //std::cout << "difference 2 : " << y_diff << std::endl;
    //std::cout << "difference 3 : " << z_diff<< std::endl;
    
    // 0< point[0] < room_dim_x
    if (fabs(x_diff) <= TOLERANCE) {
        x_diff = 0;
        point[0] = room_dim_x;
    }
    if (fabs(point[0]) <= TOLERANCE) {
        point[0] = 0;
    }
    if (fabs(y_diff) <= TOLERANCE) {
        y_diff = 0;
        point[1] = room_dim_y;
    }
    if (fabs(point[1]) <= TOLERANCE) {
        point[1] = 0;
    }
    if (fabs(z_diff) <= TOLERANCE) {
        z_diff = 0;
        point[2] = room_dim_z;
    }
    if (fabs(point[2]) <= TOLERANCE) {
        point[2] = 0;
    }
    
    //std::cout << "intersection point: "<< point << std::endl;
//    std::cout << "x_diff: "<<x_diff << std::endl;
//    std::cout << "y_diff: "<<y_diff << std::endl;
//    std::cout << "z_diff: "<<z_diff << std::endl;
    
    if ( x_diff <= 0 && y_diff <= 0 && z_diff <= 0 && point[0] >= -TOLERANCE && point[1] >= -TOLERANCE && point[2] >= -TOLERANCE) {
        //std::cout << "this is a valid point" << std::endl;
        //std::cout<< "more accurate point: "<<point<<std::endl;
        
        return true;
    }
    return false;
    
}

int RoomModel::getCurrentValidSurfaceID(Ray* ray, const Eigen::VectorXd& point) {
    
    Eigen::VectorXd direction_vector = ray->getRayDirection();
    //std::cout << "point : " << point << std::endl;
    //std::cout<< "direction_vector: " << direction_vector << std::endl;
    Eigen::VectorXd intersection_point;
    
    for (int surface_id = 0; surface_id < 6; surface_id++) {
        //interPoint(:,j) = point(1:3)-(surfaces(numOfSurfaces(j),:)*point/(surfaces(numOfSurfaces(j),1:3)*directionVectors))*directionVectors;
        
        //std::cout << "something: " << direction_vector.dot(normal_surfaces.row(surface_id))<< std::endl;
        if (direction_vector.dot(normal_surfaces.row(surface_id)) < 0) {
            // pick up one point in each surface
            //std::cout << "surface_id:" << surface_id << std::endl;
            //std::cout << direction_vector.dot(normal_surfaces.row(surface_id)) << std::endl;
            Eigen::VectorXd old_point(4,1);
            old_point<<point,1;
            //std::cout << "old_point: "<< old_point << std::endl;
            //std::cout << "surfaces : "<<surfaces << std::endl;
            Eigen::VectorXd surface;
            //std::cout << "surface: " << surfaces.row(surface_id)<<std::endl;
            surface = surfaces.row(surface_id);
            float numerator = surface.dot(old_point);
            float denominator = surface.head(3).dot(direction_vector);
            intersection_point = point - (numerator / denominator) * direction_vector;
            //std::cout<< "intersection_point: "<<intersection_point << std::endl;
            
            if (isValidIntersectionPoint(intersection_point) && point!=intersection_point) {
                ray->setStartTracingPoint(intersection_point);
                return surface_id;
            }

            }
        
    }
    return -1;
}



Eigen::VectorXd RoomModel::calReflection(const Eigen::VectorXd& direction_vector, int id) {
    
    //Eigen::VectorXd direction_vector = ray->getRayDirection();
    Eigen::VectorXd surface_normal_vector = normal_surfaces.row(id);
    
    Eigen::VectorXd reversed_direction = -direction_vector;
    //std::cout<<newDirec<<std::endl;
    Eigen::VectorXd temp(3,1);
    Eigen::VectorXd reflected_direction(3,1);
    //std::cout<<newDirec.cwiseProduct(normalOfSurface)<<std::endl;
    //Eigen::VectorXd normalOfSurface = normalOfSurfaces.row(0);
    temp = 2*(reversed_direction.cwiseProduct(surface_normal_vector));
    //std::cout << "temp: " << temp << std::endl;
   // std::cout << "haha: " << temp.cwiseProduct(surface_normal_vector) << std::endl;
    reflected_direction = temp.cwiseProduct(surface_normal_vector)-reversed_direction;
    
    //std::cout<<"reflected_direction: " << reflected_direction << std::endl;
    
    return reflected_direction;
    
}


bool RoomModel::isInListenerArea(Ray* ray) {
    // point 3by1 vector , listener_position 3by1 vector
    // direction_vector is a row vector (1by3) in the 3-d case
    // point_listener_vector is column vector
    // ray : energy, direction, sign, distance, point
    
    Eigen::VectorXd start_tracing_point = ray->getStartTracingPoint();
    Eigen::VectorXd listener_point = getListenerPosition();
    Eigen::VectorXd point_listener_vector = start_tracing_point - listener_point;
   // std::cout << "point_listener_vector: " << point_listener_vector << std::endl;
    // o is source, c is center of the sphere
    // point_listener_vector
    // here direction_vector's norm should be equal to 1
    // temp = 1by3 * 3by1
    Eigen::VectorXd direction_vector = ray->getRayDirection();
    float temp = direction_vector.dot(point_listener_vector);
    // need to compute the norm of the point_listener_vector
    float norm_point_listener_vector = point_listener_vector.norm();
    float b = 2 * temp;
    float a = 1; // a = square (direction_vector_norm)
    float c = norm_point_listener_vector * norm_point_listener_vector - listener_radius * listener_radius;
    float d = b * b - 4 * a * c;
    //std::cout << "d is : " << d << std::endl;
    if (d < 0) {
        return false;
    }
    return true;

}

