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

#ifndef MATRIX_SCAN_DELAY
#define MATRIX_SCAN_DELAY 2
#endif

#ifndef MATRIX_REVERSE_DIODE
#define MATRIX_REVERSE_DIODE 0
#endif

// Matrix Module Name
#define MatrixName "Matrix"
class MatrixInput : public GPAddon {
public:
	virtual bool available();
	virtual void setup();       // Matrix Setup
	virtual void process();     // Matrix Process
	virtual void preprocess() {}
	virtual std::string name() { return MatrixName; }
};

#endif  // _Matrix_H