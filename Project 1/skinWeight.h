#ifndef SKINWEIGHT_H
#define SKINWEIGHT_H
class skinWeight
{
private:
	int index;
	float weight;
public:
	skinWeight();
	void setIndex(int i) { index = i; }
	void setWeight(float w) { weight = w; }
	int getIndex() { return index; }
	float getWeight() { return weight; }
	~skinWeight();
};
#endif
