/*
 * FullSlamImu.h
 *
 *  Created on: Sep 8, 2014
 *      Author: davide
 */

#ifndef FULLSLAMIMU_H_
#define FULLSLAMIMU_H_

#include <Eigen/Dense>

#include <ros/ros.h>
#include <tf/tf.h>

#include <sensor_msgs/Imu.h>
#include <c_slam_msgs/TrackedObject.h>

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

#include "ImuHandler.h"
#include "RectangleHandler.h"

namespace roamfree_c_slam
{

class FullSlamImu
{

public:

	FullSlamImu(std::string imuTopic);
	virtual ~FullSlamImu();
	void run();

protected:
	void imuCb(const sensor_msgs::Imu &msg);
	void tracksCb(const c_slam_msgs::TrackedObject &msg);

private:
	void initRoamfree();
	void initCamera();
	void publishFeatureMarkers();
	void publishCameraPose();

private:
	ros::NodeHandle n;
	ros::Subscriber tracks_sub;
	ros::Subscriber imu_sub;

	ros::Publisher markers_pub;
	tf::TransformBroadcaster pose_tf_br;

	tf::Transform T_OC_tf; // transformation from robot to camera;

private:
	ROAMestimation::FactorGraphFilter* filter;
	ImuHandler* imuHandler;
	RectangleHandler *tracksHandler;

};

} /* namespace roamfree_c_slam */

#endif /* FULLSLAMIMU_H_ */