# Naming
- Namespaces: **snake_case**  
- Functions: **camelCase**
- Types: **PascalCase**

# Includes
Order includes as follows:
1. First-party headers.
2. System headers.
3. Thrid-party library headers.

Example:
```cpp
#include "window.h"
#include <iostream>
#include <imgui.h>
```

# Documentation
Use doxygen-like documentation. Start comments with ```/** */``` and make use of the following tags:  

- **@brief**: Short description of a function, class, or variable.
- **@param**: Describes a function parameter.
- **@throws**: Describes potential exceptions.
- **@return**: Describes the return value of a function.  

- **@struct**: For struct documentation.
- **@class**: For class documentation.
- **@enum**: For enum documentation.

- **@todo**: Marks something to be done in the future.
- **@deprecated**: Marks a function or class as deprecated.
- **@example**: Provides example usage.
- **@see**: Links to related functions or classes.
- **@warning**: Used to highlight potential issues or warnings.
- **@file**: Describes the overall file.
- **@namespace**: Describes a namespace.
- **@ingroup**: Groups related functions or classes.
- **@author**: Specifies the author.
- **@version**: Specifies the version of the code.

# File Header Comments
Every file should start with a brief header comment using Doxygen-style comments to describe its purpose and relevant metadata.  
  
Format:
```
/**
 * @file <filename>
 * @brief <Short description of the file's purpose>
 * @author <Your Name>
 * @version <Version number (MAJOR, MINOR, PATCH)>
 * @date <Date of creation or last modification>
 */
 ```
