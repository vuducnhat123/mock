#ifndef DECODER_WRAPPER_H
#define DECODER_WRAPPER_H

#include "mock_dec.h"

#include <type_traits>

class Decoder {
public:
    explicit Decoder(uint8_t key = 0u, size_t max_chunk = 0) noexcept
        : ctx_{}, is_open_(false)
    {
        if (key == 0u) {
            return;
        }

        dec_cfg_t cfg{key, max_chunk};
        if (dec_init(&ctx_, &cfg) == DEC_OK) {
            is_open_ = true;
        }
    }

    ~Decoder() noexcept
    {
        close();
    }

    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder&) = delete;

    Decoder(Decoder&& other) noexcept
        : ctx_(other.ctx_), is_open_(other.is_open_)
    {
        other.ctx_ = {};
        other.is_open_ = false;
    }

    Decoder& operator=(Decoder&& other) noexcept
    {
        if (this != &other) {
            close();
            ctx_ = other.ctx_;
            is_open_ = other.is_open_;
            other.ctx_ = {};
            other.is_open_ = false;
        }
        return *this;
    }

    bool ok() const noexcept
    {
        return is_open_;
    }

    int feed(const uint8_t* in, size_t n, uint8_t* out, size_t out_cap)
    {
        if (!is_open_) {
            return DEC_ERR_STATE;
        }
        return dec_feed(&ctx_, in, n, out, out_cap);
    }

    void close() noexcept
    {
        if (is_open_) {
            dec_close(&ctx_);
            is_open_ = false;
        }
    }

private:
    dec_ctx_t ctx_;
    bool is_open_;
};

static_assert(!std::is_copy_constructible_v<Decoder>);
static_assert(!std::is_copy_assignable_v<Decoder>);

#endif /* DECODER_WRAPPER_H */
