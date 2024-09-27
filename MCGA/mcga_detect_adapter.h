/**
 *
 *  @brief     
 *  @details   ~
 *  @author    Jeremy Thornton
 *  @date      4.11.2023
 *  @copyright © Jeremy Thornton, 2023. All right reserved.
 *
 */
#ifndef 

/*

Checking the Interrupt 15h, (AH)=0xC0 for the BIOS ROM configuration table would yield the following information, since the MCGA video adapter was only implemented on the IBM PS/2 models 25, 25L, and 30.

Model    Submdl   Rev     BIOS date       System
********************************************************
FAh       00h     00h     09/02/86        PS/2 Model 30 (8 MHz 8086)
FAh       00h     01h     12/12/86        PS/2 Model 30
FAh       00h     02h     02/05/87        PS/2 Model 30
FAh       01h     00h     06/26/87        PS/2 Model 25/25L (8 MHz 8086)

Machine Identification Byte

		  Model  Sub-Model		Machine

		FF	??	Original IBM PC  4/24/81
			??	IBM PC	10/19/81
			??	IBM PC	10/27/82
		FE	??	IBM XT (Original)
			??	IBM portable PC
			??	Compaq DeskPro
		FD	??	PCjr
		FC	??	IBM AT (6 MHz)
			01	IBM AT 3x9 (8 MHz)
			02	IBM XT 286
			04	IBM PS/2 Model 50
			05	IBM PS/2 Model 60
			0B	IBM PS/1
		FB	00	IBM 256/640K XT (aka XT/2)
		FA	00	IBM PS/2 Model 30
			01	IBM PS/2 Model 25
		F9	00	IBM PC Convertible
		F8	00	IBM PS/2 Model 80  (16 MHz)
			01	IBM PS/2 Model 80  (20 MHz)
			04	IBM PS/2 Model 70  (20 MHz)
			09	IBM PS/2 Model 70  (16 MHz)
		B6	??	Hewlett Packard 110
		9A	??	Compaq Plus
		86		XT (BIOS 11/82+) & AT (BIOS 1/84+) (see note)
		80		PC & PCjr (see note)
		2D	??	Compaq PC


	- found at memory location F000:FFFE or via INT 15,C0
	- model values of 80 & 86 are returned by BIOS versions previous
	  to the PS/2 but after the BIOS dates marked

	- see	INT 15,C0

*/