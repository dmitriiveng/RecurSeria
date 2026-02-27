export module serde.core;

// Runtime

//mapper
export import serde.core.runtime.mapper;

//type erased functions generator
export import serde.core.runtime.functions_generator.deserialization;
export import serde.core.runtime.functions_generator.serialization;

//type erased functions
export import serde.core.runtime.type_erasure.void_ptr_any;
export import serde.core.runtime.type_erasure.functions.deserialization;
export import serde.core.runtime.type_erasure.functions.serialization;

// Meta

//types
export import serde.core.meta.functions.types.serialization;
export import serde.core.meta.functions.types.deserialization;

//sequence
export import serde.core.meta.functions.data_structures.sequence.grouping;
export import serde.core.meta.functions.data_structures.sequence.decomposing;

//default

//containers
export import serde.core.meta.functions.default_functions.template_containers.deserialization;
export import serde.core.meta.functions.default_functions.template_containers.serialization;

//aggregate
export import serde.core.meta.functions.default_functions.aggregate_types.deserialization;
export import serde.core.meta.functions.default_functions.aggregate_types.serialization;
