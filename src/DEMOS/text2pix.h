#ifndef TEXT2PIX_H
#define TEXT2PIX_H

/**
* @brief    Demonstrates using retrolib libraries and the coding idioms for them
* @details  Code as a Creative Medium 001
* Visualize the patterns in text layout of a document by condensing characters into pixels.
* Converts a text file's characters to white pixels, punctuation to black pixels and newlines to blank rows of pixels.
* HGA is 720 x 348 pixels, so read upto 720 bytes from the text file at a time and then convert to pixels
*/
int demo_text2pix(int argc, char** argv);

#endif
