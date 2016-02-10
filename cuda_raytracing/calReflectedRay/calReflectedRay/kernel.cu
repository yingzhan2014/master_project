#pragma once
#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>
#include <iostream>
#include <thrust/device_ptr.h>
#include <cmath>
#include <assert.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include "random"
 
// generateRays in CPU 
// copy rays_direction into Struct Ray
// copy num_rays Ray struct into GPU

// need to figure it out how create bunch of rays in main function

// Done
float** generateRays(int num_rays) {
	// all_rays_directions store num_rays by 3 matrix
	float** all_rays_directions;
	all_rays_directions = new float*[num_rays];
		
	for (int i = 0; i < num_rays; i++)
	{
		all_rays_directions[i] = new float [3];
	}

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
	std::uniform_real_distribution<float> dist(0.0, 1.0);
	const long double PI = 3.14159265358979323846264338328;
	float* phi;
	phi = new float[num_rays];
	for (int i = 0; i < num_rays; i++)
	{
		all_rays_directions[i][0] = distribution(generator);
		//std::cout << all_rays_directions[i][0] << std::endl;
		phi[i] = dist(generator);
		phi[i] = 2 * PI * phi[i];

	}
	

	for (int i = 0; i < num_rays; i++) {

		double coeff = sqrt(1 - all_rays_directions[i][0]*all_rays_directions[i][0]);
		
			all_rays_directions[i][1] = coeff*sin(phi[i]);
			all_rays_directions[i][2] = coeff*cos(phi[i]);
		

	}

	return all_rays_directions;


}

struct Ray
{
	int ray_sign;
	float ray_energy;
	float ray_distance;
	float ray_direction[3];

	float ray_start_point[3];
	Ray(float* start_point, float* direction) {
		ray_energy = 1;
		ray_sign = 1;
		ray_distance = 0;
		for (int i = 0; i < 3; i++)
		{
			ray_direction[i] = direction[i];
			ray_start_point[i] = start_point[i];

		}
		
		for (int i = 0; i < 3; i++)
		{
			std::cout << "initialized ray_direction: " << ray_direction[i] << std::endl;
			//std::cout << "initialized start_point: " << ray_start_point[i] << std::endl;

		}
		//ray_direction = direction;
	}
};

// allocate one room info in CPU and keep one copy in GPU
// how to initialize surfaces and normal_surfaces
struct Room {
	float room_dimension[3];
	float listener_radius;
	float source[3];
	float l_position[3];
	float surfaces[6][4];
	float normal_surfaces[6][3];

	Room(float*source, float* lis_position, float* room_dim) {
		listener_radius = 0.2;
		//printf("radius in constructor: %f", listener_radius);
		for (int i = 0; i < 3; i++)
		{
			room_dimension[i] = room_dim[i];
			source[i] = source[i];
			l_position[i] = lis_position[i];
		}
		/*for (int idx = 0; idx < 6; idx++)
		{
			for (int col = 0; col < 4; col++)
			{
				if (col < 3)
				{
					normal_surfaces[idx][col] = normal_surfaces[idx][col];
					surfaces[idx][col] = surfaces[idx][col];
					
				}
				surfaces[idx][col] = surfaces[idx][col];
				printf("this is surfaces matrix:%f\n", surfaces[idx][col]);
			}
		}*/

		/*for (int i = 0; i < 3; i++)
		{
			std::cout << "this is init room_dim: " << room_dimension[i] << std::endl;
			std::cout << "this is init srouce: " << source[i] << std::endl;
			std::cout << "this is init l_position: " << l_position[i] << std::endl;
		}
    */


	}
};
// helper function
// calDist checked
__device__ float  calPointsDistance(float* point1, float* point2) {
	float distance;
	for (int i = 0; i < 3; i++)
	{
		float d = point1[i] - point2[i];
		/*printf("this is the point1 in cal function: %f\n", point1[i]);
		printf("this is the point2 in cal function: %f\n", point2[i]);
		printf("this is the dist between two points: %f\n", distance);*/
		distance = distance + d*d;

	}
	distance = sqrt(distance);
	return distance;

}  // checked
// cal dot product between vectors
__device__ float calDotProduct(float* vec1, float* vec2) {
	float dot_product = 0;
	// here calculate the dot product between 
	for (int i = 0; i < 3; i++)
	{
		dot_product = dot_product + vec1[i] * vec2[i];
	}

	return dot_product;

}

// Ray related function  // done
__device__ void setStartingPoint(Ray* dev_ray, float* new_start_point) {

	for (int i = 0; i < 3; i++)
	{
		
		dev_ray->ray_start_point[i] = new_start_point[i];
		//printf("new point in set function: %f\n", dev_ray->ray_start_point[i]);

	}
}
__device__ float* getStartingPoint(Ray* dev_ray) {
	//float point[3];
	//for (int i = 0; i < 3; i++)
	//{
	//	//printf("starting point in get function: %f\n", dev_ray->ray_start_point[i]);
	//	point[i] = dev_ray->ray_start_point[i];
	//}
	/*for (int i = 0; i < 3; i++)
	{
		printf("starting point in get function: %f\n", dev_ray->ray_start_point[i]);
		
	}*/
	return dev_ray->ray_start_point;
	//return point;
}
__device__ void setSign(Ray* dev_ray, int sign) {
	dev_ray->ray_sign = sign;
}
__device__ int getSign(Ray* dev_ray) {
	return dev_ray->ray_sign;
}
__device__ void setEnergy(Ray* dev_ray, float energy) {
	dev_ray->ray_energy = energy;
}
__device__ void setDistance(Ray* dev_ray, float dist) {
	dev_ray->ray_distance = dist;
}
__device__ float getDistance(Ray* dev_ray) {
	return dev_ray->ray_distance;
}
__device__ float getEnergy(Ray* dev_ray) {
	return dev_ray->ray_energy;
}
__device__ float* getRayDirection(Ray* dev_ray) {
	return dev_ray->ray_direction;
}
__device__ void setRayDirection(Ray* dev_ray, float* new_direction) {
	for (int i = 0; i < 3; i++)
	{

		dev_ray->ray_direction[i] = new_direction[i];

	}
}

// Room related functoin
//done
__device__ float* getListPos(Room* d_room) {
	/*for (int i = 0; i < 3; i++)
	{
		printf("lis point in get function: %f\n", d_room->l_position[i]);
	}*/
	return d_room->l_position;
}
__device__ bool isPointOnEdge(Room*d_room, float* point) {

	float room_dim_x = d_room->room_dimension[0];
	float room_dim_y = d_room->room_dimension[1];
	float room_dim_z = d_room->room_dimension[2];
	if (point[0] == 0 && (point[2] == 0 || point[2] == room_dim_z)) {
		return true;
	}

	if (point[0] == room_dim_x && (point[2] == 0 || point[2] == room_dim_z)) {
		return true;
	}

	if (point[0] == 0 && (point[1] == 0 || point[1] == room_dim_y)) {
		return true;
	}

	if (point[0] == room_dim_x && (point[1] == 0 || point[1] == room_dim_y)) {
		return true;
	}

	if (point[1] == 0 && (point[2] == 0 || point[2] == room_dim_z)) {
		return true;
	}

	if (point[1] == room_dim_y && (point[2] == 0 || point[2] == room_dim_z)) {
		return true;
	}

	return false;
}
__device__ float* getRoomDimension(Room* d_room) {
	return d_room->room_dimension;
}

// isInSphere checked
__device__ bool isInSphere(Ray* dev_ray, Room* d_room) {

	
	float *start_point = getStartingPoint(dev_ray);
	for (int i = 0; i < 3; i++)
	{
		printf("start_point in isInSphere function: %f\n", start_point[i]);
	}
	float *lis_point = getListPos(d_room);
	
	float point_listen_vec[3];
	float* direction = getRayDirection(dev_ray);
	float norm_point_listen_vec = calPointsDistance(start_point, lis_point);
	float dot_product = 0;
	// here calculate the dot product between point_listen_vec and direction
	for (int i = 0; i < 3; i++)
	{   
		//printf("this is start_point: %f\n", start_point[i]);
		//printf("this is lis_point in bool function: %f\n", lis_point[i]);
		point_listen_vec[i] = start_point[i] - lis_point[i];
		//printf("this is lis_point in point_lis_vec: %f\n", point_listen_vec[i]);
		dot_product = dot_product + point_listen_vec[i] * direction[i];
	}
	/*for (int j = 0; j < 3; j++)
	{
		printf("this is start_point: %f\n", start_point[j]);
		printf("this is direction: %f\n", direction[j]);
		printf("this is point_lis_vec: %f\n", point_listen_vec[j]);
	}*/
	
	float b = 2 * dot_product;
	float a = 1; 
	float c = norm_point_listen_vec*norm_point_listen_vec - d_room->listener_radius * d_room->listener_radius;
	//printf("this is c: %f\n", c);
	float d = b * b - 4 * a * c;
	printf("this is d: %f\n", d);
	if (d < 0) {
		return false;
	}

	return true;
}
// checked with one example
__device__ int isValidIntersectionPoint(Room* d_room, float* point, int id) {


	float* room_dim = getRoomDimension(d_room);
	float room_dim_x = room_dim[0];
	float room_dim_y = room_dim[1];
	float room_dim_z = room_dim[2];
	const float TOLERANCE = 0.00001;

	float x_diff = point[0] - room_dim_x;
	float y_diff = point[1] - room_dim_y;
	float z_diff = point[2] - room_dim_z;
	/*printf("this is the x_diff: %f\n", x_diff);
	printf("this is the y_diff: %f\n", y_diff);
	printf("this is the z_diff: %f\n", z_diff);*/

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

	if (x_diff <= 0 && y_diff <= 0 && z_diff <= 0 && point[0] >= -TOLERANCE && point[1] >= -TOLERANCE && point[2] >= -TOLERANCE) {
		printf("this is the id inside isValidfunction:%d\n", id);
		return id;

	}

	return -1;
}
// checked with one example
__device__ int getCurrentValudSurfaceID(Ray* dev_ray, float* point, Room* d_room) {
	
	float* room_dim = getRoomDimension(d_room);
	
	float surfaces[6][4] = {
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, -room_dim[2] },
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, -room_dim[0] },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, -room_dim[1] }
	};
	float normal_surfaces[6][3] = { { 0, 0, 1 }, { 0, 0, -1 }, { 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 } };
	float* direction_vector = getRayDirection(dev_ray);
	float intersection_point[3];
	int surface_id = 0;
	/*for (int i = 0; i < 3; i++)
	{
		printf("this is the room_dim: %f\n", room_dim[i]);
		printf("this is the point: %f\n", point[i]);
		printf("this is the direction: %f\n", direction_vector[i]);
	}*/
	for ( surface_id = 0; surface_id < 6; surface_id++) {

		float dot_product = calDotProduct(direction_vector, normal_surfaces[surface_id]);
		printf("this is the all surface_id: %d\n", surface_id);
		
		//printf("this is the dot_product: %f\n", dot_product);
		if (dot_product < 0) {

				float* surface;
				surface = surfaces[surface_id];
				
				float numerator = 0;
				float denominator = 0;
				for (int i = 0; i < 4; i++)
				{
					if (i < 3) {
						denominator = denominator + surface[i] * direction_vector[i];
						numerator = numerator + surface[i] * point[i];
					}
					else {
						numerator = numerator + surface[i];
					}

				}

				/*printf("this is numerator: %f\n", numerator);
				printf("this is denominator: %f\n", denominator);*/

				for (int k = 0; k < 3; k++)
				{
					//printf("KKKK: %d\n", k);
					intersection_point[k] = point[k] - (numerator / denominator) * direction_vector[k];
					/*printf("this is the point: %f\n", point[k]);
					printf("this is the direction: %f\n", direction_vector[k]);*/
					
					//printf("this is intersection point: %f\n", intersection_point[k]);
				}

				
				//printf("this is the didid surface_id: %d\n", surface_id);
				//printf("this is the dadada surface_id: %d\n", surface_id);
				int valid;
				valid = isValidIntersectionPoint(d_room, intersection_point, surface_id);
				
				/*if (isValidIntersectionPoint(d_room, intersection_point, surface_id) && point != intersection_point) {
					setStartingPoint(dev_ray, intersection_point);
					printf("this is the valid surface_id: %f\n", surface_id);
					for (int k = 0; k < 3; k++)
					{
						printf("this is intersection point: %f\n", intersection_point[k]);
						
					}
					
					return surface_id;
				}*/
				
				if (valid != -1 && point != intersection_point)
				{
					//printf("VALID: %d\n", valid);
					/*for (int k = 0; k < 3; k++)
					{
						printf("this is the intersection point: %f\n", intersection_point[k]);

					}*/
					setStartingPoint(dev_ray, intersection_point);
					
					/*for (int k = 0; k < 3; k++)
					{
						printf("this is the point inside loop: %f\n", point[k]);

					}*/
					return valid;
				}

				if (valid == -1)
				{
					printf("is no valid intersection point detected in isValidIntersectionPoint funtion");
				}

				if (point == intersection_point)
				{
					printf("something worong");
				}

		}
		
	}
	return -1;
}
// checked
__device__ float* calReflecRay(float* ray_direction, int surface_id) {
	float reflected_ray[3];
	float normal_surfaces[6][3] = { { 0, 0, 1 }, { 0, 0, -1 }, { 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 } };
	float temp[3];
	
	for (int i = 0; i < 3; i++)
	{
		//printf("this is the normal_surface: %f\n", normal_surfaces[surface_id][i]);
		temp[i] = -ray_direction[i] * normal_surfaces[surface_id][i]*2;
		//printf("this is the temp: %f\n", temp[i]);
		reflected_ray[i] = temp[i] * normal_surfaces[surface_id][i] + ray_direction[i];

	}

	return reflected_ray;
}
__device__ float absorbEnergy(float energy, int surface_id) {
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


// you can not make this function 'global'-> it's not allowed
// but you can do recursion in device function
__device__ void processRay(Ray* dev_ray, Room* d_room) {

	// DEBUG INFO: point is a pointer to dev_ray->ray_start_point
	/*Process Here: 
	a) get start point of the ray 
	b) copy the start point to old point
	c) check if this segment of ray has intersection with sphere or not
	d) if there is no intersection between sphere and the segment, then cal reflection and intersection point
		IMPORTANT NOTE HERE: You call the setRayStartPoint function inside calValidSurfaceID function, which assign
							 intersection point to dev_ray->ray_start_point, and since point var in processRay function is the pointer
							 of dev_ray->ray_start_point (that's why you copy the data inside the point to the other var old point)*/
	while (fabs(getEnergy(dev_ray)) > 0.01) // || isPointOnEdge(d_room, dev_ray->ray_start_point)
	{
		float* point = getStartingPoint(dev_ray);
		for (int i = 0; i < 3; i++)
		{
			printf("this is the start point: %f\n", point[i]);

		}
		// if you do not copy point data to old_point, after you call function setStartingPoint(intersection_point),
		// you are actually set intersection_point data to point too. So under this you got intersection_point == point
		float old_point[3];
		for (int i = 0; i < 3; i++)
		{
			old_point[i] = point[i];
			printf("this is the OLD POINT: %f\n", old_point[i]);
		}
		float energy = getEnergy(dev_ray);
		printf("this is the start energy: %f\n", energy);

		/*if (fabs(energy) < 0.05 || isPointOnEdge(d_room, dev_ray->ray_start_point)) {
			setEnergy(dev_ray, 0);
			setDistance(dev_ray, 0);
			return;
		}*/

		if (isInSphere(dev_ray, d_room)) {
			float* listen_point = getListPos(d_room);
			// here cal the distance between listen point and source
			float distance;
			distance = calPointsDistance(dev_ray->ray_start_point, listen_point);
			float total_dist = distance + getDistance(dev_ray);
			setDistance(dev_ray, total_dist);
			return;

		}

		else {
			float* incident_ray_direction = getRayDirection(dev_ray);

			for (int i = 0; i < 3; i++)
			{
				printf("this is the incident: %f\n", incident_ray_direction[i]);

			}
			int ray_sign = getSign(dev_ray);
			printf("this sign: %d\n", ray_sign);
			int surface_id = getCurrentValudSurfaceID(dev_ray, dev_ray->ray_start_point, d_room);
			printf("this VALID: %d\n", surface_id);
			if (surface_id == -1)
			{
				printf("no valid surface");
			}

			float* reflected_ray_direction = calReflecRay(incident_ray_direction, surface_id);

			for (int i = 0; i < 3; i++)
			{
				printf("this is the reflec: %f\n", reflected_ray_direction[i]);

			}
			float* intersection_point = getStartingPoint(dev_ray);
			for (int i = 0; i < 3; i++)
			{

				printf("this is the intersection: %f\n", intersection_point[i]);
			}
			float dist_one_reflection = calPointsDistance(old_point, intersection_point);
			printf("this is the dist between one reflec: %f\n", dist_one_reflection);
			float traveled_dist = dist_one_reflection + getDistance(dev_ray);
			printf("travaled_dist is : %f\n", traveled_dist);
			float energy = absorbEnergy(getEnergy(dev_ray), surface_id);
			float left_energy = ray_sign * energy;
			printf("this is the left_energy: %f\n", left_energy);

			setStartingPoint(dev_ray, intersection_point);
			setEnergy(dev_ray, left_energy);
			setDistance(dev_ray, traveled_dist);
			setSign(dev_ray, -ray_sign);
			setRayDirection(dev_ray, reflected_ray_direction);

			//processRay(dev_ray, d_room);
		}
	}
	//float* point = getStartingPoint(dev_ray);
	//for (int i = 0; i < 3; i++)
	//{
	//	printf("this is the start point: %f\n", point[i]);

	//}
	//// if you do not copy point data to old_point, after you call function setStartingPoint(intersection_point),
	//// you are actually set intersection_point data to point too. So under this you got intersection_point == point
	//float old_point[3];
	//for (int i = 0; i < 3; i++)
	//{
	//	old_point[i] = point[i];
	//	printf("this is the OLD POINT: %f\n", old_point[i]);
	//}
	//float energy = getEnergy(dev_ray);
	//printf("this is the start energy: %f\n", energy);

	//if (fabs(energy) < 0.05 || isPointOnEdge(d_room,dev_ray->ray_start_point)) {
	//	setEnergy(dev_ray, 0);
	//	setDistance(dev_ray, 0);
	//	return;
	//}

	//if (isInSphere(dev_ray, d_room)) {
	//	float* listen_point = getListPos(d_room);
	//	// here cal the distance between listen point and source
	//	float distance;
	//	distance = calPointsDistance(dev_ray->ray_start_point, listen_point);
	//	float total_dist = distance + getDistance(dev_ray);
	//	setDistance(dev_ray,total_dist);
	//	return;

	//}

	//else {
	//	float* incident_ray_direction = getRayDirection(dev_ray);
	//	
	//	for (int i = 0; i < 3; i++)
	//	{
	//		printf("this is the incident: %f\n", incident_ray_direction[i]);
	//		
	//	}
	//	int ray_sign = getSign(dev_ray);
	//	printf("this sign: %d\n", ray_sign);
	//	int surface_id = getCurrentValudSurfaceID(dev_ray, dev_ray->ray_start_point, d_room);
	//	printf("this VALID: %d\n", surface_id);
	//	if (surface_id == -1)
	//	{
	//		printf("no valid surface");
	//	}
	//	
	//	float* reflected_ray_direction = calReflecRay(incident_ray_direction, surface_id);
	//	
	//	for (int i = 0; i < 3; i++)
	//	{
	//		printf("this is the reflec: %f\n", reflected_ray_direction[i]);
	//		
	//	}
	//	float* intersection_point = getStartingPoint(dev_ray);
	//	for (int i = 0; i < 3; i++)
	//	{
	//		
	//		printf("this is the intersection: %f\n", intersection_point[i]);
	//	}
	//	float dist_one_reflection = calPointsDistance(old_point, intersection_point);
	//	printf("this is the dist between one reflec: %f\n", dist_one_reflection);
	//	float traveled_dist = dist_one_reflection + getDistance(dev_ray);
	//	printf("travaled_dist is : %f\n", traveled_dist);
	//	float energy = absorbEnergy(getEnergy(dev_ray), surface_id);
	//	float left_energy = ray_sign * energy;
	//	printf("this is the left_energy: %f\n", left_energy);

	//	setStartingPoint(dev_ray, intersection_point);
	//	setEnergy(dev_ray, left_energy);
	//	setDistance(dev_ray, traveled_dist);
	//	setSign(dev_ray, -ray_sign);
	//	setRayDirection(dev_ray, reflected_ray_direction);

	//	processRay(dev_ray, d_room);
	//}

	
}

__global__ void processAllRays(Ray** dev_ray, Room* d_room) {
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	
	printf("this is the idx: %d\n", idx);

	processRay(dev_ray[idx], d_room);
}
// these functions are designed for testing cuda
__global__ void print(Ray** dev_ray, Room*d_room)

{
	
	// test for calDistance between two points function
	/*float a[3] = { 1.2, 5, 7 };
	float b[3] = { 2.4, 5.5, -1.3 };
	float dist = calPointsDistance(a, b);
	printf("this is the distance between to point: %f\n", dist);*/

	// test for isInSphere
	//bool ok = isInSphere(dev_ray, d_room);

	// test for calReflecRay
	/*float* reflected_ray;
	reflected_ray = calReflecRay(dev_ray->ray_direction, 4);*/

	// test for getValidSurface
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	//getCurrentValudSurfaceID(dev_ray, getStartingPoint(dev_ray), d_room);
	printf("this is the idx: %d\n", idx);

	//printf("this is the ray_energy: %d\n",(*(dev_ray[idx])).ray_sign);
	processRay(dev_ray[idx], d_room);
	//for (int i = 0; i < 3; i++)
	//{

	//	//printf("THIS IS RAY_DIREC: %f", dev_ray->ray_direction[i]);

	//	printf("ray_direction: %f\n", dev_ray[idx]->ray_direction[i]);

	//	//printf("reflected_Ray: %f\n", reflected_ray[i]);
	//}

}

int main(void)
	{
		cudaError_t err_ray = cudaSuccess;
		cudaError_t err_room = cudaSuccess;
		cudaError_t err_all_rays = cudaSuccess;
		
		int num_rays = 5;
		float** all_rays_directions;
		all_rays_directions = generateRays(num_rays);

		// define the sound source position, listener position, adn room dimension info
		float source[3] = { 1.2, 3.6, 5.9 };
		float l_position[3] = { 5.3, 7.5, 8.2 };
		float room_dim[3] = { 9.5, 7.3, 11.4 };
	
		// allocate host ptr for room and ray in CPU
		Room* h_room_ptr = new Room(source, l_position, room_dim);
		Ray** h_all_rays_ptr = new Ray*[num_rays];


		if (h_room_ptr == NULL)
		{
			fprintf(stderr, "Failed to allocate host vectors!\n");
			exit(EXIT_FAILURE);
		}
		if (h_all_rays_ptr == NULL)
		{
			fprintf(stderr, "Failed to allocate host all_rays!\n");
			exit(EXIT_FAILURE);
		}

		// in order to create device array of pointers to device array, first

		// create host array of pointers to device arrays, and after that copy it

		// into device array of pointers to device arrays.
		for (int i = 0; i < num_rays; i++)
		{   

			Ray* h_ray = new Ray(source, all_rays_directions[i]);  // h_ray is the host array intermediate one
			cudaMalloc((void**)&h_all_rays_ptr[i], sizeof(Ray));
			cudaMemcpy(h_all_rays_ptr[i], h_ray, sizeof(Ray), cudaMemcpyHostToDevice); // copy it to h_all (device array of pointers)

		}

		
		size_t size_one_ray = sizeof(Ray);
		size_t roomsize = sizeof(Room);
		Ray** d_all_rays = NULL;
		Room* d_room_ptr = NULL;

		err_all_rays = cudaMalloc(&d_all_rays, sizeof(Ray*)*num_rays);
		err_room = cudaMalloc((void**)&d_room_ptr, roomsize);
	
		if (err_room != cudaSuccess)
		{
			fprintf(stderr, "Failed to allocate device memory for room(error code %s)!\n", cudaGetErrorString(err_room));
			exit(EXIT_FAILURE);
		}
		if (err_all_rays != cudaSuccess)
		{
			fprintf(stderr, "Failed to allocate device memory for all rays(error code %s)!\n", cudaGetErrorString(err_all_rays));
			exit(EXIT_FAILURE);
		}

		// Copy the host input ray and room in host memory to the device input ray and room in
		// device memory

		printf("Copy input data from the host memory to the CUDA device\n");
		err_all_rays = cudaMemcpy(d_all_rays, h_all_rays_ptr, num_rays*sizeof(Ray*), cudaMemcpyHostToDevice); // both h_ and d_ are device arrays of pointers
		err_room = cudaMemcpy(d_room_ptr, h_room_ptr, roomsize, cudaMemcpyHostToDevice);
		

		if (err_room != cudaSuccess)
		{
			fprintf(stderr, "Failed to room from host to device (error code %s)!\n", cudaGetErrorString(err_room));
			exit(EXIT_FAILURE);
		}

		
		// Launch the processAllRays function CUDA Kernel
		processAllRays << <1, num_rays>> >( d_all_rays, d_room_ptr);
		err_all_rays = cudaGetLastError();
		
		if (err_all_rays != cudaSuccess)
		{
			fprintf(stderr, "Failed to launch kernel (error code %s)!\n", cudaGetErrorString(err_all_rays));
			exit(EXIT_FAILURE);
		}

		// Copy the ray device in device memory to the host result ray
		// in host memory. Here do not need to copy room data back to CPU
		printf("Copy output data from the CUDA device to the host memory\n");
		// TODO: copy data in device back to host.
		//err_all_rays = cudaMemcpy(h_all_rays_ptr, d_all_rays, size, cudaMemcpyDeviceToHost);
		
		/*if (err_all_rays != cudaSuccess)
		{
			fprintf(stderr, "Failed to copy all ray info from device to host (error code %s)!\n", cudaGetErrorString(err_all_rays));
			exit(EXIT_FAILURE);
		}
*/

		// Free device global memory
		//err_ray = cudaFree(d_ray);
		err_room = cudaFree(d_room_ptr);
		err_all_rays = cudaFree(d_all_rays);
		cudaFree(h_all_rays_ptr);

		if (err_room != cudaSuccess)
		{
			fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err_room));
			exit(EXIT_FAILURE);
		}
		if (err_all_rays != cudaSuccess)
		{
			fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err_all_rays));
			exit(EXIT_FAILURE);
		}


		// Free host memory for ray and room
		
		free(h_room_ptr);
		//free(h_all_rays_ptr);

		//err_ray = cudaDeviceReset();
		err_room = cudaDeviceReset();
		err_all_rays = cudaDeviceReset();
		/*if (err_ray != cudaSuccess)
		{
			fprintf(stderr, "Failed to deinitialize the device for ray! error=%s\n", cudaGetErrorString(err_ray));
			exit(EXIT_FAILURE);
		}*/
		if (err_room != cudaSuccess)
		{
			fprintf(stderr, "Failed to deinitialize the device for room! error=%s\n", cudaGetErrorString(err_room));
			exit(EXIT_FAILURE);
		}
		if (err_all_rays != cudaSuccess)
		{
		fprintf(stderr, "Failed to deinitialize the device for ray! error=%s\n", cudaGetErrorString(err_all_rays));
		exit(EXIT_FAILURE);
		}

		printf("Done\n");
		return 0;
}; 
