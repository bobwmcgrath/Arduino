////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ARDUINO_SPI_h
#define _MITOV_ARDUINO_SPI_h

#include <Mitov.h>
#include <SPI.h>

namespace Mitov
{
	template<SPIClass &ISPI> class ArduinoSPI
	{
	public:
		inline uint16_t transfer16(uint16_t data)
		{
			return ISPI.transfer16( data );
		}

		inline uint8_t transfer(uint8_t data)
		{
//			Serial.println( data, HEX );
			return ISPI.transfer( data );
		}

		inline void transfer(void *buf, size_t count)
		{
#if defined(VISUINO_ESP8266) || defined(VISUINO_ESP32)
			ISPI.writeBytes( (uint8_t*)buf, count );
#else // VISUINO_ESP8266
			ISPI.transfer( buf, count );
#endif // VISUINO_ESP8266
		}

		inline void beginTransaction(SPISettings settings)
		{
		#if defined(SPI_HAS_TRANSACTION)
			ISPI.beginTransaction( settings );
		#endif
		}

		inline void endTransaction()
		{
		#if defined(SPI_HAS_TRANSACTION)
			ISPI.endTransaction();
		#endif
		}

		inline void setDataMode( uint8_t dataMode )
		{
			ISPI.setDataMode( dataMode );
		}

/*
		inline void setBitOrder( uint8_t bitOrder )
		{
			ISPI.setBitOrder( bitOrder );
		}
*/
		inline void setClockDivider( uint8_t clockDiv )
		{
			ISPI.setClockDivider( clockDiv );
		}

		inline void usingInterrupt( uint8_t interruptNumber )
		{
#if ! ( defined(VISUINO_ESP8266) || defined(VISUINO_ESP32) )
			ISPI.usingInterrupt( interruptNumber );
#endif
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SPI, T_SPI &C_SPI,
		typename T_ChipSelectOutputPin
	> class InterfaceSPI :
		public T_ChipSelectOutputPin
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	protected:
		inline void Init()
		{
			T_ChipSelectOutputPin::SetPinValue( true );
		}

		void WriteRegister8( uint8_t reg, uint8_t value )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValue( false );
//			C_SPI.transfer( reg & ( ~0x80 ) ); // write, bit 7 low
			C_SPI.transfer( reg ); // write, bit 7 low
			C_SPI.transfer( value );
//			C_SPI.transfer( reg | 0x80 | 0x40 );
			T_ChipSelectOutputPin::SetPinValue( true );
			C_SPI.endTransaction();
		}

		uint8_t ReadRegister8( uint8_t reg )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValue( false );
			C_SPI.transfer( reg | 0x80 ); // write, bit 7 low
			uint8_t AValue = C_SPI.transfer( 0 );
			T_ChipSelectOutputPin::SetPinValue( true );
			C_SPI.endTransaction();

			return AValue;
		}

		void StartReadRegisters( uint8_t reg, uint8_t ACount )
		{
			C_SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0) );
			T_ChipSelectOutputPin::SetPinValue( false );
			C_SPI.transfer( reg | 0x80 | 0x40 ); // write, bit 7 low
		}

		inline uint8_t ReadUInt8()
		{
			return C_SPI.transfer( 0xFF );
		}

		void EndReadRegisters()
		{
			T_ChipSelectOutputPin::SetPinValue( true );
			C_SPI.endTransaction();
		}
	};
//---------------------------------------------------------------------------
}
#endif
