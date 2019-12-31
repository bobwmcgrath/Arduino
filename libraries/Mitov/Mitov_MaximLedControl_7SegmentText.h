////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_MAXIM_LED_CONTROL_7SEGMENT_TEXT_h
#define _MITOV_MAXIM_LED_CONTROL_7SEGMENT_TEXT_h

#include <Mitov.h>
#include <SPI.h>
#include <Mitov_7SegmentText.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_INSTANCE,
		uint16_t C_COUNT_PIXELS,
		uint16_t C_START_PIXEL,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_ReversedOrder
	> class MaximLedGroupTextDisplay7Segments :
		public T_Enabled,
		public T_InitialValue,
		public T_ReversedOrder
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( ReversedOrder )

	protected:
		uint8_t GetSegmentsValue( int &ADigit, Mitov::String AInitialValue )
		{
			int ARealDigit = ( C_COUNT_PIXELS / 8 ) - ADigit - 1;
			if( ARealDigit >= AInitialValue.length() )
				return 0;

			byte AValue = AInitialValue[ ARealDigit ];

			if( AValue <= ' ' )
				return 0;

			if( AValue > 126 )
				return 0;

//			byte AResult = C_7SegmentsText[ AValue ];
			byte AResult = pgm_read_byte( C_7SegmentsText + AValue - ' ' - 1 );
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

//	protected:
//		virtual uint8_t GetSegmentsValue( int &ADigit ) = 0;

	public:
		void UpdateDisplay() // Used by Enabled setter!
		{
			for( int ADigit = 0; ADigit < C_COUNT_PIXELS / 8; ++ ADigit )
			{
				uint8_t AValue;

				if( Enabled() )
					AValue = GetSegmentsValue( ADigit, InitialValue() );

				else
					AValue = 0;

//				Serial.print( "ADigit = " ); Serial.println( ADigit );
//				Serial.print( "AValue = " ); Serial.println( AValue, HEX );
//				Serial.println( "" );

				if( ReversedOrder() )
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ( ( C_COUNT_PIXELS / 8 ) - ADigit - 1 ) * 8 + i, ( AValue >> i ) & 1 );
				}

				else
				{
					for( int i = 0; i < 8; ++i )
						C_INSTANCE.SetPixelValue( C_START_PIXEL + ADigit * 8 + i, ( AValue >> i ) & 1 );
				}
			}
		}

	public:
		void InputPin_o_Receive( void *_Data )
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

#endif
