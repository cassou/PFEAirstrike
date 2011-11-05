#ifndef REFCOUNT_H
#define REFCOUNT_H

/* Refcounting macros which require an int refcount; member to each struct */

#define REFNEW(x) ((x)->refcount = 0)

#define REF(x) ((x)->refcount += 2)

/* freefn will be called with x as argument if the refcount goes below 1 */
#define DEREF(x,freefn) (x)->refcount -= 2; \
if ((x)->refcount <= 1) {(freefn)(x);}

/* Marks the referenced obj as dead */
#define REFKILL(x) ((x)->refcount |= 1)
/* Is the obj dead? */
#define REFDEAD(x) ((x)->refcount & 1)

#endif
