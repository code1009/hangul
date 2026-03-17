# 한글 비트맵 폰트 - Johab 8×4×4 방식

## 개요

한글 11172자를 비트맵으로 렌더링하기 위해 **초성 8벌 × 중성 4벌 × 종성 4벌** 조합 방식(Johab 844)을 사용한다.각 자소를 별도 비트맵으로 보관한 뒤, 초성·중성·종성 비트맵을 OR 합성하여 완성형 한 글자를 생성한다.

- 폰트 크기: **16×16 픽셀** (2 bytes/row × 16 rows = 32 bytes/글자)
- 총 폰트 데이터 크기: `(19×8) + (21×4) + (27×4)` = **344자** × 32 bytes = **11,008 bytes**

---

## 한글 11172자 구조

유니코드 한글 범위: **U+AC00 '가' ~ U+D7A3 '힣'**

```
11172자 = 초성 19자 × 중성 21자 × 종성 28자(받침 없음 포함)
```

### 자소 분리 공식

유니코드 코드포인트 `unicode`에서 초성/중성/종성 인덱스 추출:

```
초성 index =  (unicode - 0xAC00) / (21 × 28)
중성 index = ((unicode - 0xAC00) % (21 × 28)) / 28
종성 index =  (unicode - 0xAC00) % 28
```

---

## 자소 인덱스 테이블

### 초성 (19자)

| 번호 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 |
| :--: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| 초성 | ㄱ | ㄲ | ㄴ | ㄷ | ㄸ | ㄹ | ㅁ | ㅂ | ㅃ | ㅅ | ㅆ | ㅇ | ㅈ | ㅉ | ㅊ | ㅋ | ㅌ | ㅍ | ㅎ |

### 중성 (21자)

| 번호 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| :--: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| 중성 | ㅏ | ㅐ | ㅑ | ㅒ | ㅓ | ㅔ | ㅕ | ㅖ | ㅗ | ㅘ | ㅙ | ㅚ | ㅛ | ㅜ | ㅝ | ㅞ | ㅟ | ㅠ | ㅡ | ㅢ | ㅣ |

### 종성 (28자, index 0 = 받침 없음)

| 번호 |   0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 |
| :--: | :----: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| 종성 | (없음) | ㄱ | ㄲ | ㄳ | ㄴ | ㄵ | ㄶ | ㄷ | ㄹ | ㄺ | ㄻ | ㄼ | ㄽ | ㄾ | ㄿ | ㅀ | ㅁ | ㅂ | ㅄ | ㅅ | ㅆ | ㅇ | ㅈ | ㅊ | ㅋ | ㅌ | ㅍ | ㅎ |

> 종성 0번은 "받침 없음"이며, 실제 폰트 데이터에는 포함되지 않는다.
> 따라서 폰트 배열 접근 시 `jongseong - 1`을 인덱스로 사용한다.

---

## 초성/중성/종성 벌 규칙

자소는 주변 자소의 조합에 따라 다른 모양(벌)을 사용한다.
`bf_font_bitmap_hangul_johab844.c`의 세 함수가 각각 벌 번호(0-based)를 반환한다.

### 초성 8벌 (`bf_hangul_johab844_choseong_set`)

| 벌 (set) | 조건                                                  |
| :------: | ----------------------------------------------------- |
|    0    | 종성 없음 + 중성: ㅏㅐㅑㅒㅣ ㅓㅔㅕㅖ (index 0~7, 20) |
|    1    | 종성 없음 + 중성: ㅗㅛㅡ (index 8, 12, 18)            |
|    2    | 종성 없음 + 중성: ㅜㅠ (index 13, 17)                 |
|    3    | 종성 없음 + 중성: ㅘㅙㅚㅢ (index 9, 10, 11, 19)      |
|    4    | 종성 없음 + 중성: ㅝㅞㅟ (index 14, 15, 16)           |
|    5    | 종성 있음 + set이 0이었을 경우                        |
|    6    | 종성 있음 + set이 1 또는 2였을 경우                   |
|    7    | 종성 있음 + set이 3 또는 4였을 경우                   |

### 중성 4벌 (`bf_hangul_johab844_jungseong_set`)

| 벌 (set) | 조건                                      |
| :------: | ----------------------------------------- |
|    0    | 종성 없음 + 초성: ㄱ 또는 ㅋ (index 0, 1) |
|    1    | 종성 없음 + 초성: 그 외                   |
|    2    | 종성 있음 + 초성: ㄱ 또는 ㅋ (index 0, 1) |
|    3    | 종성 있음 + 초성: 그 외                   |

### 종성 4벌 (`bf_hangul_johab844_jongseong_set`)

| 벌 (set) | 중성                                      |
| :------: | ----------------------------------------- |
|    0    | ㅏㅑㅘㅣ (index 0, 2, 9, 20)              |
|    1    | ㅓㅕㅚㅝㅟㅢ (index 4, 6, 11, 14, 16, 19) |
|    2    | ㅐㅒㅔㅖㅙㅞ (index 1, 3, 5, 7, 10, 15)   |
|    3    | ㅗㅛㅜㅠㅡ (index 8, 12, 13, 17, 18)      |

---

## 폰트 데이터 배열 구조

`_bf_font_data_16x16_hangul_johab844` 배열의 레이아웃:

```
[초성 set0: 19자][초성 set1: 19자]...[초성 set7: 19자]   → 19×8 = 152자
[중성 set0: 21자][중성 set1: 21자][중성 set2: 21자][중성 set3: 21자]  → 21×4 = 84자
[종성 set0: 27자][종성 set1: 27자][종성 set2: 27자][종성 set3: 27자]  → 27×4 = 108자
```

> 종성 데이터는 "받침 없음"(index 0)을 제외한 실제 27자만 저장된다.

`bf_font_hangul_johab844_t` 구조체의 포인터들은 `bf_font_setup_hangul_johab844_16x16()` 에서 초기화된다:

```c
ctx->choseong [set] // set: 0~7, 각 set의 초성 배열 시작 포인터
ctx->jungseong[set] // set: 0~3, 각 set의 중성 배열 시작 포인터
ctx->jongseong[set] // set: 0~3, 각 set의 종성 배열 시작 포인터
```

---

## 렌더링 흐름

```
유니코드 코드포인트 (U+AC00 ~ U+D7A3)
        ↓
  초성/중성/종성 index 분리
        ↓
  각 벌(set) 결정
  bf_hangul_johab844_choseong_set()
  bf_hangul_johab844_jungseong_set()
  bf_hangul_johab844_jongseong_set()
        ↓
  폰트 포인터 결정
  font->choseong [choseong_set] + font_glyph_size * choseong
  font->jungseong[jungseong_set] + font_glyph_size * jungseong
  font->jongseong[jongseong_set] + font_glyph_size * (jongseong - 1)
        ↓
  비트맵 OR 합성 → font_bitmap_buffer[i]
        ↓
  bf_font_bitmap_t 반환
```

---

## 한글 낱글자 (자음/모음 독립 렌더링)

완성자가 아닌 낱글자(ㄱ, ㄴ, ㅏ, ㅣ 등)는 별도 함수로 처리한다.

### 낱글자 코드 범위

| 종류           | 유니코드               | 완성형(CP949)   | 개수  |
| -------------- | ---------------------- | --------------- | ----- |
| 자음+모음 51자 | U+3131 ㄱ ~ U+3163 ㅣ  | 0xA4A1 ~ 0xA4D3 | 51자  |
| 자음 30자      | ㄱ~ㅎ (복합 자음 포함) | —              | 30자  |
| 모음 21자      | ㅏ~ㅣ                  | —              | 21 자 |

### 자음 인덱스 (낱글자 표시용, `bf_get_font_bitmap_hangul_johab844_jamo_jaeum`)

자음은 초성 또는 종성 폰트에서 해당 자소를 찾아 표시한다.

| 자음 |   index   | 매핑                    |
| :---: | :-------: | ----------------------- |
|  ㄱ  |   0x00   | choseong[0] index 0     |
|  ㄲ  |   0x01   | choseong[0] index 1     |
|  ㄳ  |   0x02   | jongseong[0] index 2    |
|  ㄴ  |   0x03   | choseong[0] index 2     |
|  ㄵ  |   0x04   | jongseong[0] index 4    |
|  ㄶ  |   0x05   | jongseong[0] index 5    |
|  ㄷ  |   0x06   | choseong[0] index 3     |
|  ㄸ  |   0x07   | choseong[0] index 4     |
|  ㄹ  |   0x08   | choseong[0] index 5     |
| ㄺ~ㅀ | 0x09~0x0F | jongseong[0] index 8~14 |
|  ㅁ  |   0x10   | choseong[0] index 6     |
|  ㅂ  |   0x11   | choseong[0] index 7     |
|  ㅃ  |   0x12   | choseong[0] index 8     |
|  ㅄ  |   0x13   | jongseong[0] index 17   |
| ㅅ~ㅎ | 0x14~0x1D | choseong[0] index 9~18  |

### 모음 (`bf_get_font_bitmap_hangul_johab844_jamo_moeum`)

모음은 `jungseong[0]` 배열에서 중성 인덱스로 직접 조회한다.

---

## 주요 API

| 함수                                                                    | 설명                                                                   |
| ----------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| `bf_context_default_setup()`                                          | 기본 컨텍스트 및 폰트 초기화                                           |
| `bf_context_default_get()`                                            | 기본 컨텍스트 포인터 반환                                              |
| `bf_get_cp949_bitmap(ctx, char_code, out)`                            | CP949 코드로 비트맵 조회                                               |
| `bf_get_unicode_bitmap(ctx, char_unicode, out)`                       | 유니코드(UCS-16)로 비트맵 조회                                         |
| `bf_get_font_bitmap(font, index, out)`                                | 폰트 배열에서 index번 비트맵 포인터 반환 (경계 검사 없음, 호출자 책임) |
| `bf_get_font_bitmap_hangul_johab844(font, cho, jung, jong, out, buf)` | 한글 완성자 비트맵 합성                                                |
| `bf_get_font_bitmap_hangul_johab844_jamo_jaeum(font, index, out)`     | 자음 낱글자 비트맵 조회                                                |
| `bf_get_font_bitmap_hangul_johab844_jamo_moeum(font, index, out)`     | 모음 낱글자 비트맵 조회                                                |

### `bf_font_bitmap_t` 구조체

```c
typedef struct {
    bf_uint8_t* font_bitmap_pointer;  // 비트맵 데이터 포인터
    bf_uint32_t font_bitmap_size;     // 바이트 수 (stride × cy)
    bf_uint32_t font_bitmap_stride;   // 행당 바이트 수 (⌈cx / 8⌉)
    bf_uint32_t font_bitmap_cx;       // 픽셀 너비
    bf_uint32_t font_bitmap_cy;       // 픽셀 높이
} bf_font_bitmap_t;
```

---

## 지원 폰트 목록

| 폰트                       |  크기  |    글자 수    | 설정 매크로                              |
| -------------------------- | :----: | :-----------: | ---------------------------------------- |
| Unknown (미지원 문자 대체) | 16×16 |       1       | 항상 포함                                |
| ASCII 저위 (0x00~0x7F)     | 8×16 |      128      | 항상 포함                                |
| 한글 조합형 Johab844       | 16×16 | 344 (벌 합산) | 항상 포함                                |
| CP949 특수문자 1128자      | 16×16 |     1128     | `BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL` |
| CP949 한자 4888자          | 16×16 |     4888     | `BF_CONFIG_SUPPORT_FONT_CP949_HANJA`   |
