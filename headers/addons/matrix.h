#ifndef _Matrix_H
#define _Matrix_H

#include "gpaddon.h"

#include "GamepadEnums.h"

#ifndef MATRIX_ENABLED
#define MATRIX_ENABLED 0
#endif

#ifndef MATRIX_ROWS
#define MATRIX_ROWS 0
#endif

#ifndef MATRIX_COLS
#define MATRIX_COLS 0
#endif

#ifndef MATRIX_ROW_PINS
#define MATRIX_ROW_PINS {}
#endif

#ifndef MATRIX_COL_PINS
#define MATRIX_COL_PINS {}
#endif

#ifndef MATRIX_LAYOUT
#define MATRIX_LAYOUT {}
#endif

#ifndef MATRIX_SCAN_DELAY
#define MATRIX_SCAN_DELAY 2
#endif

#ifndef MATRIX_REVERSE_DIODE
#define MATRIX_REVERSE_DIODE 0
#endif

// Matrix gamepad mapping
#define MATRIX_NONE				-1	// Nothing
#define MATRIX_DPAD_UP			0	// UP
#define MATRIX_DPAD_DOWN		1	// DOWN
#define MATRIX_DPAD_RIGHT		2	// RIGHT
#define MATRIX_DPAD_LEFT		3	// LEFT
#define MATRIX_BUTTON_B1		4	// B1 / A / B / Cross / 2 / K1
#define MATRIX_BUTTON_B2		5	// B2 / B / A / Circle / 3 / K2
#define MATRIX_BUTTON_R2		6	// R2 / RT / ZR / R2 / 8 / K3
#define MATRIX_BUTTON_L2		7	// L2 / LT / ZL / L2 / 7 / K4
#define MATRIX_BUTTON_B3		8	// B3 / X / Y / Square / 1 / P1
#define MATRIX_BUTTON_B4		9	// B4 / Y / X / Triangle / 4 / P2
#define MATRIX_BUTTON_R1		10	// R1 / RB / R / R1 / 6 / P3
#define MATRIX_BUTTON_L1		11	// L1 / LB / L / L1 / 5 / P4
#define MATRIX_BUTTON_S1		12	// S1 / Back / Minus / Select / 9 / Coin
#define MATRIX_BUTTON_S2		13	// S2 / Start / Plus / Start / 10 / Start
#define MATRIX_BUTTON_L3		14	// L3 / LS / LS / L3 / 11 / LS
#define MATRIX_BUTTON_R3		15	// R3 / RS / RS / R3 / 12 / RS
#define MATRIX_BUTTON_A1		16	// A1 / Guide / Home / ~ / 13 / ~
#define MATRIX_BUTTON_A2		17	// A2 / ~ / Capture / ~ / 14 / ~
#define MATRIX_BUTTON_FN		18	// Hotkey Function
#define MATRIX_BUTTON_TURBO		19	// Turbo
#define MATRIX_BUTTON_REVERSE	20	// UDLR Reverse
#define MATRIX_SLIDER_ONE		21	// Left Stick Slider
#define MATRIX_SLIDER_TWO		22	// Right Stick Slider
#define MATRIX_SLIDER_SOCD_ONE	23	// SOCD Slider Pin One
#define MATRIX_SLIDER_SOCD_TWO	24	// SOCD Slider Pin Two

// Matrix Module Name
#define MatrixName "Matrix"
class MatrixInput : public GPAddon {
public:
	virtual bool available();
	virtual void setup();
	virtual void process() {}
	virtual void preprocess();
	virtual std::string name() { return MatrixName; }
};

#endif  // _Matrix_H