export module recurseria.exceptions:tag_invoke_exception;

import std;

export namespace recurseria {
    /// Exception thrown when a `tag_invoke` call fails inside a CPO.
    ///
    /// Contains the CPO tag name (e.g. "serialize", "decompose_sequentially"),
    /// the mangled name of the type that was being processed,
    /// and the original error message from the caught exception.
    struct tag_invoke_error : std::runtime_error {
        tag_invoke_error(const std::string& tag_name, const std::string& type_name, const std::string& message)
            : std::runtime_error("tag_invoke error [" + tag_name + "] for type '" + type_name + "': " + message)
        {}
    };
}
