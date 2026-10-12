/*
 * mock_dec.h - mock C decoder for Exercise 1 (wrap a C API in a C++ class).
 *
 * "Decoding" is a simple XOR with a key, so output is easy to check.
 * The context is a plain struct that the CALLER owns (no malloc inside),
 * and counters let tests detect leaks and double closes.
 */
#ifndef MOCK_DEC_H
#define MOCK_DEC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes. dec_feed returns the number of bytes written (>= 0) on success. */
enum {
    DEC_OK           =  0,
    DEC_ERR_ARG      = -1,  /* null pointer, key == 0, or chunk larger than max_chunk */
    DEC_ERR_STATE    = -2,  /* context was not initialised or already closed */
    DEC_ERR_NOSPACE  = -3   /* out_cap is smaller than the input length */
};

typedef struct {
    uint8_t key;        /* XOR key; 0 is invalid, so dec_init fails (tests the failure path) */
    size_t  max_chunk;  /* largest input accepted per dec_feed call; 0 = unlimited */
} dec_cfg_t;

/* Caller-owned context. Treat the fields as private. */
typedef struct {
    uint32_t magic;
    uint8_t  key;
    size_t   max_chunk;
    size_t   total_in;
    uint8_t  scratch[64];   /* internal state, makes sizeof(dec_ctx_t) non-trivial */
} dec_ctx_t;

/* Initialise ctx from cfg. Returns DEC_OK or a negative error code. */
int  dec_init(dec_ctx_t *ctx, const dec_cfg_t *cfg);

/* Decode n bytes from in into out. Returns bytes written or a negative error code. */
int  dec_feed(dec_ctx_t *ctx, const uint8_t *in, size_t n, uint8_t *out, size_t out_cap);

/* Release ctx. Closing a context that is not live counts as a "bad close". */
void dec_close(dec_ctx_t *ctx);

/* ---- Test hooks (not thread-safe, fine for a single-threaded exercise) ---- */
unsigned dec_init_count(void);       /* successful dec_init calls */
unsigned dec_close_count(void);      /* dec_close calls on a live context */
unsigned dec_bad_close_count(void);  /* dec_close on an uninitialised or already-closed context */
void     dec_reset_counters(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_DEC_H */
