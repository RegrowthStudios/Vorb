#include "stdafx.h"
#include "io/YAMLConverters.h"

#include "YAMLImpl.h"
#include "YAMLInternal.inl"

#define KEG_CONVERT_YAML_AS(TYPE) \
    TYPE keg::NodeValueConverter<TYPE>::convert(Node node) { \
        return node->data.as<TYPE>(); \
    } \
    void keg::NodeValueConverter<TYPE>::write(YAMLEmitter* emitter, TYPE value) { \
            emitter->emitter << value; \
    }
#define KEG_CONVERT_YAML_AS_NUM(TYPE) \
    KEG_CONVERT_YAML_AS(TYPE ## 8) \
    KEG_CONVERT_YAML_AS(TYPE ## 16) \
    KEG_CONVERT_YAML_AS(TYPE ## 32) \
    KEG_CONVERT_YAML_AS(TYPE ## 64) \
    KEG_CONVERT_YAML_AS(TYPE ## 8v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 16v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 32v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 64v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 8v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 16v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 32v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 64v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 8v4) \
    KEG_CONVERT_YAML_AS(TYPE ## 16v4) \
    KEG_CONVERT_YAML_AS(TYPE ## 32v4) \
    KEG_CONVERT_YAML_AS(TYPE ## 64v4)

KEG_CONVERT_YAML_AS(bool)
KEG_CONVERT_YAML_AS(cString)
KEG_CONVERT_YAML_AS(nString)
KEG_CONVERT_YAML_AS_NUM(i)
KEG_CONVERT_YAML_AS_NUM(ui)
KEG_CONVERT_YAML_AS(f32)
KEG_CONVERT_YAML_AS(f32v2)
KEG_CONVERT_YAML_AS(f32v3)
KEG_CONVERT_YAML_AS(f32v4)
KEG_CONVERT_YAML_AS(f64)
KEG_CONVERT_YAML_AS(f64v2)
KEG_CONVERT_YAML_AS(f64v3)
KEG_CONVERT_YAML_AS(f64v4)
