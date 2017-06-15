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
  dividend d = (dividend) malloc(sizeof(struct dividend_));
  if (d) {
    d->div_type = DIV_CONTINUOUS;
    d->actual_div = malloc(sizeof(double));
    if (!d->actual_div) {
      delete_dividend(d);
      return NULL;
    }
    *((double *) d->actual_div) = val;
  }
  return d;
}

dividend new_discrete_dividend() {
  dividend d = (dividend) malloc(sizeof(struct dividend_));
  if (d) {
    d->div_type = DIV_DISCRETE;
    struct disc_div *dd = (struct disc_div *) malloc(sizeof(struct disc_div));
    d->actual_div = (void *) dd;
    if (!d->actual_div) {
      delete_dividend(d);
      return NULL;
    }
    dd->size = -1;
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
  if (dd->size == -1) {
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

int div_disc_set_dates(dividend d, int size, ...) {
  if (!d)
    return -1;

  if (d->div_type != DIV_DISCRETE)
    return -1;

  struct disc_div *dd = (struct disc_div *) d->actual_div;

  int n = get_correct_size(dd, size);
  dd->dates = (date *) malloc(sizeof(date) * n);

  if (!dd->dates)
    return -1;

  va_list vl;
  va_start(vl, size);

  int i;
  for (i = 0; i < n; i++) {
    dd->dates[i] = va_arg(vl, date);
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
  dd->ammounts = (date *) malloc(sizeof(double) * n);

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
