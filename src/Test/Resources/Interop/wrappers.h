#pragma once

// ==========================================
// IEnumerator

struct IEnumeratorReference;
struct IEnumerableReference;

typedef long long IEnumeratorVersionType;
constexpr IEnumeratorVersionType IEnumeratorVersion_Init = -1LL;
constexpr IEnumeratorVersionType IEnumeratorVersion_End = -2LL;
constexpr IEnumeratorVersionType IEnumeratorVersion_Min = 0LL;
constexpr IEnumeratorVersionType IEnumeratorVersion_Max = LLONG_MAX;

// ==========================================
