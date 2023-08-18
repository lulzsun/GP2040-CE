#include "addons/matrix.h"
#include "storagemanager.h"
#include "helper.h"
#include "config.pb.h"

bool MatrixInput::available() {
	return Storage::getInstance().getAddonOptions().matrixOptions.enabled;
}

void MatrixInput::setup() {
	const MatrixOptions& matrixOptions = Storage::getInstance().getAddonOptions().matrixOptions;

	if (matrixOptions.matrixRows == 0 || matrixOptions.matrixCols == 0) return;

	const uint8_t* matrixRowPins = reinterpret_cast<const uint8_t*>(matrixOptions.matrixRowPins.bytes);
	const uint8_t* matrixColPins = reinterpret_cast<const uint8_t*>(matrixOptions.matrixColPins.bytes);
	for (int i = 0; i < matrixOptions.matrixRows; i++) {
		gpio_init(matrixRowPins[i]);             		// Initialize pin
		if (matrixOptions.matrixReverseDiode) {
			gpio_set_dir(matrixRowPins[i], GPIO_IN); 	// Set as INPUT
			gpio_pull_up(matrixRowPins[i]);          	// Set as PULLUP
		}
		else gpio_set_dir(matrixRowPins[i], GPIO_OUT); 	// Set as OUTPUT
	}
	for (int i = 0; i < matrixOptions.matrixCols; i++) {
		gpio_init(matrixColPins[i]);					// Initialize pin
		if (!matrixOptions.matrixReverseDiode) {
			gpio_set_dir(matrixColPins[i], GPIO_IN);	// Set as INPUT
			gpio_pull_up(matrixColPins[i]);         	// Set as PULLUP
		}
		else gpio_set_dir(matrixColPins[i], GPIO_OUT);	// Set as OUTPUT
	}
}

void MatrixInput::process() {
	Gamepad* gamepad = Storage::getInstance().GetGamepad();
	const PinMappings& pinMappings = Storage::getInstance().getProfilePinMappings();
	const MatrixOptions& matrixOptions = Storage::getInstance().getAddonOptions().matrixOptions;

	if (matrixOptions.matrixRows == 0 || matrixOptions.matrixCols == 0) return;

	const uint8_t* matrixRowPins = reinterpret_cast<const uint8_t*>(matrixOptions.matrixRowPins.bytes);
	const uint8_t* matrixColPins = reinterpret_cast<const uint8_t*>(matrixOptions.matrixColPins.bytes);

	uint32_t values = 0;
	int pressed = 0;
	if (!matrixOptions.matrixReverseDiode) {
		for (int row = 0; row < matrixOptions.matrixRows; row++) {
			gpio_put(matrixRowPins[row], 0);
			for (int col = 0; col < matrixOptions.matrixCols; col++) {
				if (!gpio_get(matrixColPins[col])) values |= 1 << pressed;
				pressed++;
			}
			gpio_put(matrixRowPins[row], 1);
			busy_wait_us_32(matrixOptions.matrixScanDelay);
		}
	}
	else {
		for (int col = 0; col < matrixOptions.matrixCols; col++) {
			gpio_put(matrixColPins[col], 0);
			for (int row = 0; row < matrixOptions.matrixRows; row++) {
				if (!gpio_get(matrixRowPins[row])) values |= 1 << pressed;
				pressed++;
			}
			gpio_put(matrixColPins[col], 1);
			busy_wait_us_32(matrixOptions.matrixScanDelay);
		}
	}

	gamepad->state.aux = 0
		| (values & (1 << pinMappings.pinButtonFn)) ? AUX_MASK_FUNCTION : 0;

	gamepad->state.dpad = 0
		| ((values & gamepad->mapDpadUp->pinMask)    ? gamepad->mapDpadUp->buttonMask    : 0)
		| ((values & gamepad->mapDpadDown->pinMask)  ? gamepad->mapDpadDown->buttonMask  : 0)
		| ((values & gamepad->mapDpadLeft->pinMask)  ? gamepad->mapDpadLeft->buttonMask  : 0)
		| ((values & gamepad->mapDpadRight->pinMask) ? gamepad->mapDpadRight->buttonMask : 0)
	;

	gamepad->state.buttons = 0
		| ((values & gamepad->mapButtonB1->pinMask)  ? gamepad->mapButtonB1->buttonMask  : 0)
		| ((values & gamepad->mapButtonB2->pinMask)  ? gamepad->mapButtonB2->buttonMask  : 0)
		| ((values & gamepad->mapButtonB3->pinMask)  ? gamepad->mapButtonB3->buttonMask  : 0)
		| ((values & gamepad->mapButtonB4->pinMask)  ? gamepad->mapButtonB4->buttonMask  : 0)
		| ((values & gamepad->mapButtonL1->pinMask)  ? gamepad->mapButtonL1->buttonMask  : 0)
		| ((values & gamepad->mapButtonR1->pinMask)  ? gamepad->mapButtonR1->buttonMask  : 0)
		| ((values & gamepad->mapButtonL2->pinMask)  ? gamepad->mapButtonL2->buttonMask  : 0)
		| ((values & gamepad->mapButtonR2->pinMask)  ? gamepad->mapButtonR2->buttonMask  : 0)
		| ((values & gamepad->mapButtonS1->pinMask)  ? gamepad->mapButtonS1->buttonMask  : 0)
		| ((values & gamepad->mapButtonS2->pinMask)  ? gamepad->mapButtonS2->buttonMask  : 0)
		| ((values & gamepad->mapButtonL3->pinMask)  ? gamepad->mapButtonL3->buttonMask  : 0)
		| ((values & gamepad->mapButtonR3->pinMask)  ? gamepad->mapButtonR3->buttonMask  : 0)
		| ((values & gamepad->mapButtonA1->pinMask)  ? gamepad->mapButtonA1->buttonMask  : 0)
		| ((values & gamepad->mapButtonA2->pinMask)  ? gamepad->mapButtonA2->buttonMask  : 0)
	;

	gamepad->state.lx = GAMEPAD_JOYSTICK_MID;
	gamepad->state.ly = GAMEPAD_JOYSTICK_MID;
	gamepad->state.rx = GAMEPAD_JOYSTICK_MID;
	gamepad->state.ry = GAMEPAD_JOYSTICK_MID;
	gamepad->state.lt = 0;
	gamepad->state.rt = 0;
}