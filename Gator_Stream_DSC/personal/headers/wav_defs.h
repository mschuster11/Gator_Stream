  #ifndef __WAVE_DEFS_H__
#define __WAVE_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* wave file format codes */
#define WAVE_FORMAT_PCM             0x0001
#define WAVE_FORMAT_IEEE_FLOAT      0x0003
#define WAVE_FORMAT_ALAW            0x0006
#define WAVE_FORMAT_MULAW           0x0007
#define WAVE_FORMAT_EXTENSIBLE      0xfffe

#define NUM_TO_ARR(n, s)   s==4 ? ((unsigned char[]){ (unsigned char)(n),               \
                                                      (unsigned char)(n >> 8),          \
                                                      (unsigned char)(n >> 16),         \
                                                      (unsigned char)(n >> 24) }) :     \
                           s==3 ? ((unsigned char[]){ (unsigned char)(n),               \
                                                      (unsigned char)(n >> 8),          \
                                                      (unsigned char)(n >> 16) }) :     \
                           s==2 ? ((unsigned char[]){ (unsigned char)(n),               \
                                                      (unsigned char)(n >> 8) }) :      \
                           s==1 ? ((unsigned char[]){ (unsigned char)(n) }) : (NULL)
                           
#define ARR_TO_NUM(n, s)   s==4 ? (unsigned long int)(((unsigned long int)n[0] << 24) + \
                                                      ((unsigned long int)n[1] << 16) + \
                                                      ((unsigned long int)n[2] << 8)  + \
                                                      ((unsigned long int)n[3])) :      \
                           s==3 ? (unsigned long int)(((unsigned long int)n[0] << 16) + \
                                                      ((unsigned long int)n[1] << 8)  + \
                                                      ((unsigned long int)n[2])) :      \
                           s==2 ? (unsigned long int)(((unsigned long int)n[0] << 8)  + \
                                                      ((unsigned long int)n[1])) :      \
                           s==1 ? (unsigned long int)(((unsigned long int)n[0])) : 0    


typedef enum _WaveError WaveError;
enum _WaveError {
    WAVE_SUCCESS,           /* no error */
    WAVE_ERROR_FORMAT,      /* not a wave file or unsupported wave format */
    WAVE_ERROR_MODE,        /* incorrect mode when opening the wave file or calling mode-specific API */
    WAVE_ERROR_MALLOC,      /* error when {wave} called malloc */
    WAVE_ERROR_STDIO,       /* error when {wave} called a stdio function */
    WAVE_ERROR_PARAM,       /* incorrect parameter passed to the API function */
};

typedef struct _WaveFile WaveFile;

#ifdef __cplusplus
}
#endif

#endif /* __WAVE_DEFS_H__ */
