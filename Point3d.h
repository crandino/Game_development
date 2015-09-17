#ifndef __Point3d_H__
#define __Point3d_H__

template <class TYPE>
class Point3d {

public:

	TYPE x, y, z;

	Point3d operator+ (const Point3d &p) const
	{
		Point3d tmp;
		tmp.x = x + p.x;
		tmp.y = y + p.y;
		tmp.z = z + p.z;
		return tmp;
	}

	Point3d operator- (const Point3d &p) const
	{
		Point3d tmp;
		tmp.x = x - p.x;
		tmp.y = y - p.y;
		tmp.z = z - p.z;
		return tmp;
	}

	const Point3d& operator+= (const Point3d &p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return (*this);
	}

	const Point3d& operator-= (const Point3d &p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return (*this);
	}

	const Point3d& operator*= (const Point3d &p)
	{
		x *= p.x;
		y *= p.y;
		z *= p.z;
		return (*this);
	}

	bool operator== (const Point3d &p) const
	{
		return (x == p.x && y == p.y && z == p.z);
	}

	bool operator!= (const Point3d &p) const
	{
		return (x != p.x || y != p.y || z != p.z);
	}

	bool isZero() const
	{
		return (x == 0.0f && y == 0.0f && z == 0.0f);
	}

	void setZero()
	{
		x = y = z = 0;
	}

	float distanceTo(const Point3d &p) const
	{
		float fx = x - p.x;
		float fy = y - p.y;
		float fz = z - p.z;
		return sqrt((fx*fx) + (fy*fy) + (fz*fz));
	}

	bool isClosedTo(const Point3d &p, float delta) const
	{
		return (abs(p.x - x) <= delta &&
				abs(p.y - y) <= delta &&
				abs(p.z - z) <= delta);
	}

};

#endif // !__Point3d_H__
