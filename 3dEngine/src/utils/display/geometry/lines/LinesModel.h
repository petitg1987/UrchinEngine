#ifndef URCHINENGINE_LINESMODEL_H
#define URCHINENGINE_LINESMODEL_H

#include <vector>
#include "UrchinCommon.h"

#include "utils/display/geometry/GeometryModel.h"

namespace urchin
{

	class LinesModel : public GeometryModel
	{
		public:
			LinesModel(const std::vector<Point3<float>> &, int);
            LinesModel(const Line3D<float> &, int);

		protected:
			Matrix4<float> retrieveModelMatrix() const override;
			std::vector<Point3<float>> retrieveVertexArray() const override;

			void drawGeometry() const override;

		private:
			std::vector<Point3<float>> linesPoints;
			int linesSize;
	};

}

#endif
