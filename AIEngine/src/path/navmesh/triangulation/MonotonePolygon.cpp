#include <stdexcept>
#include <utility>

#include "MonotonePolygon.h"

namespace urchin
{
	bool TypedPointCmp::operator()(const TypedPoint &left, const TypedPoint &right) const
	{
		if(ccwPolygonPoints[left.pointIndex].Y == ccwPolygonPoints[right.pointIndex].Y)
		{
			return ccwPolygonPoints[left.pointIndex].X < ccwPolygonPoints[right.pointIndex].X;
		}
		return ccwPolygonPoints[left.pointIndex].Y < ccwPolygonPoints[right.pointIndex].Y;
	}

	MonotonePolygon::MonotonePolygon(const std::vector<Point2<float>> &ccwPolygonPoints) :
			ccwPolygonPoints(ccwPolygonPoints)
	{

	}

	/**
	 * Create Y-monotone polygons.
	 * Y-monotone polygon: any lines on X-axis should intersect the polygon once (point/line) or not at all.
	 */
	void MonotonePolygon::createYMonotonePolygons()
	{
		createYMonotonePolygonsDiagonals();

		std::vector<std::vector<Point2<float>>> yMonotonePolygons;
		yMonotonePolygons.resize(diagonals.size() + 1);

		if(diagonals.size()==0)
		{
			yMonotonePolygons[0] = ccwPolygonPoints;
		}else
		{
			unsigned int polygonIndex = 0;
			for(std::multimap<unsigned int, Edge>::iterator it = diagonals.begin(); it!=diagonals.end(); ++it)
			{
				Edge startDiagonal = it->second;
				if(!startDiagonal.isProcessed)
				{
					yMonotonePolygons[polygonIndex].push_back(ccwPolygonPoints[startDiagonal.startIndex]);
					yMonotonePolygons[polygonIndex].push_back(ccwPolygonPoints[startDiagonal.endIndex]);

					unsigned int previousPointIndex = startDiagonal.startIndex;
					unsigned int currentPointIndex = startDiagonal.endIndex;

					while(true)
					{
						unsigned int nextPointIndex = findNextPointIndex(previousPointIndex, currentPointIndex);
						markDiagonalProcessedIfExist(currentPointIndex, nextPointIndex);

						if(nextPointIndex==startDiagonal.startIndex)
						{
							break;
						}

						yMonotonePolygons[polygonIndex].push_back(ccwPolygonPoints[nextPointIndex]);

						previousPointIndex = currentPointIndex;
						currentPointIndex = nextPointIndex;
					}

					startDiagonal.isProcessed = true;
					++polygonIndex;
				}
			}
		}
	}

	void MonotonePolygon::createYMonotonePolygonsDiagonals()
	{
		edgeHelpers.clear();
		diagonals.clear();

		typed_points_queue typedPointsQueue = buildTypedPointsQueue();
		edgeHelpers.reserve(5);

		while(!typedPointsQueue.empty())
		{
			PointType pointType = typedPointsQueue.top().type;
			unsigned int i = typedPointsQueue.top().pointIndex;
			switch(pointType)
			{
				case PointType::START_VERTEX:
					handleStartVertex(i);
					break;
				case PointType::SPLIT_VERTEX:
					handleSplitVertex(i);
					break;
				case PointType::END_VERTEX:
					handleEndVertex(i);
					break;
				case PointType::MERGE_VERTEX:
					handleMergeVertex(i);
					break;
				case PointType::REGULAR_DOWN_VERTEX:
					handleRegularDownVertex(i);
					break;
				case PointType::REGULAR_UP_VERTEX:
					handleRegularUpVertex(i);
					break;
				default:
					throw std::runtime_error("Unknown type of the point: " + pointType);
			}

			typedPointsQueue.pop();
		}
	}

	typed_points_queue MonotonePolygon::buildTypedPointsQueue() const
	{
		TypedPointCmp typedPointCmp(ccwPolygonPoints);
		typed_points_queue typedPointsQueue(typedPointCmp);

		for(unsigned int i=0; i<ccwPolygonPoints.size(); ++i)
		{
			TypedPoint typedPoint;
			typedPoint.pointIndex = i;

			unsigned int previousIndex = (i-1)%ccwPolygonPoints.size();
			unsigned int nextIndex = (i+1)%ccwPolygonPoints.size();

			if(ccwPolygonPoints[i].Y > ccwPolygonPoints[previousIndex].Y && ccwPolygonPoints[i].Y > ccwPolygonPoints[nextIndex].Y)
			{
				Vector3<float> previousToOrigin = Vector3<float>(ccwPolygonPoints[previousIndex].vector(ccwPolygonPoints[i]), 0.0f);
				Vector3<float> originToNext = Vector3<float>(ccwPolygonPoints[i].vector(ccwPolygonPoints[nextIndex]), 0.0f);
				float dotResult = Vector3<float>(0.0, 0.0, 1.0).dotProduct(previousToOrigin.crossProduct(originToNext));

				typedPoint.type = dotResult>=0.0 ? PointType::START_VERTEX : PointType::SPLIT_VERTEX;
			}else if(ccwPolygonPoints[i].Y < ccwPolygonPoints[previousIndex].Y && ccwPolygonPoints[i].Y < ccwPolygonPoints[nextIndex].Y)
			{
				Vector3<float> previousToOrigin = Vector3<float>(ccwPolygonPoints[previousIndex].vector(ccwPolygonPoints[i]), 0.0f);
				Vector3<float> originToNext = Vector3<float>(ccwPolygonPoints[i].vector(ccwPolygonPoints[nextIndex]), 0.0f);
				float dotResult = Vector3<float>(0.0, 0.0, 1.0).dotProduct(previousToOrigin.crossProduct(originToNext));

				typedPoint.type = dotResult>=0.0 ? PointType::END_VERTEX : PointType::MERGE_VERTEX;
			}else
			{
				if(ccwPolygonPoints[i].Y < ccwPolygonPoints[previousIndex].Y && ccwPolygonPoints[i].Y > ccwPolygonPoints[nextIndex].Y)
				{
					typedPoint.type = PointType::REGULAR_DOWN_VERTEX;
				}else
				{
					typedPoint.type = PointType::REGULAR_UP_VERTEX;
				}
			}

			typedPointsQueue.push(typedPoint);
		}

		return typedPointsQueue;
	}

	void MonotonePolygon::handleStartVertex(unsigned int i)
	{
		createEdgeHelper(i, i, PointType::START_VERTEX);
	}

	void MonotonePolygon::handleSplitVertex(unsigned int i)
	{
		std::vector<EdgeHelper>::iterator edgeHelperIt = findNearestLeftEdgeHelper(i);

		createDiagonals(i, edgeHelperIt->helperPointIndex);

		edgeHelperIt->helperPointIndex = i;
		edgeHelperIt->helperPointType = PointType::SPLIT_VERTEX;

		createEdgeHelper(i, i, PointType::SPLIT_VERTEX);
	}

	void MonotonePolygon::handleEndVertex(unsigned int i)
	{
		unsigned int previousEdgeIndex = i-1;
		std::vector<EdgeHelper>::const_iterator edgeHelperIt = findEdgeHelper(previousEdgeIndex);
		if(edgeHelperIt->helperPointType==PointType::MERGE_VERTEX)
		{
			createDiagonals(i, edgeHelperIt->helperPointIndex);
		}
		edgeHelpers.erase(edgeHelperIt);
	}

	void MonotonePolygon::handleMergeVertex(unsigned int i)
	{
		unsigned int previousEdgeIndex = i-1;
		std::vector<EdgeHelper>::const_iterator edgeHelperConstIt = findEdgeHelper(previousEdgeIndex);
		if(edgeHelperConstIt->helperPointType==PointType::MERGE_VERTEX)
		{
			createDiagonals(i, edgeHelperConstIt->helperPointIndex);
		}
		edgeHelpers.erase(edgeHelperConstIt);

		std::vector<EdgeHelper>::iterator edgeHelperIt = findNearestLeftEdgeHelper(i);
		if(edgeHelperIt->helperPointType==PointType::MERGE_VERTEX)
		{
			createDiagonals(i, edgeHelperIt->helperPointIndex);
		}
		edgeHelperIt->helperPointIndex = i;
		edgeHelperIt->helperPointType = PointType::MERGE_VERTEX;
	}

	void MonotonePolygon::handleRegularDownVertex(unsigned int i)
	{
		unsigned int previousEdgeIndex = i-1;
		std::vector<EdgeHelper>::const_iterator edgeHelperIt = findEdgeHelper(previousEdgeIndex);
		if(edgeHelperIt->helperPointType==PointType::MERGE_VERTEX)
		{
			createDiagonals(i, edgeHelperIt->helperPointIndex);
		}
		edgeHelpers.erase(edgeHelperIt);

		createEdgeHelper(i, i, PointType::REGULAR_DOWN_VERTEX);
	}

	void MonotonePolygon::handleRegularUpVertex(unsigned int i)
	{
		std::vector<EdgeHelper>::iterator edgeHelperIt = findNearestLeftEdgeHelper(i);
		if(edgeHelperIt->helperPointType==PointType::MERGE_VERTEX)
		{
			createDiagonals(i, edgeHelperIt->helperPointIndex);
		}
		edgeHelperIt->helperPointIndex = i;
		edgeHelperIt->helperPointType = PointType::REGULAR_UP_VERTEX;
	}

	void MonotonePolygon::createEdgeHelper(unsigned int edgeStartIndex, unsigned int helperPointIndex, PointType pointType)
	{
		EdgeHelper edgeHelper;
		edgeHelper.edge.startIndex = edgeStartIndex;
		edgeHelper.edge.endIndex = (edgeStartIndex+1)%ccwPolygonPoints.size();
		edgeHelper.helperPointIndex = helperPointIndex;
		edgeHelper.helperPointType = pointType;

		edgeHelpers.push_back(edgeHelper);
	}

	std::vector<EdgeHelper>::const_iterator MonotonePolygon::findEdgeHelper(unsigned int edgeIndex) const
	{
		for(std::vector<EdgeHelper>::const_iterator it=edgeHelpers.begin(); it!=edgeHelpers.end(); ++it)
		{
			if(it->edge.startIndex==edgeIndex)
			{
				return it;
			}
		}

		throw std::runtime_error("Impossible to find edge and his helper for edge index: " + edgeIndex);
	}

	std::vector<EdgeHelper>::iterator MonotonePolygon::findNearestLeftEdgeHelper(unsigned int pointIndex)
	{
		Point2<float> point = ccwPolygonPoints[pointIndex];

		float minDistance = std::numeric_limits<float>::max();
		std::vector<EdgeHelper>::iterator nearestLeftEdgeHelperIt = edgeHelpers.end();

		for(std::vector<EdgeHelper>::iterator it=edgeHelpers.begin(); it!=edgeHelpers.end(); ++it)
		{
			Line2D<float> edge(ccwPolygonPoints[it->edge.startIndex], ccwPolygonPoints[it->edge.endIndex]);
			Point2<float> nearestPointOnEdge = edge.orthogonalProjection(point);
			if(nearestPointOnEdge.X  < point.X)
			{ //edge is on left of point
				float edgePointDistance = nearestPointOnEdge.squareDistance(point);
				if(edgePointDistance < minDistance)
				{
					minDistance = edgePointDistance;
					nearestLeftEdgeHelperIt = it;
				}
			}
		}

		if(nearestLeftEdgeHelperIt==edgeHelpers.end())
		{
			throw std::runtime_error("Impossible to find edge on left for point index: " + std::to_string(pointIndex));
		}

		return nearestLeftEdgeHelperIt;
	}

	void MonotonePolygon::createDiagonals(unsigned int index1, unsigned int index2)
	{
		Edge diagonal1;
		diagonal1.startIndex = index1;
		diagonal1.endIndex = index2;
		diagonal1.isProcessed = false;
		diagonals.insert(std::make_pair(index1, diagonal1));

		Edge diagonal2;
		diagonal2.startIndex = index2;
		diagonal2.endIndex = index1;
		diagonal2.isProcessed = false;
		diagonals.insert(std::make_pair(index2, diagonal2));
	}

	unsigned int MonotonePolygon::findNextPointIndex(unsigned int edgeStartIndex, unsigned int edgeEndIndex) const
	{
		std::vector<int> possibleNextPoints;

		possibleNextPoints.push_back((edgeEndIndex + 1) % ccwPolygonPoints.size());
		auto range =diagonals.equal_range(edgeEndIndex);
		for(auto it = range.first; it != range.second; ++it)
		{
			possibleNextPoints.push_back(it->second.endIndex);
		}

		Vector3<float> edgeVector = Vector3<float>(ccwPolygonPoints[edgeStartIndex].vector(ccwPolygonPoints[edgeEndIndex]), 0.0f);
		for(unsigned int i=0; i<possibleNextPoints.size(); ++i)
		{
			Vector3<float> nextEdgeVector = Vector3<float>(ccwPolygonPoints[edgeEndIndex].vector(ccwPolygonPoints[possibleNextPoints[i]]), 0.0f);
			float dotResult = Vector3<float>(0.0, 0.0, 1.0).dotProduct(edgeVector.crossProduct(nextEdgeVector));

			//TODO ...
		}
	}

	void MonotonePolygon::markDiagonalProcessedIfExist(unsigned int edgeStartIndex, unsigned int edgeEndIndex)
	{ //TODO move this source code in findNextPointIndex for better performance...
		auto range = diagonals.equal_range(edgeStartIndex);
		for(auto it = range.first; it != range.second; ++it)
		{
			if(edgeEndIndex == it->second.endIndex)
			{
				it->second.isProcessed = true;
				break;
			}
		}
	}

}