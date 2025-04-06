FROM debian:bookworm

RUN apt update && apt install -y \
  linuxcnc-uspace \
  x11-apps \
  && apt clean

RUN apt install -y build-essential cmake
RUN apt install -y git
RUN apt install -y linuxcnc-uspace-dev

WORKDIR /tmp/sockpp
RUN git clone https://github.com/fpagliughi/sockpp .
RUN cmake . -B build -DSOCKPP_BUILD_STATIC=TRUE SOCKPP_BUILD_SHARED=FALSE -DCMAKE_POSITION_INDEPENDENT_CODE=ON
RUN cmake --build build --target=install

WORKDIR /app
RUN mkdir -p build

COPY ./configs ./configs
COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./src ./src

RUN cmake -S . -B build -DHAL2CPP_MAIN_CONFIG_HAL_FILE="/app/configs/hal2cpp-config/hal2cpp-config.hal" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

RUN cmake --build build --target hal2cpp
RUN cmake --build build --target install

CMD $START_CMD $CONFIG_FILE
