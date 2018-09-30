#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include <iostream>
#include <string>
#include <fstream>
#include "Vectors.h"
#include "Ray.h"
#include "Renderable.h"
#include "Camera.h"
#include "Material.h"
#include "consoleUtilities.h"

#include <random>
#include <chrono>
using namespace RayMath;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::steady_clock> Timer;


struct Pixel
{
	Pixel() :r(0), g(0), b(0) {}
	Pixel(uint8_t red, uint8_t green, uint8_t blue) :r(red), g(green), b(blue) {}


	uint8_t r, g, b;
};

//float RandomRange()
//{
//	return (((float)rand() / (RAND_MAX)));
//}
//
//vec3 random_in_unit_sphere()
//{
//	vec3 p;
//	do {
//		float bla = RandomRange();
//		p = 2.0 * vec3(RandomRange(), RandomRange(), RandomRange()) - vec3(1, 1, 1);
//	} while (p.squared_length() >= 1.0);
//	return p;
//}



vec3 createImage(const ray& r, Renderable* world, int depth)
{

	hit_record rec;
	if (world->Render(r, 0.001, std::numeric_limits<float>::max(), rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * createImage(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.GetDirection());
		float t = 0.5 * (unit_direction.y + 1.0f);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}

}


RenderList* ConstructRandomScene()
{
	int n = 500;
	Renderable **list = new Renderable*[n + 1];
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			float choose_mat = RandomRange();
			vec3 center(a + 0.9*RandomRange(), 0.2, b + 0.9*RandomRange());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8) //diffuse
					list[i++] = new Sphere(center, 0.2, new Lambertian(vec3(RandomRange()*RandomRange(), RandomRange()*RandomRange(), RandomRange()*RandomRange())));

				else if (choose_mat < 0.95) //metal
					list[i++] = new Sphere(center, 0.2, new Metal(vec3(0.5*(1+ RandomRange()), 0.5*(1 + RandomRange()), 0.5*(1 + RandomRange())), 0.5 * RandomRange()));

				else  //glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
			}
		}
	}

	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));

	return new RenderList(list, i);
}


bool PollTimeMilliseconds(Timer& start, Timer& end, double& duration, const float pollFriequencyMilliseconds)
{
	end = Clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	if (time >= pollFriequencyMilliseconds)
	{
		duration = time;
		start = Clock::now();
		return true;
	}
	return false;
}

void DummyDrawToImage(std::string filePath, Renderable* world, Camera& cam, const int nx, const int ny, const int ns)
{

	double timeDuration = 0.0;
	float estimateInSeconds = 0.0f;
	int completedPixels = 0;

	Timer start;
	Timer end;

	Pixel* Data = new Pixel[ny * nx]();

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	//Renderable* list[5];
	//list[0] = new Sphere(vec3(0, 0, -1.0f), 0.5f, new Lambertian(vec3(0.8f, 0.3f, 0.3f)));
	//list[1] = new Sphere(vec3(0, -100.5f, -1.0f), 100.f, new Lambertian(vec3(0.8f, 0.8f, 0.0f)));
	//list[2] = new Sphere(vec3(1.0f, 0, -1.0f), 0.5f, new Metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	//
	//list[3] = new Sphere(vec3(-1.0f, 0, -1.0f), 0.5f, new Dielectric(1.5f));
	//list[4] = new Sphere(vec3(-1.0f, 0, -1.0f), -0.45f, new Dielectric(1.5f));

	//Renderable* world = new RenderList(list, 5);

	//camera settings


	for (int j = 0; j < ny; ++j)
	{

		for (int i = 0; i < nx; i++)
		{


			vec3 color(0, 0, 0);

			for (int s = 0; s < ns; ++s)
			{
				float u = float(i + (((float)rand() / (RAND_MAX)) + 1)) / float(nx);
				float v = float(j + (((float)rand() / (RAND_MAX)) + 1)) / float(ny);

				ray r = cam.GetRay(u, v);
				vec3 p = r.PointAtParimeter(2.0);
				color += createImage(r, world, 0);
			}



			//ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			color /= float(ns);
			color = vec3(sqrtf(color[0]), sqrtf(color[1]), sqrtf(color[2]));
			int ir = int(255.99*color[0]);
			int ig = int(255.99*color[1]);
			int ib = int(255.99*color[2]);

			Data[j*nx + i] = Pixel(ir, ig, ib);


			if (PollTimeMilliseconds(start, end, timeDuration, 3000.0))
			{
				int diff = (j*nx + i) - completedPixels;
				float pixelsPerSecond = float(diff) / (timeDuration * 100.0);
				estimateInSeconds = pixelsPerSecond * (float(ny*nx) - float(j*nx + i));
				completedPixels = j * nx + i;

				float progress = (float(j*nx + i) / float(ny*nx)) * 100.0f;
				DrawProgressUpdate(progress, estimateInSeconds);
			}


		}


		
	}
	
	stbi_write_bmp(filePath.c_str(), nx, ny, 3, Data);
}

int main()
{
	stbi_flip_vertically_on_write(1);
	const int nx = 200; //resolution width
	const int ny = 200; //resolution height
	const int ns = 200;  //samples per pixel



	vec3 lookFrom(-13, 2, -3);
	vec3 lookAt(0, 0, 0);
	float dist_to_focus = 10.0;//(lookFrom - lookAt).length();
	float aperture = 0.1f;
	Camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

	Renderable* world = ConstructRandomScene();
	DummyDrawToImage("./Images/dummyImage.bmp", world, cam, nx, ny, ns);
	return 0;
}