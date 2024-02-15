#include <stdio.h>
#include <string.h>
#include "bitmap.h"

#define MAX_BITS 32
struct slid_wid {
    unsigned long buffer[BITS_TO_LONGS(MAX_BITS)];   
    uint64_t update_time;
    int base;
    int count;
    int expect;
    int res;
};

#define WIND_SIZE 4
#define SEQ_BITS  4
#define SEQ_MASK ((1 << SEQ_BITS) - 1)
#define SEQ_EXPECT(seq, offset) ((seq + offset) & SEQ_MASK)
#define SEQ_NEXT(seq) SEQ_EXPECT(seq, 1) 
#define SEQ_BEFORE(seq) SEQ_EXPECT(seq, -1)
#define SEQ_DIFF(base, dest) ((dest - base) & SEQ_MASK)

enum {
    RET_SEQ_NORMAL = 0,
    RET_SEQ_OVERFLOW,
    RET_SEQ_LOSS_PKT
};

void slid_win_reinit(struct slid_wid* pwid)
{
    memset(pwid->buffer, 0, sizeof(pwid->buffer));
    pwid->count = 0;
    pwid->expect = -1;
    pwid->base = -1;
    pwid->update_time = 0;

    return;
}

int out_window_process(struct slid_wid *pw, int seq)
{
    int cnt = 0, i = 0;
    int current_bit = pw->expect - pw->base;
    for (i = current_bit; i <MAX_BITS; i++) {
        if (bitmap_get(pw->buffer, i)) {
            pw->count--;
            bitmap_clear(pw->buffer, i);
        } else {
            cnt++;
            printf(" seq [%d] loss\n", SEQ_EXPECT(pw->base, i));
        }

        if (pw->count <= 0 &&
                (SEQ_EXPECT(pw->base, i) == SEQ_EXPECT(seq, -1))) {
            break;
        }
    }

    return cnt;
}

void slid_wind_store(struct slid_wid *pw, int seq)
{
    int cnt = 0;

    if (pw->base == -1) {
        pw->base = pw->expect;
    }

    if (SEQ_DIFF(pw->expect, seq) > WIND_SIZE) {
        cnt = out_window_process(pw, seq); 
        printf("out of window, count is %d\n", cnt);
        slid_win_reinit(pw); 
        pw->expect = SEQ_NEXT(seq);
        return;
    }

    bitmap_set(pw->buffer, SEQ_DIFF(pw->base, seq));
    pw->count++;
    /* TODO: real time */
    pw->update_time = 0;
}

void set_next_expect(struct slid_wid *pw)
{
    int i = 0;
    int current_bit = pw->expect - pw->base;
    for (i = current_bit + 1; i < MAX_BITS; i++) {
        if (bitmap_get(pw->buffer, i)) {
            pw->count--;
            bitmap_clear(pw->buffer, i);
        } else {
            pw->expect = pw->base + i;
            if (pw->count <= 0) {
                slid_win_reinit(pw); 
            }
            break;
        }
    }
}

int process_slide_window(int seq, struct slid_wid *pw)
{
    //initial
    if (pw->expect == -1 || (pw->expect == seq && pw->base == -1)) {
        pw->expect = SEQ_NEXT(seq);
        return RET_SEQ_NORMAL;
    }

    //1. seq is next expect, process buffer
    if (seq == pw->expect && pw->count) {
        //next seq is expect, but buffer has some seq out of order before.
        set_next_expect(pw);
        return RET_SEQ_NORMAL;
    }
    //2. seq is not next expect, store seq to buffer
    slid_wind_store(pw, seq);
    return RET_SEQ_LOSS_PKT;
}

void test1(void)
{
    int A[] = {0, 1, 2, 3, 4, 5, 7,8,9,10,11,12};
    int i = 0;
    struct slid_wid sw;

    slid_win_reinit(&sw);
    for (i = 0; i< sizeof(A)/sizeof(int); i++) {
        process_slide_window(A[i], &sw);
    }
}

void test2(void)
{
    int A[] = {0, 3, 4, 1, 5, 7,10,9,12,11,13};
    int i = 0;
    struct slid_wid sw;

    slid_win_reinit(&sw);
    for (i = 0; i< sizeof(A)/sizeof(int); i++) {
        process_slide_window(A[i], &sw);
    }
}

int main(int argc, char* argv[])
{
    test1();
    test2();
    printf("%d, %d\n", SEQ_NEXT(0), SEQ_NEXT(15));
    printf("%d, %d\n", SEQ_BEFORE(0), SEQ_DIFF(0, 1));
}
