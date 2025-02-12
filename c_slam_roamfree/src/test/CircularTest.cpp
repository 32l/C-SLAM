/*
 * c_slam_roamfree,
 *
 *
 * Copyright (C) 2014 Davide Tateo
 * Versione 1.0
 *
 * This file is part of c_slam_roamfree.
 *
 * c_slam_roamfree is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * c_slam_roamfree is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with c_slam_roamfree.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test/TestPublisher.h"

#include <tf_conversions/tf_eigen.h>

using namespace std;

namespace roamfree_c_slam
{
class CircularTestPublisher : public TestPublisher
{
public:
	CircularTestPublisher() : TestPublisher()
	{
		imuPublisher = n.advertise<sensor_msgs::Imu>("/ardrone/imu", 6000);
	}

	void publishIMU(vector<double>& za, vector<double>& zw, double t)
	{
		sensor_msgs::Imu msg;

		msg.angular_velocity.x = zw[0];
		msg.angular_velocity.y = zw[1];
		msg.angular_velocity.z = zw[2];

		msg.linear_acceleration.x = za[0];
		msg.linear_acceleration.y = za[1];
		msg.linear_acceleration.z = za[2];

		msg.header.stamp.fromSec(t);

		imuPublisher.publish(msg);
	}

private:
	ros::Publisher imuPublisher;
};

}


void computeCameraPose(Eigen::Matrix4d& H, double t, double theta0, double w0,
			double alpha, double r)
{
	double thetaRobot = theta0 + w0 * t + 0.5 * alpha * std::pow(t, 2);
	double theta = thetaRobot - M_PI / 2;

	double x = r * cos(theta);
	double y = r * sin(theta);
	double z = 0;

	Eigen::Matrix4d H_WR;

	H_WR << cos(thetaRobot), -sin(thetaRobot), 0, x, //
	sin(thetaRobot), cos(thetaRobot), 0, y, //
	0, 0, 1, z, //
	0, 0, 0, 1; //

	Eigen::Matrix4d H_RC;

	H_RC << 0, 0, 1, 0, //
	-1, 0, 0, 0, //
	0, -1, 0, 0, //
	0, 0, 0, 1;

	H = H_WR * H_RC;
	H = H / H(3, 3);

}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "circular_test");

	roamfree_c_slam::CircularTestPublisher publisher;

	//IMU data
	double r = 1; // meters
	double alpha = 0.01; // radians / s^2
	double w0 = 0.0; //initial angular speed
	double thetaRobot0 = 0; //-M_PI / 2.0;
	double t = 0.0;
	double imuRate = 50;

	//Tracks data setup
	publisher.setTracksSquare(2.5);
	//publisher.setTracksCircle(2.5);
	//publisher.setTracksCarpet();

	ROS_INFO("Waiting other nodes to start...");

	ros::Duration(5.0).sleep();

	ROS_INFO("Simulation started");

	ros::Rate rate(imuRate);
	for (int i = 0; i < 10000 && ros::ok(); i++)
	{
		double w = w0 + alpha * t;

		vector<double> za =
		{ alpha * r, std::pow(w, 2) * r, 9.8 };
		vector<double> zw =
		{ 0.0, 0.0, w };

		Eigen::Matrix4d H_WC;
		computeCameraPose(H_WC, t, thetaRobot0, w0, alpha, r);

		publisher.publishIMU(za, zw, t);
		if (i % 10 == 0)
		{
			publisher.publishTracks(H_WC, t);
			publisher.publishGroundTruth(H_WC);
			publisher.publishGroundTruthLandmark();
			publisher.publishGroundTruthLandmarkPoints();
		}

		t += 1.0 / imuRate;

		rate.sleep();
	}

	ROS_INFO("Simulation completed");

	return 0;
}
