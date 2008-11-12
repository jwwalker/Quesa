#include <Carbon.r>


resource 'DITL' (128, purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{115, 179, 135, 237},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{10, 31, 95, 221},
		Picture {
			enabled,
			128
		}
	}
};

resource 'DLOG' (128, purgeable) {
	{74, 129, 219, 375},
	dBoxProc,
	visible,
	goAway,
	0x0,
	128,
	"",
	alertPositionParentWindowScreen
};

resource 'MBAR' (128) {
	{	/* array MenuArray: 5 elements */
		/* [1] */
		128,
		/* [2] */
		129,
		/* [3] */
		130,
		/* [4] */
		131,
		/* [5] */
		132
	}
};

resource 'MENU' (128) {
	128,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	apple,
	{	/* array: 2 elements */
		/* [1] */
		"About Qut…", noIcon, noKey, noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (129) {
	129,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	"File",
	{	/* array: 3 elements */
		/* [1] */
		"Close", noIcon, "W", noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (130) {
	130,
	textMenuProc,
	0x7FFFFFC0,
	enabled,
	"Edit",
	{	/* array: 6 elements */
		/* [1] */
		"Undo", noIcon, "Z", noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"Cut", noIcon, "X", noMark, plain,
		/* [4] */
		"Copy", noIcon, "C", noMark, plain,
		/* [5] */
		"Paste", noIcon, "V", noMark, plain,
		/* [6] */
		"Clear", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (131) {
	131,
	textMenuProc,
	0x7FFFFFF7,
	enabled,
	"Renderer",
	{	/* array: 4 elements */
		/* [1] */
		"Show FPS", noIcon, noKey, noMark, plain,
		/* [2] */
		"16-bit Depth Buffer", noIcon, noKey, noMark, plain,
		/* [3] */
		"32-bit Depth Buffer", noIcon, noKey, noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (132) {
	132,
	textMenuProc,
	0x7DBB7777,
	enabled,
	"Style",
	{	/* array: 36 elements */
		/* [1] */
		"Shader Null", noIcon, noKey, noMark, plain,
		/* [2] */
		"Shader Lambert", noIcon, noKey, noMark, plain,
		/* [3] */
		"Shader Phong", noIcon, noKey, noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain,
		/* [5] */
		"Fill Style Filled", noIcon, noKey, noMark, plain,
		/* [6] */
		"Fill Style Edges", noIcon, noKey, noMark, plain,
		/* [7] */
		"Fill Style Points", noIcon, noKey, noMark, plain,
		/* [8] */
		"-", noIcon, noKey, noMark, plain,
		/* [9] */
		"Backfacing Style Both", noIcon, noKey, noMark, plain,
		/* [10] */
		"Backfacing Style Remove", noIcon, noKey, noMark, plain,
		/* [11] */
		"Backfacing Style Flip", noIcon, noKey, noMark, plain,
		/* [12] */
		"-", noIcon, noKey, noMark, plain,
		/* [13] */
		"Interpolation Style None", noIcon, noKey, noMark, plain,
		/* [14] */
		"Interpolation Style Vertex", noIcon, noKey, noMark, plain,
		/* [15] */
		"Interpolation Style Pixel", noIcon, noKey, noMark, plain,
		/* [16] */
		"-", noIcon, noKey, noMark, plain,
		/* [17] */
		"Orientation Style Clockwise", noIcon, noKey, noMark, plain,
		/* [18] */
		"Orientation Style Counter-Clockwise", noIcon, noKey, noMark, plain,
		/* [19] */
		"-", noIcon, noKey, noMark, plain,
		/* [20] */
		"Anti-Alias Style Off", noIcon, noKey, noMark, plain,
		/* [21] */
		"Anti-Alias Style Edges", noIcon, noKey, noMark, plain,
		/* [22] */
		"Anti-Alias Style Filled", noIcon, noKey, noMark, plain,
		/* [23] */
		"-", noIcon, noKey, noMark, plain,
		/* [24] */
		"Fog Style On", noIcon, noKey, noMark, plain,
		/* [25] */
		"Fog Style Off", noIcon, noKey, noMark, plain,
		/* [26] */
		"-", noIcon, noKey, noMark, plain,
		/* [27] */
		"Subdivision Style Constant (5, 5)", noIcon, noKey, noMark, plain,
		/* [28] */
		"Subdivision Style Constant (25 ,25)", noIcon, noKey, noMark, plain,
		/* [29] */
		"Subdivision Style Constant (50, 50)", noIcon, noKey, noMark, plain,
		/* [30] */
		"Subdivision Style Constant (50, 5)", noIcon, noKey, noMark, plain,
		/* [31] */
		"Subdivision Style World Space (0.1)", noIcon, noKey, noMark, plain,
		/* [32] */
		"Subdivision Style World Space (0.5)", noIcon, noKey, noMark, plain,
		/* [33] */
		"Subdivision Style World Space (2.5)", noIcon, noKey, noMark, plain,
		/* [34] */
		"Subdivision Style Screen Space (3)", noIcon, noKey, noMark, plain,
		/* [35] */
		"Subdivision Style Screen Space (10)", noIcon, noKey, noMark, plain,
		/* [36] */
		"Subdivision Style Screen Space (30)", noIcon, noKey, noMark, plain
	}
};

data 'Mcmd' (131) {
	$"0004 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'Mcmd' (132) {
	$"0000"                                               /* .. */
};

resource 'PICT' (128) {
	290,
	{0, 0, 85, 190},
	$"0011 02FF 0C00 FFFF FFFF 0000 0000 0000"
	$"0000 00BE 0000 0055 0000 0000 0000 00A1"
	$"0096 000A 0200 0000 0200 0000 0000 001E"
	$"0001 000A 0000 0000 0054 00BD 0007 0000"
	$"0000 0022 F380 F380 0000 002C 0009 0003"
	$"0647 656E 6576 6100 0003 0003 0004 0100"
	$"000D 0009 002E 0004 0000 0000 002B 2A0A"
	$"1151 7574 2041 7070 2046 7261 6D65 776F"
	$"726B 0004 0000 0028 0016 001C 1E50 6172"
	$"7420 6F66 2074 6865 2051 7565 7361 2064"
	$"6973 7472 6962 7574 696F 6E00 0028 002E"
	$"0008 2646 6F72 206D 6F72 6520 696E 666F"
	$"726D 6174 696F 6E20 6F6E 2051 7565 7361"
	$"2C20 706C 6561 7365 2000 002B 1E0C 1873"
	$"6565 2074 6865 2051 7565 7361 2068 6F6D"
	$"6520 7061 6765 2E00 002B 0218 1568 7474"
	$"703A 2F2F 7777 772E 7175 6573 612E 6F72"
	$"672F 00A0 0097 00FF"
};

data 'carb' (0) {
	$"0000 0000"                                          /* .... */
};

resource 'mctb' (132) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};
