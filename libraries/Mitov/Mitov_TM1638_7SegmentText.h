////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>
#include <SPI.h>

namespace Mitov
{
// map of ASCII values/table to 7-segment, offset to position 32. 
const  PROGMEM uint8_t C_TM1638_7SegmentsText[] = 
{
  0x86, // !
  0x22, // "
  0x7E, // #
  0x6D, // $
  0xD2, // %
  0x46, // &
  0x20, // '
  0x29, // (
  0x0B, // )
  0x21, // *
  0x70, // +
  0x10, // ,
  0x40, // -
  0x80, // .
  0x52, // /
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x09, // :
  0x0D, // ;
  0x61, // <
  0x48, // =
  0x43, // >
  0xD3, // ?
  0x5F, // @
  0x77, // A
  0x7C, // B
  0x39, // C
  0x5E, // D
  0x79, // E
  0x71, // F
  0x3D, // G
  0x76, // H
//  0x1110110, // H
//  0b0110111, // H
  0x30, // I
  0x1E, // J
  0x75, // K
  0x38, // L
  0x15, // M
  0x37, // N
  0x3F, // O
  0x73, // P
  0x6B, // Q
  0x33, // R
  0x6D, // S
  0x78, // T
  0x3E, // U
  0x3E, // V
  0x2A, // W
  0x76, // X
  0x6E, // Y
  0x5B, // Z
  0x39, // [
  0x64, // \ //
  0x0F, // ]
  0x23, // ^
  0x08, // _
  0x02, // `
  0x5F, // a
  0x7C, // b
  0x58, // c
  0x5E, // d
  0x7B, // e
  0x71, // f
  0x6F, // g
  0x74, // h
  0x10, // i
  0x0C, // j
  0x75, // k
  0x30, // l
  0x14, // m
  0x54, // n
  0x5C, // o
  0x73, // p
  0x67, // q
  0x50, // r
  0x6D, // s
  0x78, // t
  0x1C, // u
  0x1C, // v
  0x14, // w
  0x76, // x
  0x6E, // y
  0x5B, // z
  0x46, // {
  0x30, // |
  0x70, // }
  0x01, // ~
  0x00 // (del)
};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t	C_COUNT_DIGITS,
		uint8_t	C_START_DIGIT,
		typename T_Enabled,
		typename T_InitialValue
	> class TM1638_TextDisplay7Segments :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		uint8_t GetSegmentsValue( int ADigit, Mitov::String AInitialValue )
		{
			int ARealDigit = C_COUNT_DIGITS - ADigit - 1;
			if( ARealDigit >= AInitialValue.length() )
				return 0;

			byte AValue = AInitialValue[ ARealDigit ];

			if( AValue <= ' ' )
				return 0;

			if( AValue > 126 )
				return 0;

//			byte AResult = C_TM1638_7SegmentsText[ AValue ];
			byte AResult = pgm_read_byte( C_TM1638_7SegmentsText + AValue - ' ' - 1 );
			if( AResult & 0x80 )
				return AResult;

			if( ARealDigit + 1 >= AInitialValue.length() )
				return AResult;

			if( AInitialValue[ ARealDigit + 1 ] != '.' )
				return AResult;

			++ARealDigit;
			AResult |= 0x80;

			return AResult;
		}

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( int ADigit = 0; ADigit < C_COUNT_DIGITS; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled() )
					AValue = GetSegmentsValue( C_COUNT_DIGITS - ADigit - 1, InitialValue() );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				C_OWNER.SetDigitSegments( C_START_DIGIT + ADigit, AValue );
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AValue = (char *)_Data;
			if( InitialValue().GetValue() == AValue )
				return;

			InitialValue() = AValue;
//			Serial.println( InitialValue );
			UpdateDisplay();
		}

	public:
		inline void SystemStart()
		{
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
}

