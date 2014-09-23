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

#include <vector>

#include <ros/ros.h>

#include <sensor_msgs/Imu.h>
#include <c_slam_msgs/TrackedObject.h>

using namespace std;

class RosPublisher
{
public:
	RosPublisher()
	{
		imuPublisher = n.advertise<sensor_msgs::Imu>("/ardrone/imu", 1024);
		trackPublisher = n.advertise<c_slam_msgs::TrackedObject>("/tracks",
					1024);

		//K << 565.59102697808, 0.0, 337.839450567586, 0.0, 563.936510489792, 199.522081717361, 0.0, 0.0, 1.0;
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
	ros::NodeHandle n;
	ros::Publisher imuPublisher;
	ros::Publisher trackPublisher;

	//Eigen::Matrix3d K;
};

/*void setTracks(vector<vector<Eigen::Vector4d> >& tracks)
{
	int numTracks = 8;

	tracks.resize(numTracks);

	for (int i = 0; i < numTracks; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			Eigen::Vector4d track;

			tracks[i].push_back(track);
		}
	}
}*/

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "circular_test");

	RosPublisher publisher;

	//IMU data
	double r = 1.0; // meters
	double alpha = 0.1; // radians / s^2
	double w0 = 0.0; //initial angular speed
	double theta0 = 0;//-M_PI / 2.0;
	double t = 0.0;
	double imuRate = 50;

	//Tracks data
	//vector<vector<Eigen::Vector4d> > tracks;

	ROS_INFO("Simulation started");

	ros::Rate rate(imuRate);
	for (int i = 0; i < 10000; i++)
	{
		double w = w0 + alpha * t;

		vector<double> za =
		{ alpha * r, std::pow(w, 2) * r, 9.80566 };
		vector<double> zw =
		{ 0.0, 0.0, w };

		publisher.publishIMU(za, zw, t);

		t += 1.0 / imuRate;

		rate.sleep();
	}

	ROS_INFO("Simulation completed");

	return 0;
}
