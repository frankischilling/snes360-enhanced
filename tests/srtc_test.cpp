#include "snes9x.h"
#include "memmap.h"
#include "srtc.h"
#include <cassert>
#include <cstring>
#include <iostream>

SSettings Settings = {};
CMemory Memory;

int main()
{
    uint8 sram[0x20000 + SRTC_SRAM_PAD] = {0};
    Memory.SRAM = sram;
    Settings.SRTC = TRUE;
    const int sizes[] = {0, 1, 5, 7, 8};
    const uint64 values[] = {0, 1, (uint64(0x01234567) << 32) | 0x89abcdef,
        (uint64(0x7fffffff) << 32) | 0xffffffff};
    for (size_t n = 0; n < sizeof(sizes) / sizeof(sizes[0]); ++n)
    {
        Memory.SRAMSize = sizes[n];
        int offset = sizes[n] ? (1 << (sizes[n] + 3)) * 128 : 0;
        if (offset > 0x20000) offset = 0x20000;
        for (size_t v = 0; v < sizeof(values) / sizeof(values[0]); ++v)
        {
            std::memset(&rtc, 0, sizeof(rtc));
            rtc.system_timestamp = static_cast<time_t>(values[v]);
            rtc.index = -1; rtc.mode = MODE_READ;
            for (int i = 0; i <= MAX_RTC_INDEX; ++i) rtc.data[i] = i;
            S9xSRTCPreSaveState();
            for (int i = 0; i < 8; ++i)
                assert(sram[offset + 5 + MAX_RTC_INDEX + i] == ((values[v] >> (i * 8)) & 0xff));
            rtc.system_timestamp = static_cast<time_t>(-1); // Loading must replace stale bits.
            std::memset(rtc.data, 0, sizeof(rtc.data));
            S9xSRTCPostLoadState();
            assert(static_cast<uint64>(rtc.system_timestamp) == values[v]);
            assert(rtc.index == -1 && rtc.mode == MODE_READ);
            for (int i = 0; i <= MAX_RTC_INDEX; ++i) assert(rtc.data[i] == i);
        }
    }
    std::cout << "S-RTC tests passed: full-width timestamp replacement, byte order and maximum SRAM offsets.\n";
}
