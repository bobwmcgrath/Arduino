////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_DISPLAY_SSD1331_h
#define _MITOV_DISPLAY_SSD1331_h

#include <Mitov.h>
#include <Mitov_Graphics_Color.h>
#include <Mitov_Display_SSD1306_SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	namespace SSD1331Const
	{
		const uint8_t SSD1331_CMD_DRAWLINE 			= 0x21;
		const uint8_t SSD1331_CMD_DRAWRECT 			= 0x22;
		const uint8_t SSD1331_CMD_FILL 				= 0x26;
		const uint8_t SSD1331_CMD_SETCOLUMN 		= 0x15;
		const uint8_t SSD1331_CMD_SETROW    		= 0x75;
		const uint8_t SSD1331_CMD_CONTRASTA 		= 0x81;
		const uint8_t SSD1331_CMD_CONTRASTB 		= 0x82;
		const uint8_t SSD1331_CMD_CONTRASTC			= 0x83;
		const uint8_t SSD1331_CMD_MASTERCURRENT 	= 0x87;
		const uint8_t SSD1331_CMD_SETREMAP 			= 0xA0;
		const uint8_t SSD1331_CMD_STARTLINE 		= 0xA1;
		const uint8_t SSD1331_CMD_DISPLAYOFFSET 	= 0xA2;
		const uint8_t SSD1331_CMD_NORMALDISPLAY 	= 0xA4;
		const uint8_t SSD1331_CMD_DISPLAYALLON  	= 0xA5;
		const uint8_t SSD1331_CMD_DISPLAYALLOFF 	= 0xA6;
		const uint8_t SSD1331_CMD_INVERTDISPLAY 	= 0xA7;
		const uint8_t SSD1331_CMD_SETMULTIPLEX  	= 0xA8;
		const uint8_t SSD1331_CMD_SETMASTER 		= 0xAD;
		const uint8_t SSD1331_CMD_DISPLAYOFF 		= 0xAE;
		const uint8_t SSD1331_CMD_DISPLAYON     	= 0xAF;
		const uint8_t SSD1331_CMD_POWERMODE 		= 0xB0;
		const uint8_t SSD1331_CMD_PRECHARGE 		= 0xB1;
		const uint8_t SSD1331_CMD_CLOCKDIV 			= 0xB3;
		const uint8_t SSD1331_CMD_PRECHARGEA 		= 0x8A;
		const uint8_t SSD1331_CMD_PRECHARGEB 		= 0x8B;
		const uint8_t SSD1331_CMD_PRECHARGEC 		= 0x8C;
		const uint8_t SSD1331_CMD_PRECHARGELEVEL 	= 0xBB;
		const uint8_t SSD1331_CMD_VCOMH 			= 0xBE;
	}
//---------------------------------------------------------------------------
	template<typename T_IMPLEMENTATION, uint8_t WIDTH, uint8_t HEIGHT> class DisplaySSD1331_Up : public T_IMPLEMENTATION
	{
		typedef DisplaySSD1331_Up<T_IMPLEMENTATION, WIDTH, HEIGHT>	T_SELF;
//		typedef GraphicsImpl<T_SELF, T_TEXT> inherited;

	protected:
		void setAddrWindow(int x1, int y1, int x2, int y2)
		{
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETCOLUMN );
			T_IMPLEMENTATION::SendCommand( x1 );
			T_IMPLEMENTATION::SendCommand( x2 );

			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETROW );
			T_IMPLEMENTATION::SendCommand( y1 );
			T_IMPLEMENTATION::SendCommand( y2 );
		}

	protected:
		static inline bool IsHorizontalBMP() { return true; }

	public:
		static inline int16_t width(void) // const
		{
			return WIDTH;
		}

		static inline int16_t height(void) // const
		{
			return HEIGHT;
		}

	public:
		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return 0x0000; // Not supported
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x < 0) ||(x >= WIDTH ) || (y < 0) || (y >= HEIGHT )) 
				return;

//			Serial.println( uint32_t( color ));

			// check rotation, move pixel around if necessary

			// x is which column
			T_IMPLEMENTATION::SendCommand6( SSD1331Const::SSD1331_CMD_SETCOLUMN, x, x, SSD1331Const::SSD1331_CMD_SETROW, y, y );

			uint8_t AAlpha = color >> 24;
			if( AAlpha != 0xFF )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + 0.5;
				g = float( g ) * ACoefficient + 0.5;
				b = float( b ) * ACoefficient + 0.5;

				color = Func::Color565( r, g, b );
			}

			else
				color = Func::Color565( color );

			T_IMPLEMENTATION::SendData16(color);

//			FModified = true;
    	}

/*
		static void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			inherited::ClearScreen( AImplementation, color ); // TEMPORARY!!!
		}
*/

		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			// rudimentary clipping (drawChar w/big text requires this)
			AImplementation.fillRectImplementation( x, y, w, h, color );
		}

		void fillRectImplementation( int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x >= WIDTH ) || (y >= HEIGHT ))
				return;

			if((x + w - 1) >= WIDTH)
				w = WIDTH - x;

			if((y + h - 1) >= HEIGHT)
				h = HEIGHT - y;

//			Serial.println( x );
//			Serial.println( y );
//			Serial.println( h );
//			Serial.println( w );

			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t AHi = color >> 8, ALo = color;
    
//			BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			for(y=h; y>0; y--)
				for(x=w; x>0; x--)
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			EndTransaction();
		}

		static inline void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			AImplementation.drawFastHLineImplementation( x, y, w, color );
		}

		void drawFastHLineImplementation( int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			if((x+w-1) >= WIDTH )
				w = WIDTH - x;

			color = Func::Color565( color );
			setAddrWindow(x, y, x+w-1, y);

			uint8_t AHi = color >> 8, ALo = color;

//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (w--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			AImplementation.drawFastVLineImplementation( x, y, h, color );
		}

		void drawFastVLineImplementation( int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			if((y+h-1) >= HEIGHT ) 
				h = HEIGHT-y;

			color = Func::Color565( color );
			setAddrWindow(x, y, x, y+h-1);

			uint8_t AHi = color >> 8, ALo = color;
    
//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (h--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			AImplementation.fillRectImplementation( 0, 0, AImplementation.width(), AImplementation.height(), 0xFF000000 | color );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_IMPLEMENTATION, uint8_t WIDTH, uint8_t HEIGHT> class DisplaySSD1331_Down : public T_IMPLEMENTATION
	{
		typedef DisplaySSD1331_Down<T_IMPLEMENTATION, WIDTH, HEIGHT>	T_SELF;

	protected:
		void setAddrWindow(int x1, int y1, int x2, int y2)
		{
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETCOLUMN );
			T_IMPLEMENTATION::SendCommand( x1 );
			T_IMPLEMENTATION::SendCommand( x2 );

			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETROW );
			T_IMPLEMENTATION::SendCommand( y1 );
			T_IMPLEMENTATION::SendCommand( y2 );
		}

	public:
		inline int16_t width(void) const
		{
			return WIDTH;
		}

		inline int16_t height(void) const
		{
			return HEIGHT;
		}

	protected:
		static inline bool IsHorizontalBMP() { return false; }

	public:
		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return 0x0000; // Not supported
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x < 0) ||(x >= WIDTH ) || (y < 0) || (y >= HEIGHT )) 
				return;

//			Serial.println( uint32_t( color ));

			// check rotation, move pixel around if necessary
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;

			// x is which column
			uint8_t AAlpha = color >> 24;
			if( AAlpha != 0xFF )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + 0.5;
				g = float( g ) * ACoefficient + 0.5;
				b = float( b ) * ACoefficient + 0.5;

				color = Func::Color565( r, g, b );
			}

			else
				color = Func::Color565( color );


			T_IMPLEMENTATION::SendCommand6( SSD1331Const::SSD1331_CMD_SETCOLUMN, x, x, SSD1331Const::SSD1331_CMD_SETROW, y, y );

//			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETCOLUMN );
//			T_IMPLEMENTATION::SendCommand( x );
//			T_IMPLEMENTATION::SendCommand( WIDTH - 1 );

//			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETROW );
//			T_IMPLEMENTATION::SendCommand( y );
//			T_IMPLEMENTATION::SendCommand( HEIGHT - 1 );

//			Serial.println( uint32_t( color ));

//			T_IMPLEMENTATION::spiwrite16(color);
			T_IMPLEMENTATION::SendData16(color);

//			FModified = true;
    	}

/*
		static void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			inherited::ClearScreen( AImplementation, color ); // TEMPORARY!!!
		}
*/

		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			// rudimentary clipping (drawChar w/big text requires this)
			AImplementation.fillRectImplementation( x, y, w, h, color );
		}

		void fillRectImplementation( int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

//return;
//			Serial.println( "TEST" );
			if((x >= WIDTH ) || (y >= HEIGHT ))
				return;

			x = WIDTH - x - w;
			y = HEIGHT - y - h;

			if((x + w - 1) >= WIDTH)
				w = WIDTH - x;

			if((y + h - 1) >= HEIGHT)
				h = HEIGHT - y;

			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t AHi = color >> 8, ALo = color;

//			Serial.println( x );
//			Serial.println( y );
//			Serial.println( h );
//			Serial.println( w );
    
//			BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			for(y=h; y>0; y--)
				for(x=w; x>0; x--)
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			EndTransaction();
		}

		static inline void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			AImplementation.drawFastHLineImplementation( x, y, w, color );
		}

		void drawFastHLineImplementation( int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			x = WIDTH - x - w;
			y = HEIGHT - y - 1;

			if((x+w-1) >= WIDTH )
				w = WIDTH - x;

			color = Func::Color565( color );
			setAddrWindow(x, y, x+w-1, y);

			uint8_t AHi = color >> 8, ALo = color;

//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (w--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			AImplementation.drawFastVLineImplementation( x, y, h, color );
		}

		void drawFastVLineImplementation( int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			x = WIDTH - x - 1;
			y = HEIGHT - y - h;

			if((y+h-1) >= HEIGHT ) 
				h = HEIGHT-y;

			color = Func::Color565( color );
			setAddrWindow(x, y, x, y+h-1);

			uint8_t AHi = color >> 8, ALo = color;
    
//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (h--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			AImplementation.fillRectImplementation( 0, 0, AImplementation.width(), AImplementation.height(), 0xFF000000 | color );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_IMPLEMENTATION, uint8_t WIDTH, uint8_t HEIGHT> class DisplaySSD1331_Left : public T_IMPLEMENTATION
	{
		typedef DisplaySSD1331_Left<T_IMPLEMENTATION, WIDTH, HEIGHT>	T_SELF;

	protected:
		void setAddrWindow(int x1, int y1, int x2, int y2)
		{
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETCOLUMN );
			T_IMPLEMENTATION::SendCommand( x1 );
			T_IMPLEMENTATION::SendCommand( x2 );

			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETROW );
			T_IMPLEMENTATION::SendCommand( y1 );
			T_IMPLEMENTATION::SendCommand( y2 );
		}

	public:
		inline int16_t width(void) const
		{
			return WIDTH;
		}

		inline int16_t height(void) const
		{
			return HEIGHT;
		}

	protected:
		static inline bool IsHorizontalBMP() { return false; }

	public:
		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return 0x0000; // Not supported
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x < 0) ||(x >= WIDTH ) || (y < 0) || (y >= HEIGHT )) 
				return;

//			Serial.println( uint32_t( color ));

			// check rotation, move pixel around if necessary
			x = WIDTH - x - 1;
			swap( x, y );

			// x is which column
			uint8_t AAlpha = color >> 24;
			if( AAlpha != 0xFF )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + 0.5;
				g = float( g ) * ACoefficient + 0.5;
				b = float( b ) * ACoefficient + 0.5;

				color = Func::Color565( r, g, b );
			}

			else
				color = Func::Color565( color );


			T_IMPLEMENTATION::SendCommand6( SSD1331Const::SSD1331_CMD_SETCOLUMN, x, x, SSD1331Const::SSD1331_CMD_SETROW, y, y );
			T_IMPLEMENTATION::SendData16(color);
    	}

/*
		static void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			inherited::ClearScreen( AImplementation, color ); // TEMPORARY!!!
		}
*/

		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			// rudimentary clipping (drawChar w/big text requires this)
			AImplementation.fillRectImplementation( x, y, w, h, color );
		}

		void fillRectImplementation( int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x >= WIDTH ) || (y >= HEIGHT ))
				return;

/*
			if((x + w - 1) >= WIDTH)
				w = WIDTH - x;

			y = HEIGHT - y - h;

			if((y + h - 1) >= HEIGHT)
				h = HEIGHT - y;
*/
//			Serial.println( x );
//			Serial.println( y );
//			Serial.println( h );
//			Serial.println( w );

			x = WIDTH - x - w;

			if((x + w - 1) >= WIDTH)
				w = WIDTH - x;

			if((y + h - 1) >= HEIGHT)
				h = HEIGHT - y;

//			Serial.println( x );
//			Serial.println( y );
//			Serial.println( h );
//			Serial.println( w );

			swap( w, h );
			swap( x, y );

			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t AHi = color >> 8, ALo = color;
    
//			BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			for(y=h; y>0; y--)
				for(x=w; x>0; x--)
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			EndTransaction();
		}

		static inline void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			AImplementation.drawFastHLineImplementation( x, y, w, color );
		}

		void drawFastHLineImplementation( int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			x = WIDTH - x - w;

			if((x+w-1) >= WIDTH )
				w = WIDTH - x;

			swap( x, y );

			color = Func::Color565( color );
			setAddrWindow( x, y, x, y+w-1 );

			uint8_t AHi = color >> 8, ALo = color;

//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (w--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			AImplementation.drawFastVLineImplementation( x, y, h, color );
		}

		void drawFastVLineImplementation( int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			if((y+h-1) >= HEIGHT ) 
				h = HEIGHT-y;

			x = WIDTH - x - 1;

			swap( x, y );

			color = Func::Color565( color );
			setAddrWindow(x, y, x+h-1, y);

			uint8_t AHi = color >> 8, ALo = color;
    
//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (h--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			AImplementation.fillRectImplementation( 0, 0, AImplementation.width(), AImplementation.height(), 0xFF000000 | color );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_IMPLEMENTATION, uint8_t WIDTH, uint8_t HEIGHT> class DisplaySSD1331_Right : public T_IMPLEMENTATION
	{
		typedef DisplaySSD1331_Right<T_IMPLEMENTATION, WIDTH, HEIGHT>	T_SELF;

	protected:
		void setAddrWindow(int x1, int y1, int x2, int y2)
		{
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETCOLUMN );
			T_IMPLEMENTATION::SendCommand( x1 );
			T_IMPLEMENTATION::SendCommand( x2 );

			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETROW );
			T_IMPLEMENTATION::SendCommand( y1 );
			T_IMPLEMENTATION::SendCommand( y2 );
		}

	public:
		inline int16_t width(void) const
		{
			return WIDTH;
		}

		inline int16_t height(void) const
		{
			return HEIGHT;
		}

	protected:
		static inline bool IsHorizontalBMP() { return false; }

	public:
		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return 0x0000; // Not supported
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x < 0) ||(x >= WIDTH ) || (y < 0) || (y >= HEIGHT )) 
				return;

			y = HEIGHT - y - 1;
//			x = WIDTH - x - 1;
			swap( x, y );

			// x is which column
			uint8_t AAlpha = color >> 24;
			if( AAlpha != 0xFF )
			{
				float ACoefficient = float( AAlpha ) / 255.0f;
				uint8_t	r = color >> 16;
				uint8_t	g = color >> 8;
				uint8_t	b = color;

//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

				r = float( r ) * ACoefficient + 0.5;
				g = float( g ) * ACoefficient + 0.5;
				b = float( b ) * ACoefficient + 0.5;

				color = Func::Color565( r, g, b );
			}

			else
				color = Func::Color565( color );


			T_IMPLEMENTATION::SendCommand6( SSD1331Const::SSD1331_CMD_SETCOLUMN, x, x, SSD1331Const::SSD1331_CMD_SETROW, y, y );

//			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETCOLUMN );
//			T_IMPLEMENTATION::SendCommand( x );
//			T_IMPLEMENTATION::SendCommand( WIDTH - 1 );

//			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETROW );
//			T_IMPLEMENTATION::SendCommand( y );
//			T_IMPLEMENTATION::SendCommand( HEIGHT - 1 );

//			Serial.println( uint32_t( color ));

//			T_IMPLEMENTATION::spiwrite16(color);
			T_IMPLEMENTATION::SendData16(color);

//			FModified = true;
    	}

/*
		static void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			inherited::ClearScreen( AImplementation, color ); // TEMPORARY!!!
		}
*/

		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			// rudimentary clipping (drawChar w/big text requires this)
			AImplementation.fillRectImplementation( x, y, w, h, color );
		}

		void fillRectImplementation( int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			if((x >= WIDTH ) || (y >= HEIGHT ))
				return;

			if((x + w - 1) >= WIDTH)
				w = WIDTH - x;

			if((y + h - 1) >= HEIGHT)
				h = HEIGHT - y;

			y = HEIGHT - y - h;

			if((x + w - 1) >= WIDTH)
				w = WIDTH - x;

			if((y + h - 1) >= HEIGHT)
				h = HEIGHT - y;

//			Serial.println( x );
//			Serial.println( y );
//			Serial.println( h );
//			Serial.println( w );

			swap( w, h );
			swap( x, y );


			setAddrWindow(x, y, x+w-1, y+h-1);

			color = Func::Color565( color );

			uint8_t AHi = color >> 8, ALo = color;
    
//			BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			for(y=h; y>0; y--)
				for(x=w; x>0; x--)
					T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			EndTransaction();
		}

		static inline void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			AImplementation.drawFastHLineImplementation( x, y, w, color );
		}

		void drawFastHLineImplementation( int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			y = HEIGHT - y - 1;

			if((x+w-1) >= WIDTH )
				w = WIDTH - x;

			swap( x, y );

			color = Func::Color565( color );
			setAddrWindow(x, y, x, y+w-1);

			uint8_t AHi = color >> 8, ALo = color;

//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (w--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			AImplementation.drawFastVLineImplementation( x, y, h, color );
		}

		void drawFastVLineImplementation( int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			if( !( color & 0xFF000000 ) )
				return;

			// Rudimentary clipping
			if((x >= WIDTH ) || (y >= HEIGHT )) 
				return;

			if((y+h-1) >= HEIGHT ) 
				h = HEIGHT-y;

			y = HEIGHT - y - h;
//			x = WIDTH - x - 1;
			swap( x, y );

			color = Func::Color565( color );
			setAddrWindow(x, y, x+h-1, y);

			uint8_t AHi = color >> 8, ALo = color;
    
//			T_IMPLEMENTATION::BeginTransaction();
			T_IMPLEMENTATION::SetRegisterSelectHigh();
			T_IMPLEMENTATION::SetChipSelectLow();
			while (h--) 
				T_IMPLEMENTATION::spiwrite( AHi, ALo );

			T_IMPLEMENTATION::SetChipSelectHigh();
//			T_IMPLEMENTATION::EndTransaction();
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			AImplementation.fillRectImplementation( 0, 0, AImplementation.width(), AImplementation.height(), 0xFF000000 | color );
		}

	};
//---------------------------------------------------------------------------
	template<
        typename T_IMPLEMENTATION,
        uint8_t C_PIXEL_MODE
    > class DisplaySSD1331 :
        public T_IMPLEMENTATION,
        public GraphicsImpl<
            DisplaySSD1331<
                        T_IMPLEMENTATION,
                        C_PIXEL_MODE
                    >, 
                typename T_IMPLEMENTATION::TYPE_T_BACKGROUND_COLOR,
				typename T_IMPLEMENTATION::TYPE_T_FONT,
				typename T_IMPLEMENTATION::TYPE_T_TEXT
            >
	{
		typedef DisplaySSD1331<T_IMPLEMENTATION, C_PIXEL_MODE>	T_SELF;
		typedef T_IMPLEMENTATION inherited;
		typedef GraphicsImpl<
                T_SELF,
                typename T_IMPLEMENTATION::TYPE_T_BACKGROUND_COLOR,
				typename T_IMPLEMENTATION::TYPE_T_FONT,
				typename T_IMPLEMENTATION::TYPE_T_TEXT
            > T_GRAPHICS;

	public:
		void SetPowerOn( bool AValue )
		{
			if( T_IMPLEMENTATION::PowerOn().GetValue() == AValue )
				return;

			T_IMPLEMENTATION::PowerOn() = AValue;
			if( T_IMPLEMENTATION::PowerOn().GetValue() )
				DoPowerOn();

			else
				DoPowerOff();
		}

	public:
		void UpdateContrast()
		{
//			T_IMPLEMENTATION::SendCommand( SSD1306Const::SSD1306_SETCONTRAST );
//			T_IMPLEMENTATION::SendCommand( Contrast * 255 + 0.5 );

			// Temporary hardcoded
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_CONTRASTA );  	// 0x81
			T_IMPLEMENTATION::SendCommand( 0x91 );
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_CONTRASTB );  	// 0x82
			T_IMPLEMENTATION::SendCommand( 0x50 );
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_CONTRASTC );  	// 0x83
			T_IMPLEMENTATION::SendCommand( 0x7D );
		}

	protected:
/*
		inline void BeginTransaction()
		{
#if defined (SPI_HAS_TRANSACTION)
//			C_SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
#endif
		}

		inline void EndTransaction()
		{
#if defined (SPI_HAS_TRANSACTION)
//			C_SPI.endTransaction();
#endif
		}
*/
		void SendCommands( const uint8_t ACommands[], int ASize ) 
		{
			for( int i = 0; i < ASize; ++ i )
				T_IMPLEMENTATION::SendCommand( ACommands[ i ] );
		}

	protected:
		void DoPowerOff()
		{
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_DISPLAYOFF );
			delay( 100 );
		}

		void DoPowerOn()
		{
			T_IMPLEMENTATION::Reset();

			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_DISPLAYOFF );  	// 0xAE
			T_IMPLEMENTATION::SendCommand( SSD1331Const::SSD1331_CMD_SETREMAP ); 	// 0xA0

//			T_IMPLEMENTATION::SendCommand( C_PIXEL_MODE | 0b00001000 ); 	// 0xA0
//			T_IMPLEMENTATION::SendCommand( C_PIXEL_MODE | 0b00010000 ); 	// 0xA0
//			T_IMPLEMENTATION::SendCommand( C_PIXEL_MODE | 0b00000001 ); 	// 0xA0 // Rotated
//			T_IMPLEMENTATION::SendCommand( C_PIXEL_MODE | 0b00010000 ); 	// 0xA0
			T_IMPLEMENTATION::SendCommand( C_PIXEL_MODE ); 	// 0xA0

			static const uint8_t InitCommands[] =
			{
				SSD1331Const::SSD1331_CMD_STARTLINE, 	// 0xA1
				0x0,
				SSD1331Const::SSD1331_CMD_DISPLAYOFFSET, 	// 0xA2
				0x0,
				SSD1331Const::SSD1331_CMD_NORMALDISPLAY,  	// 0xA4
				SSD1331Const::SSD1331_CMD_SETMULTIPLEX, 	// 0xA8
				0x3F,  			// 0x3F 1/64 duty
				SSD1331Const::SSD1331_CMD_SETMASTER,  	// 0xAD
				0x8E,
				SSD1331Const::SSD1331_CMD_POWERMODE,  	// 0xB0
				0x0B,
				SSD1331Const::SSD1331_CMD_PRECHARGE,  	// 0xB1
				0x31,
				SSD1331Const::SSD1331_CMD_CLOCKDIV,  	// 0xB3
				0xF0,  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
				SSD1331Const::SSD1331_CMD_PRECHARGEA,  	// 0x8A
				0x64,
				SSD1331Const::SSD1331_CMD_PRECHARGEB,  	// 0x8B
				0x78,
				SSD1331Const::SSD1331_CMD_PRECHARGEA,  	// 0x8C
				0x64,
				SSD1331Const::SSD1331_CMD_PRECHARGELEVEL,  	// 0xBB
				0x3A,
				SSD1331Const::SSD1331_CMD_VCOMH,  		// 0xBE
				0x3E,
				SSD1331Const::SSD1331_CMD_MASTERCURRENT,  	// 0x87
				0x06,
				SSD1331Const::SSD1331_CMD_CONTRASTA,  	// 0x81
				0x91,
				SSD1331Const::SSD1331_CMD_CONTRASTB,  	// 0x82
				0x50,
				SSD1331Const::SSD1331_CMD_CONTRASTC,  	// 0x83
				0x7D,
				SSD1331Const::SSD1331_CMD_DISPLAYON	//--turn on oled panel    
			};

			SendCommands( ARRAY_PARAM( InitCommands ) );

//			T_IMPLEMENTATION::SendPinsConfigCommand();
			UpdateContrast();

//			T_IMPLEMENTATION::SendCommand(SSD1306Const::SSD1306_SETPRECHARGE);                  // 0xd9
//			T_IMPLEMENTATION::SendCommand( ( uint8_t( PreChargePeriod.Phase2 ) << 4 ) | PreChargePeriod.Phase1 );

//			T_GRAPHICS::setTextSize( T_IMPLEMENTATION::Text().Size() );
//			T_GRAPHICS::setTextColor( T_IMPLEMENTATION::Text().Color(), T_IMPLEMENTATION::Text().BackgroundColor() );
//			T_GRAPHICS::setTextWrap( T_IMPLEMENTATION::Text().Wrap() );
			

//			FDisplay.begin( SSD1306Const::SSD1306_SWITCHCAPVCC, Address, false );
//			FValue = InitialValue;

//			drawPixel( 20, 20, WHITE );

			inherited::ClearScreen( *this, Func::SwapRB( T_GRAPHICS::BackgroundColor() ) );

			T_GRAPHICS::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );

//			display();
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "Test1" );

			T_IMPLEMENTATION::IntSystemInitStart();
		}

		inline void SystemStart()
		{
			if( T_IMPLEMENTATION::PowerOn() )
				DoPowerOn();

//			T_IMPLEMENTATION::IntSystemInitEnd();
		}

	public:
		template<typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return T_GRAPHICS::write( AImplementation, AConfig, AValue );
		}

		template<typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template<typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template<typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_GRAPHICS::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template<typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &T_GRAPHICS::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template<typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template<typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}
        
	public:
		static void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			T_IMPLEMENTATION::ClearScreen( AImplementation, color );
		}

		static inline void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			T_IMPLEMENTATION::fillRect( AImplementation, x, y, w, h, color );
		}

		static inline void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color)
		{
			T_IMPLEMENTATION::drawFastHLine( AImplementation, x, y, w, color );
		}

		static inline void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			T_IMPLEMENTATION::drawFastVLine( AImplementation, x, y, h, color );
		}

		static void drawColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint32_t AGreen = pgm_read_byte( bitmap ++ );
					uint32_t ABlue = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGreen << 8 ) | ( ARed << 16 ) | ABlue );

					AImplementation.spiwrite( AColor >> 8, AColor );
				}

			AImplementation.w_data_end();
		}

		static void drawColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint32_t AGreenBlue = pgm_read_byte( bitmap ++ );

					AImplementation.spiwrite( ARedGreen, AGreenBlue );
				}

			AImplementation.w_data_end();
		}

		static void drawRepeatColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawRepeatColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawRepeatColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint32_t AGreen = pgm_read_byte( bitmap ++ );
				uint32_t ABlue = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( ARed << 16 ) | ( AGreen << 8 ) | ABlue );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.spiwrite( AHi, ALo );

			}

			AImplementation.w_data_end();
		}

		static void drawRepeatColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawRepeatColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawRepeatColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.spiwrite( ARedGreen, AGreenBlue );

			}

			AImplementation.w_data_end();
		}

		static void drawRepeatAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint8_t	r = pgm_read_byte( bitmap++ );
				uint8_t	g = pgm_read_byte( bitmap++ );
				uint8_t	b = pgm_read_byte( bitmap++ );
				uint8_t AAlpha = pgm_read_byte( bitmap++ );

				uint8_t	AHi, ALo;

				if( AAlpha == 0xFF )
				{
					uint16_t AColor = Func::Color565( r, g, b );
					AHi = ( AColor >> 8 );
					ALo = AColor;
				}

				else if( AAlpha == 0 )
				{
					AHi = 0;
					ALo = 0;
				}

				else
				{
					float ACoefficient = float( AAlpha ) / 255.0f;

					r = float( r ) * ACoefficient + 0.5;
					g = float( g ) * ACoefficient + 0.5;
					b = float( b ) * ACoefficient + 0.5;

					uint16_t AColor = Func::Color565( r, g, b );
					AHi = ( AColor >> 8 );
					ALo = AColor;
				}

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.spiwrite( AHi, ALo );

			}

			AImplementation.w_data_end();
		}

		static void drawRepeatAlphaColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawRepeatAlphaColorBitmap_565( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
				uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );
				uint8_t AAlpha = pgm_read_byte( bitmap ++ );

				uint32_t r = ARedGreen & 0b11111000;
				uint32_t b = 0xFF & ( AGreenBlue << 3 );
				uint32_t g = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

				uint8_t AHi, ALo;

				if( AAlpha == 0xFF )
				{
					uint16_t AColor = Func::Color565( r, g, b );
					AHi = AColor >> 8;
					ALo = AColor;
				}

				else if( AAlpha == 0 )
				{
					AHi = 0;
					ALo = 0;
				}

				else
				{
					float ACoefficient = float( AAlpha ) / 255.0f;

					r = float( r ) * ACoefficient + 0.5;
					g = float( g ) * ACoefficient + 0.5;
					b = float( b ) * ACoefficient + 0.5;

					uint16_t AColor = Func::Color565( r, g, b );
					AHi = AColor >> 8;
					ALo = AColor;
				}

				for( int16_t j = 0; j <= ACount; ++ j )
					AImplementation.spiwrite( AHi, ALo );

			}

			AImplementation.w_data_end();
		}

		static void drawAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARGBColor = pgm_read_dword( bitmap++ );
					uint8_t AAlpha = ARGBColor >> 24;
					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( ARGBColor );
						AImplementation.spiwrite( AColor >> 8, AColor );
					}

					else if( AAlpha == 0 )
						AImplementation.spiwrite( 0, 0 );

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;
						uint8_t	r = ARGBColor >> 16;
						uint8_t	g = ARGBColor >> 8;
						uint8_t	b = ARGBColor;

		//				Serial.print( OldR ); Serial.print( " " );Serial.print( OldG ); Serial.print( " " );Serial.println( OldB );

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.spiwrite( AColor >> 8, AColor );
					}
				}

			AImplementation.w_data_end();
		}

		static void drawAlphaColorBitmap_565( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			if( ! T_IMPLEMENTATION::IsHorizontalBMP() )
			{
				T_GRAPHICS::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( y + h > AHeight ))
			{
				T_GRAPHICS::drawAlphaColorBitmap_565( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.SendCommand( 0x2C );
			AImplementation.StartSendData();

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint8_t ARedGreen = pgm_read_byte( bitmap ++ );
					uint8_t AGreenBlue = pgm_read_byte( bitmap ++ );
					uint8_t AAlpha = pgm_read_byte( bitmap ++ );

					uint32_t r = ARedGreen & 0b11111000;
					uint32_t b = 0xFF & ( AGreenBlue << 3 );
					uint32_t g = 0xFF & (( ARedGreen << 5 ) | ( AGreenBlue >> 3 ));

					if( AAlpha == 0xFF )
					{
						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.spiwrite( AColor >> 8, AColor );
					}

					else if( AAlpha == 0 )
						AImplementation.spiwrite( 0, 0 );

					else
					{
						float ACoefficient = float( AAlpha ) / 255.0f;

						r = float( r ) * ACoefficient + 0.5;
						g = float( g ) * ACoefficient + 0.5;
						b = float( b ) * ACoefficient + 0.5;

						uint16_t AColor = Func::Color565( r, g, b );
						AImplementation.spiwrite( AColor >> 8, AColor );
					}
				}

			AImplementation.w_data_end();
		}

	public:
		using T_GRAPHICS::T_GRAPHICS;

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_DISPLAY_SSD1331_h
