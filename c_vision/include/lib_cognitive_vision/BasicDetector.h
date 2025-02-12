/*
 * c_vision,
 *
 *
 * Copyright (C) 2014 Davide Tateo
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

#ifndef BASICDETECTOR_H_
#define BASICDETECTOR_H_

#include "LineDetector.h"

#include <opencv2/core/core.hpp>


class BasicDetector
{
public:
	BasicDetector(ParameterServer& parameters);
	virtual void deleteDetections();

	inline void setRoll(double roll)
	{
		this->roll = -roll;
	}

	std::vector<Pole>* getPoles() const
	{
		return poles;
	}

	std::vector<Rectangle>* getRectangles() const
	{
		return rectangles;
	}

	std::vector<cv::Point>* getPoints() const
	{
		return points;
	}


	virtual ~BasicDetector();

protected:
	void detectLines(cv::Mat& image, const cv::Mat& mask = cv::Mat(), bool showCanny = true);
	void detectQuadrilaterals(bool skipCheck = false);

private:
	//detectors and classifiers
	LineDetector lineDetector;

	//Parameter
	QDetectorParam& quadParams;
	CornerClassParam& cornerParams;

	//envirorment data
	double roll;
	unsigned int width;
	unsigned int height;

protected:
	std::vector<cv::Vec4i>* verticalLines;
	std::vector<cv::Vec4i>* horizontalLines;

	std::vector<Rectangle>* rectangles;
	std::vector<Pole>* poles;
	std::vector<cv::Point>* points;
};

#endif /* BASICDETECTOR_H_ */
