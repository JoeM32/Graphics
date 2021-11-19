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
	float height = 45;
	float scale = 16.0f; //16
};