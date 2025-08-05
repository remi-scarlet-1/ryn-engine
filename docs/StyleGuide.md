# Naming
- Namespaces: **snake_case**  
- Functions: **camelCase**
- Types: **PascalCase**

# Includes
Order includes as follows:
1. First-party headers.
2. Thrid-party library headers.
3. System headers.

Example:
```cpp
#include "window.h"
#include <iostream>
#include <imgui.h>
```

# Documentation
Use doxygen documentation. Start comments with ```/** */``` and make use of the following tags:  

## Language Style Guide
| Aspect              | Style                                                                |
| ------------------- | -------------------------------------------------------------------- |
| **Tone**            | Neutral, technical, no fluff.                                        |
| **Person**          | Third person.                                                        |
| **Verb Tense**      | Present tense only.                                                  |
| **Voice**           | Active preferred.                                                    |
| **Grammar**         | Grammar optional – clarity first, brevity second.                    |
| **Punctuation**     | Periods optional in lists/summaries.                                 |
| **Capitalization**  | Sentence case. First word capitalized, rest lower unless identifier. |


## Doxygen Tags Allowed
- @author { list of authors }
- @version { version string }
- @date	{ date description }

- @file	<name>
- @namespace <name>
- @class <name>	[<header-file-path>] [<optional-label>]
- @struct <name> [<header-file-path>] [<optional-label>]
- @enum	<name>
- @concept <name>

- @param <parameter-name> { parameter description }	
- @tparam <parameter-name> { parameter description }
- @return { description of the return value }
- @throws <exception-object> { exception description }
- @warning { warning message }
- @deprecated { description }

- @brief { brief description }
- @details { detailed description }
- @attention { attention text }
- @pre { description of required precondition }
- @note { text }
- @todo	{ paragraph describing what is to be done }
- @code{.cpp}, @endcode
- @see { references }
``` example:
@see MyClass
@see MyClass::myFunction
@see myFunction()
@see file_name.h
@see @myGroup  // links to a Doxygen group
```

# File Header Comments
Every file should start with a brief header comment using Doxygen-style comments to describe its purpose and relevant metadata.  
  
Format:
```
/**
 * @file <name>
 * @brief { brief description }
 *
 * @author { list of authors }
 * @version { version string }
 * @date { date description }
 */
 ```
