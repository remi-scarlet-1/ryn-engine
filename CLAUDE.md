# RynEngine: rules for contributors (human and AI)

## Code structure

- Use C++ modules (`.ixx`) for all engine code. Use headers only for things modules can't express: macros
  (export/log/assert macros, RYN_PLUGIN) and similar preprocessor-only definitions.
- A subdirectory made of module partitions has one primary module file named `_Mod.ixx` that collects and
  re-exports them, so it sorts to the top of the directory and the path alone says what it bundles.
  Examples: `source/core/_Mod.ixx` is the primary `ryn.core` module; `source/internal/_Mod.ixx` is the primary
  `ryn.internal` module. Umbrella modules that re-export whole modules (e.g. `source/Ryn.ixx`) are not primary
  modules and keep their own names.
- Module names follow who may import them, not which binary they live in:
  - `ryn`, `ryn.core`, `ryn.platform`, ...: public engine API in the runtime, importable by plugins.
  - `ryn.internal`: runtime-private (private CMake file set), never importable by plugins.
  - `ryn.editor`: the editor plugin.
  - Game code never uses `ryn.*`; it uses its own prefix (e.g. `game.*`).

## Naming

- Function pointers (members, variables and function pointer types) start with `Fn`: `FnConstruct`, `FnPluginGetDesc`.
- Function pointers meant as callbacks, invoked by someone else in response to an event rather than called at will,
  start with `On` instead: `OnLoad`, `OnUnload`.
- `Desc` abbreviates "descriptor" everywhere (`PluginDesc`, `PLUGIN_GET_DESC_SYMBOL`).

## Documentation

- Not everything needs documenting. Three comment styles, by audience:
  - `/** ... */` (Doxygen style): longer documentation for consumers of an entity (types, functions, variables).
  - `///`: a one-line description for consumers, when that is all an entity needs.
  - `//`: notes for engine developers about internals and complex code. Belongs in implementation, not in
    consumer-facing declarations.
- A `/** ... */` block uses these sections, in this order, omitting the ones that don't apply: `@brief`, `@details`,
  `@tparam`, `@param`, `@return`, `@warning`, `@todo`.
- `@brief` is short and declarative: it states what the entity does for its consumer, never how it is implemented.
- `@details` adds what a consumer needs to know (behaviour, constraints, ownership). It only describes the
  implementation when that is so complex the entity would be incomprehensible otherwise.

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
