/*
/**************************************************************************************************************
*																											 *
*  QHash is made to be a fast and performative 32 / 64-bit hashing algorithm for runtime data integrity      *
*																											 *
***************************************************************************************************************
*/

#pragma region Header Guard

#ifndef QHASH_H
#define QHASH_H

#pragma endregion

#pragma region Imports

#pragma region qengine

#include "../qbase/qpreprocess.hpp"

#pragma endregion

#pragma region std

#include <cstdint>

#pragma endregion

#pragma endregion

#pragma region Preprocessor

#pragma optimize("", on)
#pragma inline_depth(255)
#pragma inline_recursion(on) 

#pragma pack(push, 1)

#pragma endregion

#pragma region Algorithm Constants

#pragma region 32-bit

// qimutexpr fixes a prior qengine detection vector in which the plaintext constants were aggressively inlined to output machine code, creating a profile of qengine
qimutexpr(std::uint32_t, QHBASE32, 0xFAE9E8D7ui32);

qimutexpr(std::uint32_t, QHEPILOGUE32, 0xAEBDCCDBui32);

qimutexpr(std::uint32_t, QHENTROPYSWITCH32, 0xAAAAAAAAui32);

#pragma endregion

#pragma region 64-bit

#ifdef _WIN64

qimutexpr(std::uint64_t, QHBASE64, 0xFAE9E8D7C6B5A493ui64);

qimutexpr(std::uint64_t, QHEPILOGUE64, 0xAEBDCCDB0A192837ui64);

qimutexpr(std::uint64_t, QHENTROPYSWITCH64, 0xAAAAAAAAAAAAAAAAui64);

#endif

#pragma endregion

//#define MOST_SIGNIFICANT_DEFAULT 0xAEui8
qimutexpr(std::uint8_t, QMOST_SIGNIFICANT_DEFAULT, 0xAEui8);

#pragma endregion

namespace qengine {

	namespace qhash {

#pragma region Singleton

#pragma region 32-bit

		extern bool _qhash_initialized32;

		extern std::uint32_t _qtable32[];

#pragma endregion

#pragma region 64-bit

#ifdef _WIN64

		extern bool _qhash_initialized64;

		extern std::uint64_t _qtable64[];

#endif

#pragma endregion

#pragma endregion

#pragma region Table Generation

		__compelled_inline static void qtable32_gen() nex {

			static std::int32_t seed = 0xFEEDDCCBui32;

			static imut imutexpr std::uint16_t decrementor = 0xFFFFui16;

			for (auto i = 0; i < 256; ++i) {
				_qtable32[i] = (((seed ^ QHBASE32) ^ QHEPILOGUE32) ^ 0xFFFFFFFFui32);

				seed -= decrementor;
			}
		}

#ifdef _WIN64

		__compelled_inline static void __stackcall qtable64_gen() nex {

			static std::uint64_t seed = 0xFEEDDCCBBAA99887ui64;

			static imut imutexpr std::uint32_t decrementor = 0xFFFFFFFFui32;

			for (auto i = 0; i < 256; ++i) {

				_qtable64[i] = (((seed ^ QHBASE64) ^ QHEPILOGUE64) ^ 0xFFFFFFFFFFFFFFFFui64);

				seed -= decrementor;
			}
		}

#endif

#pragma endregion

#pragma region Hashing

		// 0.0000000233% collision rate among 65535 unique 2-byte data sets ( 1 out of 4,294,770,690 possible collisions )
		__compelled_inline static std::uint32_t __regcall qhash32(void* data, std::uint32_t length) nex {
			/* check if our global variables have been initialized */
			if (!_qhash_initialized32) {
				qtable32_gen();

				_qhash_initialized32 = true;
			}

			/* initialize base hash32 value constant within structure */
			std::uint32_t hash_r = QHBASE32;

			for (std::size_t i = 0; i < length; ++i) {
				for (auto x = 0; x < sizeof(decltype(hash_r)); ++x)
					reinterpret_cast<std::uint8_t*>(&hash_r)[x] ^= reinterpret_cast<std::uint8_t*>(data)[i];

				hash_r ^= _qtable32[reinterpret_cast<std::uint8_t*>(data)[i] ^ 0xFF];
			}

			static std::uint32_t preamble_result = hash_r;

			/* initialize epilogue with base epilogue constant */
			static std::uint32_t epilogue = QHEPILOGUE32;

			std::uint8_t least_significant = reinterpret_cast<std::uint8_t*>(data)[0];
			std::uint8_t most_significant = (length > 1 ? reinterpret_cast<std::uint8_t*>(data)[length - 1] : QMOST_SIGNIFICANT_DEFAULT);

			std::size_t least_significant_length = length;
			std::size_t most_significant_length = length;

			for (std::size_t i = 0; i < sizeof(decltype(length)); ++i) {

				*reinterpret_cast<std::uint8_t*>(&least_significant_length) ^= least_significant;
				*reinterpret_cast<std::uint8_t*>(&most_significant_length) ^= most_significant;
			}

			static bool significance_switch = false;

			for (std::size_t i = 0; i < sizeof(hash_r); ++i) {

				/* Inverse significance flag */
				significance_switch = (significance_switch ? false : true);

				/* Incrementally shift the bits left to ensure the entire output word has been marked by the sizeof(data) (this is more important for smaller datasets to reduce collision rates) */
				epilogue ^= static_cast<uint32_t>((QHEPILOGUE32 ^ (length << ((length * 8) % (((i == 0 ? 1 : i) * 8))))));
				reinterpret_cast<std::uint8_t*>(&hash_r)[i] ^= reinterpret_cast<std::uint8_t*>(&epilogue)[i];

				/* shift length left i * (sizeof( byte in bits ), xor hash by results to make the result more unique */
				hash_r ^= (((significance_switch ? most_significant_length : least_significant_length) << (32 - ((i * 8) > 0 ? (i * 8) : 8))));
			}

			hash_r = (preamble_result ^ hash_r) ^ ((QHENTROPYSWITCH32 - (most_significant * 255) - (least_significant * 128)));

			// Wipe Singletons / Locals
			RtlZeroMemory(&preamble_result, sizeof(std::uint32_t));
			RtlZeroMemory(&epilogue, sizeof(std::uint32_t));
			RtlZeroMemory(&least_significant, sizeof(std::uint8_t));
			RtlZeroMemory(&most_significant, sizeof(std::uint8_t));
			RtlZeroMemory(&least_significant_length, sizeof(std::size_t));
			RtlZeroMemory(&most_significant_length, sizeof(std::size_t));
			RtlZeroMemory(&significance_switch, sizeof(bool));

			return std::move(hash_r);
		}

#ifdef _WIN64
		//  0.00% collision rate among every possible 2-byte data set ( 0 out of 4,294,770,690 possible collisions )
		__compelled_inline static std::uint64_t __regcall qhash64(void* data, size_t length) nex {
			/* check if our global variables have been initialized */
			if (!_qhash_initialized64) {

				qtable64_gen();

				_qhash_initialized64 = true;
			}

			/* initialize base hash64 value constant within structure */
			std::uint64_t hash_r = QHBASE64;

			for (auto i = 0; i < length; ++i) {

				for (auto x = 0; x < sizeof(decltype(hash_r)); ++x)
					reinterpret_cast<std::uint8_t*>(&hash_r)[x] ^= reinterpret_cast<std::uint8_t*>(data)[i];

				hash_r ^= _qtable64[reinterpret_cast<std::uint8_t*>(data)[i] ^ 0xFF];
			}
			static std::uint64_t preamble_result = hash_r;

			/* Initialize epilogue local with constant */
			static std::uint64_t epilogue = QHEPILOGUE64;

			std::uint8_t least_significant = reinterpret_cast<std::uint8_t*>(data)[0];
			std::uint8_t most_significant = (length > 1 ? reinterpret_cast<std::uint8_t*>(data)[length - 1] : QMOST_SIGNIFICANT_DEFAULT);

			std::size_t least_significant_length = length;
			std::size_t most_significant_length = length;

			for (auto i = 0; i < sizeof(decltype(length)); ++i) {

				*reinterpret_cast<std::uint8_t*>(&least_significant_length) ^= least_significant;
				*reinterpret_cast<std::uint8_t*>(&most_significant_length) ^= most_significant;
			}

			static bool significance_switch = false;

			for (auto i = 0; i < sizeof(hash_r); ++i) {
				/* inverse significance flag */
				significance_switch = (significance_switch ? false : true);

				/* Incrementally shift the bits left to ensure the entire output word has been marked by the sizeof(data) (this is more important for smaller datasets) */
				epilogue ^= static_cast<uint64_t>((QHEPILOGUE64 ^ (length << ((length * 8) % (((i == 0 ? 1 : i) * 8))))));

				reinterpret_cast<std::uint8_t*>(&hash_r)[i] ^= reinterpret_cast<std::uint8_t*>(&epilogue)[i];

				/* Shift length left i * (sizeof( byte in bits ), run XOR algorithm on hash by randomized source data to make the result more unique */
				hash_r ^= (((significance_switch ? most_significant_length : least_significant_length) << (64 - ((i * 8) > 0 ? (i * 8) : 8))));
			}

			hash_r = (preamble_result ^ hash_r) ^ ((QHENTROPYSWITCH64 - (most_significant * 255) - (least_significant * 128)));

			// Wipe Singletons / Locals
			RtlZeroMemory(&preamble_result, sizeof(std::uint64_t));
			RtlZeroMemory(&epilogue, sizeof(std::uint64_t));
			RtlZeroMemory(&least_significant, sizeof(std::uint8_t));
			RtlZeroMemory(&most_significant, sizeof(std::uint8_t));
			RtlZeroMemory(&least_significant_length, sizeof(std::size_t));
			RtlZeroMemory(&most_significant_length, sizeof(std::size_t));
			RtlZeroMemory(&significance_switch, sizeof(bool));

			return std::move(hash_r);
		}

#endif

#pragma endregion

#pragma region CPU-Safe Template Accessor

		static __compelled_inline decltype(auto) __regcall qhash_cpu(c_void data, std::size_t length) nex {

#ifdef _WIN64

			return qhash64(data, length);

#else

			return qhash32(data, length);

#endif

		}

#pragma endregion

	}

	bool qhash::_qhash_initialized32 = false;

#ifdef _WIN64

	bool qhash::_qhash_initialized64 = false;

#endif
	std::uint32_t qhash::_qtable32[256] { };

#ifdef _WIN64

	std::uint64_t qhash::_qtable64[256] { };

#endif

}

#pragma region Preprocessor

#pragma pack(pop)

#pragma endregion

#endif