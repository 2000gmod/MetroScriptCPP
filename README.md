# MetroScriptCPP

The official MetroScript implementation, written in C++.

## Introduction
An imperative, statically typed, interpreted language, primarily designed for CLI applications.

----

## Roadmaps

### Implementation roadmap overview

| Feature                      | Status                                   |
|------------------------------|------------------------------------------|
| Tokenizer/Lexer              | Complete                                 |
| Parser                       | Complete                                 |
| Syntax Context Evaluator     | Partial implementation                   |
| Symbol Loader                | Partial implementation                   |
| Execution Engine             | Implemented                              |
| MetroScript Standard Library | Partial definition only, not implemented |

---

### Language feature roadmap overview

The following features are to be implemented:

* Function overloading.
* Typed function pointers (return type is strongly typed, argument type and number is dynamic).
* Object oriented programming.
* Overloadable operators for user-defined types.
* Manual memory management (`_new()` and `_delete()` functions) for long-lived objects.