#include <sstream>

#include "shape/CollisionShape3D.h"

#define DEFAULT_INNER_MARGIN 0.04

namespace urchin
{

	CollisionShape3D::CollisionShape3D() :
			innerMargin(DEFAULT_INNER_MARGIN),
			initialInnerMargin(DEFAULT_INNER_MARGIN)
	{

	}

	/**
	 * @param innerMargin This value is to avoid doing costly penetration depth calculation.
	 * - if define too small, the performance degrades
	 * - if define too big, the objects will be too rounded
	 */
	CollisionShape3D::CollisionShape3D(float innerMargin) :
			innerMargin(innerMargin),
			initialInnerMargin(innerMargin)
	{

	}

	CollisionShape3D::~CollisionShape3D()
	{

	}

	void CollisionShape3D::refreshInnerMargin(float maximumInnerMargin)
	{
		if(this->innerMargin > maximumInnerMargin)
		{
			this->innerMargin = maximumInnerMargin;
		}

	}

	float CollisionShape3D::getInnerMargin() const
	{
		return innerMargin;
	}

	void CollisionShape3D::checkInnerMarginQuality(const std::string &shapeId) const
	{
		if(initialInnerMargin > innerMargin)
		{
			AABBox<float> aabbox = toAABBox(PhysicsTransform());
			float shapeLength = aabbox.getMin().vector(aabbox.getMax()).length();
			bool isBigShape = shapeLength > initialInnerMargin * 20.0;

			if(isBigShape)
			{
				std::stringstream logStream;
				logStream<<"Not optimal margin on shape id "<<shapeId<<"."<<std::endl;
				logStream<<" - Current margin: "<<innerMargin<<std::endl;
				logStream<<" - Expected margin: "<<initialInnerMargin<<std::endl;
				logStream<<" - Shape length: "<<shapeLength;
				Logger::logger().logWarning(logStream.str());
			}
		}
	}
}
