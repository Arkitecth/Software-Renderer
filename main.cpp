#include "tgaimage.h"
#include <cmath>
#include <cstdint>
#include <utility>

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};


void drawLine(int ax, int ay, int bx, int by, TGAImage& buffer, TGAColor color) {
	bool steep = std::abs(ay - by) > std::abs(ax - bx); 
	if (steep) {
		std::swap(ax, ay); 
		std::swap(bx, by); 
	}
	if(ax > bx) {
		std::swap(ax, bx); 
		std::swap(ay, by); 
	}
	for(int x = ax; x <= bx; x++) {
		float t = (x - ax) / static_cast<float>(bx - ax);
		int y = std::round(ay + (by - ay) * t); 
		if (steep) {
			buffer.set(y, x, color);
		} else {
			buffer.set(x, y, color); 
		}
	}
}





int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    drawLine(ax, ay, bx, by, framebuffer, blue); 
    drawLine(cx, cy, bx, by, framebuffer, green); 
    drawLine(cx, cy, ax, ay, framebuffer, yellow); 
    drawLine(ax, ay, cx, cy, framebuffer, red); 


    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);

    std::srand(std::time({})); 
    for(int i = 0; i < 1 << 24; i++) {
	    int ax = rand() % width, ay = rand() % height;
	    int bx = rand() % width, by = rand() % height; 
	    drawLine(ax, ay, bx, by, framebuffer, {std::uint8_t(rand() % 255), std::uint8_t(rand() % 255), std::uint8_t(rand() % 255), std::uint8_t(rand() % 255) });
    }



    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
