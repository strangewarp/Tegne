
// Reset every sequence
void resetAllSeqs() {
	memset(SEQ_CMD, 0, sizeof(SEQ_CMD));
	memset(SEQ_POS, 0, sizeof(SEQ_POS));
}

// Reset a seq's cued-commands, playing-byte, and tick-position
void resetSeq(uint8_c s) {
	SEQ_CMD[s] = 0;
	SEQ_POS[s] = 0;
}

// Send MIDI-OFF commands for all currently-sustained notes
void haltAllSustains() {
	for (uint8_c i = 0; i < (SUST_COUNT * 3); i++) { // For every active sustain...
		Serial.write(SUST[n + 1]); // Send a premature NOTE-OFF for the sustain
		Serial.write(SUST[n + 2]); // ^
		Serial.write(127); // ^
	}
	SUST_COUNT = 0;
}

// Process one 16th-note worth of duration for all notes in the SUSTAIN system
void processSustains() {
	uint8_c n = 0; // Value to iterate through each sustain-position
	while (n < (SUST_COUNT * 3)) { // For every sustain-entry in the SUST array...
		if (SUST[n]) { // If any duration remains...
			SUST[n]--; // Reduce the duration by 1
			n += 3; // Move on to the next sustain-position normally
		} else { // Else, if the remaining duration is 0...
			Serial.write(SUST[n + 1]); // Send a NOTE-OFF for the sustain
			Serial.write(SUST[n + 2]); // ^
			Serial.write(127); // ^
			if (n < 21) { // If this isn't the lowest sustain-slot...
				memmove(SUST + n, SUST + n + 3, 24 - (n + 3)); // Move all lower sustains one slot upwards
			}
			SUST_COUNT--; // Reduce the sustain-count
			// n doesn't need to be increased here, since the next sustain occupies the same index now
		}
	}
}

// Send all outgoing MIDI commands in a single burst
void flushNoteOns() {
	if (!MOUT_COUNT) { return; } // If there are no commands in the NOTE-ON buffer, exit the function
	Serial.write(MOUT, MOUT_COUNT * 3); // Send all outgoing MIDI-command bytes at once
	MOUT_COUNT = 0; // Clear the MIDI buffer's counting-byte
}

// Compare a seq's CUE-commands to the global CUE-point, and parse them if the timing is correct
void parseCues(uint8_c s, uint16_c size) {

	if (
		(!SEQ_CMD[s]) // If the seq has no cued commands...
		|| (CUR16 != ((SEQ_CMD[s] & B11100000) >> 1)) // Or the global 16th-note doesn't correspnd to the seq's cue-point...
	) { return; } // ...Exit the function

	// Enable or disable the sequence's playing-bit
	SEQ_STATS[s] = (SEQ_STATS[s] & B01111111) | ((SEQ_CMD[s] & 2) << 6);

	// Set the sequence's internal tick to a position based on the incoming SLICE bits
	SEQ_POS[s] = (size >> 3) * ((SEQ_CMD[s] & B00011100) >> 2);

	// Flag the sequence's LED-row for an update
	TO_UPDATE |= (s % 24) >> 2;

}

// Get the notes from the current tick in a given seq, and add them to the MIDI-OUT buffer
void getTickNotes(uint8_c s) {
	if (MOUT_COUNT == 8) { return; } // If the MIDI-OUT queue is full, exit the function
	file.seekSet(49 + SEQ_POS[s] + (s * 8192)); // Navigate to the note's absolute position
	file.read(buf, 8); // Read the data of the tick's notes
	for (uint8_c bn = 0; bn < 8; bn += 4) { // For each of the two note-slots on a given tick...
		if (
			(!buf[bn + 3]) // If the note doesn't exist...
			|| (MOUT_COUNT == 8) // Or the MIDI buffer is full...
		) { break; } // Don't add any notes to the MIDI buffer
		uint8_c pos = MOUT_COUNT * 3; // Get the lowest empty MOUT location
		memcpy(MOUT + pos, buf + bn + 1, 3); // Copy the note into the MIDI buffer
		MOUT_COUNT++; // Increase the counter that tracks the number of bytes in the MIDI buffer
		if (SUST_COUNT == 8) { // If the SUSTAIN buffer is already full...
			Serial.write(SUST[23]); // Send a premature NOTE-OFF for the oldest active sustain
			Serial.write(SUST[24]); // ^
			Serial.write(127); // ^
			SUST_COUNT--; // Reduce the number of active sustains by 1
		}
		memmove(SUST, SUST + 3, (SUST_COUNT * 3)); // Move all sustains one space downward
		memcpy(SUST, buf + bn, 3); // Create a new sustain corresponding to this note
		SUST[1] ^= 16; // Turn the new sustain's NOTE-ON into a NOTE-OFF preemptively
		SUST_COUNT++; // Increase the number of active sustains by 1
	}
}

// Advance global tick, and iterate through all currently-active sequences
void iterateAll() {

	processSustains(); // Process one 16th-note's worth of duration for all sustained notes

	if (PLAYING) { // If the sequencer is currently in PLAYING mode...

		uint8_c buf[7]; // Buffer for reading note-events from the datafile

		for (uint8_c i = 0; i < 48; i++) { // For every loaded sequence...

			uint16_c seq16 = (SEQ_STATS[i] & 127) << 4; // Get seq's absolute size, in 16th-notes

			parseCues(i, seq16); // Parse a sequence's cued commands, if any

			// If the seq isn't currently playing, go to the next seq's iteration-routine
			if (!(SEQ_STATS[i] & 128)) { continue; }

			// If RECORD MODE is active, and the ERASE-NOTES command is being held,
			// and notes are being recorded into this seq...
			if (RECORDMODE && ERASENOTES && (RECORDNOTES == i)) {
				file.seekSet(49 + SEQ_POS[i] + (i * 8192)); // Set position to start of tick's first note
				file.write(EMPTY_TICK, 8); // Write in an entire empty tick's worth of bytes
			} else { // Else, if any other combination of states applies...
				getTickNotes(i); // Get the notes from this tick in a given seq, and add them to the MIDI-OUT buffer
			}

			// Increase the seq's 16th-note position by one increment, wrapping it around its top limit
			SEQ_POS[i] = (SEQ_POS[i] + 1) % seq16;

		}

	}

	if (MOUT_COUNT) { // If any notes are in the MIDI-OUT buffer...
		flushNoteOns(); // Send them all at once
	}

}
