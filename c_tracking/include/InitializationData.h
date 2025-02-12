/*
 * c_tracking,
 *
 *
 * Copyright (C) 2014 Davide Tateo
 * Versione 1.0
 *
 * This file is part of c_tracking.
 *
 * c_tracking is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * c_tracking is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with c_tracking.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INITIALIZATIONDATA_H_
#define INITIALIZATIONDATA_H_

struct InitializationData
{
	//Object keypoints and features
	std::vector<cv::KeyPoint> selected_keypoints;
	cv::Mat selected_features;

	//background keypoints and features
	std::vector<cv::KeyPoint> background_keypoints;
	cv::Mat background_features;

	//initial polygon
	std::vector<cv::Point2f> polygon;
};

#endif /* INITIALIZATIONDATA_H_ */
