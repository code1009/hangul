
#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////

const static int UNICODE20_HANGUL_CHOSEONG_INDEX_TABLE[30] = {
	 0, // 0x3131 ㄱ
	 1, // 0x3132 ㄲ
	-1, // 0x3133 ㄳ x
	 2, // 0x3134 ㄴ
	-1, // 0x3135 ㄵ x
	-1, // 0x3136 ㄶ x
	 3, // 0x3137 ㄷ
	 4, // 0x3138 ㄸ
	 5, // 0x3139 ㄹ
	-1, // 0x313a ㄺ x
	-1, // 0x313b ㄻ x 
	-1, // 0x313c ㄼ x
	-1, // 0x313d ㄽ x
	-1, // 0x313e ㄾ x
	-1, // 0x313f ㄿ x
	-1, // 0x3140 ㅀ x
	 6, // 0x3141 ㅁ
	 7, // 0x3142 ㅂ
	 8, // 0x3143 ㅃ
	-1, // 0x3144 ㅄ x
	 9, // 0x3145 ㅅ
	10, // 0x3146 ㅆ
	11, // 0x3147 ㅇ
	12, // 0x3148 ㅈ
	13, // 0x3149 ㅉ
	14, // 0x314a ㅊ
	15, // 0x314b ㅋ
	16, // 0x314c ㅌ
	17, // 0x314d ㅍ
	18  // 0x314e ㅎ
	};

const static int UNICODE20_HANGUL_JONGSEONG_INDEX_TABLE[30] = {
	 1, // 0x3131 ㄱ
	 2, // 0x3132 ㄲ
	 3, // 0x3133 ㄳ
	 4, // 0x3134 ㄴ
	 5, // 0x3135 ㄵ
	 6, // 0x3136 ㄶ
	 7, // 0x3137 ㄷ
	-1, // 0x3138 ㄸ x
	 8, // 0x3139 ㄹ
	 9, // 0x313a ㄺ
	10, // 0x313b ㄻ
	11, // 0x313c ㄼ
	12, // 0x313d ㄽ
	13, // 0x313e ㄾ
	14, // 0x313f ㄿ
	15, // 0x3140 ㅀ
	16, // 0x3141 ㅁ
	17, // 0x3142 ㅂ
	-1, // 0x3143 ㅃ x
	18, // 0x3144 ㅄ
	19, // 0x3145 ㅅ
	20, // 0x3146 ㅆ
	21, // 0x3147 ㅇ
	22, // 0x3148 ㅈ
	-1, // 0x3149 ㅉ x
	23, // 0x314a ㅊ
	24, // 0x314b ㅋ
	25, // 0x314c ㅌ
	26, // 0x314d ㅍ
	27  // 0x314e ㅎ
	};

const static WCHAR UNICODE20_HANGUL_JONGSEONG_CODE_TABLE[28] = {
	0x0000, // Banlk
	0x3131, // ㄱ
	0x3132, // ㄲ
	0x3133, // ㄳ
	0x3134, // ㄴ
	0x3135, // ㄵ
	0x3136, // ㄶ
	0x3137, // ㄷ
	0x3139, // ㄹ
	0x313a, // ㄺ
	0x313b, // ㄻ
	0x313c, // ㄼ
	0x313d, // ㄽ
	0x313e, // ㄾ
	0x313f, // ㄿ
	0x3140, // ㅀ
	0x3141, // ㅁ
	0x3142, // ㅂ
	0x3144, // ㅄ
	0x3145, // ㅅ
	0x3146, // ㅆ
	0x3147, // ㅇ
	0x3148, // ㅈ
	0x314a, // ㅊ
	0x314b, // ㅋ
	0x314c, // ㅌ
	0x314d, // ㅍ
	0x314e  // ㅎ
	};

const static WCHAR UNICODE20_HANGUL_CHOSEONG_CODE_TABLE[19] = {
	0x3131, // ㄱ
	0x3132, // ㄲ
	0x3134, // ㄴ
	0x3137, // ㄷ
	0x3138, // ㄸ
	0x3139, // ㄹ
	0x3141, // ㅁ
	0x3142, // ㅂ
	0x3143, // ㅃ
	0x3145, // ㅅ
	0x3146, // ㅆ
	0x3147, // ㅇ
	0x3148, // ㅈ
	0x3149, // ㅉ
	0x314a, // ㅊ
	0x314b, // ㅋ
	0x314c, // ㅌ
	0x314d, // ㅍ
	0x314e  // ㅎ
	};





///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
// # SafeStringCopyW ()
//
// - Description:
//                유니코드 문자열을 복사한다.
//
// - Parameters:
//                WCHAR *pTarget        대상 문자열 메모리 주소
//                int    nTargetSize    대상 문자열 메모리 최대 크기
//                WCHAR *pSource        원본 문자열 메모리 주소
//                int    nSourceLength  원본 문자열 길이

// - Return:
//                int  문자열을 복사한 크기
//
//-----------------------------------------------------------------------------

static int SafeStringCopyW (WCHAR *pTarget, int nTargetSize, WCHAR *pSource, int nSourceLength)
{
	int nIndex;
	int nCount;
	
	
	nCount = nTargetSize-1;
	for (nIndex=0; *(pSource+nIndex) != 0x0000 && nIndex<nCount; nIndex++)
	{
		if (nIndex>=nSourceLength && nSourceLength>=0)
		{
			nIndex++;
			break;
		}

		*(pTarget+nIndex) = *(pSource+nIndex);
	}
		
	*(pTarget+nIndex) = 0x0000;

	return nIndex;
}


//-----------------------------------------------------------------------------
//
// # SafeStringLengthW ()
//
// - Description: 
//               strlen() 대체 함수로 숫자를 UNICODE20 문자열 길이를 반환한다.
//               (NULL 문자는 포함되지 않는다.)
//
// - Parameters:
//               WCHAR *pText       UNICODE20 문자열 버퍼
//               int    nMaxLength  UNICODE20 문자열 버퍼 최대크기
//
// - Return:
//               int  문자열 길이
//
//-----------------------------------------------------------------------------

static int SafeStringLengthW (WCHAR *pText, int nMaxLength)
{
	int nIndex;

	for (nIndex=0; nIndex<nMaxLength; nIndex++)
		if (*(pText+nIndex) == 0x0000)
			return nIndex;

	return nMaxLength;
}

//-----------------------------------------------------------------------------
//
// # SafeStringRemoveCharW ()
//
// - Description: 
//               UNICODE20 문자열에서 글자 하나를 지운다.
//
// - Parameters:
//               WCHAR *pText        원본 UNICODE20 문자열
//               int    nTextLength  원본 UNICODE20 문자열 길이
//                                   -1 설정시 SafeStringLengthW()를
//                                   호출하여 길이를 얻음
//               int    nMaxLength   원본 UNICODE20 문자열 최대길이
//               int    nIndex       지워야할 문자 인덱스
// - Return:
//               int  0 실패
//                    1 성공
//
//-----------------------------------------------------------------------------

static int SafeStringRemoveCharW (WCHAR *pText, int nTextLength, int nMaxLength, int nIndex)
{
	int nCharIndex;


	if (nTextLength <  0)
		nTextLength = SafeStringLengthW (pText, nMaxLength);

	if (nTextLength-1 < 0)
		return 0;

	if (nTextLength <= nIndex || nIndex < 0)
		return 0;

	for (nCharIndex=nIndex; nCharIndex<nTextLength; nCharIndex++) // With NULL character
		*(pText+nCharIndex) = *(pText+nCharIndex+1);

	return 1;
}


//-----------------------------------------------------------------------------
//
// # SafeStringInsertCharW ()
//
// - Description: 
//               UNICODE20 문자열에서 글자 하나를 추가한다.
//
// - Parameters:
//               WCHAR *pText        원본 UNICODE20 문자열
//               int    nTextLength  원본 UNICODE20 문자열 길이
//                                   -1 설정시 SafeStringLengthW()를
//                                   호출하여 길이를 얻음
//               int    nMaxLength   원본 UNICODE20 문자열 최대길이
//               WCHAR  nChar        추가할 문자 UNICODE20 코드
//               int    nIndex       추가해야 할 문자 인덱스
// - Return:
//               int  0 실패
//                    1 성공
//
//-----------------------------------------------------------------------------

static int SafeStringInsertCharW (WCHAR *pText, int nTextLength, int nMaxLength, int nIndex, WCHAR nChar)
{
	int nCharIndex;


	if (nTextLength < 0)
		nTextLength = SafeStringLengthW (pText, nMaxLength);

	if (nTextLength >= nMaxLength)
		return 0;

	if (nTextLength < nIndex || nIndex < 0)
		return 0;

	for (nCharIndex=nTextLength+1; nCharIndex>nIndex; nCharIndex--) // With NULL character
		*(pText+nCharIndex) = *(pText+nCharIndex-1);

	*(pText+nIndex) = nChar;

	return 1;
}

//-----------------------------------------------------------------------------
//
// # GetUnionHangulCode ()
//
// - Description:
//                한글을 조합한다.
//
// - Parameters:
//                [IN]  WCHAR nUnionCode    조합중인 UNICODE20 한글 코드
//                [IN]  WCHAR nElementCode  추가할 한글 낱글자 코드
//                [OUT] WCHAR *pResultCode  조합된 코드
//
// - Return:
//                BOOL  한글조합 성공 여부
//
//-----------------------------------------------------------------------------

static BOOL GetUnionHangulCode (WCHAR nUnionCode, WCHAR nElementCode, WCHAR *pResultCode)
{
	int nChoSeongIndex;
	int nJungSeongIndex;
	int nJongSeongIndex;
	int nAdditionalIndex;
	int nUnionElementIndex;


	// UNICODE20 한글 낱글자를 입력받았을 경우 조합을 한다.
	if (0x3131<= nElementCode && nElementCode <= 0x3163)
	{
		//=====================================================================
		// nUnionCode가 최소한 초성+중성 결합상태인지 확인
		//=====================================================================
		if (0xac00 <= nUnionCode && nUnionCode <= 0xd7a3) 
		{
			//-----------------------------------------------------------------
			// nElementCode UNICODE20 한글 자음 낱글자
			//-----------------------------------------------------------------
			if (nElementCode < 0x314f)
			{
				nChoSeongIndex     = ( (nUnionCode-0xac00) / (21*28) );
				nJungSeongIndex    = ( (nUnionCode-0xac00) % (21*28) ) / 28;
				nJongSeongIndex    = ( (nUnionCode-0xac00) % 28      );

				nAdditionalIndex   = UNICODE20_HANGUL_JONGSEONG_INDEX_TABLE[nElementCode - 0x3131];
				nUnionElementIndex = -1;


				if (nJongSeongIndex == 0)
				{
					//---------------------------------------------------------
					// 종성이 없는 문자
					//---------------------------------------------------------
					// 가 + ㄱ = 각
					if (nAdditionalIndex != -1)
					{
						*pResultCode = nUnionCode + nAdditionalIndex;

						return TRUE;
					}
				}
				else
				{
					//---------------------------------------------------------
					// 종성이 두개의 자음으로 이루어진 경우 검사
					//---------------------------------------------------------
					// 각 + ㅅ = 갃
					switch (nJongSeongIndex)
					{
					case 1:  // ㄱ
						if (nAdditionalIndex == 19) nUnionElementIndex =  3; // ㄱ + ㅅ 
						break;

					case 4:  // ㄴ
						if (nAdditionalIndex == 22) nUnionElementIndex =  5; // ㄴ + ㅈ 
						if (nAdditionalIndex == 27) nUnionElementIndex =  6; // ㄴ + ㅎ
						break;

					case 8:  // ㄹ
						if (nAdditionalIndex ==  1) nUnionElementIndex =  9; // ㄹ + ㄱ
						if (nAdditionalIndex == 16) nUnionElementIndex = 10; // ㄹ + ㅁ
						if (nAdditionalIndex == 17) nUnionElementIndex = 11; // ㄹ + ㅂ
						if (nAdditionalIndex == 19) nUnionElementIndex = 12; // ㄹ + ㅅ
						if (nAdditionalIndex == 25) nUnionElementIndex = 13; // ㄹ + ㅌ
						if (nAdditionalIndex == 26) nUnionElementIndex = 14; // ㄹ + ㅍ
						if (nAdditionalIndex == 27) nUnionElementIndex = 15; // ㄹ + ㅎ
						break;

					case 17: // ㅂ
						if (nAdditionalIndex == 19) nUnionElementIndex = 18; // ㅂ + ㅅ
						break;
					}

					if (nUnionElementIndex != -1)
					{
						*pResultCode = 0xac00 + nChoSeongIndex*28*21 + nJungSeongIndex*28 + nUnionElementIndex;
						return TRUE;
					}
				}
			}

			//-----------------------------------------------------------------
			// nElementCode UNICODE20 한글 모음 낱글자
			//-----------------------------------------------------------------
			else 
			{
				nChoSeongIndex     = ( (nUnionCode-0xac00) / (21*28) );
				nJungSeongIndex    = ( (nUnionCode-0xac00) % (21*28) ) / 28;
				nJongSeongIndex    = ( (nUnionCode-0xac00) % 28      );

				//-------------------------------------------------------------
				// 종성이 없는 문자
				//-------------------------------------------------------------
				// 고 + ㅏ = 과
				if (nJongSeongIndex == 0)
				{
					nAdditionalIndex   = nElementCode-0x314f;
					nUnionElementIndex = -1;

					switch (nJungSeongIndex)
					{
					case  8: // ㅗ
						if (nAdditionalIndex == 0 ) nUnionElementIndex =  9;  // ㅗ + ㅏ 
						if (nAdditionalIndex == 1 ) nUnionElementIndex = 10;  // ㅗ + ㅒ
						if (nAdditionalIndex == 20) nUnionElementIndex = 11;  // ㅗ + ㅣ
						break;

					case 13: // ㅜ
						if (nAdditionalIndex == 4 ) nUnionElementIndex = 14;  // ㅜ + ㅓ
						if (nAdditionalIndex == 5 ) nUnionElementIndex = 15;  // ㅜ + ㅖ
						if (nAdditionalIndex == 20) nUnionElementIndex = 16;  // ㅜ + ㅣ
						break;

					case 18: // ㅡ
						if (nAdditionalIndex == 20) nUnionElementIndex = 19;  // ㅡ + ㅣ
						break;
					}

					if (nUnionElementIndex != -1)
					{
						*pResultCode = 0xac00 + nChoSeongIndex*28*21 + nUnionElementIndex*28;
						return TRUE;
					}
				}
			}
		}


		//=====================================================================
		// nUnionCode가 자음일 경우 모음이 입력된 경우 문자 완성
		//=====================================================================
		else if (0x3131 <= nUnionCode && nUnionCode <= 0x314e) // 자음
		{
			//-----------------------------------------------------------------
			// 모음 nElementCode 와 조합하여 글자 완성
			//-----------------------------------------------------------------
			// ㄱ + ㅏ = 가
			if (nElementCode > 0x314e)
			{
				if ( (nChoSeongIndex = UNICODE20_HANGUL_CHOSEONG_INDEX_TABLE[nUnionCode - 0x3131]) != -1)
				{
					nJungSeongIndex = nElementCode-0x314f;
					*pResultCode    = 0xac00 + nChoSeongIndex*28*21 + nJungSeongIndex*28;
					return TRUE;
				}
			}
		}


		//=====================================================================
		// 첫번째 초성 입력 상태라 가정
		//=====================================================================
		else
		{
			//-----------------------------------------------------------------
			// 모음이라도 조합중이라는 표시를 한다.
			//-----------------------------------------------------------------
			// XX + ㄱ or ㅏ = XXㄱ or XXㅏ
			*pResultCode = nElementCode;
			return TRUE;
		}
	}

	*pResultCode = nElementCode;

	return FALSE;
}


//-----------------------------------------------------------------------------
//
// # PopBackUnionHangulCode ()
//
// - Description:
//                조합된 한글 코드로 부터 마지막 코드를 분리한다.
//                nUnionCode = *pResultCode + *pPopBackCode
//
// - Parameters:
//                 [IN]  WCHAR nUnionCode    조합된 한글 코드
//                 [OUT] WCHAR *pResultCode  분리하여 저장될 코드
//                 [OUT] WCHAR *pPopBackCode 분리된 마지막 코드
// - Return:
//                 BOOL  한글 분리 성공 여부
//
//-----------------------------------------------------------------------------

static BOOL PopBackUnionHangulCode (WCHAR nUnionCode, WCHAR *pResultCode, WCHAR *pPopBackCode)
{
	int nChoSeongIndex;
	int nJungSeongIndex;
	int nJongSeongIndex;
	int nAdditionalIndex;
	int nUnionElementIndex;


	// 한글 조합이 완료된 코드인지 확인
	if (0xac00 <= nUnionCode && nUnionCode <= 0xd7a3)
	{
		nChoSeongIndex   = ( (nUnionCode-0xac00) / (21*28) );
		nJungSeongIndex  = ( (nUnionCode-0xac00) % (21*28) ) / 28;
		nJongSeongIndex  = ( (nUnionCode-0xac00) % 28      );
		nAdditionalIndex = -1;


		//=====================================================================
		// 종성이 없는 경우
		//=====================================================================
		if (nJongSeongIndex == 0)
		{
			switch (nJungSeongIndex)
			{
			case  9: // ㅘ
				nUnionElementIndex =  8; // ㅗ
				nAdditionalIndex   =  0; // ㅏ
				break;
			case 10: // ㅙ
				nUnionElementIndex =  8; // ㅗ
				nAdditionalIndex   =  1; // ㅐ
				break;
			case 11: // ㅚ
				nUnionElementIndex =  8; // ㅗ
				nAdditionalIndex   = 20; // ㅣ
				break;

			case 14: // ㅝ
				nUnionElementIndex = 13; // ㅜ
				nAdditionalIndex   =  4; // ㅓ
				break;
			case 15: // ㅞ
				nUnionElementIndex = 13; // ㅜ
				nAdditionalIndex   =  5; // ㅔ
				break;
			case 16: // ㅟ
				nUnionElementIndex = 13; // ㅜ
				nAdditionalIndex   = 20; // ㅣ
				break;

			case 19: // ㅢ
				nUnionElementIndex = 18; // ㅡ
				nAdditionalIndex   = 20; // ㅣ
				break;
			}

			//-----------------------------------------------------------------
			// 모음 2개로 구성된 중성
			//-----------------------------------------------------------------
			if (nAdditionalIndex != -1)
			{
				// [초성 + 중성(모음1개)]
				*pResultCode  = 0xac00 + nChoSeongIndex*21*28 + nUnionElementIndex*28;
				// [중성(모음1개)]
				*pPopBackCode = 0x314f + nAdditionalIndex;
			}

			//-----------------------------------------------------------------
			// 모음 1개로 구성된 중성
			//-----------------------------------------------------------------
			else
			{
				// [초성]
				*pResultCode  = UNICODE20_HANGUL_CHOSEONG_CODE_TABLE[nChoSeongIndex];
				// [중성(모음1개)]
				*pPopBackCode = 0x314f + nJungSeongIndex;
			}
		}

		//=====================================================================
		// 종성이 있는 경우
		//=====================================================================
		else
		{
			switch (nJongSeongIndex)
			{
			case  3: // ㄳ
				nUnionElementIndex =  1; // ㄱ
				nAdditionalIndex   = 19; // ㅅ
				break;

			case  5: // ㄵ
				nUnionElementIndex =  4; // ㄴ
				nAdditionalIndex   = 22; // ㅈ
				break;
			case  6: // ㄶ
				nUnionElementIndex =  4; // ㄴ
				nAdditionalIndex   = 27; // ㅎ
				break;

			case  9: // ㄺ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   =  1; // ㄱ
				break;
			case 10: // ㄻ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   = 16; // ㅁ
				break;
			case 11: // ㄼ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   = 17; // ㅂ
				break;
			case 12: // ㄽ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   = 19; // ㅅ
				break;
			case 13: // ㄾ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   = 25; // ㅌ
				break;
			case 14: // ㄿ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   = 26; // ㅍ
				break;
			case 15: // ㅀ
				nUnionElementIndex =  8; // ㄹ
				nAdditionalIndex   = 27; // ㅎ
				break;

			case 18: // ㅄ
				nUnionElementIndex = 17; // ㅂ
				nAdditionalIndex   = 19; // ㅅ
				break;
			}

			//-----------------------------------------------------------------
			// 자음 2개로 구성된 종성
			//-----------------------------------------------------------------
			if (nAdditionalIndex != -1)
			{
				*pResultCode  = 0xac00 + nChoSeongIndex*21*28 + nJungSeongIndex*28 + nUnionElementIndex;
				*pPopBackCode = UNICODE20_HANGUL_JONGSEONG_CODE_TABLE[nAdditionalIndex];
			}

			//-----------------------------------------------------------------
			// 자음 2개로 구성된 종성
			//-----------------------------------------------------------------
			else
			{
				*pResultCode  = 0xac00 + nChoSeongIndex*21*28 + nJungSeongIndex*28;
				*pPopBackCode = UNICODE20_HANGUL_JONGSEONG_CODE_TABLE[nJongSeongIndex];
			}
		}

		return TRUE;
	}

	*pResultCode  = 0x0000;
	*pPopBackCode = nUnionCode;

	return FALSE;
}

BOOL BuildInputTextW (BOOL   bSingleLine, BOOL bHangulUnion,
                      WCHAR *pText,       int  nMaxLength,
                      WCHAR  nCharCode,   int *pCursor)
{
	WCHAR nUnionCode;
	WCHAR nResultCode;
	WCHAR nPopBackCode;


	//=========================================================================
	// New-line
	//=========================================================================
	if (bSingleLine == TRUE)
	{
		if (nCharCode == 0x000d || nCharCode == 0x000a)
			return FALSE;
	}
	else
	{
		if (nCharCode == 0x000d || nCharCode == 0x000a)
		{
			if (bHangulUnion == TRUE)
				*pCursor = *pCursor +1;

			if (!SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor, '\r'))
				return FALSE;

			*pCursor = *pCursor +1;

			if (!SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor, '\n'))
				return FALSE;

			*pCursor = *pCursor +1;

			return FALSE;
		}
	}


	nUnionCode = *(pText + *pCursor);


	//=========================================================================
	// Backspace
	//=========================================================================
	if (nCharCode == 0x0008)
	{
		if (*pCursor==0 && bHangulUnion == FALSE)
			return FALSE;

		if (bHangulUnion == TRUE)
		{
			bHangulUnion = PopBackUnionHangulCode (nUnionCode, &nResultCode, &nPopBackCode);

			if (nResultCode != 0x0000)
			{
				*(pText + *pCursor) = nResultCode;
				bHangulUnion = TRUE;
			}
			else
			{
				SafeStringRemoveCharW (pText, -1, nMaxLength, *pCursor);
				bHangulUnion = FALSE;
			}
		}
		else
		{
			if (*pCursor>0)
			{
				if (*pCursor>1)
				{
					if (*(pText + *pCursor - 1) == '\n' && *(pText + *pCursor - 2) == '\r')
					{
						*pCursor = *pCursor - 1;
	
						SafeStringRemoveCharW (pText, -1, nMaxLength, *pCursor);
	
						*pCursor = *pCursor - 1;
	
						SafeStringRemoveCharW (pText, -1, nMaxLength, *pCursor);
						
						return FALSE;
					}
				}
				
				*pCursor = *pCursor - 1;
				SafeStringRemoveCharW (pText, -1, nMaxLength, *pCursor);
			}
			else
				*(pText + *pCursor) = 0x0000;
			
			bHangulUnion = FALSE;
		}
	}

	//=========================================================================
	// 일반 글자 추가
	//=========================================================================
	else
	{
		if (nMaxLength <= *pCursor)
			return FALSE;


		//=====================================================================
		// UNICODE20 한글 낱글자 추가
		//=====================================================================
		if (0x3131<= nCharCode && nCharCode <= 0x3163)
		{
			//-----------------------------------------------------------------
			// 한글 조합 중
			//-----------------------------------------------------------------
			if (bHangulUnion == TRUE)
			{
				bHangulUnion = GetUnionHangulCode (nUnionCode, nCharCode, &nResultCode);

				// 한글 조합 실패
				if (bHangulUnion == FALSE)
				{
					// 모음이 입력된 경우 이전 종성을 검사하여 새로운 글자를 조합한다.
					if (0x314f <= nCharCode && nCharCode<= 0x3163) // 모음
					{
						PopBackUnionHangulCode (nUnionCode, &nResultCode, &nPopBackCode);
						if (0x3131 <= nPopBackCode && nPopBackCode < 0x314f) // 자음
						{
							// 갃 + ㅏ = 가사
							*(pText + *pCursor) = nResultCode;

							*pCursor = *pCursor + 1;

							GetUnionHangulCode (nPopBackCode, nCharCode, &nUnionCode);


							if (SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor, nUnionCode))
								bHangulUnion     = TRUE;
							else
								bHangulUnion     = FALSE;
						}
						else
						{
							// 가 + ㅏ = 가ㅏ
							*pCursor = *pCursor + 1;

							bHangulUnion = FALSE;
							if (SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor, nResultCode))
								if (0x3131 <= nResultCode && nResultCode < 0x314f) // 자음
									bHangulUnion = TRUE;
						}
					}
					else
					{
						// 각 + ㄱ = 각ㄱ
						*(pText + *pCursor) = nUnionCode;

						*pCursor = *pCursor + 1;

						bHangulUnion = FALSE;

						if (SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor, nResultCode))
							if (0x3131 <= nResultCode && nResultCode < 0x314f) // 자음
								bHangulUnion = TRUE;
					}
				}
				// 한글 조합 성공
				else
				{
					// 가 + ㄱ = 각
					*(pText + *pCursor) = nResultCode;
					bHangulUnion = TRUE;
				}
			}
			//-----------------------------------------------------------------
			// 한글 조합이 아닌경우
			//-----------------------------------------------------------------
			else
			{
				// XX + ㄱ = XXㄱ
				if (SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor,  nCharCode))
				{
					if (0x3131 <= nCharCode && nCharCode < 0x314f) // 자음
						bHangulUnion = TRUE;
					else
					{
						bHangulUnion = FALSE;

						*pCursor = *pCursor + 1;
					}
				}
			}
		}

		//=====================================================================
		// UNICODE20 한글 낱글자가 아닌경우
		//=====================================================================
		else
		{
			// 한글 조합 중
			// 가 + A = 가A
			if (bHangulUnion == TRUE)
				*pCursor = *pCursor + 1;

			if (SafeStringInsertCharW (pText, -1, nMaxLength, *pCursor, nCharCode))
				*pCursor = *pCursor + 1;

			bHangulUnion = FALSE;
		}
	}

	return bHangulUnion;
}
