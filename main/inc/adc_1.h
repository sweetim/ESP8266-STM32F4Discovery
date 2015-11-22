#ifndef __ADC_1_H__
#define __ADC_1_H__

#include <stdint.h>
#include <stdbool.h>

void adc_1_init(void);
bool adc_1_is_interupt(void);
void adc_1_clear_interupt(void);
uint16_t adc_1_get_value(void);

#ifdef __cplusplus
extern "C" {
#endif

    void ADC_IRQHandler(void);

#ifdef __cplusplus
}
#endif


#endif // __ADC_1_H__