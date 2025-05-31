/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkPathMeasure.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPath.h"
#include "include/core/SkPaint.h"

#include "SkTextOnPath.h"

static void morphpoints(SkPoint dst[], const SkPoint src[], int count,
                        SkPathMeasure& meas, const SkMatrix& matrix) {
    for (int i = 0; i < count; i++) {
        SkPoint pos;
        SkVector tangent;

        matrix.mapXY(src[i].fX, src[i].fY, &pos);
        SkScalar sx = pos.fX;
        SkScalar sy = pos.fY;

        if (!meas.getPosTan(sx, &pos, &tangent)) {
            // set to 0 if the measure failed, so that we just set dst == pos
            tangent.set(0, 0);
        }

        /*  This is the old way (that explains our approach but is way too slow
         SkMatrix    matrix;
         SkPoint     pt;

         pt.set(sx, sy);
         matrix.setSinCos(tangent.fY, tangent.fX);
         matrix.preTranslate(-sx, 0);
         matrix.postTranslate(pos.fX, pos.fY);
         matrix.mapPoints(&dst[i], &pt, 1);
         */
        dst[i].set(pos.fX - tangent.fY * sy, pos.fY + tangent.fX * sy);
    }
}

/*  TODO

 Need differentially more subdivisions when the follow-path is curvy. Not sure how to
 determine that, but we need it. I guess a cheap answer is let the caller tell us,
 but that seems like a cop-out. Another answer is to get Rob Johnson to figure it out.
 */
static void morphpath(SkPath* dst, const SkPath& src, SkPathMeasure& meas,
                      const SkMatrix& matrix) {
    SkPath::Iter    iter(src, false);
    SkPoint         srcP[4], dstP[3];
    SkPath::Verb    verb;

    while ((verb = iter.next(srcP)) != SkPath::kDone_Verb) {
        switch (verb) {
            case SkPath::kMove_Verb:
                morphpoints(dstP, srcP, 1, meas, matrix);
                dst->moveTo(dstP[0]);
                break;
            case SkPath::kLine_Verb:
                // turn lines into quads to look bendy
                srcP[0].fX = SkScalarAve(srcP[0].fX, srcP[1].fX);
                srcP[0].fY = SkScalarAve(srcP[0].fY, srcP[1].fY);
                morphpoints(dstP, srcP, 2, meas, matrix);
                dst->quadTo(dstP[0], dstP[1]);
                break;
            case SkPath::kQuad_Verb:
                morphpoints(dstP, &srcP[1], 2, meas, matrix);
                dst->quadTo(dstP[0], dstP[1]);
                break;
            case SkPath::kConic_Verb:
                morphpoints(dstP, &srcP[1], 2, meas, matrix);
                dst->conicTo(dstP[0], dstP[1], iter.conicWeight());
                break;
            case SkPath::kCubic_Verb:
                morphpoints(dstP, &srcP[1], 3, meas, matrix);
                dst->cubicTo(dstP[0], dstP[1], dstP[2]);
                break;
            case SkPath::kClose_Verb:
                dst->close();
                break;
            default:
                SkDEBUGFAIL("unknown verb");
                break;
        }
    }
}

void SkVisitTextOnPath(const void* text, size_t byteLength, const SkPaint& paint, const SkFont& font,
                       const SkPath& follow, const SkMatrix* matrix,
                       const std::function<void(const SkPath&)>& visitor) {
    if (byteLength == 0) {
        return;
    }

    // Convert to glyph IDs and advances
    int glyphCount = font.countText(text, byteLength, SkTextEncoding::kUTF8);
    if (glyphCount <= 0) return;
    std::vector<SkGlyphID> glyphs(glyphCount);
    font.textToGlyphs(text, byteLength, SkTextEncoding::kUTF8, glyphs.data(), glyphCount);
    std::vector<SkScalar> advances(glyphCount);
    font.getWidths(glyphs.data(), glyphCount, advances.data());

    // Prepare path measuring
    SkPathMeasure       meas(follow, false);
    SkScalar            hOffset = 0;

    SkPath          iterPath;
    SkScalar        xpos = 0.0;
    SkMatrix        scaledMatrix;
    SkScalar        scale = 1.0;

    scaledMatrix.setScale(scale, scale);

    SkScalar pathLength = meas.getLength();
    for (int i = 0; i < glyphCount; ++i) {
        if (xpos > pathLength)
            break;

        if (font.getPath(glyphs[i], &iterPath)) {
            SkPath      tmp;
            SkMatrix    m(scaledMatrix);

            tmp.setIsVolatile(true);
            m.postTranslate(xpos + hOffset, 0);
            if (matrix) {
                m.postConcat(*matrix);
            }
            morphpath(&tmp, iterPath, meas, m);
            visitor(tmp);
        }
        xpos += advances[i];
    }
}

void SkDrawTextOnPath(const void* text, size_t byteLength, const SkPaint& paint, const SkFont& font,
                      const SkPath& follow, const SkMatrix* matrix, SkCanvas* canvas) {
    SkVisitTextOnPath(text, byteLength, paint, font, follow, matrix, [canvas, paint](const SkPath& path) {
        canvas->drawPath(path, paint);
    });
}

void SkDrawTextOnPathHV(const void* text, size_t byteLength, const SkPaint& paint, const SkFont& font,
                        const SkPath& follow, SkScalar h, SkScalar v, SkCanvas* canvas) {
    SkMatrix matrix = SkMatrix::Translate(h, v);
    SkDrawTextOnPath(text, byteLength, paint, font, follow, &matrix, canvas);
}
