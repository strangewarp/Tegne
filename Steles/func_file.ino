

// Check whether savefiles exist, and create whichever ones don't exist
void createFiles() {

	char name[8]; // Will hold a given filename

	for (byte i = 0; i < 168; i++) { // For every song-slot...

		if (!(i % 21)) { // After a certain amount of files, switch to the next logo letter
			for (byte row = 0; row < 7; row++) { // For each row in the 7-row-tall logo text...
				// Set the corresponding row to the corresponding letter slice
				sendRow(row + 1, pgm_read_byte_near(LOGO + (row * 4) + ((i / 21) % 4)));
			}
		}

		getFilename(name, i); // Get the filename that corresponds to this song-slot

		if (sd.exists(name)) { continue; } // If the file exists, skip the file-creation process for this filename

		file.createContiguous(sd.vwd(), name, FILE_BYTES); // Create a contiguous file
		file.close(); // Close the newly-created file

		file.open(name, O_WRITE); // Open the file explicitly in WRITE mode

		file.seekSet(FILE_BPM_BYTE); // Go to the BPM-byte
		file.write(byte(80)); // Write a default BPM value of 80
		file.seekSet(FILE_SGRAN_BYTE); // Go to the SWING-GRANULARITY byte
		file.write(byte(1)); // Write a default SWING-GRANULARITY value of 1
		file.seekSet(FILE_SAMOUNT_BYTE); // Go to the SWING AMOUNT byte
		file.write(byte(64)); // Write a default SWING-AMOUNT of 64

		for (byte j = FILE_SQS_START; j <= FILE_SQS_END; j++) { // For every seq-size byte in the header...
			file.seekSet(j); // Go to that byte's position
			file.write(byte(8)); // Write a default sequence-size value of 8
		}

		file.close(); // Close the file

	}

}

// Get the name of a target song-slot's savefile (format: "000.DAT" etc.)
void getFilename(char source[], byte fnum) {
	source[0] = char(fnum >= 100) + 48;
	source[1] = (char(floor(fnum / 10)) % 10) + 48;
	source[2] = char((fnum % 10) + 48);
	source[3] = 46;
	source[4] = 68;
	source[5] = 65;
	source[6] = 84;
	source[7] = 0;
}

// Update a given byte within a given song-file.
// This is used only to update various bytes in the header, so "byte" is acceptable.
void updateFileByte(byte pos, byte b) {
	file.seekSet(pos); // Go to the given insert-point
	file.write(b); // Write the new byte into the file
	file.sync(); // Make sure the changes are recorded
}

// Check whether a given header-byte is a duplicate, and if not, update it in the savefile.
// This is used only to update various bytes in the header, so "byte" is acceptable.
void updateNonMatch(byte pos, byte b) {
	file.seekSet(pos); // Go to the corresponding byte in the savefile
	if (file.read() != b) { // Read the header-byte. If it isn't the same as the given byte-value...
		updateFileByte(pos, b); // Put the local header-byte into its corresponding savefile header-byte
	}
}

// Update the savefile's SWING-data, if it has been changed
void updateSwingVals() {
	updateNonMatch(FILE_SGRAN_BYTE, SGRAN); // Save the SWING GRANULARITY value, if it has been changed
	updateNonMatch(FILE_SAMOUNT_BYTE, SAMOUNT); // ^ Same, but for SWING AMOUNT
}

// Update the sequence's size-byte in the savefile, if it has been changed
void updateSeqSize() {
	updateNonMatch(FILE_SQS_START + RECORDSEQ, STATS[RECORDSEQ] & 63);
}

// Put the current prefs-related global vars into a given buffer
void makePrefBuf(byte buf[]) {
	buf[0] = PAGE;
	buf[1] = OCTAVE;
	buf[2] = VELO;
	buf[3] = HUMANIZE;
	buf[4] = CHAN;
	buf[5] = QRESET;
	buf[6] = QUANTIZE;
	buf[7] = DURATION;
	buf[8] = COPYPOS;
	buf[9] = COPYSEQ;
	buf[10] = SONG;
	buf[11] = CLOCKMASTER;
}

// Get the prefs-file's filename out of PROGMEM
void getPrefsFilename(char pn[]) {
	for (byte i = 0; i < 6; i++) { // For each letter in the filename...
		pn[i] = pgm_read_byte_near(PREFS_FILENAME + i); // Get that letter out of PROGMEM
	}
}

// Write the current relevant global vars into PRF.DAT
void writePrefs() {

	file.close(); // Temporarily close the current song-file

	byte buf[13]; // Make a buffer that will contain all the prefs
	makePrefBuf(buf); // Fill it with all the relevant pref values

	byte cbuf[13]; // Make a buffer for checking the old prefs-contents

	char pn[6]; // Will contain the prefs-file's filename
	getPrefsFilename(pn); // Get the prefs-file's filename out of PROGMEM

	file.open(pn, O_RDWR); // Open the prefs-file in read-write mode

	file.seekSet(0); // Ensure we're on the first byte of the file
	file.read(cbuf, 12); // Read the old prefs-values

	for (byte i = 0; i < 12; i++) { // For every byte in the prefs-file...
		if (buf[i] != cbuf[i]) { // If the new byte doesn't match the old byte...
			file.seekSet(i); // Go to the byte's location
			file.write(buf[i]); // Replace the old byte
		}
	}

	file.close(); // Close the prefs-file

	char name[8];
	getFilename(name, SONG); // Get the name of the current song-file

	file.open(name, O_RDWR); // Reopen the current song-file

	file.seekSet(FILE_BPM_BYTE); // Go to the BPM-byte's location
	if (file.read() != BPM) { // If the BPM-byte doesn't match the current BPM...
		updateFileByte(FILE_BPM_BYTE, BPM); // Update the BPM-byte in the song's savefile
	}

}

// Load the contents of the preferences file ("PRF.DAT"), and put its contents into global variables
void loadPrefs() {

	byte buf[13]; // Make a buffer that will contain all the prefs

	char pn[6]; // Will contain the prefs-file's filename
	getPrefsFilename(pn); // Get the prefs-file's filename out of PROGMEM

	if (!sd.exists(pn)) { // If the prefs-file doesn't exist, create the file

		makePrefBuf(buf); // Fill the prefs-data buffer with the current user-prefs

		file.createContiguous(sd.vwd(), pn, 12); // Create a contiguous prefs-file
		file.close(); // Close the newly-created file

		file.open(pn, O_WRITE); // Create a prefs file and open it in write-mode
		file.write(buf, 12); // Write the pref vars into the file

	} else { // Else, if the prefs-file does exist...

		file.open(pn, O_READ); // Open the prefs-file in read-mode
		file.read(buf, 12); // Read all the prefs-bytes into the buffer

		// Assign the buffer's-worth of pref-bytes to their respective global-vars
		PAGE = buf[0];
		OCTAVE = buf[1];
		VELO = buf[2];
		HUMANIZE = buf[3];
		CHAN = buf[4];
		QRESET = buf[5];
		QUANTIZE = buf[6];
		DURATION = buf[7];
		COPYPOS = buf[8];
		COPYSEQ = buf[9];
		SONG = buf[10];
		CLOCKMASTER = buf[11];

	}

	file.close(); // Close the prefs file, regardless of whether it already exists or was just created

}

// Load a given song, or create its savefile if it doesn't exist
void loadSong(byte slot) {

	if (file.isOpen()) { // If a savefile is already open...
		file.close(); // Close it
	}

	char name[8];
	getFilename(name, slot); // Get the name of the target song-slot's savefile

	file.open(name, O_RDWR); // Open the new savefile

	file.seekSet(FILE_BPM_BYTE); // Go to the BPM-byte location
	BPM = file.read(); // Read it
	if (BPM == 255) { // If the BPM is set to an erroneous value (this is a known bug that occurs on startup for an unknown reason)...
		restart(); // Restart the entire device
	}

	file.seekSet(FILE_SGRAN_BYTE); // Go to the SWING GRANULARITY byte
	SGRAN = file.read(); // Read it
	file.seekSet(FILE_SAMOUNT_BYTE); // Go to the SWING AMOUNT byte
	SAMOUNT = file.read(); // Read it

	byte sbuf[49]; // Create a buffer for incoming STATS bytes, to save read-time
	file.seekSet(FILE_SQS_START); // Go to the file-header's SEQ-SIZE block
	file.read(sbuf, 48); // Read every seq's new size
	for (byte i = 0; i < 48; i++) { // For each sequence...
		POS[i] %= sbuf[i] * 16; // Wrap each seq's previous position by its new length
		STATS[i] = (STATS[i] & 128) | sbuf[i]; // Combine the new size-value with the seq's current on/off byte
	}

	memset(CMD, 0, 48); // Clear every seq's CUED-COMMANDS...
	memset(SCATTER, 0, 48); // ...And SCATTER-values

	updateTickSize(); // Update the internal tick-size (in microseconds) to match the new BPM value

	SPART = 0; // Set the current SWING PART to 0, as all timing elements are being reset
	//CUR16 = 127; // Set the global cue-position to arrive at 1 immediately
	//ELAPSED = 0; // Reset the elapsed-time-since-last-16th-note counter

	SONG = slot; // Set the currently-active SONG-position to the given save-slot

	LOADHOLD = 9500; // Give LOADHOLD around 2/3 of a second of decay, so the song-number is displayed for that long

	TO_UPDATE = 255; // Flag entire GUI for an LED-update

}
