// 2023/3/19
// zhangzhong
// run length encoding

#include "compress/run_length_encoding.h"
#include "util/bitbuf.h"
#include "util/file.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_length_compress(const char* input, const char* output) {
    // open the file
    // read run length
    // print that many copies of the current bit
    // switch the bit
    // then repeat
    // until the input file in exhausted
    // close the file

    FILE* infp = open_file_for_read(input);
    bitbuf_t* inbuf = make_bitbuf(infp);
    FILE* outfp = open_file_for_write(output);
    bitbuf_t* outbuf = make_bitbuf(outfp);

    int prev_bit = 0;
    int curr_bit = 0;
    int curr_run_length = 0;
    while (bitbuf_read_bit(inbuf, &curr_bit) != EOF) {
        if (curr_bit == prev_bit) {
            // read the run length
            curr_run_length++;
            if (curr_run_length > RUN_LENGTH) {
                // 我们必须将目前的run拆开
                // 255 0 x
                bitbuf_write_byte(outbuf, RUN_LENGTH);
                bitbuf_write_byte(outbuf, 0);
                // 不需要switch bit
                // 但是我们需要将curr_run_length减去255
                curr_run_length = 1;
            }
        } else {
            // switch the bit
            // 将curr_run_length写入文件
            bitbuf_write_byte(outbuf, curr_run_length);
            // 因为我们已经读出了一个bit 所以curr run length 应该是1
            curr_run_length = 1;
            // switch bit
            prev_bit = curr_bit;
        }
    }

    bitbuf_flush(outbuf);
    free_bitbuf(inbuf);
    free_bitbuf(outbuf);
    (void)fclose(infp);
    (void)fclose(outfp);
    return 0;
}

int run_length_decompress(const char* input, const char* output) {

    // input就是一个一个的数字
    // read byte
    // 然后根据byte的数量write 相应的bit就可以了
    FILE* infp = open_file_for_read(input);
    bitbuf_t* inbuf = make_bitbuf(infp);
    FILE* outfp = open_file_for_write(output);
    bitbuf_t* outbuf = make_bitbuf(outfp);

    int bit = 0;
    unsigned int run_length = 0;
    while (bitbuf_read_byte(inbuf, &run_length) == 0) {
        for (unsigned int i = 0; i < run_length; i++) {
            bitbuf_write_bit(outbuf, bit);
        }
        // switch the bit
        bit = !bit;
        run_length = 0;
    }

    bitbuf_flush(outbuf);
    free_bitbuf(inbuf);
    free_bitbuf(outbuf);
    (void)fclose(infp);
    (void)fclose(outfp);
    return 0;
}
