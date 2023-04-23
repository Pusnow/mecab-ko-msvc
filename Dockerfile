from debian:bullseye-slim as build
LABEL maintainer="wonsup@pusnow.com"
LABEL org.opencontainers.image.source https://github.com/Pusnow/mecab-ko-msvc

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y &&\
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -y \
    build-essential \
    python3-pip \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /var/cache/apt/

RUN pip3 --no-cache-dir install ninja meson

COPY . /src/
WORKDIR /src

RUN meson setup --prefix=/opt/mecab -Dbuildtype=debugoptimized builddir
RUN meson compile -C builddir
RUN meson install -C builddir

from debian:bullseye-slim
LABEL maintainer="wonsup@pusnow.com"
LABEL org.opencontainers.image.source https://github.com/Pusnow/mecab-ko-msvc

COPY --from=build /opt/mecab /opt/mecab
ENV PATH=/opt/mecab/bin:$PATH
CMD ["mecab"]