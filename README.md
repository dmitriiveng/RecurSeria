# RecurSeria

RecurSeria is a C++26 macro-free serialization library that uses modules and reflection.

> [!NOTE]
> Work in progress: the project is still raw and not thoroughly tested.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Idea](#idea)
- [Fetch](#fetch)
- [Build](#build)
- [Tutorial](#tutorial)
  - [Import the library](#import-the-library)
  - [Serialize an object](#serialize-an-object)
  - [Deserialize an object](#deserialize-an-object)
  - [Add a new type](#add-a-new-type)
    - [Containers](#containers)
    - [Other types](#other-types)
  - [Add a new format](#add-a-new-format)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Idea

The goal of the library is to simplify adding support for new types and formats by introducing an abstraction layer between them.

The library maps each type to a format. Adding a new format only requires writing the conversion logic for primitives into that format and providing the following tools for working with it:

1. concatenation of a sequence of objects of the type mapped to the format into a single object of the same type, and vice versa;

2. concatenation of an associative data structure of objects of the type mapped to the format into a single object of the same type, and vice versa.

Functions for working with specific types (or generic functions for working with types that satisfy some concept) use the functionality described above and thus can uniformly work with any format.

Thus, when writing code to support a new type, there is no need to write separate support for each format individually. See the [Tutorial](#tutorial) for details.

## Fetch

The library is distributed as source modules, so consumers build it themselves. Two ways are supported:

**1. `FetchContent`** — fetch and use directly:

```cmake
include(FetchContent)

FetchContent_Declare(
  recur_seria
  GIT_REPOSITORY https://github.com/dmitriiveng/RecurSeria.git
  GIT_TAG        v0.1.0
)
FetchContent_MakeAvailable(recur_seria)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE RecurSeria::yaml)
```

**2. `find_package`** — after installing (see [Install](#install)):

```cmake
find_package(RecurSeria CONFIG REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE RecurSeria::yaml)
```

> [!NOTE]
> `import std` support is still experimental in both GCC and CMake. The
> consuming project must enable it and must build the std module with
> `-freflection` (otherwise the reflection facilities of `std::meta` are not
> visible to RecurSeria's modules). A minimal consuming `CMakeLists.txt` looks
> like this:
>
> ```cmake
> cmake_minimum_required(VERSION 4.0.3)
>
> set(CMAKE_CXX_SCAN_FOR_MODULES ON)
> set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
> set(CMAKE_CXX_MODULE_STD ON)
>
> project(app LANGUAGES CXX)
>
> set(CMAKE_CXX_STANDARD 26)
> set(CMAKE_CXX_STANDARD_REQUIRED ON)
> set(CMAKE_CXX_EXTENSIONS OFF)
> set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -freflection")
>
> find_package(RecurSeria CONFIG REQUIRED)
>
> add_executable(app main.cpp)
> target_link_libraries(app PRIVATE RecurSeria::yaml)
> ```

## Build

**GCC 16.2** or later, **CMake 4.0.3** or later, and **Ninja** are required.

```sh
git clone https://github.com/dmitriiveng/RecurSeria.git

cd RecurSeria

cmake -B build -G Ninja .

ninja -C build

./build/recurseria_tests
```

## Install

```sh
git clone https://github.com/dmitriiveng/RecurSeria.git

cd RecurSeria

cmake -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release .

ninja -C build-release

cmake --install build-release --prefix /path/to/prefix
```

Installed layout:

- `include/recurseria/{core,yaml,binary}/` — the library modules (`.ixx`);
- `lib*/cmake/RecurSeria/` — `RecurSeriaConfig.cmake` + exported targets;
- `share/licenses/RecurSeria/` — licenses.

Then point consumers at the prefix with `-DCMAKE_PREFIX_PATH=/path/to/prefix`.

## Tutorial

Out of the box support:

Types:
- STL containers (`std::vector`, `std::map`, ...);
- tuples and `std::pair`;
- aggregates (via reflection).

Formats:
- YAML (via yaml-cpp)
- Binary (minimal implementation)

### Import the library

The library can be imported using modules
```cpp
// core
import recurseria;
// yaml target
import recurseria.yaml;
// binary
import recurseria.binary;
```
### Serialize an object

```cpp
Output result = recurseria::serialize.as<FormatTag, Output, Input>(value);
```

- `FormatTag` is a tag of the format (e.g. `recurseria::yaml::yaml_format_tag`).
- `Output` is the output/return type
- `Input` is a type of input. It is deduced from `value`, so usually it can be omitted.
- `value` is an instance of `Input` type (the object to serialize).
- `result` is a *serialized* object of type `Output`

**Example**
```cpp
import std;
import recurseria.yaml;

struct A{
	std::vector<int> vector_field;
	std::map<std::string, int> map_field;
	double double_field;
};

int main(){
	A object{
		{ 1, 2, 3, 4, 5 },
		{
			{"a", 1},
			{"b", 2},
			{"c", 3}
		},
		41593.4213
	};
	
	YAML::Node serialized = recurseria::serialize.as<
		recurseria::yaml::yaml_format_tag,
		YAML::Node
	>(object);
	
	// YAML::Node is part of yaml-cpp, let's print the result.
	YAML::Emitter out;
	out << serialized;
	std::cout << out.c_str() << std::endl;
}
```

### Deserialize an object

```cpp
recurseria::deserialize.as<FormatTag, Output, Input>(value);
```

- `FormatTag` is a tag of the format (e.g. `recurseria::yaml::yaml_format_tag`).
- `Output` is the output/return type
- `Input` is a type of input. It is deduced from `value`, so usually it can be omitted.
- `value` is an instance of `Input` type (the object to deserialize).
- `result` is a *deserialized* object of type `Output`

**Example**
```cpp
import std;
import recurseria.yaml;

struct A{
	std::vector<int> vector_field;
	std::map<std::string, int> map_field;
	double double_field;
};

int main(){
	A object{
		{ 1, 2, 3, 4, 5 },
		{
			{"a", 1},
			{"b", 2},
			{"c", 3}
		},
		41593.4213
	};
	
	YAML::Node serialized = recurseria::serialize.as<
		recurseria::yaml::yaml_format_tag,
		YAML::Node
	>(object);
	
	// YAML::Node is part of yaml-cpp, let's print the result.
	YAML::Emitter out;
	out << serialized;
	std::cout << out.c_str() << std::endl;

	// Deserialization part.
	A deserialized = recurseria::deserialize.as<
		recurseria::yaml::yaml_format_tag,
		A
	>(serialized);
}
```

### Add a new type

#### Containers

A container should satisfy `std::ranges::input_range`, and there should be a way to insert elements into it:

- a `push_back` function,
```cpp
requires(Container& c) {
    c.push_back(std::ranges::range_value_t<Container>{});
}
```
- an `insert` function,
```cpp
requires(Container& c) {
    c.insert(c.end(), std::ranges::range_value_t<Container>{});
}
```
- or a standalone `tag_invoke` function with insertion logic,
```cpp
template <typename Container>
auto tag_invoke(recurseria::get_output_iterator_tag, Container& container){
    // Insertion logic.
}
```

#### Other types

Two functions need to be defined: one for *serialization*, and one for *deserialization*:

```cpp
export module some_module;

import std;
import recurseria;

namespace my_ns{
	
	struct SomeNewType{
		private:
		
		int a;
		double b;
		
		public:
		
		SomeNewType(int a, double b) : a(a), b(b) {};
		
		const int get_a() { return a; }
		void set_a(int new_value) { a = new_value; }
		
		const double get_b() { return b; }
		void set_b(double new_value) { b = new_value; }
	};
	
	// Serialization
	
	template <typename FormatTag, typename Output>
	requires 
		// Ensure requirements of group_sequentially 
		// and decompose_sequentially are met:
		recurseria::SequenceOpsSupported<FormatTag, Output> &&
		// Ensure requirements for the support of fields are met:
		recurseria::SerializableDeserializableTuple<
			FormatTag, 
			std::tuple<int, double>, 
			Output
		>
	Output tag_invoke(
		FormatTag, 
		recurseria::serialize_tag, 
		recurseria::type_tag<Output>, 
		const SomeNewType& input
	) {
		std::vector<Output> fields{
			recurseria::serialize.as<FormatTag, Output>(
				input.get_a()
			),
			recurseria::serialize.as<FormatTag, Output>(
				input.get_b()
			),
		}; // It is not zero-copy so far.
		
		return recurseria::group_sequentially(FormatTag{}, fields);
	}
	
	// Deserialization
	
	template <typename FormatTag, typename Input>
	requires 
		recurseria::SequenceOpsSupported<FormatTag, Input> &&
		recurseria::SerializableDeserializableTuple<
			FormatTag, 
			std::tuple<int, double>, 
			Input
		>
	SomeNewType tag_invoke(
		FormatTag, 
		recurseria::deserialize_tag, 
		recurseria::type_tag<SomeNewType>,
		const Input& input
	) {
		// Here fields is a view.
		auto fields = recurseria::decompose_sequentially(FormatTag{}, input);
		auto it = fields.begin();
		
		int a = recurseria::deserialize.as<FormatTag, int>(*it);
		++it;
		double b = recurseria::deserialize.as<FormatTag, double>(*it);
		
		return SomeNewType{a, b};
	}
	
}
```

We only used the tools for working with the format (group_sequentially, decompose_sequentially) and `serialize.as<>()`, so these two functions give support for all formats in which group_sequentially and decompose_sequentially are specified (see the next section).

### Add a new format

Firstly, create a module for the new format that re-exports its partitions:

```cpp
export module new_format;

export import :format_tag;
export import :primitives;
export import :sequence_ops_support;
```

> [!NOTE]
> If the format has associative types, add `export import :associative_ops_support;` as well.

Now create a namespace containing a tag for the format:

```cpp
export module new_format:format_tag;

export namespace new_format {
    struct format_tag {};
}

```

Then, define functions for serializing / deserializing primitives and other
types that need to be processed in a special way (e.g. `std::string`, by
default it's processed as an STL container of `char`s).

```cpp
export module new_format:primitives;

import std;
import recurseria;

import :format_tag;

export namespace new_format {

	// Serialization.
	
	TypeOfSerializedValue tag_invoke(
		format_tag, 
		recurseria::serialize_tag, 
		recurseria::type_tag<TypeOfSerializedValue>, 
		const int& input
	) {
		// Serialization logic.
	}
	// And so on...
	
	// Deserialization.
	
	int tag_invoke(
		format_tag, 
		recurseria::deserialize_tag, 
		recurseria::type_tag<int>, 
		const TypeOfSerializedValue& input
	) {
		// Deserialization logic.
	}
	// And so on...
}
```

Finally, define grouping and decomposing operations for `TypeOfSerializedValue`, which is the type mapped to the new format (like `YAML::Node` in the built-in YAML target).

```cpp
export module new_format:sequence_ops_support;

import std;
import recurseria;

import :format_tag;

export namespace new_format {
	
	// Grouping.
	
	auto tag_invoke(
		format_tag,
		recurseria::group_sequentially_tag,
		std::ranges::input_range auto&& input
	) -> TypeOfSerializedValue
		requires std::convertible_to<
			std::ranges::range_value_t<decltype(input)>, 
			TypeOfSerializedValue
		>		
	{
		// Grouping logic.
	}
	
	// Decomposing.
	
	// decompose_sequentially should return a view, so create one.
	struct my_view ...
	
	auto tag_invoke(
		format_tag,
		recurseria::decompose_sequentially_tag,
		const TypeOfSerializedValue& input
	) -> my_view {
		// Decomposing logic.
	}

}
```

To support associative types, functions to work with them can be defined (once again: grouping and decomposing). Associative containers and aggregates will work even with formats without associative operations defined; however, they will use the sequential ones (like in the built-in binary target).

```cpp
export module new_format:associative_ops_support;

import std;
import recurseria;

import :format_tag;

export namespace new_format {
	
	// Grouping.
	
	auto tag_invoke(
		format_tag,
		recurseria::group_associatively_tag,
		std::ranges::input_range auto&& input
	) -> TypeOfSerializedValue {
		// Grouping logic.
	}
	
	// Decomposing.
	
	// decompose_associatively should also return a view, so create one.
	struct my_assoc_view ...
	
	auto tag_invoke(
		format_tag,
		recurseria::decompose_associatively_tag,
		const TypeOfSerializedValue& input
	) -> my_assoc_view {
		// Decomposing logic.
	}

}
```

See the [built-in YAML target](./src/targets/YAML/) for reference.
