// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/GeometryGenerator.h"
#include "Misc/Bezier.h"
#include <numbers>

namespace GeometryGeneratorInternal
{
    constexpr float SQRT2 = 1.41421356237309504880f;
    constexpr float SQRT3 = 1.73205080756887729352f;
    constexpr float SQRT6 = 2.44948974278317809820f;

    constexpr float HalfPI = std::numbers::pi_v<float> * 0.5f;
    constexpr float PIx2 = std::numbers::pi_v<float> * 2.0f;

    // Collection types used when generating the geometry.
    inline void index_push_back(GeometryGenerator::IndexCollection& indices, size_t value)
    {
        indices.push_back(static_cast<uint32>(value));
    }


    // Helper for flipping winding of geometric primitives for LH vs. RH coords
    inline void ReverseWinding(GeometryGenerator::IndexCollection& indices, GeometryGenerator::VertexCollection& vertices)
    {
        for (auto it = indices.begin(); it != indices.end(); it += 3)
        {
            std::swap(*it, *(it + 2));
        }

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            it->TexCoord.X = (1.f - it->TexCoord.X);
        }
    }


    // Helper for inverting normals of geometric primitives for 'inside' vs. 'outside' viewing
    inline void InvertNormals(GeometryGenerator::VertexCollection& vertices)
    {
        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            it->Normal.X = -it->Normal.X;
            it->Normal.Y = -it->Normal.Y;
            it->Normal.Z = -it->Normal.Z;
        }
    }

    Vector3 IdentityR0 = Vector3(1, 0, 0);
    Vector3 IdentityR1 = Vector3(0, 1, 0);
    Vector3 IdentityR2 = Vector3(0, 0, 1);
}


//--------------------------------------------------------------------------------------
// Cube (aka a Hexahedron) or Box
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeBox(VertexCollection& vertices, IndexCollection& indices, const Vector3& size, bool rhcoords, bool invertn)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    // A box has six faces, each one pointing in a different direction.
    const int FaceCount = 6;

    static const Vector3 faceNormals[FaceCount] =
    {
        {  0,  0,  1 },
        {  0,  0, -1 },
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
    };

    static const Vector2 textureCoordinates[4] =
    {
        { 1, 0 },
        { 1, 1 },
        { 0, 1 },
        { 0, 0 },
    };

    Vector3 tsize = size;
    tsize = size / 2.0f;

    // Create each face in turn.
    for (int i = 0; i < FaceCount; i++)
    {
        Vector3 Normal = faceNormals[i];

        // Get two vectors perpendicular both to the face Normal and to each other.
        Vector3 basis = (i >= 4) ? IdentityR2 : IdentityR1;

        Vector3 side1 = Normal ^ basis;
        Vector3 side2 = Normal ^ side1;

        // Six indices (two triangles) per face.
        size_t vbase = vertices.size();
        index_push_back(indices, vbase + 0);
        index_push_back(indices, vbase + 1);
        index_push_back(indices, vbase + 2);

        index_push_back(indices, vbase + 0);
        index_push_back(indices, vbase + 2);
        index_push_back(indices, vbase + 3);

        // Four vertices per face.
        // (Normal - side1 - side2) * tsize // Normal // t0
        vertices.emplace_back((((Normal - side1) - side2) * tsize), Normal, NamedColors::White, textureCoordinates[0]);

        // (Normal - side1 + side2) * tsize // Normal // t1
        vertices.emplace_back((((Normal - side1) + side2) * tsize), Normal, NamedColors::White, textureCoordinates[1]);

        // (Normal + side1 + side2) * tsize // Normal // t2
        vertices.emplace_back(((Normal + (side1 + side2)) * tsize), Normal, NamedColors::White, textureCoordinates[2]);

        // (Normal + side1 - side2) * tsize // Normal // t3
        vertices.emplace_back((((Normal + side1) - side2) * tsize), Normal, NamedColors::White, textureCoordinates[3]);
    }

    // Build RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);

    if (invertn)
        InvertNormals(vertices);
}


//--------------------------------------------------------------------------------------
// Sphere
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeSphere(VertexCollection& vertices, IndexCollection& indices, float diameter, size_t tessellation, bool rhcoords, bool invertn)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    size_t verticalSegments = tessellation;
    size_t horizontalSegments = tessellation * 2;

    float radius = diameter / 2;

    // Create rings of vertices at progressively higher latitudes.
    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - float(i) / verticalSegments;

        float latitude = (i * std::numbers::pi_v<float> / verticalSegments) - HalfPI;
        float dy, dxz;

        MathEx::SinCos(latitude, dy, dxz);

        // Create a single ring of vertices at this latitude.
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = float(j) / horizontalSegments;

            float longitude = j * PIx2 / horizontalSegments;
            float dx, dz;

            MathEx::SinCos(longitude, dx, dz);

            dx *= dxz;
            dz *= dxz;

            Vector3 Normal = Vector3(dx, dy, dz);
            Vector2 Tex = Vector2(u, v);

            vertices.emplace_back((Normal * radius), Normal, NamedColors::White, Tex);
        }
    }

    // Fill the index buffer with triangles joining each pair of latitude rings.
    size_t stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            size_t nextI = i + 1;
            size_t nextJ = (j + 1) % stride;

            index_push_back(indices, i * stride + j);
            index_push_back(indices, nextI * stride + j);
            index_push_back(indices, i * stride + nextJ);

            index_push_back(indices, i * stride + nextJ);
            index_push_back(indices, nextI * stride + j);
            index_push_back(indices, nextI * stride + nextJ);
        }
    }

    // Build RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);

    if (invertn)
        InvertNormals(vertices);
}


//--------------------------------------------------------------------------------------
// Geodesic sphere
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeGeoSphere(VertexCollection& vertices, IndexCollection& indices, float diameter, size_t tessellation, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    // An undirected edge between two vertices, represented by a pair of indexes into a vertex array.
    // Becuse this edge is undirected, (a,b) is the same as (b,a).
    typedef std::pair<uint32, uint32> UndirectedEdge;

    // Makes an undirected edge. Rather than overloading comparison operators to give us the (a,b)==(b,a) property,
    // we'll just ensure that the larger of the two goes first. This'll simplify things greatly.
    auto makeUndirectedEdge = [](uint32 a, uint32 b)
    {
        return std::make_pair(std::max(a, b), std::min(a, b));
    };

    // Key: an edge
    // Value: the index of the vertex which lies midway between the two vertices pointed to by the key value
    // This map is used to avoid duplicating vertices when subdividing triangles along edges.
    typedef std::map<UndirectedEdge, uint32> EdgeSubdivisionMap;


    static const Vector3 OctahedronVertices[] =
    {
        // when looking down the negative z-axis (into the screen)
        Vector3(0,  1,  0), // 0 top
        Vector3(0,  0, -1), // 1 front
        Vector3(1,  0,  0), // 2 right
        Vector3(0,  0,  1), // 3 back
        Vector3(-1,  0,  0), // 4 left
        Vector3(0, -1,  0), // 5 bottom
    };
    static const uint32 OctahedronIndices[] =
    {
        0, 1, 2, // top front-right face
        0, 2, 3, // top back-right face
        0, 3, 4, // top back-left face
        0, 4, 1, // top front-left face
        5, 1, 4, // bottom front-left face
        5, 4, 3, // bottom back-left face
        5, 3, 2, // bottom back-right face
        5, 2, 1, // bottom front-right face
    };

    const float radius = diameter / 2.0f;

    // Start with an octahedron; copy the data into the vertex/index collection.

    std::vector<Vector3> vertexPositions(std::begin(OctahedronVertices), std::end(OctahedronVertices));

    indices.insert(indices.begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));

    // We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
    // about to go on, these values aren't ever going to change because the vertices don't move around in the array.
    // We'll need these values later on to fix the singularities that show up at the poles.
    const uint32 northPoleIndex = 0;
    const uint32 southPoleIndex = 5;

    for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision)
    {
        checkf(indices.size() % 3 == 0, L"Internal logic error."); // sanity

        // We use this to keep track of which edges have already been subdivided.
        EdgeSubdivisionMap subdividedEdges;

        // The new index collection after subdivision.
        IndexCollection newIndices;

        const size_t triangleCount = indices.size() / 3;
        for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
        {
            // For each edge on this triangle, create a new vertex in the middle of that edge.
            // The winding order of the triangles we output are the same as the winding order of the inputs.

            // Indices of the vertices making up this triangle
            uint32 iv0 = indices[iTriangle * 3 + 0];
            uint32 iv1 = indices[iTriangle * 3 + 1];
            uint32 iv2 = indices[iTriangle * 3 + 2];

            // Get the new vertices
            Vector3 v01; // vertex on the midpoint of v0 and v1
            Vector3 v12; // ditto v1 and v2
            Vector3 v20; // ditto v2 and v0
            uint32 iv01; // index of v01
            uint32 iv12; // index of v12
            uint32 iv20; // index of v20

            // Function that, when given the index of two vertices, creates a new vertex at the midpoint of those vertices.
            auto divideEdge = [&](uint32 i0, uint32 i1, Vector3& outVertex, uint32& outIndex)
            {
                const UndirectedEdge edge = makeUndirectedEdge(i0, i1);

                // Check to see if we've already generated this vertex
                auto it = subdividedEdges.find(edge);
                if (it != subdividedEdges.end())
                {
                    // We've already generated this vertex before
                    outIndex = it->second; // the index of this vertex
                    outVertex = vertexPositions[outIndex]; // and the vertex itself
                }
                else
                {
                    // Haven't generated this vertex before: so add it now

                    // outVertex = (vertices[i0] + vertices[i1]) / 2
                    outVertex = (vertexPositions[i0] + vertexPositions[i1]) * 0.5f;

                    outIndex = static_cast<uint32>(vertexPositions.size());
                    vertexPositions.push_back(outVertex);

                    // Now add it to the map.
                    auto entry = std::make_pair(edge, outIndex);
                    subdividedEdges.insert(entry);
                }
            };

            // Add/get new vertices and their indices
            divideEdge(iv0, iv1, v01, iv01);
            divideEdge(iv1, iv2, v12, iv12);
            divideEdge(iv0, iv2, v20, iv20);

            // Add the new indices. We have four new triangles from our original one:
            //        v0
            //        o
            //       /a\
            //  v20 o---o v01
            //     /b\c/d\
            // v2 o---o---o v1
            //       v12
            const uint32 indicesToAdd[] =
            {
                 iv0, iv01, iv20, // a
                iv20, iv12,  iv2, // b
                iv20, iv01, iv12, // c
                iv01,  iv1, iv12, // d
            };
            newIndices.insert(newIndices.end(), std::begin(indicesToAdd), std::end(indicesToAdd));
        }

        indices = std::move(newIndices);
    }

    // Now that we've completed subdivision, fill in the final vertex collection
    vertices.reserve(vertexPositions.size());
    for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it)
    {
        auto vertexValue = *it;

        auto Normal = Vector<>::Normalize(vertexValue);
        auto pos = Normal * radius;

        Vector3 normalFloat3 = Normal;

        // calculate texture coordinates for this vertex
        float longitude = atan2(normalFloat3.X, -normalFloat3.Z);
        float latitude = acos(normalFloat3.Y);

        float u = longitude / PIx2 + 0.5f;
        float v = latitude / std::numbers::pi_v<float>;

        auto texcoord = Vector2(1.0f - u, v);
        vertices.emplace_back(pos, Normal, NamedColors::White, texcoord);
    }

    // There are a couple of fixes to do. One is a texture coordinate wraparound fixup. At some point, there will be
    // a set of triangles somewhere in the mesh with texture coordinates such that the wraparound across 0.0/1.0
    // occurs across that triangle. Eg. when the left hand side of the triangle has a U coordinate of 0.98 and the
    // right hand side has a U coordinate of 0.0. The intent is that such a triangle should render with a U of 0.98 to
    // 1.0, not 0.98 to 0.0. If we don't do this fixup, there will be a visible seam across one side of the sphere.
    // 
    // Luckily this is relatively easy to fix. There is a straight edge which runs down the prime meridian of the
    // completed sphere. If you imagine the vertices along that edge, they circumscribe a semicircular arc starting at
    // y=1 and ending at y=-1, and sweeping across the range of z=0 to z=1. x stays zero. It's along this edge that we
    // need to duplicate our vertices - and provide the correct texture coordinates.
    size_t preFixupVertexCount = vertices.size();
    for (size_t i = 0; i < preFixupVertexCount; ++i)
    {
        // This vertex is on the prime meridian if Pos.x and texcoord.u are both zero (allowing for small epsilon).
        bool isOnPrimeMeridian = Vector2(vertices[i].Position.X, vertices[i].TexCoord.X).NearlyEquals(Vector2::Zero(), MathEx::SmallNumber);

        if (isOnPrimeMeridian)
        {
            size_t newIndex = vertices.size(); // the index of this vertex that we're about to add

            // copy this vertex, correct the texture coordinate, and add the vertex
            RHIVertex v = vertices[i];
            v.TexCoord.X = 1.0f;
            vertices.emplace_back(v);

            // Now find all the triangles which contain this vertex and update them if necessary
            for (size_t j = 0; j < indices.size(); j += 3)
            {
                uint32* triIndex0 = &indices[j + 0];
                uint32* triIndex1 = &indices[j + 1];
                uint32* triIndex2 = &indices[j + 2];

                if (*triIndex0 == i)
                {
                    // nothing; just keep going
                }
                else if (*triIndex1 == i)
                {
                    std::swap(triIndex0, triIndex1); // swap the pointers (not the values)
                }
                else if (*triIndex2 == i)
                {
                    std::swap(triIndex0, triIndex2); // swap the pointers (not the values)
                }
                else
                {
                    // this triangle doesn't use the vertex we're interested in
                    continue;
                }

                // If we got to this point then triIndex0 is the pointer to the index to the vertex we're looking at
                checkf(*triIndex0 == i, L"Internal logic error.");
                checkf(*triIndex1 != i && *triIndex2 != i, L"assume no degenerate triangles");

                const RHIVertex& v0 = vertices[*triIndex0];
                const RHIVertex& v1 = vertices[*triIndex1];
                const RHIVertex& v2 = vertices[*triIndex2];

                // check the other two vertices to see if we might need to fix this triangle

                if (abs(v0.TexCoord.X - v1.TexCoord.X) > 0.5f ||
                    abs(v0.TexCoord.X - v2.TexCoord.X) > 0.5f)
                {
                    // yep; replace the specified index to point to the new, corrected vertex
                    *triIndex0 = static_cast<uint32>(newIndex);
                }
            }
        }
    }

    // And one last fix we need to do: the poles. A common use-case of a sphere mesh is to map a rectangular texture onto
    // it. If that happens, then the poles become singularities which map the entire top and bottom rows of the texture
    // onto a single point. In general there's no real way to do that right. But to match the behavior of non-geodesic
    // spheres, we need to duplicate the pole vertex for every triangle that uses it. This will introduce seams near the
    // poles, but reduce stretching.
    auto fixPole = [&](size_t poleIndex)
    {
        auto poleVertex = vertices[poleIndex];
        bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            // These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
            // entry in the index array which represents the pole index, and the other two pointers point to the other
            // two indices making up this triangle.
            uint32* pPoleIndex;
            uint32* pOtherIndex0;
            uint32* pOtherIndex1;
            if (indices[i + 0] == poleIndex)
            {
                pPoleIndex = &indices[i + 0];
                pOtherIndex0 = &indices[i + 1];
                pOtherIndex1 = &indices[i + 2];
            }
            else if (indices[i + 1] == poleIndex)
            {
                pPoleIndex = &indices[i + 1];
                pOtherIndex0 = &indices[i + 2];
                pOtherIndex1 = &indices[i + 0];
            }
            else if (indices[i + 2] == poleIndex)
            {
                pPoleIndex = &indices[i + 2];
                pOtherIndex0 = &indices[i + 0];
                pOtherIndex1 = &indices[i + 1];
            }
            else
            {
                continue;
            }

            const auto& otherVertex0 = vertices[*pOtherIndex0];
            const auto& otherVertex1 = vertices[*pOtherIndex1];

            // Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
            RHIVertex newPoleVertex = poleVertex;
            newPoleVertex.TexCoord.X = (otherVertex0.TexCoord.X + otherVertex1.TexCoord.X) / 2;
            newPoleVertex.TexCoord.Y = poleVertex.TexCoord.Y;

            if (!overwrittenPoleVertex)
            {
                vertices[poleIndex] = newPoleVertex;
                overwrittenPoleVertex = true;
            }
            else
            {
                *pPoleIndex = static_cast<uint32>(vertices.size());
                vertices.push_back(newPoleVertex);
            }
        }
    };

    fixPole(northPoleIndex);
    fixPole(southPoleIndex);

    // Build RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);
}


//--------------------------------------------------------------------------------------
// Cylinder / Cone
//--------------------------------------------------------------------------------------
namespace GeometryGeneratorInternal
{
    // Helper computes a point on a unit circle, aligned to the x/z plane and centered on the origin.
    inline Vector3 GetCircleVector(size_t i, size_t tessellation)
    {
        float angle = i * PIx2 / tessellation;
        float dx, dz;

        MathEx::SinCos(angle, dx, dz);

        Vector3 v = { dx, 0, dz };
        return v;
    }

    inline Vector3 GetCircleTangent(size_t i, size_t tessellation)
    {
        float angle = (i * PIx2 / tessellation) + HalfPI;
        float dx, dz;

        MathEx::SinCos(angle, dx, dz);

        Vector3 v = { dx, 0, dz };
        return v;
    }


    // Helper creates a triangle fan to close the end of a cylinder / cone
    void CreateCylinderCap(GeometryGenerator::VertexCollection& vertices, GeometryGenerator::IndexCollection& indices, size_t tessellation, float height, float radius, bool isTop)
    {
        // Create cap indices.
        for (size_t i = 0; i < tessellation - 2; i++)
        {
            size_t i1 = (i + 1) % tessellation;
            size_t i2 = (i + 2) % tessellation;

            if (isTop)
            {
                std::swap(i1, i2);
            }

            size_t vbase = vertices.size();
            index_push_back(indices, vbase);
            index_push_back(indices, vbase + i1);
            index_push_back(indices, vbase + i2);
        }

        // Which end of the cylinder is this?
        Vector3 Normal = IdentityR1;
        Vector2 textureScale = -0.5f;

        if (!isTop)
        {
            Normal = -Normal;
            textureScale = textureScale * Vector2(-1.0f, 1.0f);
        }

        // Create cap vertices.
        for (size_t i = 0; i < tessellation; i++)
        {
            Vector3 circleVector = GetCircleVector(i, tessellation);
            Vector3 Pos = (circleVector * radius) + (Normal * height);
            Vector2 Tex = Vector<>::Swizzling<0, 2>(circleVector) * textureScale + Vector2(0.5f);

            vertices.emplace_back(Pos, Normal, NamedColors::White, Tex);
        }
    }
}

void GeometryGenerator::ComputeCylinder(VertexCollection& vertices, IndexCollection& indices, float height, float diameter, size_t tessellation, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    checkf(tessellation >= 3, L"tesselation parameter out of range");

    height /= 2;

    Vector3 topOffset = IdentityR1 * height;

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    // Create a ring of triangles around the outside of the cylinder.
    for (size_t i = 0; i <= tessellation; i++)
    {
        Vector3 Normal = GetCircleVector(i, tessellation);
        Vector3 sideOffset = Normal * radius;
        float u = float(i) / tessellation;
        Vector2 Tex = u;

        vertices.emplace_back(sideOffset + topOffset, Normal, NamedColors::White, Tex);
        vertices.emplace_back(sideOffset - topOffset, Normal, NamedColors::White, Tex + Vector2(0, 1.0f));

        index_push_back(indices, i * 2);
        index_push_back(indices, (i * 2 + 2) % (stride * 2));
        index_push_back(indices, i * 2 + 1);

        index_push_back(indices, i * 2 + 1);
        index_push_back(indices, (i * 2 + 2) % (stride * 2));
        index_push_back(indices, (i * 2 + 3) % (stride * 2));
    }

    // Create flat triangle fan caps to seal the top and bottom.
    CreateCylinderCap(vertices, indices, tessellation, height, radius, true);
    CreateCylinderCap(vertices, indices, tessellation, height, radius, false);

    // Build RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);
}


// Creates a cone primitive.
void GeometryGenerator::ComputeCone(VertexCollection& vertices, IndexCollection& indices, float diameter, float height, size_t tessellation, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    height /= 2;

    Vector3 topOffset = IdentityR1 * height;

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    // Create a ring of triangles around the outside of the cone.
    for (size_t i = 0; i <= tessellation; i++)
    {
        Vector3 circlevec = GetCircleVector(i, tessellation);
        Vector3 sideOffset = circlevec * radius;
        float u = float(i) / tessellation;
        Vector2 Tex = u;
        Vector3 pt = sideOffset - topOffset;
        Vector3 Normal = GetCircleTangent(i, tessellation) ^ (topOffset - pt);
        Normal = Vector<>::Normalize(Normal);

        // Duplicate the top vertex for distinct normals
        vertices.emplace_back(topOffset, Normal, NamedColors::White, Vector2::Zero());
        vertices.emplace_back(pt, Normal, NamedColors::White, Tex + Vector2(0, 1.0f));

        index_push_back(indices, i * 2);
        index_push_back(indices, (i * 2 + 3) % (stride * 2));
        index_push_back(indices, (i * 2 + 1) % (stride * 2));
    }

    // Create flat triangle fan caps to seal the bottom.
    CreateCylinderCap(vertices, indices, tessellation, height, radius, false);

    // Build RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);
}


//--------------------------------------------------------------------------------------
// Torus
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeTorus(VertexCollection& vertices, IndexCollection& indices, float diameter, float thickness, size_t tessellation, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    size_t stride = tessellation + 1;

    // First we loop around the main ring of the torus.
    for (size_t i = 0; i <= tessellation; i++)
    {
        float u = float(i) / tessellation;

        float outerAngle = i * PIx2 / tessellation - HalfPI;

        // Create a transform matrix that will align geometry to
        // slice perpendicularly though the current ring Pos.
        Matrix4x4 transform = Matrix4x4::Multiply(Matrix4x4::Translation(Vector3(diameter / 2, 0, 0)), Matrix4x4::RotationY(outerAngle));

        // Now we loop along the other axis, around the side of the tube.
        for (size_t j = 0; j <= tessellation; j++)
        {
            float v = 1 - float(j) / tessellation;

            float innerAngle = j * PIx2 / tessellation + std::numbers::pi_v<float>;
            float dx, dy;

            MathEx::SinCos(innerAngle, dy, dx);

            // Create a vertex.
            Vector3 Normal = Vector3(dx, dy, 0);
            Vector3 Pos = Normal * (thickness * 0.5f);
            Vector2 Tex = Vector2(u, v);

            Pos = transform.TransformPoint(Pos);
            Normal = transform.TransformVector(Normal);

            vertices.emplace_back(Pos, Normal, NamedColors::White, Tex);

            // And create indices for two triangles.
            size_t nextI = (i + 1) % stride;
            size_t nextJ = (j + 1) % stride;

            index_push_back(indices, i * stride + j);
            index_push_back(indices, i * stride + nextJ);
            index_push_back(indices, nextI * stride + j);

            index_push_back(indices, i * stride + nextJ);
            index_push_back(indices, nextI * stride + nextJ);
            index_push_back(indices, nextI * stride + j);
        }
    }

    // Build RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);
}


//--------------------------------------------------------------------------------------
// Tetrahedron
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeTetrahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    static const Vector3 verts[4] =
    {
        {               0.f,          0.f,        1.f },
        { 2.f * SQRT2 / 3.f,          0.f, -1.f / 3.f },
        {      -SQRT2 / 3.f,  SQRT6 / 3.f, -1.f / 3.f },
        {      -SQRT2 / 3.f, -SQRT6 / 3.f, -1.f / 3.f }
    };

    static const uint32_t faces[4 * 3] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 1,
        1, 3, 2,
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        Vector3 Normal = Vector<>::Cross(verts[v1] - verts[v0], verts[v2] - verts[v0]);
        Normal = Vector<>::Normalize(Normal);

        size_t base = vertices.size();
        index_push_back(indices, base);
        index_push_back(indices, base + 1);
        index_push_back(indices, base + 2);

        // Duplicate vertices to use face normals
        Vector3 Pos = verts[v0] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2::Zero());

        Pos = verts[v1] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2(1.0f, 0));

        Pos = verts[v2] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2(0, 1.0f));
    }

    // Built LH above
    if (rhcoords)
        ReverseWinding(indices, vertices);

    checkf(vertices.size() == 4 * 3, L"Internal logic error!");
    checkf(indices.size() == 4 * 3, L"Internal logic error!");
}


//--------------------------------------------------------------------------------------
// Octahedron
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeOctahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    static const Vector3 verts[6] =
    {
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
        {  0,  0,  1 },
        {  0,  0, -1 }
    };

    static const uint32_t faces[8 * 3] =
    {
        4, 0, 2,
        4, 2, 1,
        4, 1, 3,
        4, 3, 0,
        5, 2, 0,
        5, 1, 2,
        5, 3, 1,
        5, 0, 3
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        Vector3 Normal = Vector<>::Cross(verts[v1] - verts[v0], verts[v2] - verts[v0]);
        Normal = Vector<>::Normalize(Normal);

        size_t base = vertices.size();
        index_push_back(indices, base);
        index_push_back(indices, base + 1);
        index_push_back(indices, base + 2);

        // Duplicate vertices to use face normals
        Vector3 Pos = verts[v0] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2::Zero());

        Pos = verts[v1] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2(1.0f, 0));

        Pos = verts[v2] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2(0, 1.0f));
    }

    // Built LH above
    if (rhcoords)
        ReverseWinding(indices, vertices);

    checkf(vertices.size() == 8 * 3, L"Internal logic error.");
    checkf(indices.size() == 8 * 3, L"Internal logic error.");
}


//--------------------------------------------------------------------------------------
// Dodecahedron
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeDodecahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    static const float a = 1.f / SQRT3;
    static const float b = 0.356822089773089931942f; // sqrt( ( 3 - sqrt(5) ) / 6 )
    static const float c = 0.934172358962715696451f; // sqrt( ( 3 + sqrt(5) ) / 6 );

    static const Vector3 verts[20] =
    {
        {  a,  a,  a },
        {  a,  a, -a },
        {  a, -a,  a },
        {  a, -a, -a },
        { -a,  a,  a },
        { -a,  a, -a },
        { -a, -a,  a },
        { -a, -a, -a },
        {  b,  c,  0 },
        { -b,  c,  0 },
        {  b, -c,  0 },
        { -b, -c,  0 },
        {  c,  0,  b },
        {  c,  0, -b },
        { -c,  0,  b },
        { -c,  0, -b },
        {  0,  b,  c },
        {  0, -b,  c },
        {  0,  b, -c },
        {  0, -b, -c }
    };

    static const uint32_t faces[12 * 5] =
    {
        0, 8, 9, 4, 16,
        0, 16, 17, 2, 12,
        12, 2, 10, 3, 13,
        9, 5, 15, 14, 4,
        3, 19, 18, 1, 13,
        7, 11, 6, 14, 15,
        0, 12, 13, 1, 8,
        8, 1, 18, 5, 9,
        16, 4, 14, 6, 17,
        6, 11, 10, 2, 17,
        7, 15, 5, 18, 19,
        7, 19, 3, 10, 11,
    };

    static const Vector2 textureCoordinates[5] =
    {
        {  0.654508f, 0.0244717f },
        { 0.0954915f,  0.206107f },
        { 0.0954915f,  0.793893f },
        {  0.654508f,  0.975528f },
        {        1.f,       0.5f }
    };

    static const uint32_t textureIndex[12][5] =
    {
        { 0, 1, 2, 3, 4 },
        { 2, 3, 4, 0, 1 },
        { 4, 0, 1, 2, 3 },
        { 1, 2, 3, 4, 0 },
        { 2, 3, 4, 0, 1 },
        { 0, 1, 2, 3, 4 },
        { 1, 2, 3, 4, 0 },
        { 4, 0, 1, 2, 3 },
        { 4, 0, 1, 2, 3 },
        { 1, 2, 3, 4, 0 },
        { 0, 1, 2, 3, 4 },
        { 2, 3, 4, 0, 1 },
    };

    size_t t = 0;
    for (size_t j = 0; j < _countof(faces); j += 5, ++t)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];
        uint32_t v3 = faces[j + 3];
        uint32_t v4 = faces[j + 4];

        Vector3 Normal = Vector<>::Cross(verts[v1] - verts[v0], verts[v2] - verts[v0]);
        Normal = Vector<>::Normalize(Normal);

        size_t base = vertices.size();

        index_push_back(indices, base);
        index_push_back(indices, base + 1);
        index_push_back(indices, base + 2);

        index_push_back(indices, base);
        index_push_back(indices, base + 2);
        index_push_back(indices, base + 3);

        index_push_back(indices, base);
        index_push_back(indices, base + 3);
        index_push_back(indices, base + 4);

        // Duplicate vertices to use face normals
        Vector3 Pos = verts[v0] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, textureCoordinates[textureIndex[t][0]]);

        Pos = verts[v1] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, textureCoordinates[textureIndex[t][1]]);

        Pos = verts[v2] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, textureCoordinates[textureIndex[t][2]]);

        Pos = verts[v3] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, textureCoordinates[textureIndex[t][3]]);

        Pos = verts[v4] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, textureCoordinates[textureIndex[t][4]]);
    }

    // Built LH above
    if (rhcoords)
        ReverseWinding(indices, vertices);

    checkf(vertices.size() == 12 * 5, L"Internal logic error.");
    checkf(indices.size() == 12 * 3 * 3, L"Internal logic error.");
}


//--------------------------------------------------------------------------------------
// Icosahedron
//--------------------------------------------------------------------------------------
void GeometryGenerator::ComputeIcosahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    vertices.clear();
    indices.clear();

    static const float  t = 1.618033988749894848205f; // (1 + sqrt(5)) / 2
    static const float t2 = 1.519544995837552493271f; // sqrt( 1 + sqr( (1 + sqrt(5)) / 2 ) )

    static const Vector3 verts[12] =
    {
        {    t / t2,  1.f / t2,       0 },
        {   -t / t2,  1.f / t2,       0 },
        {    t / t2, -1.f / t2,       0 },
        {   -t / t2, -1.f / t2,       0 },
        {  1.f / t2,       0,    t / t2 },
        {  1.f / t2,       0,   -t / t2 },
        { -1.f / t2,       0,    t / t2 },
        { -1.f / t2,       0,   -t / t2 },
        {       0,    t / t2,  1.f / t2 },
        {       0,   -t / t2,  1.f / t2 },
        {       0,    t / t2, -1.f / t2 },
        {       0,   -t / t2, -1.f / t2 }
    };

    static const uint32_t faces[20 * 3] =
    {
        0, 8, 4,
        0, 5, 10,
        2, 4, 9,
        2, 11, 5,
        1, 6, 8,
        1, 10, 7,
        3, 9, 6,
        3, 7, 11,
        0, 10, 8,
        1, 8, 10,
        2, 9, 11,
        3, 11, 9,
        4, 2, 0,
        5, 0, 2,
        6, 1, 3,
        7, 3, 1,
        8, 6, 4,
        9, 4, 6,
        10, 5, 7,
        11, 7, 5
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        Vector3 Normal = Vector<>::Cross(verts[v1] - verts[v0], verts[v2] - verts[v0]);
        Normal = Vector<>::Normalize(Normal);

        size_t base = vertices.size();
        index_push_back(indices, base);
        index_push_back(indices, base + 1);
        index_push_back(indices, base + 2);

        // Duplicate vertices to use face normals
        Vector3 Pos = verts[v0] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2::Zero());

        Pos = verts[v1] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2(1.0f, 0));

        Pos = verts[v2] * size;
        vertices.emplace_back(Pos, Normal, NamedColors::White, Vector2(0, 1.0f));
    }

    // Built LH above
    if (rhcoords)
        ReverseWinding(indices, vertices);

    checkf(vertices.size() == 20 * 3, L"Internal logic error.");
    checkf(indices.size() == 20 * 3, L"Internal logic error.");
}


//--------------------------------------------------------------------------------------
// Teapot
//--------------------------------------------------------------------------------------

// Include the teapot control point data.
namespace GeometryGeneratorInternal
{
#include "Misc/TeapotData.inl"

    // Tessellates the specified bezier patch.
    void TessellatePatch(GeometryGenerator::VertexCollection& vertices, GeometryGenerator::IndexCollection& indices, TeapotPatch const& patch, size_t tessellation, Vector3 scale, bool isMirrored)
    {
        // Look up the 16 control points for this patch.
        Vector3 controlPoints[16];

        for (int i = 0; i < 16; i++)
        {
            controlPoints[i] = TeapotControlPoints[patch.indices[i]] * scale;
        }

        // Create the index data.
        size_t vbase = vertices.size();
        Bezier::CreatePatchIndices(tessellation, isMirrored, [&](size_t index)
        {
            index_push_back(indices, vbase + index);
        });

        // Create the vertex data.
        Bezier::CreatePatchVertices(controlPoints, tessellation, isMirrored, [&](Vector3 Pos, Vector3 Normal, Vector2 Tex)
        {
            vertices.emplace_back(Pos, Normal, NamedColors::White, Tex);
        });
    }
}


// Creates a teapot primitive.
void GeometryGenerator::ComputeTeapot(VertexCollection& vertices, IndexCollection& indices, float size, size_t tessellation, bool rhcoords)
{
    using namespace GeometryGeneratorInternal;

    const Vector3 g_XMNegateX = Vector3(-1.0f, 1.0f, 1.0f);
    const Vector3 g_XMNegateZ = Vector3(1.0f, 1.0f, -1.0f);

    vertices.clear();
    indices.clear();

    if (tessellation < 1)
        throw std::out_of_range("tesselation parameter out of range");

    Vector3 scaleVector = size;

    Vector3 scaleNegateX = scaleVector * g_XMNegateX;
    Vector3 scaleNegateZ = scaleVector * g_XMNegateZ;
    Vector3 scaleNegateXZ = scaleVector * g_XMNegateX * g_XMNegateZ;

    for (size_t i = 0; i < _countof(TeapotPatches); i++)
    {
        TeapotPatch const& patch = TeapotPatches[i];

        // Because the teapot is symmetrical from left to right, we only store
        // data for one side, then tessellate each patch twice, mirroring in X.
        TessellatePatch(vertices, indices, patch, tessellation, scaleVector, false);
        TessellatePatch(vertices, indices, patch, tessellation, scaleNegateX, true);

        if (patch.mirrorZ)
        {
            // Some parts of the teapot (the body, lid, and rim, but not the
            // handle or spout) are also symmetrical from front to back, so
            // we tessellate them four times, mirroring in Z as well as X.
            TessellatePatch(vertices, indices, patch, tessellation, scaleNegateZ, true);
            TessellatePatch(vertices, indices, patch, tessellation, scaleNegateXZ, false);
        }
    }

    // Built RH above
    if (!rhcoords)
        ReverseWinding(indices, vertices);
}
