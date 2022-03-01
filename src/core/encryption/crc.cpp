#include "crc.h"

/*
* Code Adapted from: w3.org
* Source: https://www.w3.org/TR/PNG/#D-CRCAppendix
*/

namespace CppRayTracerChallenge::Core::Encryption::CRC
{
	/* Table of CRCs of all 8-bit messages. */
	unsigned long crc_table[256];

	/* Flag: has the table been computed? Initially false. */
	int crc_table_computed = 0;

	/* Make the table for a fast CRC. */
	void m_makeCrcTable(void)
	{
		unsigned long c;
		int n, k;

		for (n = 0; n < 256; n++) {
			c = (unsigned long)n;
			for (k = 0; k < 8; k++) {
				if (c & 1)
					c = 0xedb88320L ^ (c >> 1);
				else
					c = c >> 1;
			}
			crc_table[n] = c;
		}
		crc_table_computed = 1;
	}

	/* Update a running CRC with the bytes buf[0..len-1]--the CRC
		should be initialized to all 1's, and the transmitted value
		is the 1's complement of the final running CRC (see the
		crc() routine below). */
	unsigned long m_updateCrc(unsigned long crc, unsigned char* buf, int len)
	{
		unsigned long c = crc;
		int n;

		if (!crc_table_computed)
			m_makeCrcTable();
		for (n = 0; n < len; n++) {
			c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
		}
		return c;
	}

	std::vector<unsigned char> m_longToBytes(unsigned long value)
	{
		std::vector<unsigned char> result;

		result.push_back((unsigned char)(value >> 24) & 0xFF);
		result.push_back((unsigned char)(value >> 16) & 0xFF);
		result.push_back((unsigned char)(value >> 8) & 0xFF);
		result.push_back((unsigned char)value & 0xFF);

		return result;
	}

	std::vector<unsigned char> run(std::vector<unsigned char> vec)
	{
		return m_longToBytes(m_updateCrc(0xffffffffL, vec.data(), (int)vec.size()) ^ 0xffffffffL);
	}
}
