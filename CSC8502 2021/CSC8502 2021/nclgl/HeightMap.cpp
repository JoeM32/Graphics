#include "HeightMap.h"
HeightMap::HeightMap(const  std:: string& name, float height, float scale) {
    this->scale = scale;
    this->height = height;
    int  iWidth , iHeight , iChans;
    unsigned  char* data = SOIL_load_image(name.c_str(),
        &iWidth , &iHeight , &iChans , 1);
    if (!data) {
        //std::cout  << "Heightmap  can’t load  file!\n";
        return;
    }
    numVertices = iWidth * iHeight; 
    numIndices = (iWidth - 1) * (iHeight - 1) * 6; 
    vertices = new  Vector3[numVertices]; 
    textureCoords = new  Vector2[numVertices]; 
    indices = new  GLuint[numIndices];
    Vector3  vertexScale = Vector3(scale, height, scale);
    Vector2  textureScale = Vector2( (1/16.0f), (1 / 16.0f));
    for (int z = 0; z < iHeight; ++z) { 
        for (int x = 0; x < iWidth; ++x) { 

            int  offset = (z * iWidth) + x; //My blender imported PNG was bizzarely tiered so I have smoothed them out
            int total = 0;
            for (int i = 0; i < smoothing; i++)
            {
                for (int z1 = -1; z1 < 2; ++z1) {
                    for (int x1 = -1; x1 < 2; ++x1) {
                        if (x + x1 < 0 || z + z1 < 0 || x + x1 >= iWidth || z + z1 >= iHeight || (x1 == 0 && z1 == 0))
                        {
                            total += data[offset];
                        }
                        else
                        {
                            total += data[((z + z1) * iWidth) + (x + x1)];
                        }
                    }
                }
            }
            vertices[offset] = (Vector3(x, total / (9.0f * (float)smoothing), z) * vertexScale) -(Vector3(0, height * 100, 0));
            if (rand() % 1000 == 0)
            {
                rockSpots.emplace_back(vertices[offset]);
            }
            textureCoords[offset] = Vector2(x, z) * textureScale;
        }
    }
    SOIL_free_image_data(data);
    int i = 0; 
    for (int z = 0; z < iHeight - 1; ++z) { 
        for (int x = 0; x < iWidth - 1; ++x) { 
            int a = (z * (iWidth)) + x; 
            int b = (z * (iWidth)) + (x + 1); 
            int c = ((z + 1) * (iWidth)) + (x + 1); 
            int d = ((z + 1) * (iWidth)) + x; 
            indices[i++] = a; 
            indices[i++] = c; 
            indices[i++] = b; 
            indices[i++] = c; 
            indices[i++] = a; 
            indices[i++] = d; 
        }
    }
	GenerateNormals();
	GenerateTangents();
    BufferData(); 
    heightmapSize.x = vertexScale.x * (iWidth - 1); 
    heightmapSize.y = vertexScale.y * 255.0f;//each  height  is a byte!
    heightmapSize.z = vertexScale.z * (iHeight  - 1);
}

HeightMap::HeightMap(int widthx, int heightz, float height, float scale) {
    this->scale = scale;
    this->height = heightz;
    numVertices = widthx * heightz;
    numIndices = (widthx - 1) * (heightz - 1) * 6;
    vertices = new  Vector3[numVertices];
    textureCoords = new  Vector2[numVertices];
    indices = new  GLuint[numIndices];

    Vector3  vertexScale = Vector3(scale, heightz, scale);
    Vector2  textureScale = Vector2((1 / 70), (1 / 70));

    for (int z = 0; z < heightz; ++z) {
        for (int x = 0; x < widthx; ++x) {

            int  offset = (z * widthx) + x; 
            vertices[offset] = (Vector3(x, 0, z) * vertexScale) - ((Vector3(widthx/4, 0, heightz/4) * vertexScale));
            //textureCoords[offset] = Vector2(x, z) * textureScale;
            textureCoords[offset] = Vector2(vertices[offset].x, vertices[offset].z);
        }
    }
    int i = 0;
    for (int z = 0; z < heightz - 1; ++z) {
        for (int x = 0; x < widthx - 1; ++x) {
            int a = (z * (widthx)) + x;
            int b = (z * (widthx)) + (x + 1);
            int c = ((z + 1) * (widthx)) + (x + 1);
            int d = ((z + 1) * (widthx)) + x;
            indices[i++] = a;
            indices[i++] = c;
            indices[i++] = b;
            indices[i++] = c;
            indices[i++] = a;
            indices[i++] = d;
        }
    }
    GenerateNormals();
    GenerateTangents();
    BufferData();
    heightmapSize.x = vertexScale.x * (widthx - 1);
    heightmapSize.y = vertexScale.y * 255.0f;//each  height  is a byte!
    heightmapSize.z = vertexScale.z * (heightz - 1);
}