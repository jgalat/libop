#include <stdlib.h>
#include <stdarg.h>
#include <dividend.h>

struct dividend_ {
  dividend_type div_type;
  void          *actual_div;
};

struct disc_div {
  int size;
  date *dates;
  double *ammounts;
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
    dd->ammounts = NULL;
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
          free(dd->ammounts);
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
  if (!d)
    return -1;

  if (d->div_type != DIV_DISCRETE)
    return -1;

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

int div_disc_set_ammounts(dividend d, int size, ...) {
  if (!d)
    return -1;

  if (d->div_type != DIV_DISCRETE)
    return -1;

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

  if (n < 0)
    return -1;

  if (dd->ammounts)
    free(dd->ammounts);

  dd->ammounts = (double *) malloc(sizeof(double) * n);

  if (!dd->ammounts)
    return -1;

  va_list vl;
  va_start(vl, size);

  int i;
  for (i = 0; i < n; i++) {
    dd->ammounts[i] = va_arg(vl, double);
  }

  va_end(vl);
  return 0;
}

int div_disc_set_dates_(dividend d, time_period tp, int *days) {
  if (!d)
    return -1;

  if (d->div_type != DIV_DISCRETE)
    return -1;

  if (!days)
    return -1;

  int size = sizeof(days) / sizeof(int);

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

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

int div_disc_set_ammounts_(dividend d, double *ammounts) {
  if (!d)
    return -1;

  if (d->div_type != DIV_DISCRETE)
    return -1;

  if (!ammounts)
    return -1;

  int size = sizeof(ammounts) / sizeof(double);

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);

  if (dd->ammounts)
    free(dd->ammounts);

  dd->ammounts = (double *) malloc(sizeof(double) * n);

  if (!dd->ammounts)
    return -1;

  int i;
  for (i = 0; i < n; i++) {
    dd->ammounts[i] = ammounts[i];
  }

  return 0;
}

dividend_type div_get_type(dividend d) {
  return d->div_type;
}

double div_cont_get_val(dividend d) {
  if (d && d->div_type == DIV_CONTINUOUS)
    return *((double *) d->actual_div);
  return -1;
}

int div_disc_get_n(dividend d) {
  if (d && d->div_type == DIV_DISCRETE)
    return ((struct disc_div *) d->actual_div)->size;
  return -1;
}

date *div_disc_get_dates(dividend d) {
  if (d && d->div_type == DIV_DISCRETE)
    return ((struct disc_div *) d->actual_div)->dates;
  return NULL;
}

double *div_disc_get_ammounts(dividend d) {
  if (d && d->div_type == DIV_DISCRETE)
    return ((struct disc_div *) d->actual_div)->ammounts;
  return NULL;
}
