#include <cstdlib> /* EXIT_SUCCESS */
#include <cstdio>  /* printf */

#include "fmt/rgb9e5.hh"
#include "fmt/rgb9e5_sqrt.hh"
#include "fmt/r11g11b10.hh"
#include "rand.hh"

/* Encode & Decode functions to test */
#define ENCODER(X) (encode_r11g11b10(X))
#define DECODER(X) (decode_r11g11b10(X))
// #define ENCODER(X) X
// #define DECODER(X) X

/* Process seed */
u32 proc_seed = 0x21386574u;

struct eval_t {
    f32 err_mean = 0.0f;
    f32 err_variance = 0.0f;
    f32 err_min = 0.0f;
    f32 err_max = 0.0f;
};

/* Evaluate the encoder & decoder. */
eval_t eval_range(const f32 start, const f32 end, const u32 steps = 256u) {
    const f32 range = end - start;

    f32 err_acc = 0.0f;
    f32 err_min = 1e30f;
    f32 err_max = 0.0f;

    f32* errors = new f32[steps];

    for (u32 i = 0u; i < steps; ++i) {
        /* Random input */
        const f32 r = start + random_f32(proc_seed) * range;
        const f32 g = start + random_f32(proc_seed) * range;
        const f32 b = start + random_f32(proc_seed) * range;
        const v3 rgb_in { r, g, b };
        
        /* Encode & decode */
        const auto packed = ENCODER(rgb_in);
        const v3 rgb_out = DECODER(packed);

        /* Record the error */
        const f32 r_err = abs(rgb_in.x - rgb_out.x);
        const f32 g_err = abs(rgb_in.y - rgb_out.y);
        const f32 b_err = abs(rgb_in.z - rgb_out.z);
        const f32 err = (r_err + g_err + b_err) / 3.0f;
        errors[i] = err;
        err_acc += err;
    }

    const f32 err_mean = err_acc / steps;
    err_acc = 0.0f; /* re-use accumulator */

    /* Analyse */
    for (u32 i = 0u; i < steps; ++i) {
        const f32 err = errors[i];
        err_min = min(err_min, err);
        err_max = max(err_max, err);
        err_acc += (err - err_mean) * (err - err_mean);
    }

    const f32 err_variance = err_acc / steps;

    return { err_mean, err_variance, err_min, err_max };
}

void print_eval(const char* name, const eval_t r) {
    printf("~ range [ %s ]\n", name);
    printf("  error mean: %f, ", r.err_mean);
    printf("error dev: %f, ", sqrt(r.err_variance));
    printf("error min: %f, ", r.err_min);
    printf("error max: %f\n", r.err_max);
}

int main() {
    const u32 steps = 4096u;
    const eval_t r0_10 = eval_range(0.0f, 10.0f, steps);
    const eval_t r10_100 = eval_range(10.0f, 100.0f, steps);
    const eval_t r100_1000 = eval_range(100.0f, 1000.0f, steps);
    const eval_t r1000_10000 = eval_range(1000.0f, 10000.0f, steps);

    printf("\n");
    print_eval("0..10", r0_10);
    print_eval("10..100", r10_100);
    print_eval("100..1000", r100_1000);
    print_eval("1000..10000", r1000_10000);
    
    const eval_t r0_1000 = eval_range(0.0f, 1000.0f, steps);
    print_eval("0..1000", r0_1000);

    return EXIT_SUCCESS;
}
