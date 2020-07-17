#!/usr/bin/env python3

import licant

licant.include("heimer")

licant.cxx_shared_library("heimer.so",
	mdepends=["heimer"],
)

licant.ex("heimer.so")