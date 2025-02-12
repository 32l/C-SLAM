#!/bin/bash

rosservice call /getDependencyGraph | sed 's/graph: //' > ~/classifier.dot 
rosservice call /getReasoningGraph | sed 's/graph: //' > ~/reasoning.dot 

dot -Tpdf ~/classifier.dot -o ~/classifier.pdf 
dot -Tpdf ~/reasoning.dot -o ~/reasoning.pdf 

sed -i 's/\/Group.*R//g'  ~/classifier.pdf 
sed -i 's/\/Group.*R//g'  ~/reasoning.pdf 