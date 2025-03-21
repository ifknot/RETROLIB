/**
* @brief    Demonstrates using retrolib libraries and the coding idioms for them
* @details  Code as a Creative Medium 001
* Visualize the patterns in text layout of a document by condensing characters into pixels.
* Converts a text file's characters to white pixels, punctuation to black pixels and newlines to blank rows of pixels.
* HGA is 720 x 348 pixels, so read upto 720 bytes from the text file at a time and then convert to pixels
*/
#ifndef PIXELATE_H
#define PIXELATE_H

typedef enum tokens {
  TOK_NO_PIXEL,
  TOK_PIXEL,
  TOK_LF,
  TOK_CR,
  TOK_UNDEFINED
} token_t;


/**
* @brief tokenize character no_pixel, pixel, new_line, etc...
* @note assumes IBM codepage 437 after 128
*/
token_t tokenize_character(char c);

/**
* pixelate a text file according to decision function above
*/
int pixelate(int argc, char** argv);

#endif
