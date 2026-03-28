#include <HsFFI.h>
#if defined(__cplusplus)
extern "C" {
#endif
extern void tzdbDestroy(HsPtr a1);
extern void duckTimeDestroy(HsPtr a1);
extern HsPtr duckTimeRepr(HsPtr a1);
extern void langDestroy(HsPtr a1);
extern HsPtr langRepr(HsPtr a1);
extern void localeDestroy(HsPtr a1);
extern HsPtr localeRepr(HsPtr a1);
extern void dimensionDestroy(HsPtr a1);
extern HsPtr dimensionListCreate(HsPtr a1, HsInt32 a2);
extern HsInt32 dimensionListLength(HsPtr a1);
extern HsPtr dimensionListPtrs(HsPtr a1);
extern void dimensionListDestroy(HsPtr a1);
extern HsPtr wcurrentReftime(HsPtr a1, HsPtr a2);
extern HsPtr wloadTimeZoneSeries(HsPtr a1);
extern HsPtr wparseRefTime(HsPtr a1, HsPtr a2, HsInt32 a3);
extern HsPtr wparseLang(HsPtr a1);
extern HsPtr wmakeDefaultLocale(HsPtr a1);
extern HsPtr wparseLocale(HsPtr a1, HsPtr a2);
extern HsPtr wparseDimensions(HsInt32 a1, HsPtr a2);
extern HsPtr wparseText(HsPtr a1, HsPtr a2, HsPtr a3, HsPtr a4, HsWord8 a5);
#if defined(__cplusplus)
}
#endif

