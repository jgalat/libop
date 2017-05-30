#ifndef __RESULT_H__
#define __RESULT_H__

typedef double * result;

result new_result();
void delete_result(result);
void set_result(result, double);

#endif /* __RESULT_H__ */
