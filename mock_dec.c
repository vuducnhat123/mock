#include "mock_dec.h"

#include <string.h>

#define DEC_MAGIC 0xDEC0DE01u

static unsigned g_init_count;
static unsigned g_close_count;
static unsigned g_bad_close_count;

int dec_init(dec_ctx_t *ctx, const dec_cfg_t *cfg)
{
    if (ctx == NULL || cfg == NULL || cfg->key == 0) {
        return DEC_ERR_ARG;
    }
    memset(ctx, 0, sizeof *ctx);
    ctx->magic     = DEC_MAGIC;
    ctx->key       = cfg->key;
    ctx->max_chunk = cfg->max_chunk;
    ++g_init_count;
    return DEC_OK;
}

int dec_feed(dec_ctx_t *ctx, const uint8_t *in, size_t n, uint8_t *out, size_t out_cap)
{
    if (ctx == NULL || (n > 0 && (in == NULL || out == NULL))) {
        return DEC_ERR_ARG;
    }
    if (ctx->magic != DEC_MAGIC) {
        return DEC_ERR_STATE;
    }
    if (ctx->max_chunk != 0 && n > ctx->max_chunk) {
        return DEC_ERR_ARG;
    }
    if (out_cap < n) {
        return DEC_ERR_NOSPACE;
    }

    for (size_t i = 0; i < n; ++i) {
        out[i] = (uint8_t)(in[i] ^ ctx->key);
        ctx->scratch[i % sizeof ctx->scratch] = out[i];
    }
    ctx->total_in += n;
    return (int)n;   /* fine for an exercise; real code would guard n > INT_MAX */
}

void dec_close(dec_ctx_t *ctx)
{
    if (ctx == NULL || ctx->magic != DEC_MAGIC) {
        ++g_bad_close_count;
        return;
    }
    ctx->magic = 0;   /* a second close on the SAME object is now detectable */
    ++g_close_count;
}

unsigned dec_init_count(void)      { return g_init_count; }
unsigned dec_close_count(void)     { return g_close_count; }
unsigned dec_bad_close_count(void) { return g_bad_close_count; }

void dec_reset_counters(void)
{
    g_init_count = 0;
    g_close_count = 0;
    g_bad_close_count = 0;
}
