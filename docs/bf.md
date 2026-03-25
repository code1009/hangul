# 한글 비트맵 폰트 - Johab 8×4×4 방식

---

## 폰트 데이터 배열 구조

`_bf_font_data_16x16_hangul_johab844` 배열의 레이아웃:

```
[초성 set0: 19자]...[초성 set7: 19자]  → 19×8 = 152자
[중성 set0: 21자]...[중성 set3: 21자]  → 21×4 =  84자
[종성 set0: 27자]...[종성 set3: 27자]  → 27×4 = 108자
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

### 자음 

`bf_get_font_bitmap_hangul_johab844_jaeum`

자음은 초성 또는 종성 폰트에서 해당 자소를 찾아 표시한다.

### 모음

`bf_get_font_bitmap_hangul_johab844_moeum`

모음은 `jungseong[0]` 배열에서 중성 인덱스로 직접 조회한다.

---

## 주요 API

| 함수                                                                  | 설명                                                                   |
| --------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| `bf_context_default_setup()`                                          | 기본 컨텍스트 및 폰트 초기화                                           |
| `bf_context_default_get()`                                            | 기본 컨텍스트 포인터 반환                                              |
| `bf_get_cp949_bitmap(ctx, char_code, out)`                            | CP949 코드로 비트맵 조회                                               |
| `bf_get_unicode_bitmap(ctx, char_unicode, out)`                       | 유니코드(UCS-16)로 비트맵 조회                                         |
| `bf_get_font_bitmap(font, index, out)`                                | 폰트 배열에서 index번 비트맵 포인터 반환 (경계 검사 없음, 호출자 책임) |
| `bf_get_font_bitmap_hangul_johab844(font, cho, jung, jong, out, buf)` | 한글 완성자 비트맵 합성                                                |
| `bf_get_font_bitmap_hangul_johab844_jaeum(font, index, out)`          | 자음 낱글자 비트맵 조회                                                |
| `bf_get_font_bitmap_hangul_johab844_moeum(font, index, out)`          | 모음 낱글자 비트맵 조회                                                |

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
| ASCII 저위 (0x00~0x7F)     |  8×16 |      128      | 항상 포함                                |
| 한글 조합형 Johab844       | 16×16 | 344 (벌 합산) | 항상 포함                                |
| CP949 특수문자 1128자      | 16×16 |     1128      | `BF_CONFIG_SUPPORT_FONT_CP949_SPECIAL`   |
| CP949 한자 4888자          | 16×16 |     4888      | `BF_CONFIG_SUPPORT_FONT_CP949_HANJA`     |
