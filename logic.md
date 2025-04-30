# 📘 Minishell - Parsing and Expansion Overview

Minishell is designed to emulate the behavior of Bash as faithfully as possible, within the limitations of the project. This document provides an overview of the **parsing and expansion** mechanisms, explaining their logic and structure in clear terms.

---

## 🌱 Environment Initialization

At shell startup, Minishell constructs its internal environment based on the system's `envp[]` array.

-   The environment is currently stored using a **hash map** (`hmap`) implementation for fast lookups. This may later be replaced with a more memory-efficient array structure.

-   The environment also contains **internal variables**, which are identified by names beginning with `@`. These are:
    -   not displayed in the exported environment,
    -   not modifiable through built-in commands,
    -   used for internal purposes only (e.g., `@ARGV0`, `@DEFAULT_PATH`).

### 🔧 SHLVL management

-   The `SHLVL` variable is adjusted to reflect the new shell level:

    -   If missing → `SHLVL=1`
    -   If `SHLVL < 0` → reset to `0`
    -   If `SHLVL >= 100` → reset to `1`
    -   Otherwise → incremented by `1`

-   If `envp[]` is empty, a minimal environment is created:
    -   `PWD` is set using `getcwd()`
    -   `SHLVL=1`
    -   A fallback `PATH` is defined using **internal variable**

---

## 🧩 Parsing Strategy

The parser transforms a stream of tokens into an Abstract Syntax Tree (AST) using a **recursive descent** approach. It processes the input based on operator precedence, starting from the most loosely bound operations down to the most tightly bound.

### 📐 Parsing Hierarchy

| **Level**          | **Handler Function**         | **AST Node Type**       |
| ------------------ | ---------------------------- | ----------------------- |
| Logical OR / AND   | `binop_parser(..., ND_OR)`   | `ND_OR`, `ND_AND`       |
| Pipe               | `binop_parser(..., ND_PIPE)` | `ND_PIPE`               |
| Redirection        | `redir_parser()`             | `ND_REDIR`              |
| Command / Subshell | `primary_parser()`           | `ND_CMD`, `ND_SUBSHELL` |

This structure ensures proper parsing order:

-   Simple commands and subshells are parsed first
-   Redirections wrap around them
-   Pipes and logical operators combine them

### 🚨 Error Handling

The parser distinguishes between two types of errors:

-   **Internal errors** (e.g., memory allocation failure):

    -   The AST is immediately freed
    -   Parsing returns `NULL`

-   **Syntax errors** (e.g., missing token, bad input):
    -   The offending token is returned via `*errtok`
    -   An appropriate error code is stored in `*errcode`
    -   Parsing returns `NULL`

---

## 🔄 Expansion Flow

Before parsing, the shell performs **token expansion** to interpret variables, handle quotes, split arguments, and expand wildcards.

### 💲 Variable Expansion

-   Any `$` followed by a valid identifier is expanded:
    -   `$VAR`, `$HOME` → replaced with the value from the environment
    -   `$?` → replaced with the last exit code
    -   `$0` → replaced with the program name from `$ARGV0`
    -   `$1` to `$9` → replaced with an empty string
    -   `$` followed by anything else → treated literally

### 🔗 Token Joining

-   Consecutive `TK_WORD` tokens that are marked as glued are **merged** into a single argument.
-   This mimics how Bash treats words that are not separated by whitespace.
-   It happens at same time we process the expansion of variable (precisly after var expansion of the curent token).

### 🔪 Whitespace Splitting

-   It occurs **after variable expansion** but **before wildcard processing**.
-   If an expanded argument contains unquoted spaces, it is **split into multiple tokens**.
    -   Their positions are recorded during token processing (if `QUOTE_NONE`).

### 🌟 Wildcard Expansion

-   Wildcards (`*`) are **only expanded** when unquoted.
-   If present:
    -   Their positions are recorded during token processing (if `QUOTE_NONE`)
    -   The argument is matched against the current directory's entries
    -   If no matches are found, the pattern is kept as-is
    -   If matches are found, the token is replaced by a token for each file

---

## 🧠 Final Flow Summary

Minishell’s input processing is broken down into four key stages:

1. **Lexing**: Convert raw input into tokens
2. **Expansion**: Perform variable resolution, join/glue, whitespace split, and wildcards
3. **Parsing**: Convert expanded tokens into a structured AST using precedence rules
4. **Execution**: Walk the AST to execute commands (not covered in this document)
