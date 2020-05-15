#ifndef _COMMON_H_
#define _COMMON_H_

#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

template <typename T>
sk_sp<T> CloneFlattenable(const T& flattenable) {
    auto data = flattenable.serialize();
    return T::Deserialize(data->data(), data->size());
}

sk_sp<SkColorSpace> CloneColorSpace(const SkColorSpace* cs);
sk_sp<SkImage> CloneImage(const SkImage& image);

#endif  // _COMMON_H_
