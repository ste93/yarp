/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017-2020 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#ifndef CHOCOBO1_SHA1_H
#define CHOCOBO1_SHA1_H

#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>

#if (__cplusplus > 201703L)
#include <version>
#endif

#ifndef USE_STD_SPAN_CHOCOBO1_HASH
#if (__cpp_lib_span >= 202002L)
#define USE_STD_SPAN_CHOCOBO1_HASH 1
#else
#define USE_STD_SPAN_CHOCOBO1_HASH 0
#endif
#endif

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
#include <span>
#else
#include "gsl/span"
#endif


namespace Chocobo1
{
	// Use these!!
	// SHA1();
}


namespace Chocobo1
{
// users should ignore things in this namespace

namespace Hash
{
#ifndef CONSTEXPR_CPP17_CHOCOBO1_HASH
#if __cplusplus >= 201703L
#define CONSTEXPR_CPP17_CHOCOBO1_HASH constexpr
#else
#define CONSTEXPR_CPP17_CHOCOBO1_HASH
#endif
#endif

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
	using IndexType = std::size_t;
#else
	using IndexType = gsl::index;
#endif

#ifndef CHOCOBO1_HASH_BUFFER_IMPL
#define CHOCOBO1_HASH_BUFFER_IMPL
	template <typename T, IndexType N>
	class Buffer
	{
		public:
			using value_type = T;
			using index_type = IndexType;
			using size_type = std::size_t;

			constexpr Buffer() = default;
			constexpr Buffer(const Buffer &) = default;

			CONSTEXPR_CPP17_CHOCOBO1_HASH Buffer(const std::initializer_list<T> initList)
			{
#if !defined(NDEBUG)
				// check if out-of-bounds
				static_cast<void>(m_array.at(m_dataEndIdx + initList.size() - 1));
#endif

				for (const auto &i : initList)
				{
					m_array[m_dataEndIdx] = i;
					++m_dataEndIdx;
				}
			}

			template <typename InputIt>
			constexpr Buffer(const InputIt first, const InputIt last)
			{
				for (InputIt iter = first; iter != last; ++iter)
				{
					this->fill(*iter);
				}
			}

			constexpr T& operator[](const index_type pos)
			{
				return m_array[pos];
			}

			constexpr T operator[](const index_type pos) const
			{
				return m_array[pos];
			}

			CONSTEXPR_CPP17_CHOCOBO1_HASH void fill(const T &value, const index_type count = 1)
			{
#if !defined(NDEBUG)
				// check if out-of-bounds
				static_cast<void>(m_array.at(m_dataEndIdx + count - 1));
#endif

				for (index_type i = 0; i < count; ++i)
				{
					m_array[m_dataEndIdx] = value;
					++m_dataEndIdx;
				}
			}

			template <typename InputIt>
			constexpr void push_back(const InputIt first, const InputIt last)
			{
				for (InputIt iter = first; iter != last; ++iter)
				{
					this->fill(*iter);
				}
			}

			constexpr void clear()
			{
				m_array = {};
				m_dataEndIdx = 0;
			}

			constexpr bool empty() const
			{
				return (m_dataEndIdx == 0);
			}

			constexpr size_type size() const
			{
				return m_dataEndIdx;
			}

			constexpr const T* data() const
			{
				return m_array.data();
			}

		private:
			std::array<T, N> m_array {};
			index_type m_dataEndIdx = 0;
	};
#endif


namespace SHA1_NS
{
	class SHA1
	{
		// https://tools.ietf.org/html/rfc3174

		public:
			using Byte = uint8_t;
			using ResultArrayType = std::array<Byte, 20>;

#if (USE_STD_SPAN_CHOCOBO1_HASH == 1)
			template <typename T, std::size_t Extent = std::dynamic_extent>
			using Span = std::span<T, Extent>;
#else
			template <typename T, std::size_t Extent = gsl::dynamic_extent>
			using Span = gsl::span<T, Extent>;
#endif


			constexpr SHA1();

			constexpr void reset();
			CONSTEXPR_CPP17_CHOCOBO1_HASH SHA1& finalize();  // after this, only `toArray()`, `toString()`, `toVector()`, `reset()` are available

			std::string toString() const;
			std::vector<Byte> toVector() const;
			CONSTEXPR_CPP17_CHOCOBO1_HASH ResultArrayType toArray() const;

			constexpr SHA1& addData(const Span<const Byte> inData);
			constexpr SHA1& addData(const void *ptr, const std::size_t length);
			template <std::size_t N>
			constexpr SHA1& addData(const Byte (&array)[N]);
			template <typename T, std::size_t N>
			SHA1& addData(const T (&array)[N]);
			template <typename T>
			SHA1& addData(const Span<T> inSpan);

		private:
			constexpr void addDataImpl(const Span<const Byte> data);

			static constexpr int BLOCK_SIZE = 64;

			Buffer<Byte, (BLOCK_SIZE * 2)> m_buffer;  // x2 for paddings
			uint64_t m_sizeCounter = 0;

			uint32_t m_state[5] = {};
	};


	// helpers
	template <typename T>
	class Loader
	{
		// this class workaround loading data from unaligned memory boundaries
		// also eliminate endianness issues
		public:
			explicit constexpr Loader(const uint8_t *ptr)
				: m_ptr(ptr)
			{
			}

			constexpr T operator[](const IndexType idx) const
			{
				static_assert(std::is_same<T, uint32_t>::value, "");
				// handle specific endianness here
				const uint8_t *ptr = m_ptr + (sizeof(T) * idx);
				return  ( (static_cast<T>(*(ptr + 0)) << 24)
						| (static_cast<T>(*(ptr + 1)) << 16)
						| (static_cast<T>(*(ptr + 2)) <<  8)
						| (static_cast<T>(*(ptr + 3)) <<  0));
			}

		private:
			const uint8_t *m_ptr;
	};

	template <typename R, typename T>
	constexpr R ror(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<R>::value, "");
		static_assert(std::is_unsigned<T>::value, "");
		return static_cast<R>(x >> s);
	}

	template <typename T>
	constexpr T rotl(const T x, const unsigned int s)
	{
		static_assert(std::is_unsigned<T>::value, "");
		if (s == 0)
			return x;
		return ((x << s) | (x >> ((sizeof(T) * 8) - s)));
	}


	//
	constexpr SHA1::SHA1()
	{
		static_assert((CHAR_BIT == 8), "Sorry, we don't support exotic CPUs");
		reset();
	}

	constexpr void SHA1::reset()
	{
		m_buffer.clear();
		m_sizeCounter = 0;

		m_state[0] = 0x67452301;
		m_state[1] = 0xEFCDAB89;
		m_state[2] = 0x98BADCFE;
		m_state[3] = 0x10325476;
		m_state[4] = 0xC3D2E1F0;
	}

	CONSTEXPR_CPP17_CHOCOBO1_HASH SHA1& SHA1::finalize()
	{
		m_sizeCounter += m_buffer.size();

		// append 1 bit
		m_buffer.fill(1 << 7);

		// append paddings
		const auto len = static_cast<int>(((2 * BLOCK_SIZE) - (m_buffer.size() + 8)) % BLOCK_SIZE);
		m_buffer.fill(0, (len + 8));

		// append size in bits
		const uint64_t sizeCounterBits = m_sizeCounter * 8;
		const uint32_t sizeCounterBitsL = ror<uint32_t>(sizeCounterBits, 0);
		const uint32_t sizeCounterBitsH = ror<uint32_t>(sizeCounterBits, 32);
		for (int i = 0; i < 4; ++i)
		{
			m_buffer[m_buffer.size() - 8 + i] = ror<Byte>(sizeCounterBitsH, (8 * (3 - i)));
			m_buffer[m_buffer.size() - 4 + i] = ror<Byte>(sizeCounterBitsL, (8 * (3 - i)));
		}

		addDataImpl({m_buffer.data(), m_buffer.size()});
		m_buffer.clear();

		return (*this);
	}

	std::string SHA1::toString() const
	{
		const auto a = toArray();
		std::string ret;
		ret.resize(2 * a.size());

		auto retPtr = &ret.front();
		for (const auto c : a)
		{
			const Byte upper = ror<Byte>(c, 4);
			*(retPtr++) = static_cast<char>((upper < 10) ? (upper + '0') : (upper - 10 + 'a'));

			const Byte lower = c & 0xf;
			*(retPtr++) = static_cast<char>((lower < 10) ? (lower + '0') : (lower - 10 + 'a'));
		}

		return ret;
	}

	std::vector<SHA1::Byte> SHA1::toVector() const
	{
		const auto a = toArray();
		return {a.begin(), a.end()};
	}

	CONSTEXPR_CPP17_CHOCOBO1_HASH SHA1::ResultArrayType SHA1::toArray() const
	{
		const Span<const uint32_t> state(m_state);
		const int dataSize = sizeof(decltype(state)::value_type);

		ResultArrayType ret {};
		auto retPtr = ret.data();
		for (const auto i : state)
		{
			for (int j = (dataSize - 1); j >= 0; --j)
				*(retPtr++) = ror<Byte>(i, (j * 8));
		}

		return ret;
	}

	constexpr SHA1& SHA1::addData(const Span<const Byte> inData)
	{
		Span<const Byte> data = inData;

		if (!m_buffer.empty())
		{
			const size_t len = std::min<size_t>((BLOCK_SIZE - m_buffer.size()), data.size());  // try fill to BLOCK_SIZE bytes
			m_buffer.push_back(data.begin(), (data.begin() + len));

			if (m_buffer.size() < BLOCK_SIZE)  // still doesn't fill the buffer
				return (*this);

			addDataImpl({m_buffer.data(), m_buffer.size()});
			m_buffer.clear();

			data = data.subspan(len);
		}

		const size_t dataSize = data.size();
		if (dataSize < BLOCK_SIZE)
		{
			m_buffer = {data.begin(), data.end()};
			return (*this);
		}

		const size_t len = dataSize - (dataSize % BLOCK_SIZE);  // align on BLOCK_SIZE bytes
		addDataImpl(data.first(len));

		if (len < dataSize)  // didn't consume all data
			m_buffer = {(data.begin() + len), data.end()};

		return (*this);
	}

	constexpr SHA1& SHA1::addData(const void *ptr, const std::size_t length)
	{
		// Span::size_type = std::size_t
		return addData({static_cast<const Byte*>(ptr), length});
	}

	template <std::size_t N>
	constexpr SHA1& SHA1::addData(const Byte (&array)[N])
	{
		return addData({array, N});
	}

	template <typename T, std::size_t N>
	SHA1& SHA1::addData(const T (&array)[N])
	{
		return addData({reinterpret_cast<const Byte*>(array), (sizeof(T) * N)});
	}

	template <typename T>
	SHA1& SHA1::addData(const Span<T> inSpan)
	{
		return addData({reinterpret_cast<const Byte*>(inSpan.data()), inSpan.size_bytes()});
	}

	constexpr void SHA1::addDataImpl(const Span<const Byte> data)
	{
		assert((data.size() % BLOCK_SIZE) == 0);

		m_sizeCounter += data.size();

		for (size_t i = 0, iend = static_cast<size_t>(data.size() / BLOCK_SIZE); i < iend; ++i)
		{
			const Loader<uint32_t> m(static_cast<const Byte *>(data.data() + (i * BLOCK_SIZE)));

			uint32_t a = m_state[0];
			uint32_t b = m_state[1];
			uint32_t c = m_state[2];
			uint32_t d = m_state[3];
			uint32_t e = m_state[4];

			uint32_t wTable[80] = {};

			#ifdef sha1Round1
			#error "macro name clash"
			#else
			#define sha1Round1(a, b, c, d, e, t) \
				wTable[t] = m[t]; \
				e = rotl(a, 5) + ((b & (c ^ d)) ^ d) + e + wTable[t] + 0x5A827999;  /* alternative f */ \
				b = rotl(b, 30);

			sha1Round1(a, b, c, d, e, 0);
			sha1Round1(e, a, b, c, d, 1);
			sha1Round1(d, e, a, b, c, 2);
			sha1Round1(c, d, e, a, b, 3);
			sha1Round1(b, c, d, e, a, 4);
			sha1Round1(a, b, c, d, e, 5);
			sha1Round1(e, a, b, c, d, 6);
			sha1Round1(d, e, a, b, c, 7);
			sha1Round1(c, d, e, a, b, 8);
			sha1Round1(b, c, d, e, a, 9);
			sha1Round1(a, b, c, d, e, 10);
			sha1Round1(e, a, b, c, d, 11);
			sha1Round1(d, e, a, b, c, 12);
			sha1Round1(c, d, e, a, b, 13);
			sha1Round1(b, c, d, e, a, 14);
			sha1Round1(a, b, c, d, e, 15);
			#undef sha1Round1
			#endif

			#ifdef sha1Round1a
			#error "macro name clash"
			#else
			#define sha1Round1a(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 3] ^ wTable[t - 8] ^ wTable[t - 14] ^ wTable[t - 16]), 1); \
				e = rotl(a, 5) + ((b & (c ^ d)) ^ d) + e + wTable[t] + 0x5A827999;  /* alternative f */ \
				b = rotl(b, 30);

			sha1Round1a(e, a, b, c, d, 16);
			sha1Round1a(d, e, a, b, c, 17);
			sha1Round1a(c, d, e, a, b, 18);
			sha1Round1a(b, c, d, e, a, 19);
			#undef sha1Round1a
			#endif

			#ifdef sha1Round2
			#error "macro name clash"
			#else
			#define sha1Round2(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 3] ^ wTable[t - 8] ^ wTable[t - 14] ^ wTable[t - 16]), 1); \
				e = rotl(a, 5) + (b ^ c ^ d) + e + wTable[t] + 0x6ED9EBA1; \
				b = rotl(b, 30);

			sha1Round2(a, b, c, d, e, 20);
			sha1Round2(e, a, b, c, d, 21);
			sha1Round2(d, e, a, b, c, 22);
			sha1Round2(c, d, e, a, b, 23);
			sha1Round2(b, c, d, e, a, 24);
			sha1Round2(a, b, c, d, e, 25);
			sha1Round2(e, a, b, c, d, 26);
			sha1Round2(d, e, a, b, c, 27);
			sha1Round2(c, d, e, a, b, 28);
			sha1Round2(b, c, d, e, a, 29);
			sha1Round2(a, b, c, d, e, 30);
			sha1Round2(e, a, b, c, d, 31);
			#undef sha1Round2
			#endif

			#ifdef sha1Round2a
			#error "macro name clash"
			#else
			#define sha1Round2a(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 6] ^ wTable[t - 16] ^ wTable[t - 28] ^ wTable[t - 32]), 2);  /* alternative */ \
				e = rotl(a, 5) + (b ^ c ^ d) + e + wTable[t] + 0x6ED9EBA1; \
				b = rotl(b, 30);

			sha1Round2a(d, e, a, b, c, 32);
			sha1Round2a(c, d, e, a, b, 33);
			sha1Round2a(b, c, d, e, a, 34);
			sha1Round2a(a, b, c, d, e, 35);
			sha1Round2a(e, a, b, c, d, 36);
			sha1Round2a(d, e, a, b, c, 37);
			sha1Round2a(c, d, e, a, b, 38);
			sha1Round2a(b, c, d, e, a, 39);
			#undef sha1Round2a
			#endif

			#ifdef sha1Round3
			#error "macro name clash"
			#else
			#define sha1Round3(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 6] ^ wTable[t - 16] ^ wTable[t - 28] ^ wTable[t - 32]), 2);  /* alternative */ \
				e = rotl(a, 5) + ((b & c) | (d & (b | c))) + e + wTable[t] + 0x8F1BBCDC; \
				b = rotl(b, 30);

			sha1Round3(a, b, c, d, e, 40);
			sha1Round3(e, a, b, c, d, 41);
			sha1Round3(d, e, a, b, c, 42);
			sha1Round3(c, d, e, a, b, 43);
			sha1Round3(b, c, d, e, a, 44);
			sha1Round3(a, b, c, d, e, 45);
			sha1Round3(e, a, b, c, d, 46);
			sha1Round3(d, e, a, b, c, 47);
			sha1Round3(c, d, e, a, b, 48);
			sha1Round3(b, c, d, e, a, 49);
			sha1Round3(a, b, c, d, e, 50);
			sha1Round3(e, a, b, c, d, 51);
			sha1Round3(d, e, a, b, c, 52);
			sha1Round3(c, d, e, a, b, 53);
			sha1Round3(b, c, d, e, a, 54);
			sha1Round3(a, b, c, d, e, 55);
			sha1Round3(e, a, b, c, d, 56);
			sha1Round3(d, e, a, b, c, 57);
			sha1Round3(c, d, e, a, b, 58);
			sha1Round3(b, c, d, e, a, 59);
			#undef sha1Round3
			#endif

			#ifdef sha1Round4
			#error "macro name clash"
			#else
			#define sha1Round4(a, b, c, d, e, t) \
				wTable[t] = rotl((wTable[t - 6] ^ wTable[t - 16] ^ wTable[t - 28] ^ wTable[t - 32]), 2);  /* alternative */ \
				e = rotl(a, 5) + (b ^ c ^ d) + e + wTable[t] + 0xCA62C1D6; \
				b = rotl(b, 30);

			sha1Round4(a, b, c, d, e, 60);
			sha1Round4(e, a, b, c, d, 61);
			sha1Round4(d, e, a, b, c, 62);
			sha1Round4(c, d, e, a, b, 63);
			sha1Round4(b, c, d, e, a, 64);
			sha1Round4(a, b, c, d, e, 65);
			sha1Round4(e, a, b, c, d, 66);
			sha1Round4(d, e, a, b, c, 67);
			sha1Round4(c, d, e, a, b, 68);
			sha1Round4(b, c, d, e, a, 69);
			sha1Round4(a, b, c, d, e, 70);
			sha1Round4(e, a, b, c, d, 71);
			sha1Round4(d, e, a, b, c, 72);
			sha1Round4(c, d, e, a, b, 73);
			sha1Round4(b, c, d, e, a, 74);
			sha1Round4(a, b, c, d, e, 75);
			sha1Round4(e, a, b, c, d, 76);
			sha1Round4(d, e, a, b, c, 77);
			sha1Round4(c, d, e, a, b, 78);
			sha1Round4(b, c, d, e, a, 79);
			#undef sha1Round4
			#endif

			// Let H0 = H0 + A, H1 = H1 + B, H2 = H2 + C, H3 = H3 + D, H4 = H4 + E.
			m_state[0] += a;
			m_state[1] += b;
			m_state[2] += c;
			m_state[3] += d;
			m_state[4] += e;
		}
	}
}
}
	using SHA1 = Hash::SHA1_NS::SHA1;
}

#endif  // CHOCOBO1_SHA1_H



#if 0


/*
    sha1.hpp - source code of

    ============
    SHA-1 in C++
    ============

    100% Public Domain.

    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Diels-Grabsch <v@njh.eu>
    Safety fixes
        -- Eugene Hopkinson <slowriot at voxelstorm dot com>
    Header-only library
        -- Zlatko Michailov <zlatko@michailov.org>
*/

#ifndef SHA1_HPP
#define SHA1_HPP


#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


class SHA1
{
public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();
    static std::string from_file(const std::string &filename);

private:
    uint32_t digest[5];
    std::string buffer;
    uint64_t transforms;
};


static const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
static const size_t BLOCK_BYTES = BLOCK_INTS * 4;


inline static void reset(uint32_t digest[], std::string &buffer, uint64_t &transforms)
{
    /* SHA1 initialization constants */
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;
    digest[4] = 0xc3d2e1f0;

    /* Reset counters */
    buffer = "";
    transforms = 0;
}


inline static uint32_t rol(const uint32_t value, const size_t bits)
{
    return (value << bits) | (value >> (32 - bits));
}


inline static uint32_t blk(const uint32_t block[BLOCK_INTS], const size_t i)
{
    return rol(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i], 1);
}


/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

inline static void R0(const uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


inline static void R1(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


inline static void R2(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (w^x^y) + block[i] + 0x6ed9eba1 + rol(v, 5);
    w = rol(w, 30);
}


inline static void R3(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (((w|x)&y)|(w&x)) + block[i] + 0x8f1bbcdc + rol(v, 5);
    w = rol(w, 30);
}


inline static void R4(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (w^x^y) + block[i] + 0xca62c1d6 + rol(v, 5);
    w = rol(w, 30);
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

inline static void transform(uint32_t digest[], uint32_t block[BLOCK_INTS], uint64_t &transforms)
{
    /* Copy digest[] to working vars */
    uint32_t a = digest[0];
    uint32_t b = digest[1];
    uint32_t c = digest[2];
    uint32_t d = digest[3];
    uint32_t e = digest[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(block, a, b, c, d, e,  0);
    R0(block, e, a, b, c, d,  1);
    R0(block, d, e, a, b, c,  2);
    R0(block, c, d, e, a, b,  3);
    R0(block, b, c, d, e, a,  4);
    R0(block, a, b, c, d, e,  5);
    R0(block, e, a, b, c, d,  6);
    R0(block, d, e, a, b, c,  7);
    R0(block, c, d, e, a, b,  8);
    R0(block, b, c, d, e, a,  9);
    R0(block, a, b, c, d, e, 10);
    R0(block, e, a, b, c, d, 11);
    R0(block, d, e, a, b, c, 12);
    R0(block, c, d, e, a, b, 13);
    R0(block, b, c, d, e, a, 14);
    R0(block, a, b, c, d, e, 15);
    R1(block, e, a, b, c, d,  0);
    R1(block, d, e, a, b, c,  1);
    R1(block, c, d, e, a, b,  2);
    R1(block, b, c, d, e, a,  3);
    R2(block, a, b, c, d, e,  4);
    R2(block, e, a, b, c, d,  5);
    R2(block, d, e, a, b, c,  6);
    R2(block, c, d, e, a, b,  7);
    R2(block, b, c, d, e, a,  8);
    R2(block, a, b, c, d, e,  9);
    R2(block, e, a, b, c, d, 10);
    R2(block, d, e, a, b, c, 11);
    R2(block, c, d, e, a, b, 12);
    R2(block, b, c, d, e, a, 13);
    R2(block, a, b, c, d, e, 14);
    R2(block, e, a, b, c, d, 15);
    R2(block, d, e, a, b, c,  0);
    R2(block, c, d, e, a, b,  1);
    R2(block, b, c, d, e, a,  2);
    R2(block, a, b, c, d, e,  3);
    R2(block, e, a, b, c, d,  4);
    R2(block, d, e, a, b, c,  5);
    R2(block, c, d, e, a, b,  6);
    R2(block, b, c, d, e, a,  7);
    R3(block, a, b, c, d, e,  8);
    R3(block, e, a, b, c, d,  9);
    R3(block, d, e, a, b, c, 10);
    R3(block, c, d, e, a, b, 11);
    R3(block, b, c, d, e, a, 12);
    R3(block, a, b, c, d, e, 13);
    R3(block, e, a, b, c, d, 14);
    R3(block, d, e, a, b, c, 15);
    R3(block, c, d, e, a, b,  0);
    R3(block, b, c, d, e, a,  1);
    R3(block, a, b, c, d, e,  2);
    R3(block, e, a, b, c, d,  3);
    R3(block, d, e, a, b, c,  4);
    R3(block, c, d, e, a, b,  5);
    R3(block, b, c, d, e, a,  6);
    R3(block, a, b, c, d, e,  7);
    R3(block, e, a, b, c, d,  8);
    R3(block, d, e, a, b, c,  9);
    R3(block, c, d, e, a, b, 10);
    R3(block, b, c, d, e, a, 11);
    R4(block, a, b, c, d, e, 12);
    R4(block, e, a, b, c, d, 13);
    R4(block, d, e, a, b, c, 14);
    R4(block, c, d, e, a, b, 15);
    R4(block, b, c, d, e, a,  0);
    R4(block, a, b, c, d, e,  1);
    R4(block, e, a, b, c, d,  2);
    R4(block, d, e, a, b, c,  3);
    R4(block, c, d, e, a, b,  4);
    R4(block, b, c, d, e, a,  5);
    R4(block, a, b, c, d, e,  6);
    R4(block, e, a, b, c, d,  7);
    R4(block, d, e, a, b, c,  8);
    R4(block, c, d, e, a, b,  9);
    R4(block, b, c, d, e, a, 10);
    R4(block, a, b, c, d, e, 11);
    R4(block, e, a, b, c, d, 12);
    R4(block, d, e, a, b, c, 13);
    R4(block, c, d, e, a, b, 14);
    R4(block, b, c, d, e, a, 15);

    /* Add the working vars back into digest[] */
    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;

    /* Count the number of transformations */
    transforms++;
}


inline static void buffer_to_block(const std::string &buffer, uint32_t block[BLOCK_INTS])
{
    /* Convert the std::string (byte buffer) to a uint32_t array (MSB) */
    for (size_t i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4*i+3] & 0xff)
                   | (buffer[4*i+2] & 0xff)<<8
                   | (buffer[4*i+1] & 0xff)<<16
                   | (buffer[4*i+0] & 0xff)<<24;
    }
}


inline SHA1::SHA1()
{
    reset(digest, buffer, transforms);
}


inline void SHA1::update(const std::string &s)
{
    std::istringstream is(s);
    update(is);
}


inline void SHA1::update(std::istream &is)
{
    while (true)
    {
        char sbuf[BLOCK_BYTES];
        is.read(sbuf, BLOCK_BYTES - buffer.size());
        buffer.append(sbuf, (std::size_t)is.gcount());
        if (buffer.size() != BLOCK_BYTES)
        {
            return;
        }
        uint32_t block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(digest, block, transforms);
        buffer.clear();
    }
}


/*
 * Add padding and return the message digest.
 */

inline std::string SHA1::final()
{
    /* Total number of hashed bits */
    uint64_t total_bits = (transforms*BLOCK_BYTES + buffer.size()) * 8;

    /* Padding */
    buffer += (char)0x80;
    size_t orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += (char)0x00;
    }

    uint32_t block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(digest, block, transforms);
        for (size_t i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    /* Append total_bits, split this uint64_t into two uint32_t */
    block[BLOCK_INTS - 1] = (uint32_t)total_bits;
    block[BLOCK_INTS - 2] = (uint32_t)(total_bits >> 32);
    transform(digest, block, transforms);

    /* Hex std::string */
    std::ostringstream result;
    for (size_t i = 0; i < sizeof(digest) / sizeof(digest[0]); i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << digest[i];
    }

    /* Reset for next run */
    reset(digest, buffer, transforms);

    return result.str();
}


inline std::string SHA1::from_file(const std::string &filename)
{
    std::ifstream stream(filename.c_str(), std::ios::binary);
    SHA1 checksum;
    checksum.update(stream);
    return checksum.final();
}


#endif /* SHA1_HPP */



#endif // 0
