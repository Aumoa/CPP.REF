#pragma once

//--------------------------------------------------------------------------------------
// File: Bezier.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <array>
#include <algorithm>

class Bezier
{
public:
    // Performs a cubic bezier interpolation between four control points,
    // returning the value at the specified time (t ranges 0 to 1).
    template<typename T>
    inline static T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
    {
        return p1 * (1 - t) * (1 - t) * (1 - t) +
            p2 * 3 * t * (1 - t) * (1 - t) +
            p3 * 3 * t * t * (1 - t) +
            p4 * t * t * t;
    }

    template<>
    inline static Vector3 CubicInterpolate(Vector3 const& p1, Vector3 const& p2, Vector3 const& p3, Vector3 const& p4, float t)
    {
        Vector3 T0 = ((1 - t) * (1 - t) * (1 - t));
        Vector3 T1 = (3 * t * (1 - t) * (1 - t));
        Vector3 T2 = (3 * t * t * (1 - t));
        Vector3 T3 = (t * t * t);

        Vector3 Result = p1 * T0;
        Result = p2 * T1 + Result;
        Result = p3 * T2 + Result;
        Result = p4 * T3 + Result;

        return Result;
    }


    // Computes the tangent of a cubic bezier curve at the specified time.
    template<typename T>
    inline static T CubicTangent(T const& p1, T const& p2, T const& p3, T const& p4, float t)
    {
        return p1 * (-1 + 2 * t - t * t) +
            p2 * (1 - 4 * t + 3 * t * t) +
            p3 * (2 * t - 3 * t * t) +
            p4 * (t * t);
    }

    template<>
    inline static Vector3 CubicTangent(Vector3 const& p1, Vector3 const& p2, Vector3 const& p3, Vector3 const& p4, float t)
    {
        Vector3 T0 = (-1 + 2 * t - t * t);
        Vector3 T1 = (1 - 4 * t + 3 * t * t);
        Vector3 T2 = (2 * t - 3 * t * t);
        Vector3 T3 = (t * t);

        Vector3 Result = p1 * T0;
        Result = p2 * T1 + Result;
        Result = p3 * T2 + Result;
        Result = p4 * T3 + Result;

        return Result;
    }


    // Creates vertices for a patch that is tessellated at the specified level.
    // Calls the specified outputVertex function for each generated vertex,
    // passing the position, normal, and texture coordinate as parameters.
    template<typename TOutputFunc>
    static void CreatePatchVertices(_In_reads_(16) Vector3 patch[16], size_t tessellation, bool isMirrored, TOutputFunc outputVertex)
    {
        for (size_t i = 0; i <= tessellation; i++)
        {
            float u = float(i) / tessellation;

            for (size_t j = 0; j <= tessellation; j++)
            {
                float v = float(j) / tessellation;

                // Perform four horizontal bezier interpolations
                // between the control points of this patch.
                Vector3 p1 = CubicInterpolate(patch[0], patch[1], patch[2], patch[3], u);
                Vector3 p2 = CubicInterpolate(patch[4], patch[5], patch[6], patch[7], u);
                Vector3 p3 = CubicInterpolate(patch[8], patch[9], patch[10], patch[11], u);
                Vector3 p4 = CubicInterpolate(patch[12], patch[13], patch[14], patch[15], u);

                // Perform a vertical interpolation between the results of the
                // previous horizontal interpolations, to compute the position.
                Vector3 position = CubicInterpolate(p1, p2, p3, p4, v);

                // Perform another four bezier interpolations between the control
                // points, but this time vertically rather than horizontally.
                Vector3 q1 = CubicInterpolate(patch[0], patch[4], patch[8], patch[12], v);
                Vector3 q2 = CubicInterpolate(patch[1], patch[5], patch[9], patch[13], v);
                Vector3 q3 = CubicInterpolate(patch[2], patch[6], patch[10], patch[14], v);
                Vector3 q4 = CubicInterpolate(patch[3], patch[7], patch[11], patch[15], v);

                // Compute vertical and horizontal tangent vectors.
                Vector3 tangent1 = CubicTangent(p1, p2, p3, p4, v);
                Vector3 tangent2 = CubicTangent(q1, q2, q3, q4, u);

                // Cross the two tangent vectors to compute the normal.
                Vector3 normal = Vector3::CrossProduct(tangent1, tangent2);

                if (!normal.NearlyEquals(Vector3::Zero, Math::SmallNumber<>))
                {
                    normal = normal.Normalized;

                    // If this patch is mirrored, we must invert the normal.
                    if (isMirrored)
                    {
                        normal = -normal;
                    }
                }
                else
                {
                    // In a tidy and well constructed bezier patch, the preceding
                    // normal computation will always work. But the classic teapot
                    // model is not tidy or well constructed! At the top and bottom
                    // of the teapot, it contains degenerate geometry where a patch
                    // has several control points in the same place, which causes
                    // the tangent computation to fail and produce a zero normal.
                    // We 'fix' these cases by just hard-coding a normal that points
                    // either straight up or straight down, depending on whether we
                    // are on the top or bottom of the teapot. This is not a robust
                    // solution for all possible degenerate bezier patches, but hey,
                    // it's good enough to make the teapot work correctly!

                    const Vector3 IdentityR1 = Vector3(0, 1, 0);
                    const Vector3 IdentityR1Neg = Vector3(0, -1, 0);
                    normal = Vector3::Select(IdentityR1, IdentityR1Neg, Vector3::SelectControl::Less(position, Vector3::Zero));
                }

                // Compute the texture coordinate.
                float mirroredU = isMirrored ? 1 - u : u;

                Vector2 textureCoordinate = Vector2(mirroredU, v);

                // Output this vertex.
                outputVertex(position, normal, textureCoordinate);
            }
        }
    }


    // Creates indices for a patch that is tessellated at the specified level.
    // Calls the specified outputIndex function for each generated index value.
    template<typename TOutputFunc>
    static void CreatePatchIndices(size_t tessellation, bool isMirrored, TOutputFunc outputIndex)
    {
        size_t stride = tessellation + 1;

        for (size_t i = 0; i < tessellation; i++)
        {
            for (size_t j = 0; j < tessellation; j++)
            {
                // Make a list of six index values (two triangles).
                std::array<size_t, 6> indices =
                {
                    i * stride + j,
                    (i + 1) * stride + j,
                    (i + 1) * stride + j + 1,

                    i * stride + j,
                    (i + 1) * stride + j + 1,
                    i * stride + j + 1,
                };

                // If this patch is mirrored, reverse indices to fix the winding order.
                if (isMirrored)
                {
                    std::reverse(indices.begin(), indices.end());
                }

                // Output these index values.
                std::for_each(indices.begin(), indices.end(), outputIndex);
            }
        }
    }
};