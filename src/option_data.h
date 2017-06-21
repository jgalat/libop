#ifndef __OPTION_DATA_H__
#define __OPTION_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  OPTION_CALL,
  OPTION_PUT
} option_type;

typedef enum {
  AM_EXERCISE,
  EU_EXERCISE
} exercise_type;

#ifdef __cplusplus
}
#endif

#endif /* __OPTION_DATA_H__ */
