function touchDir(dir)
    if not os.isdir(dir) then 
        os.mkdir(dir)
    end
end

function safeExecute(command)
    print("Executing Command: " .. command)
    local result = os.execute(command)
    if not result then 
        error("Command failed: " .. command) 
    end
end

function appendPostfix(strings, postfix)
    local newStrings = {}
    for _, string in ipairs(strings) do
        table.insert(newStrings, string .. postfix)
    end
    return newStrings
end

function tableIsEmpty(t)
    return next(t) == nil
end

function getRelativePath(root, sub)
    -- Ensure the paths are normalized (removing trailing slashes, etc.)
    root = root:gsub("/$", "")  -- Remove trailing slash, if any
    sub = sub:gsub("/$", "")  -- Remove trailing slash, if any

    -- Check if sub starts with root
    if sub:sub(1, #root) == root then
        -- Subtract root from sub
        return sub:sub(#root + 2)  -- Add 2 to remove the '/' separator after root
    else
        error("sub is not inside root!")
    end
end

-- TODO: Support other platforms ("NUMBER_OF_PROCESSORS" is specific to windows)
function getProcessorCount()
    local cores = tonumber(os.getenv("NUMBER_OF_PROCESSORS")) or 4
    return cores
end

--[[
    Performs an advanced copy operation, which copies files from a source directory to a destination directory.
    Optionally, it can filter files by their extensions, exclude certain extensions, and copy files recursively
    to subdirectories while maintaining the directory structure.

    Arguments:
    source          (string): The path to the source directory to copy files from.
    destination     (string): The path to the destination directory to copy files to.
    recursive       (boolean): If true, the function will recursively copy files from subdirectories. Defaults to false.
    extFilters      (table or nil): A table of file extensions (as strings) to include in the copy operation. Extensions are case-insensitive.
                                    If nil, all file types are copied and `extExclusive` gets ignored.
    extExclusive    (boolean): If true, only files with extensions NOT in `extFilters` will be copied. If false, only files with extensions
                               in `extFilters` will be copied. Defaults to false.

    Example:
    advancedCopy("src", "dst", true, {".txt", ".md"}, false)  -- Copies all `.txt` and `.md` files from `src` to `dst` recursively.
    advancedCopy("src", "dst", false, {".jpg", ".png"}, true) -- Copies all files except `.jpg` and `.png` files from `src` to `dst`.
]]
function advancedCopy(source, destination, recursive, extFilters, extExclusive)
    touchDir(destination)

    if not os.isdir(source) then error(string.format([[Advanced copy failed. Source directory "%s" doesn't exist]], source)) end
    if not os.isdir(destination) then error(string.format([[Advanced copy failed. Destination directory "%s" doesn't exist]], destination)) end

    -- Create dictionary out of 'extFilters' where all extensions are lowercase keys and their values equal true
    -- Allows O(1) lookups of registered extensions
    local extensions = {}
    if extFilters ~= nil then
        for _, ext in ipairs(extFilters) do
            extensions[string.lower(ext)] = true
        end
    end

    -- Get all files in source directory, filter and copy if applicable
    local files = os.matchfiles(path.join(source, "*"))
    for _, file in ipairs(files) do
        local ext = string.lower(path.getextension(file))
        local shouldCopy = tableIsEmpty(extensions) or (extensions[ext] and not extExclusive) or (not extensions[ext] and extExclusive)

        if shouldCopy then
            os.copyfile(file, path.join(destination, path.getname(file)))
            printf([[Copied file "%s"]], tostring(file))
        end
    end

    -- If requested apply same logic to all subdirectories
    -- While doing so, retain/replicate source subdirectory structure at the copy destination
    if recursive then
        local directories = os.matchdirs(path.join(source, "*"))
        for _, dir in ipairs(directories) do
            local dirname = path.getname(dir)
            advancedCopy(dir, path.join(destination, dirname), recursive, extFilters, extExclusive)
        end
    end
end