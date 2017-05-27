#include <stdexcept>
#include <cmath>
#include <sstream>

#include "collision/narrowphase/algorithm/gjk/GJKAlgorithm.h"

namespace urchin
{

	template<class T> GJKAlgorithm<T>::GJKAlgorithm() :
		maxIteration(ConfigService::instance()->getUnsignedIntValue("narrowPhase.gjkMaxIteration")),
		terminationTolerance(ConfigService::instance()->getFloatValue("narrowPhase.gjkTerminationTolerance"))
	{

	}

	template<class T> GJKAlgorithm<T>::~GJKAlgorithm()
	{

	}

	/**
	* @param includeMargin Indicate whether algorithm operates on objects with margin
	*/
	template<class T> std::unique_ptr<GJKResult<T>> GJKAlgorithm<T>::processGJK(const CollisionConvexObject3D &convexObject1,
			const CollisionConvexObject3D &convexObject2, bool includeMargin) const
	{
		//get point which belongs to the outline of the shape (Minkowski difference)
		Vector3<T> initialDirection = Vector3<T>(1.0, 0.0, 0.0);
		Point3<T> initialSupportPointA = convexObject1.getSupportPoint(initialDirection.template cast<float>(), includeMargin).template cast<T>();
		Point3<T> initialSupportPointB = convexObject2.getSupportPoint((-initialDirection).template cast<float>(), includeMargin).template cast<T>();
		Point3<T> initialPoint = initialSupportPointA - initialSupportPointB;

		Vector3<T> direction = (-initialPoint).toVector();

		Simplex<T> simplex;
		simplex.addPoint(initialSupportPointA, initialSupportPointB);

		for(unsigned int iterationNumber=0; iterationNumber<maxIteration; ++iterationNumber)
		{
			Point3<T> supportPointA = convexObject1.getSupportPoint(direction.template cast<float>(), includeMargin).template cast<T>();
			Point3<T> supportPointB = convexObject2.getSupportPoint((-direction).template cast<float>(), includeMargin).template cast<T>();
			Point3<T> newPoint = supportPointA - supportPointB;

			const Vector3<T> &vClosestPoint = -direction; //vector from origin to closest point of simplex
			T closestPointSquareDistance = vClosestPoint.dotProduct(vClosestPoint);
			T closestPointDotNewPoint = vClosestPoint.dotProduct(newPoint.toVector());

			//check termination conditions: new point is not more extreme that existing ones OR new point already exist in simplex
			if((closestPointSquareDistance-closestPointDotNewPoint) <= terminationTolerance || simplex.isPointInSimplex(newPoint))
			{
				if(closestPointDotNewPoint <= 0.0)
				{ //collision detected
					return std::make_unique<GJKResultCollide<T>>(simplex);
				}else
				{
					return std::make_unique<GJKResultNoCollide<T>>(std::sqrt(closestPointSquareDistance), simplex);
				}
			}

			simplex.addPoint(supportPointA, supportPointB);

			direction = (-simplex.getClosestPointToOrigin()).toVector();
		}

		logMaximumIterationReach(convexObject1, convexObject2, includeMargin);
		return std::make_unique<GJKResultInvalid<T>>();
	}

	template<class T> void GJKAlgorithm<T>::logMaximumIterationReach(const CollisionConvexObject3D &convexObject1,
			const CollisionConvexObject3D &convexObject2, bool includeMargin) const
	{
		std::stringstream logStream;
		logStream<<"Maximum of iteration reached on GJK algorithm ("<<maxIteration<<")."<<std::endl;
		logStream<<" - Termination tolerance: "<<terminationTolerance<<std::endl;
		logStream<<" - Include margin: "<<includeMargin<<std::endl;
		logStream<<" - Convex object 1: "<<std::endl<<convexObject1.toString()<<std::endl;
		logStream<<" - Convex object 2: "<<std::endl<<convexObject2.toString();
		Logger::logger().logWarning(logStream.str());
	}

	//explicit template
	template class GJKAlgorithm<float>;
	template class GJKAlgorithm<double>;

}
