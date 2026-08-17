#include "tgaimage.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <utility>

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};
constexpr int width  = 128;
constexpr int height = 128;


std::vector<std::vector<float>> vertices{};
std::vector<std::vector<int>> faces{};


void parseFile(std::string_view filePath) 
{
	std::ifstream file{filePath.data()};
	if (!file.is_open()) 
	{
		std::cout << "An error occurred opening the file" << '\n';
	}
	std::string line{};
	while (std::getline(file, line)) 	
	{
		if (line.length() > 1) 
		{
			std::istringstream stream{line.substr(2)};

			if (line[0] == 'v' && line[1] == ' ') 
			{
				std::vector<float> row{};
				for(int i = 0; i < 3; i++)
				{
					std::string vertexString{};
					stream >> vertexString; 
					row.push_back(std::stof(vertexString)); 
				}
				vertices.push_back(row); 
			}

			else if (line[0] == 'f' && line[1] == ' ') 
			{
				std::vector<int> result{};
				std::string face{};
				for(int i = 2; i < line.length(); i++)
				{
					if (line[i] == '/' || line[i] == ' ') 
					{
						result.push_back(std::stoi(face));
						face = "";
						continue;
					} 
					face += line[i];
				}
				result.push_back(std::stoi(face)); 
				faces.push_back(result); 
			}
		}
	}
}

void drawLine(TGAImage& framebuffer, int ax, int ay, int bx, int by, TGAColor color)
{
	bool steep = std::abs(ax - bx) < std::abs(ay - by); 
	if (steep) 
	{
		std::swap(ax, ay); 
		std::swap(bx, by); 
	}
	if (ax > bx) 
	{
		std::swap(ax, bx); 
		std::swap(ay, by); 
	}
	for(float x = ax; x <= bx; x++)
	{
		float t = (x - ax) / static_cast<float>(bx - ax);
		int y = std::round(ay + (by - ay) * t); 
		if (steep) 
		{
			framebuffer.set(y, x, color); 
		} else {
			framebuffer.set(x, y, color); 
		}
	}
}




void renderModel(TGAImage& framebuffer)
{
	for(int i = 0; i < faces.size(); i++)
	{
		int index1 = faces[i][0];
		int index2 = faces[i][3];
		int index3 = faces[i][6];


		float ax = (vertices[index1 - 1][0] + 1.0f) * width / 2;
		float ay = (vertices[index1 - 1][1] + 1.0f) * width / 2;
		float az = vertices[index1 - 1][2];

		float bx = (vertices[index2 - 1][0] + 1.0f) * width / 2;
		float by = (vertices[index2 - 1][1] + 1.0f) * width / 2;
		float bz = vertices[index2 - 1][2];

		float cx = (vertices[index3 - 1][0] + 1.0f) * width / 2;
		float cy = (vertices[index3 - 1][1] + 1.0f) * width / 2;
		float cz = vertices[index3 - 1][2];

		drawLine(framebuffer, ax, ay, bx, by, red); 
		drawLine(framebuffer, bx, by, cx, cy, red); 
		drawLine(framebuffer, cx, cy, ax, ay, red); 
	}
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& framebuffer, TGAColor color)
{
	drawLine(framebuffer, ax, ay, bx, by, color); 
	drawLine(framebuffer, bx, by, cx, cy, color); 
	drawLine(framebuffer, cx, cy, ax, ay, color); 
}






int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);

    // int ax =  7, ay =  3;
    // int bx = 12, by = 37;
    // int cx = 62, cy = 53;
    //
    // drawLine(framebuffer, ax, ay, bx, by, blue); 
    // drawLine(framebuffer, cx, cy, bx, by, green); 
    // drawLine(framebuffer, cx, cy, ax, ay, yellow); 
    // drawLine(framebuffer, ax, ay, cx, cy, red); 
    //
    //
    // framebuffer.set(ax, ay, white);
    // framebuffer.set(bx, by, white);
    // framebuffer.set(cx, cy, white);
    //
    // framebuffer.write_tga_file("framebuffer.tga");
    //renderModel(framebuffer);
    //parseFile("./diablo3_pose.obj"); 

    triangle(7, 45, 35, 100, 45, 60, framebuffer, red); 

    triangle(120, 35, 90, 5, 45, 110, framebuffer, white); 

    triangle(115, 83, 80, 90, 85, 120, framebuffer, green); 

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}
