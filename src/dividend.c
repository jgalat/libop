#include <stdlib.h>
#include <stdarg.h>
#include "dividend.h"
#include <debug.h>

struct dividend_ {
  dividend_type div_type;
  void          *actual_div;
};

struct disc_div {
  int size;
  date *dates;
  double *amounts;
};

dividend new_continuous_dividend(double val) {
  double *cd = (double *) malloc(sizeof(double));
  if (!cd)
    return NULL;
  dividend d = (dividend) malloc(sizeof(struct dividend_));
  if (d) {
    d->div_type = DIV_CONTINUOUS;
    d->actual_div = (void *) cd;
    *cd = val;
  }
  return d;
}

dividend new_discrete_dividend() {
  struct disc_div *dd = (struct disc_div *) malloc(sizeof(struct disc_div));
  if (!dd)
    return NULL;
  dividend d = (dividend) malloc(sizeof(struct dividend_));
  if (d) {
    d->div_type = DIV_DISCRETE;
    d->actual_div = (void *) dd;
    dd->size = 0;
    dd->dates = NULL;
    dd->amounts = NULL;
  }
  return d;
}

void delete_dividend(dividend d) {
  struct disc_div *dd;
  if (d) {
    switch (d->div_type) {
      case DIV_DISCRETE:
        dd = (struct disc_div *) d->actual_div;
        if (dd) {
          free(dd->dates);
          free(dd->amounts);
        }
      default:
        free(d->actual_div);
        break;
    }
    free(d);
  }
}


static int get_correct_size(struct disc_div *dd, int size) {
  if (size < 0)
    return -1;
  if (dd->size == 0) {
    dd->size = size;
    return size;
  } else {
    if (dd->size < size) {
      return dd->size;
    } else {
      dd->size = size;
      return size;
    }
  }
}

int div_disc_set_dates(dividend d, time_period tp, int size, ...) {
  if (!d) {
    __DEBUG("Dividend is NULL");
    return -1;
  }

  if (d->div_type != DIV_DISCRETE) {
    __DEBUG("Dividend is continuous");
    return -1;
  }

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

  if (n < 0)
    return -1;

  if (dd->dates)
    free(dd->dates);

  dd->dates = (date *) malloc(sizeof(date) * n);

  if (!dd->dates)
    return -1;

  va_list vl;
  va_start(vl, size);

  int i, days;
  for (i = 0; i < n; i++) {
    days = va_arg(vl, int);
    dd->dates[i] = tp_int_to_date(tp, days);
  }

  va_end(vl);
  return 0;
}

int div_disc_set_amounts(dividend d, int size, ...) {
  if (!d) {
    __DEBUG("Dividend is NULL");
    return -1;
  }

  if (d->div_type != DIV_DISCRETE) {
    __DEBUG("Dividend is continuous");
    return -1;
  }

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

  if (n < 0)
    return -1;

  if (dd->amounts)
    free(dd->amounts);

  dd->amounts = (double *) malloc(sizeof(double) * n);

  if (!dd->amounts)
    return -1;

  va_list vl;
  va_start(vl, size);

  int i;
  for (i = 0; i < n; i++) {
    dd->amounts[i] = va_arg(vl, double);
  }

  va_end(vl);
  return 0;
}

int div_disc_set_dates_(dividend d, time_period tp, int size, int *days) {
  if (!d) {
    __DEBUG("Dividend is NULL");
    return -1;
  }

  if (d->div_type != DIV_DISCRETE) {
    __DEBUG("Dividend is continuous");
    return -1;
  }

  if (!days) {
    __DEBUG("Days is NULL");
    return -1;
  }

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

  if (n < 0)
    return -1;

  if (dd->dates)
    free(dd->dates);

  dd->dates = (date *) malloc(sizeof(date) * n);

  if (!dd->dates)
    return -1;

  int i;
  for (i = 0; i < n; i++) {
    dd->dates[i] = tp_int_to_date(tp, days[i]);
  }

  return 0;
}

int div_disc_set_amounts_(dividend d, int size, double *amounts) {
  if (!d) {
    __DEBUG("Dividend is NULL");
    return -1;
  }

  if (d->div_type != DIV_DISCRETE) {
    __DEBUG("Dividend is continuous");
    return -1;
  }

  if (!amounts){
    __DEBUG("amounts is NULL");
    return -1;
  }

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

  if (n < 0)
    return -1;

  if (dd->amounts)
    free(dd->amounts);

  dd->amounts = (double *) malloc(sizeof(double) * n);

  if (!dd->amounts)
    return -1;

  int i;
  for (i = 0; i < n; i++) {
    dd->amounts[i] = amounts[i];
  }

  return 0;
}

dividend_type div_get_type(dividend d) {
  if (!d) {
    __DEBUG("Dividend is NULL, returning continuous");
    return DIV_CONTINUOUS;
  }
  return d->div_type;
}

double div_cont_get_val(dividend d) {
  if (d && d->div_type == DIV_CONTINUOUS)
    return *((double *) d->actual_div);
  __DEBUG("Dividend is NULL or discrete, returning 0.0");
  return 0.0;
}

int div_disc_get_n(dividend d) {
  if (d && d->div_type == DIV_DISCRETE)
    return ((struct disc_div *) d->actual_div)->size;
    __DEBUG("Dividend is NULL or continuous, returning 0");
  return 0;
}

date *div_disc_get_dates(dividend d) {
  if (d && d->div_type == DIV_DISCRETE)
    return ((struct disc_div *) d->actual_div)->dates;
  __DEBUG("Dividend is NULL or continuous, returning NULL");
  return NULL;
}

double *div_disc_get_amounts(dividend d) {
  if (d && d->div_type == DIV_DISCRETE)
    return ((struct disc_div *) d->actual_div)->amounts;
  __DEBUG("Dividend is NULL or continuous, returning NULL");
  return NULL;
}
