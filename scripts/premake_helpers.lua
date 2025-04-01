function touchDir(dir)
    if not os.isdir(dir) then 
        os.mkdir(dir)
    end
end

function safeExecute(command)
    print("Executing: " .. command)
    local result = os.execute(command)
    if not result then 
        error("Command failed: " .. command) 
    end
end

function appendPostfix(paths, postfix)
    local newPaths = {}
    for _, path in ipairs(paths) do
        table.insert(newPaths, path .. postfix)
    end
    return newPaths
end

-- TODO: Support other platforms ("NUMBER_OF_PROCESSORS" is specific to windows)
function getProcessorCount()
    local cores = tonumber(os.getenv("NUMBER_OF_PROCESSORS")) or 4
    return cores
end