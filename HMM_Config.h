/**
 * @file HMM_Config.h
 * @author Mohamed Refat
 * @brief 
 * @version 0.1
 * @date 2024-04-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HMM_CONFIG_H_
#define HMM_CONFIG_H_


#ifndef SIZE_OF_PAGE
#define SIZE_OF_PAGE (4096)
#endif

#ifndef NUM_OF_PAGES
#define NUM_OF_PAGES (33)
#endif

#define FIRST_FIT (0)
#define BEST_FIT  (1)

#ifndef SEARCHING_ALGORITHM
#define SEARCHING_ALGORITHM FIRST_FIT
#endif

#ifndef ALLOCATED_SIGNATURE
#define ALLOCATED_SIGNATURE (1332001)
#endif

#if (SEARCHING_ALGORITHM != FIRST_FIT) && (SEARCHING_ALGORITHM != BEST_FIT)
    #error"[HMM.h]: Please chose the right searching algorithm [0] for First-Fit, [1] for Best-Fet"
#endif

#endif /* HMM_CONFIG_H_ */