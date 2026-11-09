# RynEngine: rules for contributors (human and AI)

## Code structure

- Use C++ modules (`.ixx`) for all engine code. Use headers only for things modules can't express: macros
  (export/log/assert macros, RYN_PLUGIN) and similar preprocessor-only definitions.

## Standard library

- No STL type in any API exported between the runtime and plugins (RYN_API functions)
- No STL containers, strings or allocating facilities in engine code. Use Ryn's own containers (Array, HashMap, String,
  StringView, Span, Box, Function, etc.); all allocations goes through Ryn allocators.
- Allowed: non-allocating, header-only facilities: <type_traits>, <concepts>, <utility>, <compare>,
  <bit>, <cstdint>, <new>, <initializer_list>, <source_location>, <atomic>, <meta>, etc.
- When a needed Ryn facility or container doesn't exist yet, say so and propose adding it rather than reaching for STL.

## Toolchain

- GCC 16+, C++26 with -freflection.
- Platforms: Windows x64 and Linux x64

## Git commits

Use following schema for commit titles: ` Type(Scope): Subject `.  

Type:
```
  - feat	    - A new feature  
  - fix		    - A bug fix  
  - docs	    - Documentation only changes  
  - style  	    - Changes that do not affect the meaning of the code (white-space, format, etc.)  
  - refactor    - A code change that neither fixes a bug nor adds a feature  
  - test  	    - Adding missing tests or correcting existing ones  
  - chore  	    - Changes to the build process or auxiliary tools and libraries  
  - perf  	    - A code change that improves performance  
  - build  	    - Changes that affect the build system  
  - temp  	    - temporary commit that won't be included in change logs  
```

Scope:
```
Optional but recommended. Can be anything specifying the scope of the commit. Ideally project or file name.  
```

Subject:
```
Brief summary of the change in simple present. Not capitalized. No period at the end.  
```

Examples of a good and bad commit:  

```
Bad commit: 'updated README.md'  
Good commit: 'docs(README.md): update with developer tips'
```
