#pragma once

#define ENABLE_INTERRUPTS

#include <avr/io/serial.hpp>

namespace AVR
{
	namespace Ext
	{
		class DustSensor
		{
			AVR::Serial m_serial;

			enum class rxState
			{
				Idle,
				HeaderH,
				HeaderL,
				LenH,
				LenL,
				DataH,
				DataL,
				ChecksumH,
				ChecksumL,
			};

			//buffer
			uint8_t *rx_buf;

			uint16_t rx_checksum;

			void sendCommandFrame(uint8_t cmd, uint16_t data)
			{
				m_serial.Print("\x42\x4D");
				m_serial.Print(cmd);
				m_serial.Print(data >> 8);
				m_serial.Print(data & 0xFF);
				uint16_t crc = 0x42 + 0x4D + cmd + (data >> 8) + (data & 0xFF);
				m_serial.Print(crc >> 8);
				m_serial.Print(crc & 0xFF);
			}

		public:
			enum class cmd : uint8_t
			{
				GetReading = 0xE2,
				OutputModeSwitch = 0xE1,
				StandbyControl = 0xE4
			};

		public:
			DustSensor(const Serial &serial)
				: m_serial(serial),
				  rx_checksum(0)
			{
				m_serial.begin(9600);
			}

			void send(cmd c, bool d = false)
			{
				uint8_t _cmd = static_cast<uint8_t>(c);
				sendCommandFrame(_cmd, d ? 0x0001 : 0x0000);
			}
		};

	} // namespace Ext
} // namespace AVR
