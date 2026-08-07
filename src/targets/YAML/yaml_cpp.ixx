module;
#include <yaml-cpp/yaml.h>
export module yaml_cpp;

export {
    namespace YAML {
        using YAML::Node;
        using YAML::iterator;
        using YAML::const_iterator;
        using YAML::convert;
        using YAML::Mark;
        using YAML::Binary;
        using YAML::Dump;
        using YAML::Clone;

        using YAML::operator<<;
        using YAML::operator==;

        using YAML::Load;
        using YAML::LoadFile;
        using YAML::LoadAll;
        using YAML::LoadAllFromFile;
        using YAML::Parser;

        using YAML::Emitter;
        using YAML::Indent;
        using YAML::Alias;
        using YAML::Anchor;
        using YAML::Comment;
        using YAML::VerbatimTag;
        using YAML::LocalTag;
        using YAML::SecondaryTag;
        using YAML::FloatPrecision;
        using YAML::DoublePrecision;
        using YAML::Precision;
        using YAML::Null;

        using YAML::EMITTER_MANIP;
        using YAML::Auto;
        using YAML::TagByKind;
        using YAML::Newline;
        using YAML::EmitNonAscii;
        using YAML::EscapeNonAscii;
        using YAML::EscapeAsJson;
        using YAML::SingleQuoted;
        using YAML::DoubleQuoted;
        using YAML::Literal;
        using YAML::LowerNull;
        using YAML::UpperNull;
        using YAML::CamelNull;
        using YAML::TildeNull;
        using YAML::YesNoBool;
        using YAML::TrueFalseBool;
        using YAML::OnOffBool;
        using YAML::UpperCase;
        using YAML::LowerCase;
        using YAML::CamelCase;
        using YAML::LongBool;
        using YAML::ShortBool;
        using YAML::Dec;
        using YAML::Hex;
        using YAML::Oct;
        using YAML::BeginDoc;
        using YAML::EndDoc;
        using YAML::BeginSeq;
        using YAML::EndSeq;
        using YAML::Flow;
        using YAML::Block;
        using YAML::BeginMap;
        using YAML::EndMap;
        using YAML::Key;
        using YAML::Value;
        using YAML::LongKey;

        using YAML::Exception;
        using YAML::ParserException;
        using YAML::RepresentationException;
        using YAML::InvalidScalar;
        using YAML::KeyNotFound;
        using YAML::TypedKeyNotFound;
        using YAML::InvalidNode;
        using YAML::BadConversion;
        using YAML::BadDereference;
        using YAML::BadSubscript;
        using YAML::BadPushback;
        using YAML::BadInsert;
        using YAML::EmitterException;
        using YAML::BadFile;

        namespace NodeType {
            using YAML::NodeType::value;
            using YAML::NodeType::Undefined;
            using YAML::NodeType::Null;
            using YAML::NodeType::Scalar;
            using YAML::NodeType::Sequence;
            using YAML::NodeType::Map;
        }

        namespace EmitterStyle {
            using YAML::EmitterStyle::value;
            using YAML::EmitterStyle::Default;
            using YAML::EmitterStyle::Block;
            using YAML::EmitterStyle::Flow;
        }
    }
}
