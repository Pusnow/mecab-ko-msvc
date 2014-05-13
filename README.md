# mecab-ko 소개

[mecab-ko](https://bitbucket.org/eunjeon/mecab-ko)는 [은전한닢 프로젝트](http://eunjeon.blogspot.kr/)에서 사용하기 위한 [MeCab](http://mecab.googlecode.com/svn/trunk/mecab/doc/index.html)의 fork 프로젝트 입니다.

최소한의 변경으로 한국어의 특성에 맞는 기능을 추가하는 것이 목표입니다.

# mecab-ko에서 추가된 기능.

## 공백 문자(white space)를 포함하는 특정 품사 비용 늘림

띄어쓰기를 하지 않는 일본어와 달리 띄어쓰기를 하는 한국어 특성에 맞게 특정 품사가 띄어쓰기 되어있는 경우 해당 품사의 비용을 늘리는 기능 (사전 설정(dicrc)에 설정 값을 지정)

__mecab을 사용하여 분석__

    :::text
    화학 이외의 것
    화학    NN,T,화학,*,*,*,*
    이      JKS,F,이,*,*,*,*
    외      NN,F,외,*,*,*,*
    의      JKG,F,의,*,*,*,*
    것      NNB,T,것,*,*,*,*
    EOS

__mecab-ko를 사용하여 분석__

    :::text
    화학 이외의 것
    화학    NN,T,화학,*,*,*,*
    이외    NN,F,이외,*,*,*,*
    의      JKG,F,의,*,*,*,*
    것      NNB,T,것,*,*,*,*
    EOS

### 설정 방법

MeCab의 사전 설정(dicrc)에서 다음과 같이 설정합니다.

    :::text
    # 좌측에 공백을 포함하는 품사의 연접 비용을 늘리기 위한 설정입니다.
    # mecab-ko에서만 사용되는 설정입니다. 다음과 같은 형식을 가집니다.
    # <posid 1>,<posid 1 penalty cost>,<posid 2>,<posid 2 penalty cost> ...
    # 
    # 예) 120,6000 => posid가 120인 품사(조사)의 좌측에 공백을 포함할 경우
    # 연접 비용을 6000만큼 늘림
    left-space-penalty-factor = 120,6000,184,6000,100,500

# mecab-ko의 설치와 사용법

## mecab-ko 설치

  [mecab-ko 다운로드 페이지](https://bitbucket.org/eunjeon/mecab-ko/downloads)에서 최신 버전의 소스를 다운 받고 설치합니다. tar.gz 압축을 해제하고 일반적인 자유 소프트웨어와 같은 순서로 설치할 수 있습니다.

    :::text
    $ tar zxfv mecab-ko-XX.tar.gz
    $ cd mecab-ko-XX
    $ ./configure 
    $ make
    $ make check
    $ su
    # make install

설치 방법은 MeCab와 동일하므로, 자세한 내용은 [MeCab 홈페이지](http://mecab.googlecode.com/svn/trunk/mecab/doc/index.html)를 참조하시기 바랍니다.

### 참고

  * 오래된 버전의 리눅스 환경에서 컴파일이 안되는 경우, 다음의 글을 참조하시기 바랍니다. [Cent OS 5.9에서 MeCab 및 mecab-ko-dic 설치하기](http://eunjeon.blogspot.kr/2013/02/cent-os-59-mecab-mecab-ko-dic.html)

## 한국어 사전(mecab-ko-dic)의 설치와 사용

  [mecab-ko-dic](https://bitbucket.org/eunjeon/mecab-ko-dic)의 설명을 참조하시기 바랍니다.

# 라이센스

mecab-ko의 라이센스는 MeCab의 라이센스를 그대로 따릅니다.

> MeCab 는 무료 소프트웨어입니다. GPL (the GNU General Public License), LGPL (Lesser GNU General Public License) 또는 BSD 라이선스에 따라 소프트웨어를 사용, 재배포할 수 있습니다. 자세한 내용은 COPYING, GPL, LGPL, BSD 각 파일을 참조하십시오.
