#include <math.h>
#include "pathFinder.h"

static float distance(point_t a, point_t b)
{
	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}

point_t dir2vec(float dir)
{
	point_t result = {cos(dir), sin(dir)};
	return result;
}

float scal(point_t a, point_t b)
{
	return a.x*b.x + a.y*b.y;
}

point_t subs(point_t from, point_t to)
{
	point_t result;
	result.x = to.x - from.x;
	result.y = to.y - from.y;
	return result;
}

float det(point_t a, point_t b)
{
	return a.x*b.y - b.x*a.y;
}

void computeTangent(point_t *center1, int r1, point_t *center2, int r2, int i, point_t *tan1, point_t *tan2)
{
	point_t h;
	int l1 = i % 2 ? 1 : -1;
	int l2 = i > 1 ? 1 : -1;
	int innerTan = -l1*l2;

	h.x = (r1*center2->x + innerTan*r2*center1->x) / (r1 + innerTan*r2);
	h.y = (r1*center2->y + innerTan*r2*center1->y) / (r1 + innerTan*r2);

	tan1->x = center1->x + (r1*r1*(h.x-center1->x) + l1*r1*(h.y-center1->y)
		* sqrt(pow((h.x - center1->x), 2) + pow((h.y - center1->y), 2) - r1*r1)) / pow(distance(h, *center1), 2);
	tan1->y = center1->y + (r1*r1*(h.y-center1->y) - l1*r1*(h.x-center1->x)
		* sqrt(pow((h.x-center1->x), 2) + pow((h.y-center1->y), 2) - r1*r1)) / pow(distance(h, *center1), 2);

	tan2->x = center2->x + (r2*r2*(h.x-center2->x) - innerTan*l2*r2*(h.y-center2->y)
		* sqrt(pow((h.x-center2->x), 2) + pow((h.y-center2->y), 2) - r2*r2)) / pow(distance(h, *center2), 2);
	tan2->y = center2->y + (r2*r2*(h.y-center2->y) + innerTan*l2*r2*(h.x-center2->x)
		* sqrt(pow((h.x-center2->x), 2) + pow((h.y-center2->y), 2) - r2*r2)) / pow(distance(h, *center2), 2);
}

void computeCenters(point_t *point, float dir, int radius, point_t *center1, point_t *center2)
{
	float dirCos = cos(dir);
	float dirSin = sin(dir);

	center1->x = point->x + radius*dirSin;
	center1->y = point->y - radius*dirCos;
	center2->x = point->x - radius*dirSin;
	center2->y = point->y + radius*dirCos;
}

interpoints_t computeInterpoints(move_t *depMove, move_t *destMove)
{
	interpoints_t result;
	int minimum = 0x7FFFFFFF;
	float tmp, depDet, destDet;
	float depDir = depMove->angle*M_PI/1800, destDir = destMove->angle*M_PI/1800;
	int startRadius = depMove->radius;
	int endRadius = destMove->radius;

	point_t tan1, tan2;
	point_t depCenters[2], destCenters[2];
	point_t dep = {depMove->x, depMove->y};
	point_t dest = {destMove->x, destMove->y};

	// both circles cannot be of the same radius because of the way we compute tangents
	if(startRadius == endRadius)
		startRadius++;

	computeCenters(&dep, depDir, startRadius, &depCenters[0], &depCenters[1]);
	computeCenters(&dest, destDir, endRadius, &destCenters[0], &destCenters[1]);

	for(int i=0; i<2; i++) { // try the 2 possible departure circles
		for(int j=0; j<2; j++) { // try the 2 possible destination circles
			depDet = det(subs(depCenters[i], dep), dir2vec(depDir));
			destDet = det(subs(destCenters[j], dest), dir2vec(destDir));

			for(int k = 0; k < 4; k++) { // try the 4 posible tangents
				computeTangent(&depCenters[i], startRadius, &destCenters[j], endRadius, k, &tan1, &tan2);

				// check if the tangent is the right one
				if(det(subs(depCenters[i], tan1), subs(tan1, tan2))*depDet > 0
					&& det(subs(destCenters[j], tan2), subs(tan1, tan2))*destDet > 0)
				{
					break;
				}
			}

			// the right tangent is the shorter one
			tmp = distance(tan1, tan2);
			if(tmp < minimum) {
				minimum = tmp;
				result.tan1 = tan1;
				result.tan2 = tan2;
			}
		}
	}

	// compute angles subtended by the arcs
	result.alpha1 = fabs(2*asin(distance(dep, result.tan1)/(2*startRadius)));
	if(scal(subs(dep, result.tan1), dir2vec(depDir)) < 0)
		result.alpha1 = 2*M_PI - result.alpha1;

	result.alpha2 = fabs(2*asin(distance(dest, result.tan2)/(2*endRadius)));
	if(scal(subs(result.tan2, dest), dir2vec(destDir)) < 0)
		result.alpha2 = 2*M_PI - result.alpha2;

	return result;
}
