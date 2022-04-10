#pragma once
#include <set>
#include "Color.h"

class Generation{
public:
	Generation();
	~Generation();

	struct Cmp{
		bool operator()(const std::pair<std::pair<int, int>, Color>&, const std::pair<std::pair<int, int>, Color>&);
	};

	void calculateNextGen();
	int cnt(std::pair<int,int> p);

	std::set<std::pair<std::pair<int, int>, Color>, Cmp> generation;
};