#ifndef __OPTION_DATA_H__
#define __OPTION_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enum of types of options available
 */
typedef enum {
  OPTION_CALL,
  OPTION_PUT
} option_type;

/**
 * Enum of types of exercises available
 */
typedef enum {
  AM_EXERCISE,
  EU_EXERCISE
} exercise_type;

#ifdef __cplusplus
}
#endif

#endif /* __OPTION_DATA_H__ */
