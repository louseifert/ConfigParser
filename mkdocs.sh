#!/bin/bash
mkdir  ./documentation
doxygen ConfigParser_doxygen.cfg
rm -rdf ./documentation/html/index.html
cp -r ./documentation/html/* ./docs/
rm -rdf ./documentation
