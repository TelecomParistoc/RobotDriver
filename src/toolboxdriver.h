#ifndef TOOLBOXDRIVER_H
#define TOOLBOXDRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* initialize the toolbox driver, needs to be called before any other operations
 * returns -1 if it was unable to init the toolbox driver, 0 otherwise*/
void initToolboxDriver();

#ifdef __cplusplus
}
#endif

#endif
