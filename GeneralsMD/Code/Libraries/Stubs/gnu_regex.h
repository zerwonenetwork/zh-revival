// gnu_regex.h — minimal stub for builds without GNU regex
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long reg_syntax_t;

typedef struct regex_t {
  int _stub;
} regex_t;

// Syntax option flags used by regexpr.cpp
#define RE_CHAR_CLASSES            0x00000001UL
#define RE_CONTEXT_INDEP_ANCHORS   0x00000002UL
#define RE_CONTEXT_INDEP_OPS       0x00000004UL
#define RE_CONTEXT_INVALID_OPS     0x00000008UL
#define RE_INTERVALS               0x00000010UL
#define RE_NO_BK_BRACES            0x00000020UL
#define RE_NO_BK_PARENS            0x00000040UL
#define RE_NO_BK_VBAR              0x00000080UL
#define RE_NO_EMPTY_RANGES         0x00000100UL

static __inline reg_syntax_t re_set_syntax(reg_syntax_t syntax) { return syntax; }
static __inline const char* re_compile_pattern(const char* pattern, unsigned int length, regex_t* buf) { (void)pattern; (void)length; (void)buf; return 0; }
static __inline int re_match(regex_t* buf, const char* string, unsigned int length, int start, void* regs) { (void)buf; (void)string; (void)length; (void)start; (void)regs; return -1; }
static __inline void regfree(regex_t* buf) { (void)buf; }

#ifdef __cplusplus
}
#endif

