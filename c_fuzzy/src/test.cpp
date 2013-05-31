/*
 * c_fuzzy,
 *
 *
 * Copyright (C) 2013 Davide Tateo
 * Versione 1.0
 *
 * This file is part of c_fuzzy.
 *
 * c_fuzzy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * c_fuzzy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with c_fuzzy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FuzzyBuilder.h"
#include "FuzzyReasoner.h"
#include <map>

int main(int argc, char *argv[])
{
	FuzzyBuilder builder;

	builder.parse(argv[1]);

	FuzzyReasoner* reasoner = builder.createReasoner();

	char stop;
	do
	{
		int value;
		std::string name;
		std::cout << "insert input name: " << std::endl;
		std::cin >> name;
		std::cout << "insert input value: " << std::endl;
		std::cin >> value;

		reasoner->addInput(name, value);

		do
		{
			std::cout << "another input? [y/n]" << std::endl;
			std::cin >> stop;
		}while(stop != 'n' && stop != 'y');

	} while (stop != 'n');

	std::map<std::string, double> results = reasoner->run();

	std::cout << "Reasoning terminato, risultati:" << std::endl;

	for (std::map<std::string, double>::iterator it = results.begin();
			it != results.end(); ++it)
	{
		std::cout << it->first << " = " << it->second << std::endl;
	}

	delete reasoner;
}
