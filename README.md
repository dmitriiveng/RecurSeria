# RecurSeria

|  [Idea](#idea)  |  [Fetch](#fetch)  |  [Build](#build)  |  [Tutorial](#tutorial)  |

RecurSeria is a C++26 macro-free serialization library that uses modules and reflection.

> [!NOTE]
> Work in progress: the project is still raw and not thoroughly tested.

# Idea

The core idea of the library is to simplify adding support for new types and formats by introducing an abstraction layer between them.

The library maps each type to a format. Adding a new format only requires writing the conversion logic for primitives into that format and providing the tools for working with it. Currently, there are two such tools:

1. concatenation of a sequence of objects of the type mapped to the format into a single object of the same type, and vice versa;

2. concatenation of an associative data structure of objects of the type mapped to the format into a single object of the same type, and vice versa.

Functions for working with specific types (or generic functions for working with types that satisfy some concept) use the functionality described above and thus can uniformly work with any format.

Thus, when writing code to support a new type, there is no need to write separate support for each format individually. See the [Tutorial](#tutorial) for details.

# Fetch

No release yet.

# Build

**GCC 16.1** or later is required.

```
git clone https://github.com/dmitriiveng/RecurSeria.git

cmake -B build -G Ninja .

ninja -C build
```

# Tutorial

Out of the box:

Types:
- STL containers (`std::vector`, `std::map`, ...);
- tuples and `std::pair`;
- aggregates (via reflection).
Formats:
- YAML (via yaml-cpp)
- Binary (minimal implementation)

### How to import library

The library can be imported using modules
```cpp
// core
import recurseria;
// yaml target
import recurseria.yaml;
// binary
import recurseria.binary;
```
### How to serialize object

**Use**
```cpp
recurseria::serialize.as<FormatTag, Output, Input>(value);
```

- *FormatTag* is a tag of the format (just a type).
- *Output* is an output type (type mapped to format).
- *Input* is a type of input. It is deduced from *value*, so you only need to specify *FormatTag* and *Output*.
- *value* is an object of ```Input``` type (the object you want to serialize).
- *Return value* is serialized object of type ```Output```

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

### How to deserialize object

**Use**
```cpp
recurseria::deserialize.as<FormatTag, Output, Input>(value);
```

- *FormatTag* is a tag of the format (just a type).
- *Output* is an output type (type of deserialized object).
- *Input* is a type of input (type mapped to format). It is deduced from *value*, so you only need to specify *FormatTag* and *Output*.
- *value* is an object of ```Input``` type (the object you want to deserialize).
- *Return value* is deserialized object of type ```Output```

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

### How to add a new type

#### Containers

A container should satisfy `std::ranges::input_range`, and there should be a way to insert elements into it:
- You can add a `push_back` function.
```cpp
requires(Container& c) {
    c.push_back(std::ranges::range_value_t<Container>{});
}
```
- You can add an `insert` function.
```cpp
requires(Container& c) {
    c.insert(c.end(), std::ranges::range_value_t<Container>{});
}
```
- You can specify a standalone `tag_invoke` function with insertion logic:
```cpp
template <typename Container>
auto tag_invoke(recurseria::get_output_iterator_tag, Container& container){
    // Insertion logic.
}
```

#### Other types

You need to specify two functions, one for serialization, and one for deserialization:

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
		// Do not forget to check if you can use group_sequentially 
		// and decompose_sequentially.
		recurseria::SequenceOpsSupported<FormatTag, Output> &&
		// As well as support for fields, you can do it via 
		// SerializableDeserializableTuple concept.
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

We only used the tools for working with the format (group_sequentially, decompose_sequentially) and serialize.as<>(), so these two functions give support for all formats in which group_sequentially and decompose_sequentially are specified (see the next section).

### How to add a new format

Firstly, you need to create a module for your format that re-exports its partitions:

```cpp
export module new_format;

export import :format_tag;
export import :primitives;
export import :sequence_ops_support;
```

(If your format has associative types, add `export import :associative_ops_support;` as well.)

Now create a tag for your format in your namespace:

```cpp
export module new_format:format_tag;

export namespace new_format {
    struct format_tag {};
}

```

You also need to create functions for serializing / deserializing primitives (and some other types that need to be processed in a special way — for example, `std::string`, because if you did not specify it here, it would be processed as an STL container of chars).

```cpp
export module new_format:primitives;

import std;
import recurseria;

import :format_tag;

export namespace new_format {

	// Serialization
	
	TypeOfSerializedValue tag_invoke(
		format_tag, 
		recurseria::serialize_tag, 
		recurseria::type_tag<TypeOfSerializedValue>, 
		const int& input
	) {
		// serialization logic
	}
	// And so on...
	
	// Deserialization
	
	int tag_invoke(
		format_tag, 
		recurseria::deserialize_tag, 
		recurseria::type_tag<int>, 
		const TypeOfSerializedValue& input
	) {
		// deserialization logic
	}
	// And so on...
}
```

Now you need to define grouping and decomposing operations for `TypeOfSerializedValue`, which is the type mapped to your format (like `YAML::Node` in the built-in YAML target).

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
		// Logic.
	}
	
	// Decomposing.
	
	// decompose_sequentially should return a view, so create one.
	struct your_view ...
	
	auto tag_invoke(
		format_tag,
		recurseria::decompose_sequentially_tag,
		const TypeOfSerializedValue& input
	) -> your_view {
		// Logic.
	}

}
```

If your format has associative types, you can also specify functions to work with them (again grouping and decomposing). Associative containers and aggregates will work even with formats where associative operations are not specified; they will just use the sequential ones (like in the built-in binary target).

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
		// Logic.
	}
	
	// Decomposing.
	
	// decompose_associatively should also return a view, so create one.
	struct your_assoc_view ...
	
	auto tag_invoke(
		format_tag,
		recurseria::decompose_associatively_tag,
		const TypeOfSerializedValue& input
	) -> your_assoc_view {
		// Logic.
	}

}
```

See the built-in YAML target for reference.
