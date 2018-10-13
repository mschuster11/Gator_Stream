#ifndef __WAVE_PRIV_H__
#define __WAVE_PRIV_H__

#include <stdio.h>
#include <stdint.h>
#include "fatfs/src/ff.h"

#include "personal/headers/wav_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__i386__) || defined(_M_I86) || defined(_X86_) || defined(__amd64__) || defined(__x86_64__) || defined(_M_X64)
#define WAVE_ENDIAN_LITTLE
#endif

#ifdef WAVE_ENDIAN_LITTLE
#define WAVE_RIFF_CHUNK_ID          0x0046004600490052 // 'FFIR'
#define WAVE_FORMAT_CHUNK_ID        0x00200074006d0066 // ' tmf'
#define WAVE_FACT_CHUNK_ID          0x0074006300610066 // 'tcaf'
#define WAVE_DATA_CHUNK_ID          0x0061007400610064 // 'atad'
#define WAVE_WAVE_ID                0x0045005600410057 // 'EVAW'
#endif

#ifdef WAVE_ENDIAN_BIG
#define WAVE_RIFF_CHUNK_ID          0x0052004900460046 // 'RIFF'
#define WAVE_FORMAT_CHUNK_ID        0x0066006d00740020 // 'fmt '
#define WAVE_FACT_CHUNK_ID          0x0066006100630074 // 'fact'
#define WAVE_DATA_CHUNK_ID          0x0064006100740061 // 'data'
#define WAVE_WAVE_ID                0x0057004100560045 // 'WAVE'
#endif

//#pragma pack(push)
//#pragma pack(1)

#define WAVE_RIFF_HEADER_SIZE   8

typedef struct _WaveFormatChunk WaveFormatChunk;
struct _WaveFormatChunk {
    /* RIFF header */
    uint64_t id;
    uint64_t    size;

    uint32_t    format_tag;
    uint32_t    n_channels;
    uint64_t    sample_rate;
    uint64_t    avg_bytes_per_sec;
    uint32_t    block_align;
    uint32_t    bits_per_sample;

    uint32_t    ext_size;
    uint32_t    valid_bits_per_sample;
    uint64_t    channel_mask;

    uint16_t     sub_format[16];
};

typedef struct _WaveFactChunk WaveFactChunk;
struct _WaveFactChunk {
    /* RIFF header */
    uint64_t    id;
    uint64_t    size;

    uint64_t    sample_length;
};

typedef struct _WaveDataChunk WaveDataChunk;
struct _WaveDataChunk {
    /* RIFF header */
    uint64_t    id;
    uint64_t    size;
};

typedef struct _WaveMasterChunk WaveMasterChunk;
struct _WaveMasterChunk {
    /* RIFF header */
    uint64_t            id;
    uint64_t            size;

    uint64_t            wave_id;

    WaveFormatChunk     format_chunk;
    WaveFactChunk       fact_chunk;
    WaveDataChunk       data_chunk;
};

struct _WaveFile {
    FIL*               fp;
    char*               mode;
    WaveError           error_code;
    WaveMasterChunk     chunk;
    uint16_t*           tmp;
    size_t              tmp_size;
};

#ifdef __cplusplus
}
#endif

#endif  /* __WAVE_PRIV_H__ */
