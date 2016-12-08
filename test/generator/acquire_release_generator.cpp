 #include "Halide.h"

namespace {

class AcquireRelease : public Halide::Generator<AcquireRelease> {
public:
    ImageParam input{ Float(32), 2, "input" };

    Func build() {
        Var x("x"), y("y");
        Func f("f");

        f(x, y) = input(x, y) * 2.0f + 1.0f;

        // Use the GPU for this f if a GPU is available.
        Target target = get_target();
        if (target.has_gpu_feature()) {
            f.gpu_tile(x, y, 16, 16).compute_root();
        } else {
            assert(!"This Generator should always be built with a GPU feature");
            exit(1);  // just in case we're build with NDEBUG
        }
        return f;
    }
};

Halide::RegisterGenerator<AcquireRelease> register_my_gen{"acquire_release"};

}  // namespace
