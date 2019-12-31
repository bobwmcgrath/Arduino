////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Mitov
{
//---------------------------------------------------------------------------
	const uint8_t PROGMEM C_7SegmentDigitSegments[16] =
	{
		0b1111110,  // = 0
		0b0110000,  // = 1
		0b1101101,  // = 2
		0b1111001,  // = 3
		0b0110011,  // = 4
		0b1011011,  // = 5
		0b1011111,  // = 6
		0b1110000,  // = 7
		0b1111111,  // = 8
		0b1111011,  // = 9
		0b1110111,  // = A
		0b0011111,  // = B
		0b0001101,  // = C
		0b0111101,  // = D
		0b1001111,  // = E
		0b1000111   // = F
	};
//---------------------------------------------------------------------------
	const uint8_t C_7SegmentDigitDecimalPoint[2] =
	{
		0b00000000,
		0b10000000
	};
//---------------------------------------------------------------------------
}

