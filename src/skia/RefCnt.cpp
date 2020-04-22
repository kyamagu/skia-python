#include "common.h"

void initRefCnt(py::module &m) {
py::class_<SkRefCntBase, sk_sp<SkRefCntBase>>(m, "RefCntBase",
    R"docstring(
    :py:class:`RefCntBase` is the base class for objects that may be shared by
    multiple objects.

    When an existing owner wants to share a reference, it calls ref(). When an
    owner wants to release its reference, it calls unref(). When the shared
    object's reference count goes to zero as the result of an unref() call, its
    (virtual) destructor is called. It is an error for the destructor to be
    called explicitly (or via the object going out of scope on the stack or
    calling delete) if getRefCnt() > 1.
    )docstring")
    .def("unique", &SkRefCntBase::unique,
        R"docstring(
        May return true if the caller is the only owner.

        Ensures that all previous owner's actions are complete.
        )docstring")
    .def("ref", &SkRefCntBase::ref,
        R"docstring(
        Increment the reference count.

        Must be balanced by a call to :py:meth:`unref`.
        )docstring")
    .def("unref", &SkRefCntBase::unref,
        R"docstring(
        Decrement the reference count.

        If the reference count is 1 before the decrement, then delete the
        object. Note that if this is the case, then the object needs to have
        been allocated via new, and not on the stack.
        )docstring")
    ;

py::class_<SkRefCnt, sk_sp<SkRefCnt>, SkRefCntBase>(m, "RefCnt");
}
