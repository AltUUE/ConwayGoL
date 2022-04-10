#pragma once

class Color{
public:
	int rgb[3];

	Color(int red, int green, int blue);
	
	Color operator+(const Color& c);
	Color operator/(const int& a);
	Color operator=(const Color& c);
	Color operator+=(const Color& c);
	bool operator==(const Color& c) const;

	void print() const;
};