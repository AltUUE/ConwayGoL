#include "Color.h"
#include <cstdio>

Color::Color(int red, int green, int blue){
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
}

Color Color::operator+(const Color& c){
	return Color(rgb[0] + c.rgb[0], rgb[1] + c.rgb[1], rgb[2] + c.rgb[2]);
}

Color Color::operator/(const int& a){
	return Color(rgb[0] / a, rgb[1] / a, rgb[2] / a);
}

Color Color::operator=(const Color& c){
	rgb[0] = c.rgb[0];
	rgb[1] = c.rgb[1];
	rgb[2] = c.rgb[2];
	return *this;
}

Color Color::operator+=(const Color& c){
	rgb[0] += c.rgb[0];
	rgb[1] += c.rgb[1];
	rgb[2] += c.rgb[2];
	return *this;
}

bool Color::operator==(const Color& c) const{
	return (rgb[0] == c.rgb[0] && rgb[1] == c.rgb[1] && rgb[2] == c.rgb[2]);
}

void Color::print() const{
	printf("0x%2x, 0x%2x, 0x%2x\n", rgb[0], rgb[1], rgb[2]);
}