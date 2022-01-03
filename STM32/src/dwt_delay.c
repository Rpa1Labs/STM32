#include "stm32f3xx_hal.h"
#include "dwt_delay.h"


void DWT_Init(void)
{
    DWT->CTRL |= 1 ; // Active le timer
    
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}


void DWT_Delay(uint32_t us) // microsecondes
{
    // Récupère la valeur actuelle du timer et calcule la valeur du timer à la fin du delay
    uint32_t startTick  = DWT->CYCCNT,
             targetTick = DWT->CYCCNT + us * (SystemCoreClock/1000000);

    // Regarde s'il n'y aura pas d'overflow de la valeur pendant la boucle while ( dépassement de la valeur 4 294 967 295 )
    if (targetTick > startTick) {
        // Pas d'overflow
        while (DWT->CYCCNT < targetTick);
    } else {
        // Overflow
        while (DWT->CYCCNT > startTick || DWT->CYCCNT < targetTick);
    }
}
