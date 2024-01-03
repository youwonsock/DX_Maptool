#include "pch.h"
#include "Collision.h"

#include "Figure.h"

bool Collision::RayToFace(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* pickPoint)
{
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;

	Vector3 pvec = ray.direction.Cross(edge2);
	float det = edge1.Dot(pvec);

	Vector3 tvec = Vector3::Zero;
	if (det > 0.0f) // shot front
	{
		tvec = ray.origin - v0;
	}
	else            // shot back
	{
		tvec = v0 - ray.origin;
		det = -det;
	}

	if (det < 0.0001f)	// parallel
		return false;

	// get u (v0 -> v1)
	float u = tvec.Dot(pvec);
	if (u < 0.0f || u > det)
		return false;

	// get v (v0 -> v2)
	Vector3 qvec = tvec.Cross(edge1);
	float v = ray.direction.Dot(qvec);
	if (v < 0.0f || u + v > det)
		return false;

	// get t (origin -> v0)
	// currentlly, t is not nercessary
	float t = edge2.Dot(qvec);
	float fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	if(pickPoint != nullptr)
		*pickPoint = v0 + edge1 * u + edge2 * v;

	return true;
}

bool Collision::CubeToRay(const Cube& cube, const Ray& ray)
{
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	Vector3 vR = ray.origin - cube.center;

	Vector3 axisVec[3];
	axisVec[0] = cube.axisVector[0]; axisVec[0].Normalize();
	axisVec[1] = cube.axisVector[1]; axisVec[1].Normalize();
	axisVec[2] = cube.axisVector[2]; axisVec[2].Normalize();

	float extent[3] = { cube.size.x * 0.5f, cube.size.y * 0.5f, cube.size.z * 0.5f };

	for (int v = 0; v < 3; v++)
	{
		f[v] = axisVec[v].Dot(ray.direction);
		s[v] = axisVec[v].Dot(vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > extent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - extent[v]) / f[v];
		float t2 = (-s[v] + extent[v]) / f[v];
		if (t1 > t2)
			std::swap(t1, t2);

		t_min = t_min > t1 ? t_min : t1;
		t_max = t_max < t2 ? t_max : t2;
		if (t_min > t_max)
			return false;
	}

	float  fCross[3], fRhs;
	Vector3 vDxR;

	ray.direction.Cross(vR, vDxR);
	// D X pBox->vAxis[0]  �и���
	fCross[0] = fabs(vDxR.Dot(axisVec[0]));
	float fAxis2 = extent[1] * fa[2];
	float fAxis1 = extent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
		return false;

	// D x pBox->vAxis[1]  �и���
	fCross[1] = fabs(vDxR.Dot(axisVec[1]));
	fRhs = extent[0] * fa[2] + extent[2] * fa[0];
	if (fCross[1] > fRhs)
		return false;

	// D x pBox->vAxis[2]  �и���
	fCross[2] = fabs(vDxR.Dot(axisVec[2]));
	fRhs = extent[0] * fa[1] + extent[1] * fa[0];
	if (fCross[2] > fRhs)
		return false;

	return true;
}

bool Collision::CubeToPlane(const Cube& cube, const Plane& plane)
{
	float fDist = 0.0f;
	float fPlaneToCenter = 0.0f;
	Vector3 vDir;

	Vector3 axisVec[3];
	axisVec[0] = cube.axisVector[0]; axisVec[0].Normalize();
	axisVec[1] = cube.axisVector[1]; axisVec[1].Normalize();
	axisVec[2] = cube.axisVector[2]; axisVec[2].Normalize();

	vDir = axisVec[0] * (cube.size.x * 0.5f);
	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
	vDir = axisVec[1] * (cube.size.y * 0.5f);
	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
	vDir = axisVec[2] * (cube.size.z * 0.5f);
	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);

	fPlaneToCenter = plane.normal.x * cube.center.x + plane.normal.y * cube.center.y 
						+ plane.normal.z * cube.center.z + plane.normal.w;

	if (fPlaneToCenter < -fDist)
		return false;

	return true;
}

bool Collision::SeperatingPlane(const Vector3& pos, const Vector3& dir, const Vector3 axixExtentVec1[], const Vector3 axixExtentVec2[])
{
	float val1 = fabs(pos.Dot(dir));

	float val2 = 0;
	val2 += fabs((axixExtentVec1[0]).Dot(dir));
	val2 += fabs((axixExtentVec1[1]).Dot(dir));
	val2 += fabs((axixExtentVec1[2]).Dot(dir));
	val2 += fabs((axixExtentVec2[0]).Dot(dir));
	val2 += fabs((axixExtentVec2[1]).Dot(dir));
	val2 += fabs((axixExtentVec2[2]).Dot(dir));

	return val1 > val2;
}

bool Collision::CubeToCube(const Cube& cube1, const Cube& cube2)
{
	Vector3 pos = cube2.center - cube1.center;

	Vector3 axisVec1[3];
	axisVec1[0] = cube1.points[1] - cube1.points[0]; axisVec1[0].Normalize();
	axisVec1[1] = cube1.points[0] - cube1.points[2]; axisVec1[1].Normalize();
	axisVec1[2] = cube1.points[4] - cube1.points[0]; axisVec1[2].Normalize();

	Vector3 axisVec2[3];
	axisVec2[0] = cube2.points[1] - cube2.points[0]; axisVec2[0].Normalize();
	axisVec2[1] = cube2.points[0] - cube2.points[2]; axisVec2[1].Normalize();
	axisVec2[2] = cube2.points[4] - cube2.points[0]; axisVec2[2].Normalize();

	Vector3 axisExtentVec1[3];
	axisExtentVec1[0] = axisVec1[0] * (cube1.size.x * 0.5f);
	axisExtentVec1[1] = axisVec1[1] * (cube1.size.y * 0.5f);
	axisExtentVec1[2] = axisVec1[2] * (cube1.size.z * 0.5f);

	Vector3 axisExtentVec2[3];
	axisExtentVec2[0] = axisVec2[0] * (cube2.size.x * 0.5f);
	axisExtentVec2[1] = axisVec2[1] * (cube2.size.y * 0.5f);
	axisExtentVec2[2] = axisVec2[2] * (cube2.size.z * 0.5f);

	if(SeperatingPlane(pos, axisVec1[0], axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[1], axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[2], axisExtentVec1, axisExtentVec2))return false;

	if(SeperatingPlane(pos, axisVec2[0], axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec2[1], axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec2[2], axisExtentVec1, axisExtentVec2))return false;

	if(SeperatingPlane(pos, axisVec1[0].Cross(axisVec2[0]), axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[0].Cross(axisVec2[1]), axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[0].Cross(axisVec2[2]), axisExtentVec1, axisExtentVec2))return false;

	if(SeperatingPlane(pos, axisVec1[1].Cross(axisVec2[0]), axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[1].Cross(axisVec2[1]), axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[1].Cross(axisVec2[2]), axisExtentVec1, axisExtentVec2))return false;

	if(SeperatingPlane(pos, axisVec1[2].Cross(axisVec2[0]), axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[2].Cross(axisVec2[1]), axisExtentVec1, axisExtentVec2))return false;
	if(SeperatingPlane(pos, axisVec1[2].Cross(axisVec2[2]), axisExtentVec1, axisExtentVec2))return false;

	return true;
}

bool Collision::CircleToPoint(const Circle& circle, const Vector3& point)
{
	Vector2 p = { point.x, point.z };

	if (fabs((circle.center - p).Length()) < circle.radius)
		return true;

	return false;
}
