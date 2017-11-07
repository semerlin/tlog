#ifndef _MDC_H_
#define _MDC_H_

#include "ttypes.h"
#include "tlist.h"

T_BEGIN_DECLS

typedef tlist mdc;

/* mdc interface */
T_EXTERN mdc* mdc_new(void);
T_EXTERN void mdc_free(mdc *pmdc);
T_EXTERN void mdc_clean(mdc *pmdc);
T_EXTERN tint mdc_put(mdc *pmdc, const tchar *key, const tchar *value);
T_EXTERN tchar *mdc_get(const mdc *pmdc, const tchar *key);
T_EXTERN void mdc_remove(mdc *pmdc, const tchar *key);

T_END_DECLS

#endif /* _MDC_H_ */
