/* This file implements all operators for the Matrix types. */

// Helper macro to add assignment operators for the currently defined vector.
#define ADD_COMPOUND_ASSIGNMENT_OPERATORS \
COMPOUND_ASSIGNMENT(+= ); \
COMPOUND_ASSIGNMENT(-= ); \
COMPOUND_ASSIGNMENT(*= ); \
COMPOUND_ASSIGNMENT(/= ); \
COMPOUND_ASSIGNMENT(%= ); \
COMPOUND_ASSIGNMENT(&= ); \
COMPOUND_ASSIGNMENT(|= ); \
COMPOUND_ASSIGNMENT(^= ); \
COMPOUND_ASSIGNMENT(<<=); \
COMPOUND_ASSIGNMENT(>>=);

// Helper macro to add global operators for the currently defined vector.
#define ADD_GLOBAL_OPERATORS \
GLOBAL_OPERATOR(+ ); \
GLOBAL_OPERATOR(- ); \
GLOBAL_OPERATOR(* ); \
GLOBAL_OPERATOR(/ ); \
GLOBAL_OPERATOR(% ); \
GLOBAL_OPERATOR(& ); \
GLOBAL_OPERATOR(| ); \
GLOBAL_OPERATOR(^ ); \
GLOBAL_OPERATOR(<<); \
GLOBAL_OPERATOR(>>); 

