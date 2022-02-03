// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIStructures.h"

class GAME_API GeometryGenerator abstract final
{
public:
    typedef std::vector<RHIVertex> VertexCollection;
    typedef std::vector<uint32> IndexCollection;

    static void ComputeBox(VertexCollection& vertices, IndexCollection& indices, const Vector3& size, bool rhcoords, bool invertn);
    static void ComputeSphere(VertexCollection& vertices, IndexCollection& indices, float diameter, size_t tessellation, bool rhcoords, bool invertn);
    static void ComputeGeoSphere(VertexCollection& vertices, IndexCollection& indices, float diameter, size_t tessellation, bool rhcoords);
    static void ComputeCylinder(VertexCollection& vertices, IndexCollection& indices, float height, float diameter, size_t tessellation, bool rhcoords);
    static void ComputeCone(VertexCollection& vertices, IndexCollection& indices, float diameter, float height, size_t tessellation, bool rhcoords);
    static void ComputeTorus(VertexCollection& vertices, IndexCollection& indices, float diameter, float thickness, size_t tessellation, bool rhcoords);
    static void ComputeTetrahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    static void ComputeOctahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    static void ComputeDodecahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    static void ComputeIcosahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    static void ComputeTeapot(VertexCollection& vertices, IndexCollection& indices, float size, size_t tessellation, bool rhcoords);
};