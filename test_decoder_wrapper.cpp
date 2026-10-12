#include "decoder_wrapper.h"

#include <array>
#include <cstdint>
#include <iostream>

int main()
{
    dec_reset_counters();

    {
        Decoder d{0x13u, 16};
        if (!d.ok()) {
            std::cerr << "decoder init failed\n";
            return 1;
        }

        const std::array<uint8_t, 5> in = {1, 2, 3, 4, 5};
        std::array<uint8_t, 5> out{};
        const int written = d.feed(in.data(), in.size(), out.data(), out.size());
        if (written != 5) {
            std::cerr << "feed returned " << written << "\n";
            return 2;
        }

        for (size_t i = 0; i < in.size(); ++i) {
            if (out[i] != static_cast<uint8_t>(in[i] ^ 0x13u)) {
                std::cerr << "decode mismatch\n";
                return 3;
            }
        }
    }

    if (dec_init_count() != dec_close_count()) {
        std::cerr << "init/close mismatch: " << dec_init_count() << " vs " << dec_close_count() << "\n";
        return 4;
    }
    std::cout << "XONG\n";
    return 0;
}
