#include <Carbon.r>

resource 'MENU' (129, "File") {
	129,
	textMenuProc,
	0x7FFFFFE3,
	enabled,
	"File",
	{	/* array: 6 elements */
		/* [1] */
		"Open...", noIcon, "O", noMark, plain,
		/* [2] */
		"Close", noIcon, "W", noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain,
		/* [4] */
		"Get Info", noIcon, "I", noMark, plain,
		/* [5] */
		"-", noIcon, noKey, noMark, plain,
		/* [6] */
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (130, "Edit") {
	130,
	textMenuProc,
	0x7FFFFFFD,
	disabled,
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

resource 'MENU' (128) {
	128,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	apple,
	{	/* array: 2 elements */
		/* [1] */
		"About Quesa Viewer…", noIcon, noKey, noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MBAR' (128) {
	{	/* array MenuArray: 3 elements */
		/* [1] */
		128,
		/* [2] */
		129,
		/* [3] */
		130
	}
};

resource 'ALRT' (128) {
	{100, 100, 250, 340},
	128,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	},
	alertPositionMainScreen
};

resource 'ALRT' (130, "unexpected error") {
	{40, 40, 180, 348},
	129,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	},
	alertPositionMainScreen
};

resource 'DITL' (128) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{118, 110, 138, 168},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{13, 13, 30, 132},
		StaticText {
			disabled,
			"Quesa Viewer"
		},
		/* [3] */
		{57, 30, 76, 254},
		UserItem {
			enabled
		},
		/* [4] */
		{139, 3, 155, 92},
		UserItem {
			disabled
		}
	}
};

resource 'DITL' (129) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{105, 120, 125, 189},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{13, 66, 69, 290},
		StaticText {
			disabled,
			"An unexpected error (^0) has occurred."
		}
	}
};

resource 'STR#' (200, "Event Handlers") {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"OnCollide",
		/* [2] */
		"OnGrab",
		/* [3] */
		"OnRelease",
		/* [4] */
		"OnMessage"
	}
};

resource 'BNDL' (128) {
	'QsVr',
	0,
	{	/* array TypeArray: 2 elements */
		/* [1] */
		'FREF',
		{	/* array IDArray: 3 elements */
			/* [1] */
			0, 128,
			/* [2] */
			1, 129,
			/* [3] */
			2, 130
		},
		/* [2] */
		'ICN#',
		{	/* array IDArray: 3 elements */
			/* [1] */
			0, 128,
			/* [2] */
			1, 129,
			/* [3] */
			2, 0
		}
	}
};

resource 'FREF' (128) {
	'APPL',
	0,
	""
};

resource 'FREF' (129) {
	'3DMF',
	1,
	""
};

resource 'FREF' (130) {
	'3DS ',
	2,
	""
};

resource 'DLOG' (128, "About Box") {
	{99, 61, 256, 340},
	movableDBoxProc,
	visible,
	goAway,
	0x0,
	128,
	"About...",
	centerMainScreen
};

resource 'CURS' (500, preload) {
	$"0300 0480 0480 0480 0480 0480 64F8 94AC"
	$"4CAA 240A 2402 1002 0802 0804 0404 0404",
	$"0300 0780 0780 0780 0780 0780 67F8 F7FC"
	$"7FFE 3FFE 3FFE 1FFE 0FFE 0FFC 07FC 07FC",
	{0, 6}
};

resource 'STR ' (128, "URL") {
	"http://www.quesa.org"
};

resource 'vers' (1) {
	0x0,
	0x21,
	development,
	0x0,
	0,
	"0.2.1",
	"v0.2.1, Copyright 2001 Joseph J. Strout"
};

resource 'vers' (2) {
	0x1,
	0x60,
	development,
	0x16,
	0,
	"Quesa v1.6d16",
	"Quesa (http://www.quesa.org)"
};

data 'QsVr' (0, "Owner resource") {
	$"00"                                                 /* . */
};
