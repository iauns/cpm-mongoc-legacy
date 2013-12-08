cpm-mongoc
==========

[![Build Status](https://travis-ci.org/iauns/cpm-mongoc-legacy.png)](https://travis-ci.org/iauns/cpm-mongoc-legacy)

CPM module for [MongoDB's Legacy C Library](https://github.com/mongodb/mongo-c-driver-legacy.git).
Avoid using this module in preference of the newer mongo-c-driver and libbson
([MongoDB's C Library](https://github.com/mongodb/mongo-c-driver.git)).

Usage
-----

This module adds mongo-c-driver's src directory to your include path. To
access mongo-c-driver's BSON library, for example, simply:

```
#include <bson.h>
```

Options
-------

This module accepts one variable: `CPM_MOD_MONGODB_USE_32BIT`. Set this
variable to true before calling `CPM_AddModule` to force a 32 bit build of the
MongoDB static library.

Requirements
------------

This module requires the 'scons' build system.

Example
-------

Here is a snippet of CMake code which includes the this module:

```
... Common CPM Setup Here ...

# ++ MODULE: MongoDB
CPM_AddModule("mongodb"
  GIT_REPOSITORY "https://github.com/iauns/cpm-mongoc-legacy"
  GIT_TAG "1.0.0")

...

CPM_Finish()

...

# Add CPM dependencies and libraries
add_dependencies(MyExe ${CPM_LIBRARIES})
target_link_libraries(MyExe ${CPM_LIBRARIES})

...

```
