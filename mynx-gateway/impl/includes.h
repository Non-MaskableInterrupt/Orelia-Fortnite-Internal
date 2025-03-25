#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2spi.h>
#include <ws2tcpip.h>

#include <Windows.h>
#include <fileapi.h>
#include <TlHelp32.h>
#include <winuser.h>
#include <sddl.h>
#include <objbase.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <locale>
#include <codecvt>
#include <type_traits>
#include <numbers>
#include <array>
#include <inttypes.h>
#include <functional>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <algorithm>
#include <thread>
#include <string_view>
#include <random>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <winternl.h>

#include <dependencies/skcrypt/skcrypt.h>
#include <dependencies/polymorphic/engine/qengine.hpp>
using namespace qengine;

#include <dependencies/vigem/Client.h>
#pragma comment(lib, "setupapi.lib")

#include <impl/log/log.hxx>
#include <impl/pipe/pipe.hxx>
#include <impl/disk/disk.hxx>
#include <workspace/exception/exception.hxx>
#include <workspace/utility/utility.hxx>

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(
    ULONG Privilege,
    BOOLEAN Enable,
    BOOLEAN CurrentThread,
    PBOOLEAN Enabled
);