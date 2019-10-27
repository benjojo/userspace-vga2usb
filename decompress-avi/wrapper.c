#include "include/v2u_lib.h"
#include "include/v2u_compression.h"

#include <stdlib.h>
#include <string.h>

#define N(a) (sizeof(a)/sizeof((a)[0]))


/* formats */
typedef struct _V2UFormat {
    const char* name;
    V2U_UINT32 value;
} V2UFormat;

/* compression context */
typedef struct _V2UCompressParams {
    const char* fname;
    int count;
    int level;
    const V2UFormat* cformat;
} V2UCompressParams;

/* decompression context */
typedef struct _V2UDecompressParams {
    const char* fname;
    const V2UFormat* format;
} V2UDecompressParams;

/* compression/decompression formats */
static const V2UFormat v2u_cformats [] = {
    { "CRGB24", V2U_GRABFRAME_FORMAT_CRGB24 },
    { "CBGR24", V2U_GRABFRAME_FORMAT_CBGR24 },
    { "CYUY2",  V2U_GRABFRAME_FORMAT_CYUY2  },
    { "C2VUY",  V2U_GRABFRAME_FORMAT_C2VUY  }
};

static const V2UFormat v2u_formats [] = {
    { "RGB24",  V2U_GRABFRAME_FORMAT_RGB24  },
    { "BGR24",  V2U_GRABFRAME_FORMAT_BGR24  },
    { "ARGB32", V2U_GRABFRAME_FORMAT_ARGB32 },
    { "YUY2",   V2U_GRABFRAME_FORMAT_YUY2   },
    { "2VUY",   V2U_GRABFRAME_FORMAT_2VUY   },
    { "Y8",     V2U_GRABFRAME_FORMAT_Y8     },
    { "YV12",   V2U_GRABFRAME_FORMAT_YV12   },
    { "I420",   V2U_GRABFRAME_FORMAT_I420   }
};

static const V2UFormat* format_by_name(const char* name, const V2UFormat* formats, int n)
{
    int i;
    for (i=0; i<n; i++) {
        if (!strcasecmp(name, formats[i].name)) {
            return formats + i;
        }
    }
    return NULL;
}

static const V2UFormat* format_by_value(V2U_UINT32 value, const V2UFormat* formats, int n)
{
    int i;
    for (i=0; i<n; i++) {
        if (V2U_GRABFRAME_FORMAT(value) == formats[i].value) {
            return formats + i;
        }
    }
    return NULL;
}

/**
 * Read frames from a file and saves them into decompressed BMP files
 */
extern int decompress_frames(int framel, char* blob, char* deblob)
{
    printf("HI 1!\n");

    V2UDecompressParams dp2;
    V2UDecompressParams* dp;
    dp = &dp2;

    memset(dp, 0, sizeof(*dp));

    printf("HI 2!\n");

    format_by_name("CRGB24",v2u_cformats, N(v2u_cformats));

    int result = 1;

    printf("HI 3!\n");

    /* Allocate enough memory for maximum supported resolution */
    const V2U_UINT32 buflen = 2048*2048*3;
    void* compressed = malloc(buflen);
    void* decompressed = malloc(buflen);
    int framenum = 0;

    printf("HI 4!\n");

    memcpy(compressed,blob,framel);

    printf("HI 5!\n");

    if (compressed && decompressed) {

        /* Initialize decompression library */
        V2U_DECOMPRESSION_LIB_CONTEXT libctx = v2udec_init_context();
        printf("HI 6!\n");

        if (libctx) {
            printf("HI 7!\n");

            /* Statistics */
            int PFramesCount = 0;
            int KeyFramesCount = 0;
            V2U_UINT32 PFramesSum = 0;
            V2U_UINT32 KeyFramesSum = 0;

            V2U_UINT32 framelen = 0;
            framelen = framel;
            printf("Debug framelen %d!\n",framelen);

            // while (fread32(in, &framelen)) {
            while (PFramesSum==0) {
                PFramesSum++;
                // if (framelen <= buflen && fread(compressed, framelen, 1, in) == 1) {
                if (1) {
                    printf("HI 8!\n");

                    V2URect rect;
                    V2U_VideoMode mode;
                    V2U_UINT32 palette = v2udec_get_palette(libctx, compressed, framelen);
                    printf("HI 8.1!\n");
                    V2U_TIME timestamp = v2udec_get_timestamp(libctx, compressed, framelen);
                    printf("HI 8.2!\n");
                    if (dp->format) {
                        printf("HI 8.3!\n");
                        palette = dp->format->value;
                        if (!v2udec_set_palette(libctx, compressed, framelen, palette)) {
                            printf("Incompatible decompression format\n");
                            result = 6;
                            break;
                        }
                    }


                    // if (!framenum) {
                    V2U_UINT32 cpalette = v2udec_get_cpalette(libctx, compressed, framelen);
                    const V2UFormat* src = format_by_value(cpalette, v2u_cformats, N(v2u_cformats));
                    const V2UFormat* dest = format_by_value(palette, v2u_formats, N(v2u_formats));
                        if (src && dest) {
                            printf("Decompressing %s -> %s\n", src->name, dest->name);
                        }
                    // }
                    printf("HI 9!\n");
                    v2udec_get_frameres(libctx, compressed, framelen, &rect);
                    printf("HI 10!\n");
                    v2udec_get_videomode(libctx, compressed, framelen, &mode);
                    printf("HI 11!\n");
                    if (v2udec_decompress_frame(libctx, compressed, framelen, decompressed, buflen) > 0) {
                        printf("HI 12!\n");

                        /* Save decompressed frame as a bitmap file */
                        // FILE* out;
                        // char bmpname[1024];
                        printf("Frame #%04d: timestamp=%u framesize=%d width=%d height=%d (%dx%d-%f)\n",
                            framenum, (unsigned int)timestamp, framelen, 
                            rect.width, rect.height, 
                            mode.width, mode.height, mode.vfreq/1000.0);
                            
                        memcpy(deblob,decompressed,buflen);
                        // if (dp->format) {
                        //     sprintf(bmpname,"%s-%s.%04d.bmp", dp->format->name, dp->fname, framenum);
                        // } else {
                        //     sprintf(bmpname,"%s.%04d.bmp", dp->fname, framenum);
                        // }
                        // out = fopen(bmpname, "wb");

                        // if (out) {
                        //     v2u_write_bmp(out, rect.width, rect.height, palette, decompressed);
                        //     fclose(out);
                        //     framenum++;
                            
                        //     /* Collect statistics */
                        //     if (v2udec_is_keyframe(libctx, compressed, framelen)) {
                        //         KeyFramesSum += framelen; 
                        //         KeyFramesCount++;
                        //     } else {
                        //         PFramesSum += framelen; 
                        //         PFramesCount++;
                        //     }
                        // } else {
                        //     printf("Failed to create file %s\n", bmpname);
                        //     result = STATUS_IOERR;
                        //     break;
                        // }
                    } else {
                        printf("Frame #%04d: decoding error\n",  framenum);
                        result = 3;
                        break;
                    }
                    printf("HI 30!\n");
                } else {
                    printf("File format error\n");
                    result = 3;
                    break;
                }
            }
            printf("HI 40!\n");
            v2udec_deinit_context(libctx);
            printf("HI 41!\n");

            /* Calculating average decompressed frame sizes */
            if (KeyFramesCount) {
                printf("Average key frame size:   %d bytes\n",
                    KeyFramesSum/KeyFramesCount);
            }
            if (PFramesCount) {
                printf("Average inner frame size: %d bytes\n",
                    PFramesSum/PFramesCount);
            }

        } else {
            printf("Failed to allocate decompression context\n");
            result = 7;
        }
        printf("HI 50!\n");
        free(compressed);
        printf("HI 51!\n");
        free(decompressed);
    }
    printf("HI 52!\n");
    return result;
}
