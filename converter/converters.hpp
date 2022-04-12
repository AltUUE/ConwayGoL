void characterCheck(std::string& s, std::ofstream& out, int& extra){
	if(s.size() + extra > 70){
		s.insert(70 - extra, "\n");
		extra = s.size() - 70;
		out << s;
		s = "";
	}
}

void squareCheck(std::string& s, std::vector<std::pair<int, int> >& v, int& cnt, int& index, int& xcur){
	while(v[index+1].first == v[index].first + 1 && v[index+1].second == v[index].second){
		cnt++; index++; xcur++;
	}
	cnt == 0 ? s += 'o' : s += std::to_string(cnt + 1) + 'o';
}

void ptxtToVan(std::ifstream& inp, std::ofstream& out){
	out << "rule: vanilla\n";
	std::string s;
	int x = 0, y = 0;
	while(inp >> s){
		for(int x = 0; x < s.length(); x++) if(s[x] == 'O') out << x << ' ' << y << ' ';
		y++;
	}
}

void vanToPtxt(std::ifstream& inp, std::ofstream& out){
	std::string s;
	std::getline(inp, s);
	int x, y;
	inp >> x >> y;
	int xmin = x, ymin = y;
	std::vector<std::pair<int, int> > v;
	v.push_back({x, y});
	while(inp >> x){
		inp >> y;
		if(x < xmin) xmin = x;
		if(y < ymin) ymin = y;
		v.push_back({x, y});
	}

	for (int i = 0; i < v.size(); i++)
	{
		v[i].first -= xmin; v[i].second -= ymin;
	}

	std::sort(v.begin(), v.end(), [](const std::pair<int, int> p1, const std::pair<int, int> p2){
		if(p1.second == p2.second) return p1.first < p2.first;
		return p1.second < p2.second;
	});

	int xcur, ycur;
	int xb = -1, yb = 0;
	// (1,3) (5,5)
	
	for (std::pair<int, int> item : v)
	{
		xcur = item.first; ycur = item.second;
		// std::cout << xcur << " " << ycur << "\n";
		if(ycur - yb > 1) out << "\n";
		for(int i = 0; i < ycur - yb - 1; i++) out << ".\n";
		if(ycur == yb){
			for(int k = 0; k < xcur - xb - 1; k++) out << ".";
				out << "O";
		}
		else{
			out << "\n";
			for(int k = 0; k < xcur; k++) out << ".";
			out << "O";
		}
		xb = xcur; yb = ycur;
	}
}

void vanToRle(std::ifstream& inp, std::ofstream& out){
	std::string s;
	std::getline(inp, s);
	int x, y;
	inp >> x >> y;
	int xmin = x, ymin = y, xmax = x, ymax = y;
	std::vector<std::pair<int, int> > v;
	v.push_back({x, y});
	while(inp >> x){
		inp >> y;
		if(x < xmin) xmin = x; if(y < ymin) ymin = y; if(x > xmax) xmax = x; if(y > ymax) ymax = y;
		v.push_back({x, y});
	}
	xmax -= xmin; ymax -= ymin;

	for (int i = 0; i < v.size(); i++){
		v[i].first -= xmin; v[i].second -= ymin;
	}

	std::sort(v.begin(), v.end(), [](const std::pair<int, int> p1, const std::pair<int, int> p2){
		if(p1.second == p2.second) return p1.first < p2.first;
		return p1.second < p2.second;
	});

	out << "x = " << xmax + 1 << ", y = " << ymax + 1 << ", rule = b3/s23\n";
	std::string scur;
	// (5, 0), (3, 3), (5, 3), (0, 5)

	int extra = 0, cnt = 0;
	int xcur, ycur;
	int xb = -1, yb = 0;
	for(int i = 0; i < v.size();){
		xcur = v[i].first; ycur = v[i].second;
		if(ycur - yb > 0){
			ycur - yb == 1 ? scur += '$' : scur += std::to_string(ycur - yb) + '$';
			characterCheck(scur, out, extra);
			if(xcur == 0) squareCheck(scur, v, cnt, i, xcur);
			else{
				xcur == 1 ? scur += 'b' : scur += std::to_string(xcur) + 'b';
				squareCheck(scur, v, cnt, i, xcur);
			}
			characterCheck(scur, out, extra);
		}
		else{
			(xcur - xb - 1) == 1 ? scur += 'b' : scur += std::to_string(xcur - xb - 1) + 'b';
			squareCheck(scur, v, cnt, i, xcur);
			characterCheck(scur, out, extra);
		}
		characterCheck(scur, out, extra);
		cnt = 0;
		xb = xcur; yb = ycur;
		i++;
	}
	scur += "!";
	if(scur.size() > 70) characterCheck(scur, out, extra);
	else out << scur;
}

void rleToVan(std::ifstream& inp, std::ofstream& out){
	// read entire file
	std::stringstream strStream;
	strStream << inp.rdbuf();
	std::string s = strStream.str();

	// input part
	std::regex_token_iterator<std::string::iterator> rend;
	std::regex regexpr("[#x].+");
	s = std::regex_replace(s, regexpr, "");
	regexpr = "\\n";
	s = std::regex_replace(s, regexpr, "");
	regexpr = "(?=[0-9]*b*[0-9]*o*[0-9]*).+?[$!]";
	// std::cout << "entire matches:";
	std::regex_token_iterator<std::string::iterator> it(s.begin(), s.end(), regexpr);
	// while (it!=rend) std::cout << " [" << *it++ << "]";
	// std::cout << std::endl;

	// process input
	out << "rule: vanilla\n";
	int xnum = 0, ynum = 0;
	while(it!=rend){
		std::string item = *it++;
		std::regex regexpr2("[0-9]*[bo\\$]");
		std::regex_token_iterator<std::string::iterator> it2(item.begin(), item.end(), regexpr2);
		while(it2 != rend){
			std::string element = *it2++;
			int len = element.length();
			if(element[len-1] == 'b'){
				element.pop_back();
				(len - 1) ? xnum += stoi(element) : xnum++;
			}
			if(element[len-1] == 'o'){
				element.pop_back();
				if(len > 1) for(int i = 0; i < stoi(element); i++, xnum++) out << xnum << " " << ynum << " ";
				else{
					out << xnum << " " << ynum << " ";
					xnum++;
				}
			}
			if(element[len-1] == '$'){
				element.pop_back();
				(len - 1) ? ynum += stoi(element) : ynum++;
			}
		}
		xnum = 0;
	}
}

void mulToVan(std::ifstream& inp, std::ofstream& out){
	std::string s;
	std::getline(inp, s);
	out << "rule: vanilla\n";

	int p1, p2, c1, c2, c3;
	while(inp >> p1){
		inp >> p2 >> c1 >> c2 >> c3;
		out << p1 << " " << p2 << " ";
	}
}
