#include <Siv3D.hpp>
#include "Debug.h"

void Debug::show_system_mic() {
	for (const auto& info : System::EnumerateMicrophones()) {
		Print << U"[{}] {}"_fmt(info.microphoneIndex, info.name);
	}
}
