#include "Utility.h"

void Utility::sheetUV(Vertex (&v)[6],int index,int divX,int divY) {
	if (divX <= 1 && divY <= 1) {
		v[0].setUv(0, 1);
		v[1].setUv(1, 0);
		v[2].setUv(0, 0);
		v[3].setUv(0, 1);
		v[4].setUv(1, 1);
		v[5].setUv(1, 0);
		return;
	}

	bool flipped = false;
	if (index < 0) {
		index *= -1;
		flipped = true;
	}

	float inc = 1.0f / divX;
	float UVx = 0, UVy = divX - 1.0f;

	while (index >(divX - 1)) {
		UVy -= 1;
		index -= divX;
	}

	UVx = (float)index / (float)divX;
	UVy /= divX;

	if (!flipped) {
		v[0].setUv(UVx + inc, UVy + inc);
		v[1].setUv(UVx, UVy + inc);
		v[2].setUv(UVx, UVy);
		v[3].setUv(UVx, UVy);
		v[4].setUv(UVx + inc, UVy);
		v[5].setUv(UVx + inc, UVy + inc);
	}
	else {
		v[0].setUv(UVx, UVy + inc);
		v[1].setUv(UVx + inc, UVy + inc);
		v[2].setUv(UVx + inc, UVy);
		v[3].setUv(UVx + inc, UVy);
		v[4].setUv(UVx, UVy);
		v[5].setUv(UVx, UVy + inc);
	}
}

glm::vec2 Utility::getOffsetOfUVIndex(int index,int divisions) {
	float x, y = (float)divisions - 1;
	while (index > (divisions - 1)) {
		y -= 1;
		index -= divisions;
	}

	x = (float)index / (float)divisions;
	y /= divisions;
	return glm::vec2(x,y);
}

void Utility::splitString(const String& s, char c, std::vector<String>& v) {
	/*string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length()));
	}*/
}
