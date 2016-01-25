//
//  main.cpp
//  RayTracingPro
//
//  Created by Ying Zhan on 10/24/15.
//  Copyright (c) 2015 Ying Zhan. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "RayTracing.h"
#include "/Users/cherry1991/Documents/GT/Ying_MasterProject/Source/Eigen/Core"
#include "thread"
#include "future"


class raytracing_info {
private:
    Eigen::MatrixXd generateRays(int);
    
public:
    
    //int RAYS_PER_THREAD = 2;
    int total_num_rays;
    Eigen::MatrixXd all_rays_directions;
    Eigen::VectorXd room_dimension;
    Eigen::VectorXd source_position;
    Eigen::VectorXd listener_position;
    raytracing_info(int num_rays): total_num_rays(num_rays), room_dimension(3,1), source_position(3,1), listener_position(3,1){
        room_dimension<<8.6, 5.2, 3.3;
        source_position << 3.5, 2.8 ,0.7;
        listener_position << 6.2, 3.9,1.6;
        all_rays_directions = generateRays(total_num_rays);
    }
  
};


Eigen::MatrixXd raytracing_info::generateRays(int num_rays) {
    Eigen::VectorXd n;
    Eigen::VectorXd phi;
    Eigen::VectorXd ones;
    Eigen::VectorXd l(num_rays);
    Eigen::VectorXd m(num_rays);
    Eigen::MatrixXd all_rays_directions(num_rays,3);
    const long double PI = 3.14159265358979323846264338328;
    n = Eigen::VectorXd::Random(num_rays); // range -1 ~1
    ones = Eigen::VectorXd::Ones(num_rays);
    phi = Eigen::VectorXd::Random(num_rays);
    phi = (phi + ones)/2; // set random range to 0~1
    phi = 2*PI*phi;
    
    for (int i = 0; i< num_rays; i++) {
        double coeff = sqrt(1-n(i)*n(i));
        l(i) = coeff*cos(phi(i));
        m(i) = coeff*sin(phi(i));
        
        all_rays_directions(i,0) = l(i);
        all_rays_directions(i,1) = m(i);
        all_rays_directions(i,2) = n(i);
    }
    return all_rays_directions;
    
}



const int num_rays = 10000;
float energy_arr[num_rays];
float distance_arr[num_rays];
raytracing_info infos(num_rays);
RayTracing tracing_ray(0.2, infos.source_position,infos.room_dimension,infos.listener_position);
const int RAYS_PER_THREAD = 500;


bool isSingleThread = true;
bool isOpenmp = false;
bool isThread = false;
bool isSeveralRaysOneThread = false;
bool isSeveralRaysOneThreadOpenmp = false;

void processRays( int i ) {
    
    for (int j = 0; j < RAYS_PER_THREAD; j++) {
       
        Ray* ray = new Ray();
        
        ray->setRayDirection(infos.all_rays_directions.row( RAYS_PER_THREAD * i + j));
        ray->setEnergy(1);
        ray->setSign(1);
        ray->setRayDistance(0);
        ray->setStartTracingPoint(infos.source_position);
        
        tracing_ray.processRay(ray);
        
        energy_arr[RAYS_PER_THREAD * i + j] = ray->getEnergy();
        distance_arr[RAYS_PER_THREAD * i + j] = ray->getRayDistance();
        
        delete ray;
    }

}


int main()
{
    std::clock_t start;
    double duration;
    start = std::clock();
    
    if (isOpenmp) {
        #pragma omp parallel for
        //this is one ray per thread
        //next, try 10 rays in one thread
        for (int ray_number = 0; ray_number < num_rays  ; ray_number ++) {
        
            Ray *ray = new Ray();
            
            ray->setRayDirection(infos.all_rays_directions.row(ray_number));
            ray->setEnergy(1);
            ray->setSign(1);
            ray->setRayDistance(0);
            ray->setStartTracingPoint(infos.source_position);
            
            tracing_ray.processRay(ray);
            
            energy_arr[ray_number] = ray->getEnergy();
            distance_arr[ray_number] = ray->getRayDistance();
            
            delete ray;
            
        }

    }
    
    if (isSeveralRaysOneThreadOpenmp) {
        #pragma omp parallel for
        for (int thread_number = 0 ; thread_number < (num_rays / RAYS_PER_THREAD); thread_number++)
        {
            processRays(thread_number);
            
        }
        
        if (num_rays % RAYS_PER_THREAD != 0) {
            int left_rays = num_rays - (num_rays/RAYS_PER_THREAD)*RAYS_PER_THREAD;
            for (int i = 0; i < left_rays; i++) {
                Ray * ray = new Ray();
                tracing_ray.processRay(ray);
                energy_arr[i + (num_rays / RAYS_PER_THREAD) * RAYS_PER_THREAD] = ray->getEnergy();
                distance_arr[i + (num_rays / RAYS_PER_THREAD) * RAYS_PER_THREAD] = ray->getRayDistance();
            }
        
        }
    }

    
    if (isThread) {
        
        for (int ray_number = 0; ray_number < num_rays; ray_number++) {
            Ray *ray = new Ray();
            ray->setRayDirection(infos.all_rays_directions.row(ray_number));
            ray->setEnergy(1);
            ray->setSign(1);
            ray->setRayDistance(0);
            ray->setStartTracingPoint(infos.source_position);
            
            auto f = std::async(std::launch::async, &RayTracing::processRay, &tracing_ray, ray);
            f.get();
            
            energy_arr[ray_number] = ray->getEnergy();
            distance_arr[ray_number] = ray->getRayDistance();
    
            delete ray;
        }

    }
    
    if (isSeveralRaysOneThread) {

        for (int thread_number = 0; thread_number < num_rays / RAYS_PER_THREAD ; thread_number++) {
            
            auto f = std::async(std::launch::async, processRays,thread_number);
            f.get();
            
        }
            
        if (num_rays % RAYS_PER_THREAD != 0) {
                
            int left_rays = num_rays - (num_rays/RAYS_PER_THREAD)*RAYS_PER_THREAD;
                
            for (int i = 0; i < left_rays; i++) {
                    
                Ray * ray = new Ray();
                ray->setRayDirection(infos.all_rays_directions.row(i + (num_rays / RAYS_PER_THREAD) * RAYS_PER_THREAD));
                ray->setEnergy(1);
                ray->setSign(1);
                ray->setRayDistance(0);
                ray->setStartTracingPoint(infos.source_position);
                tracing_ray.processRay(ray);

                energy_arr[i + (num_rays / RAYS_PER_THREAD) * RAYS_PER_THREAD] = ray->getEnergy();
                distance_arr[i + (num_rays / RAYS_PER_THREAD) * RAYS_PER_THREAD] = ray->getRayDistance();
                
            }

            
        }
    
   }
    
    
    if (isSingleThread) {
    
        for (int ray_number = 0; ray_number < num_rays  ; ray_number ++) {
            
            Ray *ray = new Ray();
            
            ray->setRayDirection(infos.all_rays_directions.row(ray_number));
            ray->setEnergy(1);

            ray->setSign(1);
            ray->setRayDistance(0);
            ray->setStartTracingPoint(infos.source_position);
            tracing_ray.processRay(ray);
            energy_arr[ray_number] = ray->getEnergy();
            distance_arr[ray_number] = ray->getRayDistance();
            delete ray;
        }

    }
    
//    for (int j = 0; j < num_rays; j++) {
//        std::cerr << "energy_array of  " <<j<< " is: "<<energy_arr[j] << std::endl;
//        std::cerr << "distance_array of  " <<j<< "is: "<< distance_arr[j] << std::endl;
//        }
//    
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    
    std::cout<<"duration: "<< duration << std::endl;;
   
    
}




