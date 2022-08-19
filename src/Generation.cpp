// #include "GoL.h"
#include "../inc/Generation.h"
#include "../inc/Color.h"

Generation::Generation(){

}

Generation::~Generation(){

}

void Generation::calculateNextGen(){
	std::set<std::pair<std::pair<int, int>, Color>, Cmp> nextgen;
	int x, y, n;
	std::set<std::pair<std::pair<int, int>, Color>, Cmp>::iterator itnew;
	Color color(0,0,0);
	for(std::set<std::pair<std::pair<int, int>, Color>, Cmp>::iterator it=generation.begin(); it!=generation.end(); it++){
		for(x=it->first.first-1; x<it->first.first+2; x++) for(y=it->first.second-1; y<it->first.second+2; y++){
			n=cnt({x, y});
			if(n==3){
				itnew = generation.find({{x, y}, Color(0x00, 0x00, 0x00)});
				if(itnew != generation.end()){
					nextgen.insert(*itnew);
				}
				else{
					color.rgb[0] = color.rgb[1] = color.rgb[2] = 0;
					itnew=generation.lower_bound({{x-1,y-1}, Color(0x00, 0x00, 0x00)});
					while(itnew!= generation.end() && itnew->first.first <= x+1 && itnew->first.second == y-1){
						color += itnew->second;
						itnew++;
					}
					itnew=generation.lower_bound({{x-1,y}, Color(0x00, 0x00, 0x00)});
					while(itnew!= generation.end() && itnew->first.first <= x+1 && itnew->first.second == y){
						color += itnew->second;
						itnew++;
					}
					itnew=generation.lower_bound({{x-1,y+1}, Color(0x00, 0x00, 0x00)});
					while(itnew!= generation.end() && itnew->first.first <= x+1 && itnew->first.second == y+1){
						color += itnew->second;
						itnew++;
					}
					nextgen.insert({{x, y}, color / 3});
				}
			}
			else if(n==4){
				itnew = generation.find({{x, y}, Color(0x00, 0x00, 0x00)});
				if(itnew != generation.end()){
					nextgen.insert(*itnew);
				}
			}
		}
	}
	generation = nextgen;
}

int Generation::cnt(std::pair<int,int> p){
	int ret=0, x=p.first, y=p.second;
	std::set<std::pair<std::pair<int, int>, Color>, Cmp>::iterator it;
	it=generation.lower_bound({{x-1,y-1}, Color(0x00, 0x00, 0x00)});
	while(it!= generation.end() && it->first.first <= x+1 && it->first.second == y-1){
		it++;
		ret++;
	}
	it=generation.lower_bound({{x-1,y}, Color(0x00, 0x00, 0x00)});
	while(it!= generation.end() && it->first.first <= x+1 && it->first.second == y){
		ret++;
		it++;
	}
	it=generation.lower_bound({{x-1,y+1}, Color(0x00, 0x00, 0x00)});
	while(it!= generation.end() && it->first.first <= x+1 && it->first.second == y+1){
		ret++;
		it++;
	}
	return ret;
}

bool Generation::Cmp::operator()(const std::pair<std::pair<int, int>, Color>& lhs, const std::pair<std::pair<int, int>, Color>& rhs){
	if(lhs.first.second < rhs.first.second) return true;
	if(lhs.first.second == rhs.first.second){
		if(lhs.first.first < rhs.first.first)
			return 1;
		return 0;
	}
	return 0;
}
