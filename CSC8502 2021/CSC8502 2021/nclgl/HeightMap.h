#pragma  once
#include  <string>
#include "Mesh.h"
class  HeightMap : public  Mesh {
public:
    HeightMap(const  std::string& name);
    ~HeightMap(void) {};
    Vector3  GetHeightmapSize()  const { return  heightmapSize; }
protected:
    Vector3  heightmapSize;
	float height = 6.0f;
    float scale = 8.0;//16
    int smoothing = 2;
};