#include "Triangle3D.h"

namespace urchin {

    template<class T> Triangle3D<T>::Triangle3D(const Point3<T>* points):
        triangleShape(TriangleShape3D<T>(points)) {

    }

    template<class T> Triangle3D<T>::Triangle3D(const Point3<T>& point1, const Point3<T>& point2, const Point3<T>& point3):
            triangleShape(TriangleShape3D<T>(point1, point2, point3)) {

    }

    template<class T> const Point3<T> *Triangle3D<T>::getPoints() const {
        return triangleShape.getPoints();
    }

    template<class T> Point3<T> Triangle3D<T>::getSupportPoint(const Vector3<T>& direction) const {
        T maxPointDotDirection = getPoints()[0].toVector().dotProduct(direction);
        Point3<T> maxPoint = getPoints()[0];

        for (unsigned int i=1;i<3; ++i) {
            T currentPointDotDirection  = getPoints()[i].toVector().dotProduct(direction);
            if (currentPointDotDirection > maxPointDotDirection) {
                maxPointDotDirection = currentPointDotDirection;
                maxPoint = getPoints()[i];
            }
        }

        return maxPoint;
    }

    template<class T> Vector3<T> Triangle3D<T>::computeNormal() const {
        const Vector3<T>& aux = this->getPoints()[1].vector(this->getPoints()[2]);
        const Vector3<T>& normal = aux.crossProduct(this->getPoints()[1].vector(this->getPoints()[0]));

        return normal.normalize();
    }

    /**
     * @param barycentrics [out] Returns barycentric coordinates for closest point
     * @return Point on triangle closest to point p
     */
    template<class T> Point3<T> Triangle3D<T>::closestPoint(const Point3<T>& p, T barycentrics[3]) const {
        const Point3<T>& a = this->getPoints()[0];
        const Point3<T>& b = this->getPoints()[1];
        const Point3<T>& c = this->getPoints()[2];

        //check if P is in voronoi region of point A
        Vector3<T> ab = a.vector(b);
        Vector3<T> ac = a.vector(c);
        Vector3<T> ap = a.vector(p);
        T abDotAp = ab.dotProduct(ap);
        T acDotAp = ac.dotProduct(ap);
        if (abDotAp <= (T)0.0 && acDotAp <= (T)0.0) {
            barycentrics[0] = 1.0;
            barycentrics[1] = 0.0;
            barycentrics[2] = 0.0;
            return a;
        }

        //check if P is in voronoi region of point B
        Vector3<T> bp = b.vector(p);
        T abDotBp = ab.dotProduct(bp);
        T acDotBp = ac.dotProduct(bp);
        if (abDotBp >= (T)0.0 && acDotBp <= abDotBp) {
            barycentrics[0] = 0.0;
            barycentrics[1] = 1.0;
            barycentrics[2] = 0.0;
            return b;
        }

        //check if P is in voronoi region of edge AB
        T vc = abDotAp*acDotBp - abDotBp*acDotAp;
        if (vc <= (T)0.0 && abDotAp >= (T)0.0 && abDotBp <= (T)0.0) {
            T v = abDotAp / (abDotAp - abDotBp);

            barycentrics[0] = (T)1.0 - v;
            barycentrics[1] = v;
            barycentrics[2] = 0.0;
            return a.translate(v * ab);
        }

        //check if P is in voronoi region of point C
        Vector3<T> cp = c.vector(p);
        T abDotCp = ab.dotProduct(cp);
        T acDotCp = ac.dotProduct(cp);
        if (acDotCp >= (T)0.0 && abDotCp <= acDotCp) {
            barycentrics[0] = 0.0;
            barycentrics[1] = 0.0;
            barycentrics[2] = 1.0;
            return c;
        }

        //check if P is in voronoi region of edge AC
        T vb = abDotCp*acDotAp - abDotAp*acDotCp;
        if (vb <= (T)0.0 && acDotAp >= (T)0.0 && acDotCp <= (T)0.0) {
            T w = acDotAp / (acDotAp - acDotCp);

            barycentrics[0] = (T)1.0 - w;
            barycentrics[1] = 0.0;
            barycentrics[2] = w;
            return a.translate(w * ac);
        }

        //check if P is in voronoi region of BC
        T va = abDotBp*acDotCp - abDotCp*acDotBp;
        if (va <= (T)0.0 && (acDotBp - abDotBp) >= (T)0.0 && (abDotCp - acDotCp) >= (T)0.0) {
            T w = (acDotBp - abDotBp) / ((acDotBp - abDotBp) + (abDotCp - acDotCp));

            barycentrics[0] = 0.0;
            barycentrics[1] = (T)1.0 - w;
            barycentrics[2] = w;
            return b + w * (c - b);
        }

        //P is inside face region
        T denom = (T)1.0 / (va + vb + vc);
        T v = vb * denom;
        T w = vc * denom;

        barycentrics[0] = (T)1.0 - v - w;
        barycentrics[1] = v;
        barycentrics[2] = w;
        return a.translate(ab * v + ac * w);
    }

    /**
     * Project point on triangle plane and check if point lies on triangle
     */
    template<class T> bool Triangle3D<T>::projectedPointInsideTriangle(const Point3<T>& point) const { //see https://stackoverflow.com/questions/25512037/how-to-determine-if-a-point-lies-over-a-triangle-in-3d
        Vector3<T> normal = computeNormal();

        for (unsigned int i=0; i<3; ++i) {
            Vector3<T> triangleVector = triangleShape.getPoints()[(i+1)%3].vector(triangleShape.getPoints()[i]);
            Vector3<T> pointToTriangle = point.vector(triangleShape.getPoints()[i]);
            Vector3<T> tetrahedronFaceNormal = triangleVector.crossProduct(pointToTriangle);
            if (tetrahedronFaceNormal.dotProduct(normal) < 0.0) {
                return false;
            }
        }

        return true;
    }

    //explicit template
    template class Triangle3D<float>;
    template class Triangle3D<double>;

}
