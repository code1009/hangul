# 텍스트 파일의 BOM (Byte Order Mark)

## BOM이란?

BOM(Byte Order Mark)은 텍스트 파일의 **맨 앞에 삽입되는 특수한 바이트 시퀀스**로, 해당 파일의 인코딩 방식과 바이트 순서(Endianness)를 식별하는 데 사용됩니다.

유니코드 표준에서 정의된 코드 포인트 `U+FEFF`(ZERO WIDTH NO-BREAK SPACE)가 파일 시작에 위치할 때 BOM으로 인식됩니다.

---

## BOM의 역할

| 역할 | 설명 |
|------|------|
| **인코딩 식별** | 파일이 UTF-8, UTF-16, UTF-32 중 어떤 인코딩인지 나타냅니다. |
| **바이트 순서 식별** | UTF-16/32에서 빅엔디언(Big-Endian)인지 리틀엔디언(Little-Endian)인지 구분합니다. |
| **자동 감지** | 일부 소프트웨어가 BOM을 읽어 인코딩을 자동으로 판단합니다. |

---

## 인코딩별 BOM 바이트 값

| 인코딩 | BOM 바이트 시퀀스 (16진수) | 설명 |
|--------|--------------------------|------|
| **UTF-8** | `EF BB BF` | 3바이트, 바이트 순서 없음 |
| **UTF-16 BE** | `FE FF` | 2바이트, 빅엔디언 |
| **UTF-16 LE** | `FF FE` | 2바이트, 리틀엔디언 |
| **UTF-32 BE** | `00 00 FE FF` | 4바이트, 빅엔디언 |
| **UTF-32 LE** | `FF FE 00 00` | 4바이트, 리틀엔디언 |

---

## UTF-8 BOM

UTF-8은 바이트 순서가 고정되어 있으므로 BOM이 **필수가 아닙니다**.  
그러나 일부 프로그램(특히 구버전 Windows 소프트웨어)은 UTF-8 BOM이 있어야 인코딩을 올바르게 인식합니다.

### UTF-8 BOM 있음 vs 없음

```
BOM 있음 (UTF-8 with BOM): EF BB BF + 본문 데이터
BOM 없음 (UTF-8 without BOM): 본문 데이터만
```

---

## BOM의 장단점

### 장점
- 인코딩을 명시적으로 표시하여 소프트웨어 간 호환성 향상
- Windows 환경에서 UTF-8 파일임을 명확히 식별 가능
- 메모장, Excel 등 일부 Windows 툴에서 UTF-8 올바르게 표시

### 단점
- **Linux/macOS 환경에서 문제 발생 가능**: BOM을 일반 문자로 인식하여 파싱 오류 유발
- **스크립트 파일 문제**: `#!/usr/bin/env python` 같은 shebang 라인 앞에 BOM이 붙으면 실행 실패
- **HTTP 헤더 파싱 오류**: 웹 서버 설정 파일에 BOM이 포함되면 헤더 파싱 실패 가능
- **XML/JSON 파싱 오류**: 일부 파서에서 BOM을 비정상 문자로 처리
- **불필요한 바이트 낭비**: 작은 파일이 대량일 경우 누적 용량 증가

---

## BOM 처리 방법 (C/C++)

### BOM 감지 예시

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ENCODING_UNKNOWN,
    ENCODING_UTF8_BOM,
    ENCODING_UTF8,
    ENCODING_UTF16_BE,
    ENCODING_UTF16_LE,
    ENCODING_UTF32_BE,
    ENCODING_UTF32_LE,
} encoding_t;

encoding_t detect_bom(const uint8_t *data, size_t size) {
    if (size >= 4 && data[0] == 0x00 && data[1] == 0x00 &&
        data[2] == 0xFE && data[3] == 0xFF)
        return ENCODING_UTF32_BE;

    if (size >= 4 && data[0] == 0xFF && data[1] == 0xFE &&
        data[2] == 0x00 && data[3] == 0x00)
        return ENCODING_UTF32_LE;

    if (size >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF)
        return ENCODING_UTF8_BOM;

    if (size >= 2 && data[0] == 0xFE && data[1] == 0xFF)
        return ENCODING_UTF16_BE;

    if (size >= 2 && data[0] == 0xFF && data[1] == 0xFE)
        return ENCODING_UTF16_LE;

    return ENCODING_UTF8;
}
```

### BOM 건너뛰기 예시

```c
// 파일 읽기 후 BOM을 건너뛰고 실제 데이터 시작 위치 반환
size_t skip_bom(const uint8_t *data, size_t size) {
    if (size >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF)
        return 3;  // UTF-8 BOM: 3바이트 건너뜀
    if (size >= 2 && (data[0] == 0xFE || data[0] == 0xFF))
        return 2;  // UTF-16 BOM: 2바이트 건너뜀
    return 0;      // BOM 없음
}
```

---

## 운영체제별 기본 동작

| 운영체제 | 기본 인코딩 | BOM 처리 |
|----------|------------|----------|
| **Windows** | UTF-16 LE (시스템), UTF-8 BOM (메모장) | BOM 선호, 자동 감지 |
| **Linux** | UTF-8 without BOM | BOM 없는 UTF-8 선호 |
| **macOS** | UTF-8 without BOM | BOM 없는 UTF-8 선호 |

---

## 권장 사항

- **소스 코드 파일**: BOM 없는 UTF-8 사용 권장 (크로스 플랫폼 호환성)
- **스크립트 파일 (sh, py 등)**: 반드시 BOM 없는 UTF-8 사용
- **Windows 전용 파일**: UTF-8 BOM 사용 시 메모장 등에서 올바르게 표시
- **데이터 교환 파일 (CSV, JSON, XML)**: BOM 없는 UTF-8 사용 권장
- **UTF-16/UTF-32**: 반드시 BOM 포함하여 바이트 순서 명시

