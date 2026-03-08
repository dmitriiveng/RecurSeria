export module serde.core;

// Runtime

//mapper
export import serde.core.runtime.mapper;

//type erased functions generator
export import serde.core.runtime.functions_generator.deserialization;
export import serde.core.runtime.functions_generator.serialization;

//type erased functions
export import serde.core.runtime.type_erasure.functions.deserialization;
export import serde.core.runtime.type_erasure.functions.serialization;

// facade
export import serde.core.runtime.facade;

// Meta

// tag invoke infrastructure
export import serde.core.meta.functions.tag_invokable;

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

// aggregate field getters
export import serde.core.meta.functions.default_functions.aggregate_types.fields_getters.boost_pfr_fields_getter;