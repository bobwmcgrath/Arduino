////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BUTTON_h
#define _MITOV_BUTTON_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace M5StackYun
	{
		const uint8_t	YUN_ADDR		= 0x38;
		const uint8_t	LED_REG_ADDR    = 0x01;
		const uint8_t	LIGHT_REG_ADDR  = 0x00;
	}
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin
	> class M5StackYunLightI2C : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{		
			ReadSensor( false );
		}

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			Wire.beginTransmission( M5StackYun::YUN_ADDR );   // Initialize the Tx buffer
			Wire.write( M5StackYun::LIGHT_REG_ADDR );            // Put slave register address in Tx buffer
			Wire.endTransmission( true );

			Wire.requestFrom( M5StackYun::YUN_ADDR, uint8_t( 2 ));
			if( Wire.available() )
			{
				uint16_t AValue16 = Wire.read();
				AValue16 |= uint16_t( Wire.read() ) << 8;
//				Serial.println( AValue16 );

				float AValue = 1 - ( float( AValue16 ) / 0x0FFF );

				T_OutputPin::SetPinValue( AValue, AChangeOnly );
			}
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		uint8_t	C_INDEX,
		typename T_InitialValue
	> class M5StackYunLEDElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart()
		{
			Wire.beginTransmission( M5StackYun::YUN_ADDR );
			Wire.write( M5StackYun::LED_REG_ADDR );
			Wire.write( C_INDEX );

			if( Enabled() )
			{
				Wire.write( uint8_t( InitialValue().GetValue() >> 16 ));
				Wire.write( uint8_t( InitialValue().GetValue() >> 8 ));
				Wire.write( uint8_t( InitialValue().GetValue() ));
			}

			else
			{
				for( int i = 0; i < 3; ++i )
					Wire.write( 0 );
			}
  
			Wire.endTransmission();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetValue() )
				return;

			InitialValue() = AColor;
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
	class M5StackYunLEDsI2C
	{
	public:
		void AllInputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;

			Wire.beginTransmission( M5StackYun::YUN_ADDR );
			Wire.write( M5StackYun::LED_REG_ADDR );
			Wire.write( 14 );


			Wire.write( AColor.Red );
			Wire.write( AColor.Green );
			Wire.write( AColor.Blue );

			Wire.endTransmission();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
