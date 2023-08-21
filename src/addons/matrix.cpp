#include "addons/matrix.h"
#include "storagemanager.h"
#include "helper.h"
#include "config.pb.h"

bool MatrixInput::available() {
	return Storage::getInstance().getAddonOptions().matrixOptions.enabled;
}

void MatrixInput::setup() {
	const MatrixOptions& matrixOptions = Storage::getInstance().getAddonOptions().matrixOptions;

	if (matrixOptions.rows == 0 || matrixOptions.cols == 0) return;

	const uint8_t* colPins = reinterpret_cast<const uint8_t*>
		((matrixOptions.reverseDiode ? matrixOptions.rowPins.bytes : matrixOptions.colPins.bytes));
	const uint8_t* rowPins = reinterpret_cast<const uint8_t*>
		((matrixOptions.reverseDiode ? matrixOptions.colPins.bytes : matrixOptions.rowPins.bytes));

	const int colLength = (matrixOptions.reverseDiode ? matrixOptions.rows : matrixOptions.cols);
	const int rowLength = (matrixOptions.reverseDiode ? matrixOptions.cols : matrixOptions.rows);

	for (int i = 0; i < colLength; i++) {
		gpio_init(colPins[i]);             	// Initialize pin
		gpio_set_dir(colPins[i], GPIO_OUT); // Set as OUTPUT
	}
	for (int i = 0; i < rowLength; i++) {
		gpio_init(rowPins[i]);				// Initialize pin
		gpio_set_dir(rowPins[i], GPIO_IN);	// Set as INPUT
		gpio_pull_up(rowPins[i]);         	// Set as PULLUP
	}
}

void MatrixInput::preprocess() {
	Gamepad* gamepad = Storage::getInstance().GetGamepad();
	const PinMappings& pinMappings = Storage::getInstance().getProfilePinMappings();
	const MatrixOptions& matrixOptions = Storage::getInstance().getAddonOptions().matrixOptions;

	if (matrixOptions.rows == 0 || matrixOptions.cols == 0) return;

	const uint8_t* layout = reinterpret_cast<const uint8_t*> (matrixOptions.layout.bytes);
	const uint8_t* colPins = reinterpret_cast<const uint8_t*>
		((matrixOptions.reverseDiode ? matrixOptions.rowPins.bytes : matrixOptions.colPins.bytes));
	const uint8_t* rowPins = reinterpret_cast<const uint8_t*>
		((matrixOptions.reverseDiode ? matrixOptions.colPins.bytes : matrixOptions.rowPins.bytes));

	const int colLength = (matrixOptions.reverseDiode ? matrixOptions.rows : matrixOptions.cols);
	const int rowLength = (matrixOptions.reverseDiode ? matrixOptions.cols : matrixOptions.rows);

	uint32_t values = 0;
	int pressed = 0;

	for (int col = 0; col < colLength; col++) {
		gpio_put(colPins[col], 0);
		for (int row = 0; row < rowLength; row++) {
			if (!gpio_get(rowPins[row])) values |= 1 << layout[pressed];
			pressed++;
		}
		gpio_put(colPins[col], 1);
		busy_wait_us_32(matrixOptions.scanDelay);
	}

	gamepad->state.aux = gamepad->state.aux
		| (values & (1 << MATRIX_BUTTON_FN)) ? AUX_MASK_FUNCTION : 0;

	gamepad->state.dpad = gamepad->state.dpad
		| ((values & (1 << MATRIX_DPAD_UP))    ? gamepad->mapDpadUp->buttonMask    : 0)
		| ((values & (1 << MATRIX_DPAD_DOWN))  ? gamepad->mapDpadDown->buttonMask  : 0)
		| ((values & (1 << MATRIX_DPAD_LEFT))  ? gamepad->mapDpadLeft->buttonMask  : 0)
		| ((values & (1 << MATRIX_DPAD_RIGHT)) ? gamepad->mapDpadRight->buttonMask : 0)
	;

	gamepad->state.buttons = gamepad->state.buttons
		| ((values & (1 << MATRIX_BUTTON_B1))  ? gamepad->mapButtonB1->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_B2))  ? gamepad->mapButtonB2->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_B3))  ? gamepad->mapButtonB3->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_B4))  ? gamepad->mapButtonB4->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_L1))  ? gamepad->mapButtonL1->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_R1))  ? gamepad->mapButtonR1->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_L2))  ? gamepad->mapButtonL2->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_R2))  ? gamepad->mapButtonR2->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_S1))  ? gamepad->mapButtonS1->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_S2))  ? gamepad->mapButtonS2->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_L3))  ? gamepad->mapButtonL3->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_R3))  ? gamepad->mapButtonR3->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_A1))  ? gamepad->mapButtonA1->buttonMask  : 0)
		| ((values & (1 << MATRIX_BUTTON_A2))  ? gamepad->mapButtonA2->buttonMask  : 0)
	;
}