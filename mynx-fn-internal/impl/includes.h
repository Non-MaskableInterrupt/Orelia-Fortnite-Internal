#pragma once
#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <string>
#include <fileapi.h>
#include <TlHelp32.h>
#include <sddl.h>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <chrono>

#include <impl/sdk/sdk.hxx>
#include <impl/rtl/memcpy.hxx>

#define process_attach 1
namespace orelia {
	std::uintptr_t base_address = 0;
}

#include <dependencies/skcrypt/skcrypt.h>
#include <dependencies/oxorany/oxorany_include.h>

#include <impl/pipe/pipe.hxx>
#include <workspace/process/process.hxx>
#include <workspace/heartbeat/heartbeat.hxx>

#include <workspace/game/unreal/unreal.h>
#include <workspace/game/features.h>

#include <impl/trap/trap.hxx>
#include <impl/vmt/vmt.hxx>

#include <workspace/drawing/drawing.h>
#include <workspace/game/core/exploits/chams.hxx>
#include <workspace/game/core/exploits/desync.hxx>
#include <workspace/game/core/exploits/psilent.hxx>
#include <workspace/game/core/exploits/weapon.hxx>
#include <workspace/game/core/console/console.hxx>
#include <workspace/game/core/targeting/targeting.hxx>
#include <workspace/game/hooks/draw_transtion.hxx>