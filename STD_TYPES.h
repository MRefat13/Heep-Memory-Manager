#ifndef STD_TYPES_H
#define STD_TYPES_H

//Standard data type
typedef unsigned int        uint32_t;
typedef unsigned short int  uint16_t;
typedef signed   long  int  sint32_t;
typedef signed   short int  sint16_t;
typedef signed   long long int  sint64_t;
typedef unsigned long long int  uint64t;

typedef unsigned char        uint8_t;
typedef signed   char        sint8_t;

typedef float               f32_t;
typedef double              f64_t;
typedef long     double     f80_t;

#define NULL_ptr ((void*)0)
#define SBRK_ERROR ((void*)-1)

typedef enum
{
    kError,
    kNoError
}error_t;

typedef enum
{
    kFalse,
    kTrue
}bool_t;
#endif /*STD_TYPES_H*/