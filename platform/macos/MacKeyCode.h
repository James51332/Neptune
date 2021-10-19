#pragma once

#include "core/KeyCode.h"

namespace Neptune
{

// Array of key tranlations
// Credit to https://eastmanreference.com/complete-list-of-applescript-key-codes
static KeyCode s_KeyTranslations[]
{
	KeyA, KeyS, KeyD, KeyF, KeyH, KeyG, KeyZ, KeyX, 															  							// 0x00 - 0x07
  KeyC, KeyV, KeyNone, KeyB, KeyQ, KeyW, KeyE, KeyR, 														  							// 0x08 - 0x0f
  KeyY, KeyT, Key1, Key2, Key3, Key4, Key6, Key5, 															  							// 0x10 - 0x17
  KeyEqual, Key9, Key7, KeyMinus, Key8, Key0, KeyRightBracket, KeyO, 						  							// 0x18 - 0x1f
  KeyU, KeyLeftBracket, KeyI, KeyP, KeyEnter, KeyL, KeyJ, KeyApostrophe,				  							// 0x20 - 0x27
  KeyK, KeySemicolon, KeyBackslash, KeyComma, KeySlash, KeyN, KeyM, KeyPeriod, 	  							// 0x28 - 0x2f
  KeyTab, KeySpace, KeyGraveAccent, KeyBackspace, KeyNone, KeyEscape, KeyNone, KeyNone,					// 0x30 - 0x37
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0x38 - 0x3f
  KeyF17, KeyPadDecimal, KeyNone, KeyPadMultiply, KeyNone, KeyPadAdd, KeyNone, KeyNone,					// 0x40 - 0x47
  KeyNone, KeyNone, KeyNone, KeyPadDivide, KeyEnter, KeyNone, KeyPadSubtract, KeyF18, 					// 0x48 - 0x4f
  KeyF19, KeyPadEqual, KeyPad0, KeyPad1, KeyPad2, KeyPad3, KeyPad4, KeyPad5, 			  						// 0x50 - 0x57
  KeyPad6, KeyPad7, KeyF20, KeyPad8, KeyPad9, KeyNone, KeyNone, KeyNone, 			  								// 0x58 - 0x5f
  KeyF5, KeyF6, KeyF7, KeyF3, KeyF8, KeyF9, KeyNone, KeyF11, 			  														// 0x60 - 0x67
  KeyNone, KeyF13, KeyF16, KeyF14, KeyNone, KeyF10, KeyNone, KeyF12, 			  										// 0x68 - 0x6f
  KeyNone, KeyF15, KeyNone, KeyHome, KeyPageUp, KeyDelete, KeyF4, KeyEnd, 			  							// 0x70 - 0x77
  KeyF2, KeyPageDown, KeyF1, KeyLeft, KeyRight, KeyDown, KeyUp, KeyNone, 						  					// 0x78 - 0x7f
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0x80 - 0x87
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0x88 - 0x8f
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0x90 - 0x97
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0x98 - 0x9f
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xa0 - 0xa7
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xa8 - 0xaf
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xb0 - 0xb7
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xb8 - 0xbf
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xc0 - 0xc7
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xc8 - 0xcf
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xd0 - 0xd7
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xd8 - 0xdf
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xe0 - 0xe7
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xe8 - 0xef
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xf0 - 0xf7
  KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, KeyNone, 			  							// 0xf8 - 0xff
};

} // namespace Neptune
