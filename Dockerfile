FROM debian:bookworm

RUN apt update && apt install -y \
  linuxcnc-uspace \
  x11-apps \
  && apt clean

CMD ["linuxcnc"]
