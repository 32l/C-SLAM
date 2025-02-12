/*
 * c_fuzzy,
 *
 *
 * Copyright (C) 2014 Davide Tateo
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

#ifndef FUZZYPREDICATEENGINE_H_
#define FUZZYPREDICATEENGINE_H_

#include <map>
#include <string>
#include <vector>

#include "Node.h"
#include "FuzzyMF.h"

//typedef std::pair<NodePtr, std::vector<DomainTablePtr>> PredicateInstance;

struct PredicateInstance
{
	PredicateInstance(NodePtr rule, std::vector<DomainTablePtr>& domains,
				std::vector<Variable>& extraVariables) :
				rule(rule), domains(domains), extraVariables(extraVariables)
	{
	}

	NodePtr rule;
	std::vector<DomainTablePtr> domains;
	std::vector<Variable> extraVariables;

};

class FuzzyPredicateEngine
{

private:
	struct PredicateData
	{
		std::vector<std::string> templateVarList;
		NodePtr definition;
		std::vector<Variable> extraVariables;
	};

	typedef std::map<std::string, PredicateData> PredicateNameMap;
	typedef std::map<std::string, PredicateNameMap> PredicateMap;

public:
	FuzzyPredicateEngine();

	void enterNamespace(std::string nameSpace);
	void enterPredicate(std::vector<std::string> templateVariableList);
	void buildDomain(std::string templateVar);
	void addTemplateMF(std::string label, FuzzyMF* mf);
	void buildPredicate(std::string name, NodePtr rule);
	PredicateInstance getPredicateInstance(std::string predicate,
				std::vector<Variable>& variable);
	PredicateInstance getPredicateInstance(std::string nameSpace,
				std::string predicate, std::vector<Variable>& variable);
	size_t getTemplateVarIndex(std::string templateVar);
	void checkPredicateConsistency();

private:
	DomainTablePtr instantiatePredicateVar(std::string nameSpace,
				std::string templateVar, std::string variable);

private:
	PredicateMap predicateMap;
	NamespaceTable table;
	std::string currentNamespace;
	std::string currentTemplateVar;
	size_t currentDomainDefCount;
	std::vector<std::string> currentTemplateVarList;

};

#endif /* FUZZYPREDICATEENGINE_H_ */
