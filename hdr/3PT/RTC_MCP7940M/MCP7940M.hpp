#pragma once

#include <stdint.h>
#include <avr/io/twi.hpp>
#include <std/string.hpp>

namespace AVR
{
	namespace Ext
	{
		class RTC_MCP7940M
		{
			static constexpr uint8_t s_addr = 0x6F;
			const AVR::TWI m_interface;

			using i2c = AVR::TWITransmission;

		public:
			struct DateTime
			{
			};

		public:
			constexpr RTC_MCP7940M(const AVR::TWI &interface)
				: m_interface(interface)
			{
			}

			inline void setCompiledDateTime() const
			{
				uint8_t seconds = 0x80 | (__TIME__[6] - '0') << 4 | (__TIME__[7] - '0');
				uint8_t minutes = 0x00 | (__TIME__[3] - '0') << 4 | (__TIME__[4] - '0');
				uint8_t hours = 0x00 | (__TIME__[0] - '0') << 4 | (__TIME__[1] - '0');

				uint8_t date = (__DATE__[4] - '0') << 4 | (__DATE__[5] - '0');
				uint8_t month = 0;
				switch (__DATE__[0])
				{
				case 'J':
					if (__DATE__[1] == 'a')
						month = 0x01;
					if (__DATE__[1] == 'u')
					{
						if (__DATE__[2] == 'l')
							month = 0x07;
						if (__DATE__[2] == 'n')
							month = 0x06;
					}
					break;
				case 'F':
					month = 0x02;
					break;
				case 'M':
					if (__DATE__[2] == 'r')
						month = 0x03;
					if (__DATE__[2] == 'y')
						month = 0x05;
					break;
				case 'A':
					if (__DATE__[1] == 'p')
						month = 0x04;
					if (__DATE__[1] == 'u')
						month = 0x08;
					break;
				case 'S':
					month = 0x09;
					break;
				case 'O':
					month = 0x10;
					break;
				case 'N':
					month = 0x11;
					break;
				case 'D':
					month = 0x12;
					break;

				default:
					break;
				}

				uint8_t year = (__DATE__[10] - '0') << 4 | (__DATE__[9] - '0');
				m_interface.beginTransmission(s_addr)
					.write(0x00)
					.write(seconds)
					.write(minutes)
					.write(hours)
					.write(0)
					.write(date)
					.write(month)
					.write(year);
			}

			inline void setDateTime(const DateTime &time) const
			{
			}

			inline void setAlarm0(const DateTime &time) const
			{
			}
			inline void setAlarm1(const DateTime &time) const
			{
			}

			inline uint8_t secondsBCD() const
			{
				uint8_t seconds;
				i2c con = m_interface.beginTransmission(s_addr);
				con
					.write(0x00)	   //addr of seconds
					.beginRepeat(true) //read
					.read(seconds, false);
				return seconds & ~0x80;
			}
			inline uint8_t minutesBCD() const
			{
				uint8_t minutes;
				i2c con = m_interface.beginTransmission(s_addr);
				con
					.write(0x01)		   //addr of minutes
					.beginRepeat(true)	   //read
					.read(minutes, false); //end read
				return minutes & ~0x80;
			}

			inline uint8_t hoursBCD() const
			{
				uint8_t hours;
				i2c con = m_interface.beginTransmission(s_addr);
				con
					.write(0x02)		 //addr of minutes
					.beginRepeat(true)	 //read
					.read(hours, false); //end read
				return hours & ~0xC0;
			}

			inline std::fixed_string timeStr(bool seconds = true) const
			{
				uint8_t hours;
				i2c con = m_interface.beginTransmission(s_addr);
				con
					.write(0x02)
					.beginRepeat(true)
					.read(hours, false);
				bool _24h = ~hours & 0x40;

				auto hex = [](uint8_t nibble) -> uint8_t {if(nibble<0x0A) return '0'+nibble; else return 'A'+nibble-0x0A; };

				uint8_t len = 5;
				if (seconds)
					len += 3;
				if (!_24h)
					len += 3;

				std::fixed_string str(len);

				hours &= 0x3F;

				if (!_24h)
				{
					str[len - 3] = ' ';
					str[len - 2] = (hours & 0x20) ? 'A' : 'P';
					str[len - 1] = 'M';
					hours &= ~0x20;
				}

				uint8_t idx = 0;
				str[idx++] = hex(hours >> 4);
				str[idx++] = hex(hours & 0x0F);

				str[idx++] = ':';
				uint8_t minutes = minutesBCD();
				str[idx++] = hex(minutes >> 4);
				str[idx++] = hex(minutes & 0x0F);

				if (seconds)
				{
					str[idx++] = ':';
					uint8_t seconds = secondsBCD();
					str[idx++] = hex(seconds >> 4);
					str[idx++] = hex(seconds & 0x0F);
				}

				return str;
			}
		};

	} // namespace Ext
} // namespace AVR
