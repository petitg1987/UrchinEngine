#ifndef ENGINE_COLLISIONCONEOBJECT_H
#define ENGINE_COLLISIONCONEOBJECT_H

#include <string>
#include "UrchinCommon.h"

#include "object/CollisionConvexObject3D.h"

namespace urchin
{

	class CollisionConeObject : public CollisionConvexObject3D
	{
		public:
			CollisionConeObject(float, float, float, typename ConeShape<float>::ConeOrientation, const Point3<float> &, const Quaternion<float> &);
			~CollisionConeObject();

			float getRadius() const;
			float getHeight() const;
			typename ConeShape<float>::ConeOrientation getConeOrientation() const;
			const Point3<float> &getCenterOfMass() const;
			const Quaternion<float> &getOrientation() const;
			const Vector3<float> &getAxis(unsigned int) const;

			Point3<float> getSupportPoint(const Vector3<float> &, bool) const;

			const Cone<float> retrieveCone() const;
			std::string toString() const;

		private:
			const Cone<float> coneObject; //object without margin
	};

}

#endif