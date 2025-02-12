/*
 * FullSlamImu.h
 *
 *  Created on: Sep 8, 2014
 *      Author: davide
 */

#ifndef FULLSLAMIMU_RECTANGLES_H_
#define FULLSLAMIMU_RECTANGLES_H_

#include <Eigen/Dense>

#include <ros/ros.h>
#include <tf/tf.h>

#include <sensor_msgs/Imu.h>
#include <c_slam_msgs/NamedPolygon.h>

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

#include "FullSlamImu.h"
#include "roamfree_extra/AnchoredRectangleHandlerBootstrap.h"

namespace roamfree_c_slam
{

class FullSlamImu_anchoredrectangles : public FullSlamImu
{

public:
	FullSlamImu_anchoredrectangles(FullSlamConfig& config);
	virtual ~FullSlamImu_anchoredrectangles();
	void run();

protected:
	void tracksCb(const c_slam_msgs::NamedPolygon &msg);

private:
	void initCamera();
	void publishFeatureMarkers();

private:
	ROAMvision::AnchoredRectangleHandlerBootstrap *tracksHandler;
	ros::Subscriber tracks_sub;

};

} /* namespace roamfree_c_slam */

#endif /* FULLSLAMIMU_RECTANGLES_H_ */
