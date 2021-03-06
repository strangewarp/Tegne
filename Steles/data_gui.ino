

// Holds which bits in the button-var correspond to which raw notes, for any given grid-configuration.
// (Within a given block in this array: first value = top left; each rightward row of note-values here represents a physical downward column of buttons)
const byte GRIDS[] PROGMEM = {

	0,  1,  2,  3,  4,  5,  // Config 0: Guitar-like, with horizontally adjacent fourths
	5,  6,  7,  8,  9,  10,
	10, 11, 12, 13, 14, 15,
	15, 16, 17, 18, 19, 20,

	5,  4,  3,  2,  1,  0,  // Config 1: Reverse-guitar-like, with horizontally adjacent fourths
	10, 9,  8,  7,  6,  5,
	15, 14, 13, 12, 11, 10,
	20, 19, 18, 17, 16, 15,

	0,  1,  2,  3,  4,  5,  // Config 2: Guitar-like, with horizontally adjacent tritones
	6,  7,  8,  9,  10, 11,
	12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23,

	5,  4,  3,  2,  1,  0,  // Config 3: Reverse-guitar-like, with horizontally adjacent tritones
	11, 10, 9,  8,  7,  6,
	17, 16, 15, 14, 13, 12,
	23, 22, 21, 20, 19, 18,

	20, 16, 12, 8,  4,  0,  // Config 4: Ascending grid, bottom-left to top-right
	21, 17, 13, 9,  5,  1,
	22, 18, 14, 10, 6,  2,
	23, 19, 15, 11, 7,  3,

	0,  4,  8,  12, 16, 20, // Config 5: Descending grid, top-left to bottom-right
	1,  5,  9,  13, 17, 21,
	2,  6,  10, 14, 18, 22,
	3,  7,  11, 15, 19, 23,

};

// Matrix of note-buttons that correspond to CHAIN DIRECTION commands:
// 0 = up-left
// 1 = up
// 2 = up-right
// 3 = left
// 4 = right
// 5 = down-left
// 6 = down
// 7 = down-right
// 255 = "clear the chain"
const byte CHAIN_MATRIX[] PROGMEM = {
	0,    1,   1,   2,
	3,  255, 255,   4,
	3,  255, 255,   4,
	3,  255, 255,   4,
	3,  255, 255,   4,
	5,    6,   6,   7
};

// Seq-position offsets that correspond to each CHAIN-DIRECTION.
// These will be processed in the form of: ((seq + offset) % 24) + ((seq > 23) * 24)
const byte CHAIN_OFFSET[] PROGMEM = {
	19, // Up-left
	20, // Up
	21, // Up-right
	23, // Left
	1,  // Right
	3,  // Down-left
	4,  // Down
	5   // Down-right
};

// Glyphs for each BLINK state (0 to 11: notes. 12: full block)
const byte GLYPH_BLINK[] PROGMEM = {
	B01100000,
	B10000000,
	B10000000,
	B01100000,
	B00000000, // C

	B00110000,
	B01000000,
	B01000000,
	B00110000, // C#
	B10000000,

	B11000000,
	B10100000,
	B10100000,
	B11000000, // D
	B00000000,

	B01100000,
	B01010000,
	B01010000, // D#
	B01100000,
	B10000000,

	B11100000,
	B11000000,
	B10000000,
	B11100000, // E
	B00000000,

	B11100000,
	B10000000,
	B11000000,
	B10000000, // F
	B00000000,

	B01110000,
	B01000000,
	B01100000, // F#
	B01000000,
	B10000000,

	B01100000,
	B10000000,
	B10100000,
	B11100000, // G
	B00000000,

	B00110000,
	B01000000,
	B01010000, // G#
	B01110000,
	B10000000,

	B01000000,
	B10100000,
	B11100000,
	B10100000, // A
	B00000000,

	B00100000,
	B01010000,
	B01110000, // A#
	B01010000,
	B10000000,

	B10000000,
	B11000000,
	B10100000,
	B11100000, // B
	B00000000,

	B11110000,
	B11110000,
	B11110000,
	B11110000, // Full block
	B11110000

};

// Number multiglyph: holds all numbers (0-9)
const byte NUMBER_GLYPHS[] PROGMEM = {

	B00000000, // 0
	B11100000,
	B10100000,
	B10100000,
	B10100000,
	B11100000,

	B00000000, // 1
	B01000000,
	B11000000,
	B01000000,
	B01000000,
	B11100000,

	B00000000, // 2
	B11100000,
	B00100000,
	B01000000,
	B10000000,
	B11100000,

	B00000000, // 3
	B11100000,
	B00100000,
	B11100000,
	B00100000,
	B11100000,

	B00000000, // 4
	B10100000,
	B10100000,
	B11100000,
	B00100000,
	B00100000,

	B00000000, // 5
	B11100000,
	B10000000,
	B11100000,
	B00100000,
	B11100000,

	B00000000, // 6
	B11100000,
	B10000000,
	B11100000,
	B10100000,
	B11100000,

	B00000000, // 7
	B11100000,
	B00100000,
	B01000000,
	B10000000,
	B10000000,

	B00000000, // 8
	B11100000,
	B10100000,
	B11100000,
	B10100000,
	B11100000,

	B00000000, // 9
	B11100000,
	B10100000,
	B11100000,
	B00100000,
	B11100000

};

// Standalone glyph: AUTOCMD
const byte GLYPH_AUTOCMD[] PROGMEM = {
	B00000000,
	B10000100,
	B11001010,
	B11101110,
	B11001010,
	B10001010
};

// Standalone glyph: ERASE NOTES
const byte GLYPH_ERASE[] PROGMEM = {
	B00000000,
	B11101010,
	B10001010,
	B11101010,
	B10000000,
	B11101010
};

// Standalone glyph: ERASE INVERSE NOTES
const byte GLYPH_ERASE_INVERSE[] PROGMEM = {
	B00000000,
	B10101110,
	B10101000,
	B10101110,
	B00001000,
	B10101110
};

// Standalone glyph: LOAD
const byte GLYPH_LOAD[] PROGMEM = {
	B00000000,
	B10000000,
	B10000000,
	B10000000,
	B10000000,
	B11100000,
};

// Standalone glyph: RESET TIMING
const byte GLYPH_RESET_TIMING[] PROGMEM = {
	B00000000,
	B11100111,
	B10010010,
	B11100010,
	B10010010,
	B10010010,
};

// Multiglyph array: holds all straightforward command-glyphs
const byte GLYPHS[] PROGMEM = {

	// Glyph: blank
	0, 0, 0, 0, 0, 0,

  // Glyph: AUTOCMD CURSOR-MOVE (note: this is a dummy-glyph)
  0, 0, 0, 0, 0, 0,
  
  // Glyph: AUTOCMD-LOAD DELETE
  B00000000,
  B10001100,
  B11001010,
  B11101010,
  B11001010,
  B10001100,
  
  // Glyph: AUTOCMD-LOAD INSERT
  B00000000,
  B10001110,
  B11000100,
  B11100100,
  B11000100,
  B10001110,
  
  // Glyph: ACTIVE ON LOAD
  B00000000,
  B10000100,
  B10000110,
  B10111111,
  B10000110,
  B11100100,
  
	// Glyph: ARM RECORDNOTES
	B00000000,
	B11100000,
	B10010000,
	B11100000,
	B10010000,
	B10010000,

	// Glyph: ARPEGGIATOR MODE
	B00000000,
	B01001011,
	B01001011,
	B01011010,
	B11011010,
	B11000010,

	// Glyph: ARPEGGIATOR REFRESH
	B00000000,
	B01001110,
	B10101001,
	B11101110,
	B10101001,
	B10101001,

	// Glyph: CHAIN DIRECTION (note: this is a dummy-glyph)
  0, 0, 0, 0, 0, 0,

	// Glyph: CHANNEL
	B00000000,
	B11101000,
	B10101000,
	B10001100,
	B10101010,
	B11101010,

	// Glyph: CLEAR NOTES
	B00000000,
	B10001001,
	B01010001,
	B00100001,
	B01010011,
	B10001011,

	// Glyph: DURATION
	B00000000,
	B11000000,
	B10100000,
	B10101110,
	B10101000,
	B11001000,

	// Glyph: DURATION-HUMANIZE
	B00000000,
	B11001010,
	B10101010,
	B10101110,
	B10101010,
	B11001010,

	// Glyph: GENERIC (should not be used)
	B00000000,
	B11001110,
	B10101000,
	B11001110,
	B10100010,
	B11001110,

	// Glyph: GRID-CONFIG
	B00000000,
	B11101110,
	B10001000,
	B11001000,
	B10101000,
	B11101110,

	// Glyph: HUMANIZE
	B00000000,
	B10100000,
	B10100000,
	B11101010,
	B10101010,
	B10101110,

	// Glyph: OCTAVE
	B00000000,
	B11100000,
	B10100000,
	B10101110,
	B10101000,
	B11101110,

	// Glyph: OFFSET
	B00000000,
	B00001100,
	B00001000,
	B01001010,
	B11011011,
	B01011010,

	// Glyph: SHIFT POSITION
	B00000000,
	B00100100,
	B01100110,
	B11100111,
	B01100110,
	B00100100,

	// Glyph: QUANTIZE-RESET
	B00000000,
	B11100111,
	B10100101,
	B10100110,
	B10100101,
	B11110101,

	// Glyph: QUANTIZE
	B00000000,
	B11100000,
	B10100000,
	B10100101,
	B10100101,
	B11110111,

	// Glyph: REPEAT
	B00000000,
	B00101110,
	B00101001,
	B00101110,
	B01101001,
	B01101001,

	// Glyph: REPEAT-SWEEP
	B00000000,
	B11100111,
	B10010100,
	B11100111,
	B10010001,
	B10010111,

	// Glyph: SEQ-SIZE
	B00000000,
	B11101110,
	B10000010,
	B01000100,
	B00101000,
	B11101110,

	// Glyph: SWITCH RECORDING-SEQUENCE
	B00000000,
	B11101110,
	B10101000,
	B11000100,
	B10100010,
	B10101110,

	// Glyph: TEMPO
	B00000000,
	B01010001,
	B01011011,
	B01010101,
	B11010001,
	B11010001,

	// Glyph: TRACK
	B00000000,
	B11100000,
	B01000000,
	B01001110,
	B01001000,
	B01001000,

	// Glyph: UPPER CHAN BITS
	B00000000,
	B10101110,
	B10101010,
	B10101000,
	B10101010,
	B11101110,

	// Glyph: VELO
	B00000000,
	B10010000,
	B10010000,
	B01010000,
	B00110000,
	B00010000

};
