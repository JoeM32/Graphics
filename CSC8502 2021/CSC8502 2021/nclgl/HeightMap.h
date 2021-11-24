#pragma  once
#include  <string>
#include "Mesh.h"
class  HeightMap : public  Mesh {
public:
    HeightMap(const  std::string& name, float height = 6.0, float scale = 8.0);
    HeightMap(int widthx, int heightz, float height = 6.0, float scale = 8.0);
    ~HeightMap(void) {};
    Vector3  GetHeightmapSize()  const { return  heightmapSize; }
    std::vector<Vector3> rockSpots;
protected:
    Vector3  heightmapSize;
	float height = 6.0f;
    float scale = 8.0;//16
    int smoothing = 2;
};