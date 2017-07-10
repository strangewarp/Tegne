
// Number of uint8_ts in each savefile
const uint32_t FILE_uint8_tS PROGMEM = 393265;

// Buffer of empty uint8_ts, for writing into a newly-emptied tick en masse
const uint8_t EMPTY_TICK[9] PROGMEM = {0, 0, 0, 0, 0, 0, 0, 0};

// Glyph: BASENOTE
const uint8_t GLYPH_BASENOTE[7] PROGMEM = {
	B11100010,
	B10010010,
	B11100010,
	B10010110,
	B10010110,
	B11100000
};

// Glyph: BPM
const uint8_t GLYPH_BPM[7] PROGMEM = {
	B01111110,
	B11001011,
	B10011001,
	B10011001,
	B11000011,
	B01111110
};

// Glyph: CHANNEL
const uint8_y GLYPH_CHAN[7] PROGMEM = {
	B11101001,
	B10101001,
	B10001111,
	B10001001,
	B10101001,
	B11101001
};

// Glyph: CLOCK-MASTER
const uint8_t GLYPH_CLOCKMASTER[7] PROGMEM = {
	B11101001,
	B10101010,
	B10001100,
	B10001010,
	B10101001,
	B11101001
};

// Glyph: DURATION
const uint8_t GLYPH_DURATION[7] PROGMEM = {
	B11001110,
	B10101001,
	B10101110,
	B10101011,
	B10101001,
	B11001001
};

// Glyph: "ERASING"
const uint8_t GLYPH_ERASE[7] PROGMEM = {
	B11110101,
	B10000101,
	B11110101,
	B10000101,
	B10000000,
	B11110101,
};

// Glyph: HUMANIZE
const uint8_t GLYPH_HUMANIZE[7] PROGMEM = {
	B10101001,
	B10101001,
	B11101001,
	B10101001,
	B10101001,
	B10100110
};

// Glyph: LISTEN-CHAN
const uint8_t GLYPH_LISTEN[7] PROGMEM = {
	B10001110,
	B10001010,
	B10001000,
	B10001000,
	B10001010,
	B11101110
};

// Glyph: OCTAVE
const uint8_t GLYPH_OCTAVE[7] PROGMEM = {
	B01100111,
	B10010101,
	B10010100,
	B10010100,
	B10010101,
	B01100111
};

// Glyph: QUANTIZE
const uint8_t GLYPH_QUANTIZE[7] PROGMEM = {
	B01100010,
	B10010010,
	B10010010,
	B10010110,
	B01100110,
	B00110000
};

// Glyph: "RECORDING"
const uint8_t GLYPH_RECORDING[7] PROGMEM = {
	B11100101,
	B10010101,
	B11100101,
	B10010101,
	B10010000,
	B10010101
};

// Glyph: "SWITCH RECORDING-SEQUENCE"
const uint8_t GLYPH_RSWITCH[7] PROGMEM = {
	B11100111,
	B10010100,
	B11100111,
	B10010001,
	B10010001,
	B10010111
};

// Glyph: "SAVE"
const uint8_t GLYPH_SAVE[7] PROGMEM = {
	B01110101,
	B10000101,
	B01100101,
	B00010101,
	B00010000,
	B11100101
};

// Glyph: "SD-CARD"
const uint8_t GLYPH_SD[7] PROGMEM = {
	B11101110,
	B10001001,
	B11101001,
	B00101001,
	B00101010,
	B11101100
};

// Glyph: "SEQ-SIZE"
const uint8_t GLYPH_SIZE[7] PROGMEM = {
	B11101111,
	B10000001,
	B11100010,
	B00100100,
	B00101000,
	B11101111
};

// Glyph: VELOCITY
const uint8_t GLYPH_VELO[7] PROGMEM = {
	B10010111,
	B10010100,
	B10010111,
	B10100100,
	B11000100,
	B10000111
};

