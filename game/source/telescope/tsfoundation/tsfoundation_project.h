#ifndef TSFOUNDATION_PROJECT_H_INCLUDED
#define TSFOUNDATION_PROJECT_H_INCLUDED

typedef enum TsProjectTokenType TsProjectTokenType;
enum TsProjectTokenType
{
    TSPROJECT_TOKEN_null,
    TSPROJECT_TOKEN_identifier,
    TSPROJECT_TOKEN_string_constant,
    TSPROJECT_TOKEN_colon,
};

typedef struct TsProjectToken TsProjectToken;
struct TsProjectToken
{
    char *string;
    int string_length;
    TsProjectTokenType type;
};

internal TsProjectToken
GetNextTsProjectTokenFromBuffer(char *buffer)
{
    TsProjectToken token = {0};
    for(int i = 0; buffer[i]; ++i)
    {
        if(CharIsAlpha(buffer[i]))
        {
            int j = i+1;
            for(; buffer[j] && (CharIsAlpha(buffer[j]) || CharIsDigit(buffer[j]) || buffer[j] == '_' || buffer[j] == ' '); ++j);
            token.string = buffer + i;
            token.string_length = j - i;
            token.type = TSPROJECT_TOKEN_identifier;
            break;
        }
        else if(buffer[i] == '"')
        {
            int j = i+1;
            for(; buffer[j] && buffer[j] != '"'; ++j);
            token.string = buffer + i;
            token.string_length = j - i + 1;
            token.type = TSPROJECT_TOKEN_string_constant;
            break;
        }
        else if(buffer[i] == ':')
        {
            token.string = buffer+i;
            token.string_length = 1;
            token.type = TSPROJECT_TOKEN_colon;
            break;
        }
    }
    return token;
}

typedef struct TsProjectTokenizer TsProjectTokenizer;
struct TsProjectTokenizer
{
    char *at;
};

internal TsProjectToken
PeekTsProjectToken(TsProjectTokenizer *tokenizer)
{
    TsProjectToken token = GetNextTsProjectTokenFromBuffer(tokenizer->at);
    return token;
}

internal b32
RequireTsProjectToken(TsProjectTokenizer *tokenizer, char *string)
{
    b32 match = 0;
    TsProjectToken token = PeekTsProjectToken(tokenizer);
    if(token.string && token.type)
    {
        match = CStringMatchCaseInsensitiveN(string, token.string, token.string_length);
        if(match)
        {
            tokenizer->at = token.string + token.string_length;
        }
    }
    return match;
}

internal b32
RequireTsProjectTokenType(TsProjectTokenizer *tokenizer, TsProjectTokenType type, TsProjectToken *token_ptr)
{
    b32 match = 0;
    TsProjectToken token = PeekTsProjectToken(tokenizer);
    if(token.string && token.type)
    {
        match = token.type == type;
        if(match)
        {
            tokenizer->at = token.string + token.string_length;
            if(token_ptr)
            {
                *token_ptr = token;
            }
        }
    }
    return match;
}


typedef struct TelescopeModule TelescopeModule;
struct TelescopeModule
{
    char *name;
    char *folder_name;
    char *relative_to_telescope_root_path;
    char *define;
    char *module_declaration_type;
    char *module_declaration_name;
    char *platform_declaration_name;
    char *required_linker_flags;
};

#define MAX_TELESCOPE_MODULES ArrayCount(global_telescope_modules)
global TelescopeModule global_telescope_modules[] =
{
    
    // NOTE(rjf): Ts2d
    {
        .name                              = "Ts2d",
        .folder_name                       = "ts2d",
        .relative_to_telescope_root_path   = "ts2d",
        .define                            = "TS2D",
        .module_declaration_type           = "Ts2d *",
        .module_declaration_name           = "renderer",
        .platform_declaration_name         = "ts2d",
        .required_linker_flags             = "opengl32.lib",
    },
    
    // NOTE(rjf): Ts3d
    {
        .name                              = "Ts3d",
        .folder_name                       = "ts3d",
        .relative_to_telescope_root_path   = "ts3d",
        .define                            = "TS3D",
        .module_declaration_type           = "Ts3d *",
        .module_declaration_name           = "renderer",
        .platform_declaration_name         = "ts3d",
        .required_linker_flags             = "opengl32.lib",
    },
    
    // NOTE(rjf): TsAssets
    {
        .name                              = "TsAssets",
        .folder_name                       = "tsassets",
        .relative_to_telescope_root_path   = "tsassets",
        .define                            = "TSASSETS",
        .module_declaration_type           = "TsAssets *",
        .module_declaration_name           = "assets",
        .platform_declaration_name         = "tsassets",
        .required_linker_flags             = "",
    },
    
    // NOTE(rjf): TsDevTerminal
    {
        .name                              = "TsDevTerminal",
        .folder_name                       = "tsdevterminal",
        .relative_to_telescope_root_path   = "tsdevterminal",
        .define                            = "TSDEVTERMINAL",
        .module_declaration_type           = "TsDevTerminal *",
        .module_declaration_name           = "developer_terminal",
        .platform_declaration_name         = "tsdevterminal",
        .required_linker_flags             = "",
    },
    
    // NOTE(rjf): TsInput
    {
        .name                              = "TsInput",
        .folder_name                       = "tsinput",
        .relative_to_telescope_root_path   = "tsinput",
        .define                            = "TSINPUT",
        .module_declaration_type           = "TsInput *",
        .module_declaration_name           = "input",
        .platform_declaration_name         = "tsinput",
        .required_linker_flags             = "",
    },
    
    // NOTE(rjf): TsScript
    {
        .name                              = "TsScript",
        .folder_name                       = "tsscript",
        .relative_to_telescope_root_path   = "tsscript",
        .define                            = "TSSCRIPT",
        .module_declaration_type           = "",
        .module_declaration_name           = "",
        .platform_declaration_name         = "",
        .required_linker_flags             = "",
    },
    
    // NOTE(rjf): TsUI
    {
        .name                              = "TsUI",
        .folder_name                       = "tsui",
        .relative_to_telescope_root_path   = "tsui",
        .define                            = "TSUI",
        .module_declaration_type           = "TsUI *",
        .module_declaration_name           = "ui",
        .platform_declaration_name         = "tsui",
        .required_linker_flags             = "",
    },
    
};

#endif // TSFOUNDATION_PROJECT_H_INCLUDED
