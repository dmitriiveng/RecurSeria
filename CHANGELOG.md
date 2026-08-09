# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.0] - 2026-08-07

First release. The library is still raw and intended for early feedback.

### Added

- YAML and binary built-in formats.
- Support for STL containers, tuples, aggregates via reflection.
- Tools for extending library with new formats and types.
- Packaged the library for consumption:
  - `cmake --install` support;
  - `find_package(RecurSeria CONFIG)` with exported targets (`RecurSeria::core`,
    `RecurSeria::yaml`, `RecurSeria::binary`);
  - compiler guard: GCC 16.2 or later is enforced at configure time.
- `yaml_cpp` module facade (global module fragment + `using`-list) exposing the
  yaml-cpp API to module consumers.
- Boost.UT is now imported through its built-in `boost.ut` module.

[0.1.0]: https://github.com/dmitriiveng/RecurSeria/releases/tag/v0.1.0
