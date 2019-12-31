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
		typename T_FCurrentIndex
	> class TexasInstrumentsTCA9548A : 
		public T_Address,
		public T_FCurrentIndex
	{
	public:
		_V_PROP_( Address )

	protected:
		_V_PROP_( FCurrentIndex )

	public:
		void SetIndex( uint8_t AIndex )
		{
			if( FCurrentIndex().GetValue() == AIndex )
				return;
			
//			Serial.println( AIndex );

			C_I2C.beginTransmission( uint8_t( Address().GetValue() ));
			C_I2C.write( uint8_t( 1 ) << AIndex );
			C_I2C.endTransmission();

			FCurrentIndex() = AIndex;
//			Serial.println( FCurrentIndex().GetValue() );
		}

		inline static T_I2C &GetI2C() { return C_I2C; }

	public:
		inline TexasInstrumentsTCA9548A()
		{
			FCurrentIndex() = 8;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX
	> class I2CSplitterChannel
	{
	public:
		inline void beginTransmission( uint8_t AAddress )
		{
			C_OWNER.SetIndex( C_INDEX );
			C_OWNER.GetI2C().beginTransmission( AAddress );
		}

		inline uint8_t endTransmission(void)
		{
			return C_OWNER.GetI2C().endTransmission();
		}

		inline uint8_t endTransmission( uint8_t AValue )
		{
			return C_OWNER.GetI2C().endTransmission( AValue );
		}

		inline uint8_t requestFrom( uint8_t AAddress, uint8_t ACount )
		{
			C_OWNER.SetIndex( C_INDEX );
			return C_OWNER.GetI2C().requestFrom( AAddress, ACount );
		}

		inline size_t write( uint8_t AValue )
		{
			return C_OWNER.GetI2C().write( AValue );
		}

		inline size_t write(const uint8_t *AValue, size_t ASize )
		{
			return C_OWNER.GetI2C().write( AValue, ASize );
		}

		inline int available(void)
		{
			return C_OWNER.GetI2C().available();
		}

		inline int read(void)
		{
			return C_OWNER.GetI2C().read();
		}

		inline int peek(void)
		{
			return C_OWNER.GetI2C().peek();
		}

		inline void flush(void)
		{
			C_OWNER.GetI2C().flush();
		}

		inline size_t write(unsigned long n)
		{
			return C_OWNER.GetI2C().write( n );
		}

		inline size_t write(long n)
		{
			return C_OWNER.GetI2C().write( n );
		}

		inline size_t write(unsigned int n)
		{
			return C_OWNER.GetI2C().write( n );
		}

		inline size_t write(int n)
		{
			return C_OWNER.GetI2C().write( n );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

