namespace YAML {
    template<>
    struct convert<cString> {
        static Node encode(const cString& rhs) {
            Node node;
            node.push_back(nString(rhs));
            return node;
        }
        static bool decode(const Node& node, cString& rhs) {
            nString s;
            bool success = convert<nString>::decode(node, s);
            if (success) {
                size_t len = s.length();
                rhs = new char[len + 1];
                memcpy(rhs, &s[0], len + 1);
            }
            return success;
        }
    };
    template<>
    struct convert<i8> {
        static Node encode(const i8& rhs) {
            Node node;
            node.push_back(rhs);
            return node;
        }
        static bool decode(const Node& node, i8& rhs) {
            ui16 v;
            bool success = convert<ui16>::decode(node, v);
            if (success) rhs = static_cast<i8>(v);
            return success;
        }
    };
    template<typename T, typename T_True, typename T_Comp, i32 C>
    struct convertVec {
        static Node encode(const T& rhs) {
            Node node;
            for (i32 i = 0; i < C; i++) node.push_back(rhs[i]);
            return node;
        }
        static bool decode(const Node& node, T& rhs) {
            if (!node.IsSequence() || node.size() != C) return false;
            for (i32 i = 0; i < C; i++) rhs[i] = static_cast<T_True>(node[i].as<T_Comp>());
            return true;
        }
    };
    template<typename T, typename T_Comp, i32 C>
    struct convertVecI8 {
        static Node encode(const T& rhs) {
            Node node;
            for (i32 i = 0; i < C; i++) node.push_back(rhs[i]);
            return node;
        }
        static bool decode(const Node& node, T& rhs) {
            if (!node.IsSequence() || node.size() != C) return false;
            for (i32 i = 0; i < C; i++) rhs[i] = node[i].as<T_Comp>();
            return true;
        }
    };
#define YAML_EMITTER_VEC(T, C) \
    YAML::Emitter& operator << (YAML::Emitter& out, const T& v) MACRO_PARAN_L \
    out << YAML::Flow; \
    out << YAML::BeginSeq; \
    for (i32 i = 0; i < C; i++) out << v[i]; \
    out << YAML::EndSeq; \
    return out; \
    MACRO_PARAN_R


#define KEG_DECL_CONV_VEC_COMP(TYPE, TC, COUNT) YAML_EMITTER_VEC(TYPE##v##COUNT, COUNT) template<> struct convert<TYPE##v##COUNT> : public convertVec<TYPE##v##COUNT, TYPE, TC, COUNT> {}
#define KEG_DECL_CONV_VEC(TYPE, COUNT) KEG_DECL_CONV_VEC_COMP(TYPE, TYPE, COUNT)
    KEG_DECL_CONV_VEC_COMP(i8, i16, 2);
    KEG_DECL_CONV_VEC_COMP(i8, i16, 3);
    KEG_DECL_CONV_VEC_COMP(i8, i16, 4);
    KEG_DECL_CONV_VEC(i16, 2);
    KEG_DECL_CONV_VEC(i16, 3);
    KEG_DECL_CONV_VEC(i16, 4);
    KEG_DECL_CONV_VEC(i32, 2);
    KEG_DECL_CONV_VEC(i32, 3);
    KEG_DECL_CONV_VEC(i32, 4);
    KEG_DECL_CONV_VEC(i64, 2);
    KEG_DECL_CONV_VEC(i64, 3);
    KEG_DECL_CONV_VEC(i64, 4);
    KEG_DECL_CONV_VEC_COMP(ui8, ui16, 2);
    KEG_DECL_CONV_VEC_COMP(ui8, ui16, 3);
    KEG_DECL_CONV_VEC_COMP(ui8, ui16, 4);
    KEG_DECL_CONV_VEC(ui16, 2);
    KEG_DECL_CONV_VEC(ui16, 3);
    KEG_DECL_CONV_VEC(ui16, 4);
    KEG_DECL_CONV_VEC(ui32, 2);
    KEG_DECL_CONV_VEC(ui32, 3);
    KEG_DECL_CONV_VEC(ui32, 4);
    KEG_DECL_CONV_VEC(ui64, 2);
    KEG_DECL_CONV_VEC(ui64, 3);
    KEG_DECL_CONV_VEC(ui64, 4);
    KEG_DECL_CONV_VEC(f32, 2);
    KEG_DECL_CONV_VEC(f32, 3);
    KEG_DECL_CONV_VEC(f32, 4);
    KEG_DECL_CONV_VEC(f64, 2);
    KEG_DECL_CONV_VEC(f64, 3);
    KEG_DECL_CONV_VEC(f64, 4);
}