import licant

licant.module("heimer",
	sources = [
		"heimer/tandem.c",
		"heimer/control.c",
		"heimer/axis.c"
	],
	include_paths=["."]
)

