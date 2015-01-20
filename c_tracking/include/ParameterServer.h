/*
 * c_tracking,
 *
 *
 * Copyright (C) 2015 Davide Tateo
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

#ifndef INCLUDE_PARAMETERSERVER_H_
#define INCLUDE_PARAMETERSERVER_H_

#include <dynamic_reconfigure/server.h>
#include <c_tracking/ParametersConfig.h>

#include <iostream>

struct ExtractionParam
{
	int threshold;
};

class ParameterServer
{
public:
	ParameterServer();

	inline ExtractionParam& getExtraction()
	{
		return extraction;
	}

private:
	void update(c_tracking::ParametersConfig &config, uint32_t level);

private:
	//Dynamic_reconfigure server
	dynamic_reconfigure::Server<c_tracking::ParametersConfig> server;

	//parameters
	ExtractionParam extraction;

};

#endif /* INCLUDE_PARAMETERSERVER_H_ */
