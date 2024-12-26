# TODOs
- [x] add imgui 
- [x] implement imgui into bgfx  
(https://github.com/pr0g/sdl-bgfx-imgui-starter/tree/main/bgfx-imgui)
(https://gist.github.com/RichardGale/6e2b74bc42b3005e08397236e4be0fd0)
- [x] cleanup code (finish window class etc.)
- [x] list debug info
- [x] cleanup imgui (backends & files)
- [x] DOCS!!!!
- [x] Engine namespace
- [x] Input system: keyboard and mouse input (hook window input fetching up, aka. glfw)
- [x] Input system: figure out what to do about modifier keys?
- [x] Input system: optimise with bitmasks
- [x] Input system: visualize internal state in imgui menu
- [x] Input system: add mouse side buttons
- [x] Input system: add more special keys
- [x] cleanup debug
- [x] cleanup input
- [x] cleanup window
- [x] cleanup main
- [x] upgrade debug tools (polish runtime statistics menu & input visualizer)
- [x] fmt debug builded without correct '_d' postfix, investigate (remove fmt)

- [x] Create defines header
- [x] Cleanup API (create a single 'master header' as a public API)
- [x] Build engine to library & test with a user (example) project

- [x] Window settings struct (with callback for detecting changes in resolution, vsync, etc...)
- [x] Consider moving input logic completely out of window class
- [x] Cleanup window class

- [x] Document input class
- [x] Intellisense hovering shows wrong documentation??? (Input::Key & Input::MouseButton)
---

- [ ] Document window class

- [ ] Apply vsync setting in ryn::Window

- [ ] Graphics system
- [ ] Cleanup build system (add deploy config; stripts imgui?)


# Future considerations

- [ ] map out dependencies (includes, to figure out what namespaces are accessible)
- [ ] controller input