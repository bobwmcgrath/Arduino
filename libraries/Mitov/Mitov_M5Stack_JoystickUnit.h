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

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_I2C, T_I2C &C_I2C,
		typename T_Address,
		typename T_ButtonOutputPin,
		typename T_Enabled,
		typename T_OutputPins_X,
		typename T_OutputPins_Y
	> class M5StackJoystickUnitI2C : 
		public T_Address,
		public T_ButtonOutputPin,
		public T_Enabled,
		public T_OutputPins_X,
		public T_OutputPins_Y
	{
	public:
		_V_PIN_( OutputPins_X )
		_V_PIN_( OutputPins_Y )
		_V_PIN_( ButtonOutputPin )

	public:
		_V_PROP_( Address )
		_V_PROP_( Enabled )

	protected:
		void ReadSensors( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
				return;

			C_I2C.requestFrom( Address().GetValue(), 3 );
			if( ! C_I2C.available() )
				return;

			float x_data = float( C_I2C.read() ) / 255;
			float y_data = float( C_I2C.read() ) / 255;
			bool button_data = ( C_I2C.read() != 0 );

			T_OutputPins_X::SetPinValue( x_data, AChangeOnly );
			T_OutputPins_Y::SetPinValue( y_data, AChangeOnly );
			T_ButtonOutputPin::SetPinValue( button_data, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadSensors( false );
		}

	public:
		inline void SystemStart()
		{
			ReadSensors( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensors( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

