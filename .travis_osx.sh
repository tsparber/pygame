#!/bin/bash

set -e
git clone https://github.com/MacPython/terryfy.git
cd terryfy
# Work with a specific commit
git checkout 5d889cd098fd4cf13d6d04c6d456f9e3f5fee447
cd ..
source terryfy/travis_tools.sh
get_python_environment $INSTALL_TYPE $PY_VERSION $(pwd)/_test_env
