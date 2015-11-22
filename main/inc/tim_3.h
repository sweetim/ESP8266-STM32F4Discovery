#ifndef __TIM_3_H__
#define __TIM_3_H__

#include <stdbool.h>

void tim_3_init(void);
bool tim_3_is_interupt(void);
void tim_3_clear_interupt(void);

#ifdef __cplusplus
extern "C" {
#endif

    void TIM3_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // __TIM_3_H__