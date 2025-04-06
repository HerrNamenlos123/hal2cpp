FROM debian:bookworm

RUN apt update && apt install -y \
  linuxcnc-uspace \
  x11-apps \
  && apt clean

RUN apt install -y build-essential cmake
RUN apt install -y git
RUN apt install -y linuxcnc-uspace-dev

WORKDIR /app
RUN mkdir -p build

COPY ./configs ./configs
COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./generate.py ./generate.py
COPY ./config.json ./config.json

RUN cmake -S . -B build -DONLY_FETCH_SOCKPP=TRUE

COPY ./src ./src
RUN cmake -S . -B build -DHAL2CPP_MAIN_CONFIG_HAL_FILE="/app/configs/hal2cpp-config/hal2cpp-config.hal" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

RUN cmake --build build --target install

CMD $START_CMD $CONFIG_FILE
