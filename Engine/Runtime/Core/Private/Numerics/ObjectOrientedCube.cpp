// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/ObjectOrientedCube.h"

#include "CoreString.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace std;

ObjectOrientedCube::ObjectOrientedCube()
{

}

ObjectOrientedCube::ObjectOrientedCube(const Vector3& center, const Vector3& extent, const Quaternion& rotation)
{
	Center = center;
	Extent = extent;
	Rotation = rotation;
}

ObjectOrientedCube::ObjectOrientedCube(const ObjectOrientedCube& copy)
{
	Center = copy.Center;
	Extent = copy.Extent;
	Rotation = copy.Rotation;
}

bool ObjectOrientedCube::Equals(const ObjectOrientedCube& rh) const
{
	return *this == rh;
}

bool ObjectOrientedCube::NearlyEquals(const ObjectOrientedCube& rh, double epsilon) const
{
	return Center.NearlyEquals(rh.Center, epsilon)
		&& Extent.NearlyEquals(rh.Extent, epsilon)
		&& Rotation.NearlyEquals(rh.Rotation, epsilon);
}

size_t ObjectOrientedCube::GetHashCode() const
{
	return Center.GetHashCode() ^ Extent.GetHashCode() ^ Rotation.GetHashCode();
}

TRefPtr<String> ObjectOrientedCube::ToString() const
{
	return String::Format(
		"{{Center: {0}, Extent: {1}, Rotation: {2}}}",
		Center,
		Extent,
		Rotation
	);
}

bool ObjectOrientedCube::IsOverlap(const ObjectOrientedCube& cube) const
{
	Vector3 Axis[3] = { AxisX, AxisY, AxisZ };
	double pExtent[3] = { Extent.X, Extent.Y, Extent.Z };
	Vector3 OtherAxis[3] = { cube.AxisX, cube.AxisY, cube.AxisZ };
	double pOtherExtent[3] = { cube.Extent.X, cube.Extent.Y, cube.Extent.Z };

	Vector3 abvec = cube.Center - Center;

	Vector3 cofactor[3];
	Vector3 absCof[3];
	double a_dot[3];

	// Check A-oriented axes.
	{
		for (int i = 0; i < 3; ++i)
		{
			//
			// Primary assign primitive values.
			cofactor[i][0] = Axis[i] | cube.AxisX;
			cofactor[i][1] = Axis[i] | cube.AxisY;
			cofactor[i][2] = Axis[i] | cube.AxisZ;

			a_dot[i] = Axis[i] | abvec;

			absCof[i][0] = abs(cofactor[i][0]);
			absCof[i][1] = abs(cofactor[i][1]);
			absCof[i][2] = abs(cofactor[i][2]);

			double R = abs(a_dot[i]);
			double R1 = cube.Extent | Vector3(absCof[i][0], absCof[i][1], absCof[i][2]);
			double R01 = pExtent[i] + R1;

			if (R > R01)
			{
				return false;
			}
		}
	}

	// Check B-oriented axes.
	{
		for (int i = 0; i < 3; ++i)
		{
			double R = abs(OtherAxis[i] | abvec);
			double R0 = Extent | Vector3(absCof[0][i], absCof[1][i], absCof[2][i]);
			double R01 = R0 + pOtherExtent[i];

			if (R > R01)
			{
				return false;
			}
		}
	}

	// Check complex axes.
	{
		int OrderA[3] = { 2, 0, 1 };
		int OrderB[3] = { 1, 2, 0 };

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				double R = abs(
					a_dot[OrderA[i]] * cofactor[OrderB[i]][j]
					-
					a_dot[OrderB[i]] * cofactor[OrderA[i]][j]
				);

				int nInc = (i == 0) ? 1 : 0;  // 1, 0, 0
				int nExc = 2 - ((i == 2) ? 1 : 0);  // 2, 2, 1
				double R0 = pExtent[nInc] * absCof[nExc][j] + pExtent[nExc] * absCof[nInc][j];

				nInc = (j == 0) ? 1 : 0;
				nExc = 2 - ((j == 2) ? 1 : 0);
				double R1 = pOtherExtent[nInc] * absCof[i][nExc] + pOtherExtent[nExc] * absCof[i][nInc];

				double R01 = R0 + R1;
				if (R > R01)
				{
					return false;
				}
			}
		}
	}

	return true;
}

vector<Vector3> ObjectOrientedCube::CalcVertices() const
{
	double Signs[2] = { +1, -1 };

	vector<Vector3> verts(8);
	int index = 0;

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				verts[index++] = Center + Signs[i] * AxisX * Extent.X + Signs[j] * AxisY * Extent.Y + Signs[k] * AxisZ * Extent.Z;
			}
		}
	}

	return verts;
}

Vector3 ObjectOrientedCube::AxisX_get() const
{
	return Rotation.RotateVector(Vector3::AxisX);
}

Vector3 ObjectOrientedCube::AxisY_get() const
{
	return Rotation.RotateVector(Vector3::AxisY);
}

Vector3 ObjectOrientedCube::AxisZ_get() const
{
	return Rotation.RotateVector(Vector3::AxisZ);
}

bool ObjectOrientedCube::operator ==(const ObjectOrientedCube& right) const
{
	return Center == right.Center && Extent == right.Extent && Rotation == right.Rotation;
}

bool ObjectOrientedCube::operator !=(const ObjectOrientedCube& right) const
{
	return Center != right.Center || Extent != right.Extent || Rotation != right.Rotation;
}