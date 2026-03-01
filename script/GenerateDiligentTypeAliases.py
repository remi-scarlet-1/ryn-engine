import re
import os

# Configuration
PATH_OUTPUT_FILE = "tools/GeneratedDiligentTypeAliases.ixx"
PATH_DILIGENT_ROOT = "cmake-build-debug/_deps/DiligentCore/Graphics/GraphicsEngine/interface"
HEADERS_TO_SCAN = [
    "BlendState.h",
    "BottomLevelAS.h",
    "Buffer.h",
    "BufferView.h",
    "CommandList.h",
    "CommandQueue.h",
    "DepthStencilState.h",
    "DeviceContext.h",
    "DeviceMemory.h",
    "DeviceObject.h",
    "EngineFactory.h",
    "Fence.h",
    "Framebuffer.h",
    "GraphicsTypes.h",
    "InputLayout.h",
    "PipelineResourceSignature.h",
    "PipelineStateCache.h",
    "PipelineState.h",
    "Query.h",
    "RasterizerState.h",
    "RenderPass.h",
    "ResourceMapping.h",
    "Sampler.h",
    "ShaderBindingTable.h",
    "Shader.h",
    "ShaderResourceBinding.h",
    "ShaderResourceVariable.h",
    "Texture.h",
    "TextureView.h",
    "TopLevelAS.h",
]

# Regex Patterns
RE_TYPED_ENUM_BLOCK = re.compile(r'DILIGENT_TYPED_ENUM\(\s*(\w+)\s*,\s*(\w+)\s*\)[^{]*\{([\s\S]*?)\};')
RE_STRUCT_TYPEDEF = re.compile(r'typedef\s+struct\s+(\w+)\s+\1\s*;')
RE_INTERFACE_BEGIN = re.compile(r'DILIGENT_BEGIN_INTERFACE\(\s*(\w+)\s*,')
RE_COMMENT_STRIP = re.compile(r'//.*|/\*[\s\S]*?\*/')
RE_IDENTIFIER = re.compile(r'([a-zA-Z_]\w*)')


# String Utilities
def to_pascal_case(snake_str):
    return "".join(word.capitalize() for word in snake_str.split("_") if word)


def get_plural_name(pascal_name):
    return pascal_name if pascal_name.endswith("s") else f"{pascal_name}s"


def get_singular_name(pascal_name):
    return pascal_name[:-1] if pascal_name.endswith("s") else pascal_name


def clean_member_name(enum_name, member_name):
    enum_parts = [p for p in enum_name.split('_') if p]
    member_parts = [p for p in member_name.split('_') if p]

    idx = 0
    while idx < len(enum_parts) and idx < len(member_parts):
        if enum_parts[idx].startswith(member_parts[idx]) or member_parts[idx].startswith(enum_parts[idx]):
            idx += 1
        else:
            break

    result_parts = member_parts[idx:]

    if not result_parts or result_parts[0][0].isdigit():
        if idx > 0:
            result_parts.insert(0, enum_parts[idx - 1])
        else:
            return member_name

    return "_".join(result_parts)


# Generation Logic
def generate_enum_snippets(content):
    blocks = []
    for match in RE_TYPED_ENUM_BLOCK.finditer(content):
        enum_name, _, body_content = match.groups()
        pascal_base = to_pascal_case(enum_name)
        singular = get_singular_name(pascal_base)
        plural = get_plural_name(pascal_base)

        lines = [
            f"using {singular} = Diligent::{enum_name};",
            f"namespace {plural} {{"
        ]

        clean_body = RE_COMMENT_STRIP.sub('', body_content)
        seen_members = set()

        for entry in clean_body.split(','):
            match_id = RE_IDENTIFIER.search(entry)
            if not match_id: continue

            raw_member = match_id.group(1)
            if raw_member in seen_members: continue

            seen_members.add(raw_member)
            cleaned_member = clean_member_name(enum_name, raw_member)
            lines.append(f"    inline constexpr {singular} {cleaned_member} = Diligent::{raw_member};")

        lines.append("}")
        blocks.append("\n".join(lines))

    return "\n\n".join(blocks)


def generate_struct_snippets(content):
    struct_names = sorted(list(set(RE_STRUCT_TYPEDEF.findall(content))))
    if not struct_names:
        return ""

    lines = [f"using {name} = Diligent::{name};" for name in struct_names]
    return "\n".join(lines)


def generate_interface_snippets(content):
    interface_names = sorted(list(set(RE_INTERFACE_BEGIN.findall(content))))
    if not interface_names:
        return ""

    lines = [f"using {name} = Diligent::{name};" for name in interface_names]
    return "\n".join(lines)


def process_header(rel_path):
    full_path = os.path.join(PATH_DILIGENT_ROOT, rel_path)
    if not os.path.exists(full_path):
        print(f"  [!] Skip: {rel_path} (Not found)")
        return "", "", ""

    with open(full_path, 'r') as f:
        content = f.read()

    struct_code = generate_struct_snippets(content)
    interface_code = generate_interface_snippets(content)
    enum_code = generate_enum_snippets(content)

    return struct_code, interface_code, enum_code


def main():
    print(f"Generating aliases for {len(HEADERS_TO_SCAN)} headers...")

    output_dir = os.path.dirname(PATH_OUTPUT_FILE)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir)

    all_struct_snippets = []
    all_interface_snippets = []
    all_enum_snippets = []

    for header in HEADERS_TO_SCAN:
        print(f"  > Processing: {header}")
        struct_code, interface_code, enum_code = process_header(header)

        if struct_code:
            all_struct_snippets.append(f"    // {header}\n" + "\n".join(f"    {l}" for l in struct_code.splitlines()))

        if interface_code:
            all_interface_snippets.append(f"    // {header}\n" + "\n".join(f"    {l}" for l in interface_code.splitlines()))

        if enum_code:
            all_enum_snippets.append(
                f"    // {header}\n" + "\n".join(f"    {l}" if l.strip() else l for l in enum_code.splitlines()))

    file_lines = [
        "module;",
        "#include \"ryn/Prelude.h\""
    ]

    for h in HEADERS_TO_SCAN:
        file_lines.append(f"#include <{h}>")

    file_lines += [
        "\nexport module RynRuntime.Core:Graphics;",
        "\n/* AUTOMATICALLY GENERATED DILIGENT ALIASES */",
        "export namespace ryn::graphics {"
    ]

    if all_struct_snippets:
        file_lines.append("\n    /* ===== STRUCT ALIASES ===== */")
        file_lines.append("\n\n".join(all_struct_snippets))

    if all_interface_snippets:
        file_lines.append("\n    /* ===== INTERFACE ALIASES ===== */")
        file_lines.append("\n\n".join(all_interface_snippets))

    if all_enum_snippets:
        file_lines.append("\n    /* ===== ENUM ALIASES ===== */")
        file_lines.append("\n\n".join(all_enum_snippets))

    file_lines.append("}")

    with open(PATH_OUTPUT_FILE, 'w') as f:
        f.write("\n".join(file_lines))

    print(f"\nSuccess! File written to: {PATH_OUTPUT_FILE}")


if __name__ == "__main__":
    main()
