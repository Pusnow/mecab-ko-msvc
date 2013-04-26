# mecab-ko ChangeLog

## mecab-ko-0.9.0 (mecab-0.996)

  - MeCab 0.996 기반
  - MeCab의 사전 설정(dicrc)에서 좌측에 공백 문자를 포함하는 품사의 연접 비용을 늘릴 수 있게 설정하는 기능 추가

### 좌측 공백 포함 문자 연접 비용 늘리는 설정 예 (dicrc)

    :::text
    # 좌측에 공백을 포함하는 품사의 연접 비용을 늘리기 위한 설정입니다.
    # mecab-ko에서만 사용되는 설정입니다. 다음과 같은 형식을 가집니다.
    # <posid 1>,<posid 1 penalty cost>,<posid 2>,<posid 2 penalty cost> ...
    # 
    # 예) 120,6000 => posid가 120인 품사(조사)의 좌측에 공백을 포함할 경우
    # 연접 비용을 6000만큼 늘림
    left-space-penalty-factor = 120,6000,184,6000,100,500
