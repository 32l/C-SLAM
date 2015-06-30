/*
 * c_vision,
 *
 *
 * Copyright (C) 2013 Davide Tateo
 * Versione 1.0
 *
 * This file is part of c_vision.
 *
 * c_vision is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * c_vision is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with c_vision.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QuadrilateralDetector.h"
#include "Lines.h"

using namespace cv;
using namespace std;

QuadrilateralDetector::QuadrilateralDetector(QDetectorParam& quadP,
			CornerClassifier& cornerClassifier) :
			quadP(quadP), cornerClassifier(cornerClassifier)
{
	rectangles = new std::vector<Rectangle>();
	poles = new std::vector<Pole>();
}

void QuadrilateralDetector::detect(std::vector<cv::Vec4i>& verticalLines,
			std::vector<cv::Vec4i>& horizontalLines, bool skipCheck)
{
	for (size_t i = 0; i + 1 < verticalLines.size(); i++)
	{	//i+1 instead size-1 to avoid integer overflow
		Vec4i& v1 = verticalLines[i];
		Vec4i& v2 = verticalLines[i + 1];

		//find possible poles
		if (hasSufficientVerticalOverlap(v1, v2))
		{

			unsigned int midLine = Lines::getMidLine(v1, v2);

			//else find possible squares
			for (size_t j = 0; j + 1 < horizontalLines.size(); j++)
			{
				Vec4i& h1 = horizontalLines[j];
				if (Lines::between_h(v1, v2, h1) && Lines::above(h1, midLine))
					for (size_t k = j + 1; k < horizontalLines.size(); k++)
					{
						Vec4i& h2 = horizontalLines[k];

						if (Lines::between_h(v1, v2, h2)
									&& hasSufficientHorizontallOverlap(h1, h2)
									&& !Lines::above(h2, midLine))
						{
							Point x, y, z, w;

							x = Lines::findInterception(h1, v1);
							y = Lines::findInterception(h1, v2);
							z = Lines::findInterception(h2, v2);
							w = Lines::findInterception(h2, v1);

							if (hasSuficientCorners(x, y, z, w)
										&& hasSufficientSupport(x, y, z, w, h1,
													h2, v1, v2))
							{
								Rectangle rectangle(x, y, z, w, quadP.omega);
								rectangles->push_back(rectangle);
							}
						}

					}
			}
		}

	}

}

bool QuadrilateralDetector::findPoles(Vec4i l1, Vec4i l2)
{
	double dx, dy;

	Point p0, p1, p2, p3;

	Lines::getPointsCoordinates(l1, p0, p1);
	Lines::getPointsCoordinates(l2, p2, p3);

	//calculate the average dx and dy
	dx = max(norm(p0 - p2), norm(p1 - p3));
	dy = max(norm(p0 - p1), norm(p2 - p3));

	if (dy / dx > quadP.polesFormFactor)
	{
		Pole pole(p0, p1, p2, p3);
		poles->push_back(pole);
		return true;
	}

	return false;

}

bool QuadrilateralDetector::hasSufficientVerticalOverlap(Vec4i& v1, Vec4i& v2)
{
	int min1 = min(v1[1], v1[3]);
	int max1 = max(v1[1], v1[3]);
	int l1 = max1 - min1; //FIXME this is wrong. rotation is needed

	int min2 = min(v2[1], v2[3]);
	int max2 = max(v2[1], v2[3]);
	int l2 = max1 - min1; //FIXME this is wrong. rotation is needed

	int overlap = max(0, min(max1, max2) - max(min1, min2));

	double percentualOverlap = (double) overlap / min(l1, l2);

	//FIXME: compute real overlap % using angle
	return percentualOverlap > quadP.verticalOverlap;

}

bool QuadrilateralDetector::hasSufficientHorizontallOverlap(Vec4i& h1,
			Vec4i& h2)
{
	int min1 = min(h1[0], h1[2]);
	int max1 = max(h1[0], h1[2]);
	int l1 = max1 - min1; //FIXME this is wrong. rotation is needed

	int min2 = min(h2[0], h2[2]);
	int max2 = max(h2[0], h2[2]);
	int l2 = max1 - min1; //FIXME this is wrong. rotation is needed

	int overlap = max(0, min(max1, max2) - max(min1, min2));

	double percentualOverlap = (double) overlap / min(l1, l2);

	//FIXME: compute real overlap % using angle
	return percentualOverlap > quadP.horizontalOverlap;

}

bool QuadrilateralDetector::hasSufficientSupport(const Point& x, const Point& y,
			const Point& z, const Point& w, Vec4i& h1, Vec4i& h2, Vec4i& v1,
			Vec4i& v2)
{
	return hasSufficientSupportH(h1, x, y) && hasSufficientSupportH(h2, z, w)
				&& hasSufficientSupportV(v1, x, w)
				&& hasSufficientSupportV(v2, y, z);
}

bool QuadrilateralDetector::hasSufficientSupportV(Vec4i& v, Point a, Point b)
{
	int ymin = min(v[1], v[3]);
	int ymax = max(v[1], v[3]);

	int overlap = min(ymax, b.y) - max(ymin, a.y);

	double percentualOverlap = (double) overlap / (b.y - a.y);

	return percentualOverlap > quadP.segmentSupport;
}

bool QuadrilateralDetector::hasSufficientSupportH(Vec4i& h, Point a, Point b)
{
	int xmin = min(h[0], h[2]);
	int xmax = max(h[0], h[2]);

	int overlap = min(xmax, b.x) - max(xmin, a.x);

	double percentualOverlap = (double) overlap / (b.x - a.x);

	return percentualOverlap > quadP.segmentSupport;
}

bool QuadrilateralDetector::hasSuficientCorners(cv::Point& x, cv::Point& y,
			cv::Point& z, cv::Point& w)
{
	//return true; ///FIXME TOGLIERE!!!
	CornerResult xRes = cornerClassifier.getResult(x);
	CornerResult yRes = cornerClassifier.getResult(y);
	CornerResult zRes = cornerClassifier.getResult(z);
	CornerResult wRes = cornerClassifier.getResult(w);

	bool compatible = xRes.isCompatible(SE) && yRes.isCompatible(SW)
				&& zRes.isCompatible(NE) && wRes.isCompatible(NW);

	int emptyCount = xRes.countEmpty() + yRes.countEmpty() + zRes.countEmpty()
				+ wRes.countEmpty();

	return compatible && emptyCount <= 1;
}

